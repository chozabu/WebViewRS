/****************************************************************
 *  RetroShare is distributed under the following license:
 *
 *  Copyright (C) 2015
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version 2
 *  of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor,
 *  Boston, MA  02110-1301, USA.
 ****************************************************************/

#include "util/rsdir.h"
#include "retroshare/rsiface.h"
#include "pqi/pqibin.h"
#include "pqi/pqistore.h"
#include "pqi/p3linkmgr.h"
#include <serialiser/rsserial.h>
#include <serialiser/rsconfigitems.h>

#include <sstream> // for std::istringstream

#include "services/p3WebView.h"
#include "services/rsWebViewItems.h"

#include <sys/time.h>

#include "gui/WebViewNotify.h"

#include "../gui/webbridgers.h"


//#define DEBUG_WebView		1


/* DEFINE INTERFACE POINTER! */
RsWebView *rsWebView = NULL;



#ifdef WINDOWS_SYS
#include <time.h>
#include <sys/timeb.h>
#endif

static double getCurrentTS()
{

#ifndef WINDOWS_SYS
        struct timeval cts_tmp;
        gettimeofday(&cts_tmp, NULL);
        double cts =  (cts_tmp.tv_sec) + ((double) cts_tmp.tv_usec) / 1000000.0;
#else
        struct _timeb timebuf;
        _ftime( &timebuf);
        double cts =  (timebuf.time) + ((double) timebuf.millitm) / 1000.0;
#endif
        return cts;
}

static uint64_t convertTsTo64bits(double ts)
{
	uint32_t secs = (uint32_t) ts;
	uint32_t usecs = (uint32_t) ((ts - (double) secs) * 1000000);
	uint64_t bits = (((uint64_t) secs) << 32) + usecs;
	return bits;
}

static double convert64bitsToTs(uint64_t bits)
{
	uint32_t usecs = (uint32_t) (bits & 0xffffffff);
	uint32_t secs = (uint32_t) ((bits >> 32) & 0xffffffff);
	double ts =  (secs) + ((double) usecs) / 1000000.0;

	return ts;
}

p3WebView::p3WebView(RsPluginHandler *handler,WebViewNotify *notifier)
	 : RsPQIService(RS_SERVICE_TYPE_WebView_PLUGIN,0,handler), mWebViewMtx("p3WebView"), mServiceControl(handler->getServiceControl()) , mNotify(notifier)
{
	addSerialType(new RsWebViewSerialiser());


		//plugin default configuration

}
RsServiceInfo p3WebView::getServiceInfo()
{
	const std::string TURTLE_APP_NAME = "WebView";
    const uint16_t TURTLE_APP_MAJOR_VERSION  =       1;
    const uint16_t TURTLE_APP_MINOR_VERSION  =       0;
    const uint16_t TURTLE_MIN_MAJOR_VERSION  =       1;
    const uint16_t TURTLE_MIN_MINOR_VERSION  =       0;

	return RsServiceInfo(RS_SERVICE_TYPE_WebView_PLUGIN,
                         TURTLE_APP_NAME,
                         TURTLE_APP_MAJOR_VERSION,
                         TURTLE_APP_MINOR_VERSION,
                         TURTLE_MIN_MAJOR_VERSION,
                         TURTLE_MIN_MINOR_VERSION);
}

int	p3WebView::tick()
{
#ifdef DEBUG_WebView
	std::cerr << "ticking p3WebView" << std::endl;
#endif

	//processIncoming();
	//sendPackets();

	return 0;
}

int	p3WebView::status()
{
	return 1;
}
#include<qjsondocument.h>
void p3WebView::str_msg_peer(RsPeerId peerID, QString strdata){
	QVariantMap map;
	map.insert("type", "chat");
	map.insert("message", strdata);

	qvm_msg_peer(peerID,map);
}

void p3WebView::qvm_msg_peer(RsPeerId peerID, QVariantMap data){
	QJsonDocument jsondoc = QJsonDocument::fromVariant(data);
	std::string msg = jsondoc.toJson().toStdString();
	raw_msg_peer(peerID, msg);
}
void p3WebView::raw_msg_peer(RsPeerId peerID, std::string msg){
	std::cout << "MSging: " << peerID.toStdString() << "\n";
	std::cout << "MSging: " << msg << "\n";
		/* create the packet */
		RsWebViewDataItem *pingPkt = new RsWebViewDataItem();
		pingPkt->PeerId(peerID);
		pingPkt->m_msg = msg;
		pingPkt->data_size = msg.size();
		//pingPkt->mSeqNo = mCounter;
		//pingPkt->mPingTS = convertTsTo64bits(ts);

		//storePingAttempt(*it, ts, mCounter);

#ifdef DEBUG_WebView
		std::cerr << "p3WebView::msg_all() With Packet:";
		std::cerr << std::endl;
		pingPkt->print(std::cerr, 10);
#endif

		sendItem(pingPkt);
}

void p3WebView::msg_all(std::string msg){
	/* we ping our peers */
	//if(!mServiceControl)
	//    return ;

	//std::set<RsPeerId> onlineIds;
	std::list< RsPeerId > onlineIds;
	//    mServiceControl->getPeersConnected(getServiceInfo().mServiceType, onlineIds);
	rsPeers->getOnlineList(onlineIds);

	double ts = getCurrentTS();

#ifdef DEBUG_WebView
	std::cerr << "p3WebView::msg_all() @ts: " << ts;
	std::cerr << std::endl;
#endif

	std::cout << "READY TO BCast: " << onlineIds.size() << "\n";
	/* prepare packets */
	std::list<RsPeerId>::iterator it;
	for(it = onlineIds.begin(); it != onlineIds.end(); it++)
	{
		raw_msg_peer(RsPeerId(*it),msg);
	}
}

void p3WebView::ping_all(){
	//TODO ping all!
}




void p3WebView::handleData(RsWebViewDataItem *item)
{
	RsStackMutex stack(mWebViewMtx); /****** LOCKED MUTEX *******/

	// store the data in a queue.

    //std::cout << "GOT MSG\n";
    /*std::string msg = item->getMessage();
    //std::cout << msg << "\n\n";
    if (msg.substr(0,4).compare("JINT") == 0){
        compatablePeers.insert(item->PeerId().c_str(),msg.c_str());
        return;
    }*/
    QVariantMap qDict;
    qDict.insert("message",QString(item->m_msg.c_str()));
    qDict.insert("peerID",QString(item->PeerId().toStdString().c_str()));
    qDict.insert("peerName",QString(rsPeers->getPeerName(item->PeerId()).c_str()));
    /*QStringList qMsg;
    qMsg.append(QString(item->getMessage().c_str()));
    qMsg.append(QString(item->PeerId().c_str()));
    qMsg.append(QString(rsPeers->getPeerName(item->PeerId()).c_str()));*/
    eBridge->pushMsgToJs(qDict);

	mNotify->notifyReceivedMsg(item->PeerId(), QString::fromStdString(item->m_msg));
}

bool	p3WebView::recvItem(RsItem *item)
{
	std::cout << "recvItem type: " << item->PacketSubType() << "\n";
	/* pass to specific handler */
	bool keep = false ;

	switch(item->PacketSubType())
	{
		case RS_PKT_SUBTYPE_WebView_DATA:
			handleData(dynamic_cast<RsWebViewDataItem*>(item));
			keep = true ;
			break;

		default:
			break;
	}

	/* clean up */
	if(!keep)
		delete item;
	return true ;
} 



RsTlvKeyValue p3WebView::push_int_value(const std::string& key,int value)
{
	RsTlvKeyValue kv ;
	kv.key = key ;
	rs_sprintf(kv.value, "%d", value);

	return kv ;
}
int p3WebView::pop_int_value(const std::string& s)
{
	std::istringstream is(s) ;

	int val ;
	is >> val ;

	return val ;
}

bool p3WebView::saveList(bool& cleanup, std::list<RsItem*>& lst)
{
	cleanup = true ;

	RsConfigKeyValueSet *vitem = new RsConfigKeyValueSet ;

	/*vitem->tlvkvs.pairs.push_back(push_int_value("P3WebView_CONFIG_ATRANSMIT",_atransmit)) ;
	vitem->tlvkvs.pairs.push_back(push_int_value("P3WebView_CONFIG_VOICEHOLD",_voice_hold)) ;
	vitem->tlvkvs.pairs.push_back(push_int_value("P3WebView_CONFIG_VADMIN"   ,_vadmin)) ;
	vitem->tlvkvs.pairs.push_back(push_int_value("P3WebView_CONFIG_VADMAX"   ,_vadmax)) ;
	vitem->tlvkvs.pairs.push_back(push_int_value("P3WebView_CONFIG_NOISE_SUP",_noise_suppress)) ;
	vitem->tlvkvs.pairs.push_back(push_int_value("P3WebView_CONFIG_MIN_LOUDN",_min_loudness)) ;
	vitem->tlvkvs.pairs.push_back(push_int_value("P3WebView_CONFIG_ECHO_CNCL",_echo_cancel)) ;*/

	lst.push_back(vitem) ;

	return true ;
}
bool p3WebView::loadList(std::list<RsItem*>& load)
{
	for(std::list<RsItem*>::const_iterator it(load.begin());it!=load.end();++it)
	{
#ifdef P3TURTLE_DEBUG
		assert(item!=NULL) ;
#endif
		RsConfigKeyValueSet *vitem = dynamic_cast<RsConfigKeyValueSet*>(*it) ;
		/*
		if(vitem != NULL)
			for(std::list<RsTlvKeyValue>::const_iterator kit = vitem->tlvkvs.pairs.begin(); kit != vitem->tlvkvs.pairs.end(); ++kit) 
				if(kit->key == "P3WebView_CONFIG_ATRANSMIT")
					_atransmit = pop_int_value(kit->value) ;
				else if(kit->key == "P3WebView_CONFIG_VOICEHOLD")
					_voice_hold = pop_int_value(kit->value) ;
				else if(kit->key == "P3WebView_CONFIG_VADMIN")
					_vadmin = pop_int_value(kit->value) ;
				else if(kit->key == "P3WebView_CONFIG_VADMAX")
					_vadmax = pop_int_value(kit->value) ;
				else if(kit->key == "P3WebView_CONFIG_NOISE_SUP")
					_noise_suppress = pop_int_value(kit->value) ;
				else if(kit->key == "P3WebView_CONFIG_MIN_LOUDN")
					_min_loudness = pop_int_value(kit->value) ;
				else if(kit->key == "P3WebView_CONFIG_ECHO_CNCL")
					_echo_cancel = pop_int_value(kit->value) ;

		delete vitem ;
	*/
	}

	return true ;
}

RsSerialiser *p3WebView::setupSerialiser()
{
	RsSerialiser *rsSerialiser = new RsSerialiser();
	rsSerialiser->addSerialType(new RsWebViewSerialiser());
	rsSerialiser->addSerialType(new RsGeneralConfigSerialiser());

	return rsSerialiser ;
}










