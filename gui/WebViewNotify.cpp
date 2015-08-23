#include "WebViewNotify.h"

WebViewNotify::WebViewNotify(QObject *parent) : QObject(parent)
{

}



void WebViewNotify::notifyReceivedMsg(const RsPeerId& peer_id, QString str)
{
	std::cout << "pNotify Recvd Packet from: " << peer_id;
	std::cout << " saying " << str.toStdString();
	std::cout << std::endl;
	emit NeMsgArrived(peer_id, str) ;
}
