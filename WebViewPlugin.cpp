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
#include <retroshare/rsplugin.h>
#include <retroshare/rsversion.h>
#include <retroshare-gui/RsAutoUpdatePage.h>
#include <QTranslator>
#include <QApplication>
#include <QString>
#include <QIcon>
#include <QMessageBox>

#include "WebViewPlugin.h"
#include "interface/rsWebView.h"
#include "gui/WebViewMainpage.h"
#include "gui/WebViewNotify.h"


#define IMAGE_WebView ":/images/talking_on.svg"

static void *inited = new WebViewPlugin() ;

extern "C" {

	// This is *the* functions required by RS plugin system to give RS access to the plugin.
	// Be careful to:
	// - always respect the C linkage convention
	// - always return an object of type RsPlugin*
	//
	void *RETROSHARE_PLUGIN_provide()
	{
		static WebViewPlugin *p = new WebViewPlugin() ;

		return (void*)p ;
	}

	// This symbol contains the svn revision number grabbed from the executable. 
	// It will be tested by RS to load the plugin automatically, since it is safe to load plugins
	// with same revision numbers, assuming that the revision numbers are up-to-date.
	//
	uint32_t RETROSHARE_PLUGIN_revision = RS_REVISION_NUMBER ;

	// This symbol contains the svn revision number grabbed from the executable. 
	// It will be tested by RS to load the plugin automatically, since it is safe to load plugins
	// with same revision numbers, assuming that the revision numbers are up-to-date.
	//
	uint32_t RETROSHARE_PLUGIN_api = RS_PLUGIN_API_VERSION ;
}

void WebViewPlugin::getPluginVersion(int& major, int& minor, int& build, int& svn_rev) const
{
	major = RS_MAJOR_VERSION ;
	minor = RS_MINOR_VERSION ;
	build = RS_BUILD_NUMBER ;
	svn_rev = RS_REVISION_NUMBER ;
}

WebViewPlugin::WebViewPlugin()
{
	qRegisterMetaType<RsPeerId>("RsPeerId");
	mainpage = NULL ;
	mWebView = NULL ;
	mPlugInHandler = NULL;
	mPeers = NULL;
	config_page = NULL ;
	mIcon = NULL ;

	mWebViewNotify = new WebViewNotify;
}

void WebViewPlugin::setInterfaces(RsPlugInInterfaces &interfaces)
{
    mPeers = interfaces.mPeers;
}

/*ConfigPage *WebViewPlugin::qt_config_page() const
{
	// The config pages are deleted when config is closed, so it's important not to static the
	// created object.
	//
	return new AudioInputConfig() ;
}*/

QDialog *WebViewPlugin::qt_about_page() const
{
	static QMessageBox *about_dialog = NULL ;
	
	if(about_dialog == NULL)
	{
		about_dialog = new QMessageBox() ;

		QString text ;
		text += QObject::tr("<h3>RetroShare WebView plugin</h3><br/>   * Contributors: Cyril Soler, Josselin Jacquard<br/>") ;
		text += QObject::tr("<br/>The WebView plugin adds WebView to the private chat window of RetroShare. to use it, proceed as follows:<UL>") ;
		text += QObject::tr("<li> setup microphone levels using the configuration panel</li>") ;
		text += QObject::tr("<li> check your microphone by looking at the VU-metters</li>") ;
		text += QObject::tr("<li> in the private chat, enable sound input/output by clicking on the two WebView icons</li></ul>") ;
		text += QObject::tr("Your friend needs to run the plugin to talk/listen to you, or course.") ;
		text += QObject::tr("<br/><br/>This is an experimental feature. Don't hesitate to send comments and suggestion to the RS dev team.") ;

		about_dialog->setText(text) ;
		about_dialog->setStandardButtons(QMessageBox::Ok) ;
	}

	return about_dialog ;
}

/*ChatWidgetHolder *WebViewPlugin::qt_get_chat_widget_holder(ChatWidget *chatWidget) const
{
	switch (chatWidget->chatType()) {
	case ChatWidget::CHATTYPE_PRIVATE:
		return new WebViewChatWidgetHolder(chatWidget, mWebViewNotify);
	case ChatWidget::CHATTYPE_UNKNOWN:
	case ChatWidget::CHATTYPE_LOBBY:
	case ChatWidget::CHATTYPE_DISTANT:
		break;
	}

	return NULL;
}*/

p3Service *WebViewPlugin::p3_service() const
{
	if(mWebView == NULL)
		rsWebView = mWebView = new p3WebView(mPlugInHandler,mWebViewNotify) ; // , 3600 * 24 * 30 * 6); // 6 Months

	return mWebView ;
}

void WebViewPlugin::setPlugInHandler(RsPluginHandler *pgHandler)
{
    mPlugInHandler = pgHandler;
}

QIcon *WebViewPlugin::qt_icon() const
{
	if (mIcon == NULL) {
		Q_INIT_RESOURCE(WebView_images);

		mIcon = new QIcon(IMAGE_WebView);
	}

	return mIcon;
}
MainPage *WebViewPlugin::qt_page() const
{
	if(mainpage == NULL){
		mainpage = new WebViewMainpage(0, mWebViewNotify);//mPeers, mFiles) ;
		//tpage = new WebViewMainpage( );
		//mainpage = tpage;
	}

	return mainpage ;
}

std::string WebViewPlugin::getShortPluginDescription() const
{
	return "WebView";
}

std::string WebViewPlugin::getPluginName() const
{
	return "WebViewPlugin";
}

QTranslator* WebViewPlugin::qt_translator(QApplication */*app*/, const QString& languageCode, const QString& externalDir) const
{
	return NULL;
}

void WebViewPlugin::qt_sound_events(SoundEvents &/*events*/) const
{
//	events.addEvent(QApplication::translate("WebView", "WebView"), QApplication::translate("WebView", "Incoming call"), WebView_SOUND_INCOMING_CALL);
}

/*ToasterNotify *WebViewPlugin::qt_toasterNotify(){
	if (!mWebViewToasterNotify) {
		mWebViewToasterNotify = new WebViewToasterNotify(mWebView, mWebViewNotify);
	}
	return mWebViewToasterNotify;
}*/
