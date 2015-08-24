#include "WebViewMainpage.h"
#include "ui_WebViewMainpage.h"
//#include "services/p3WebView.h"
#include "interface/rsWebView.h"
#include<qjsondocument.h>
#include "webbridgers.h"

//#include "webenginepagepanel.h"
#include "webtabcontents.h"


WebBridgeRS *eBridge = new WebBridgeRS();
WebViewMainpage::WebViewMainpage(QWidget *parent, WebViewNotify *notify) :
	MainPage(parent),
	mNotify(notify),
	ui(new Ui::WebViewMainpage)
{
	ui->setupUi(this);

	connect(mNotify, SIGNAL(NeMsgArrived(RsPeerId,QString)), this , SLOT(NeMsgArrived(RsPeerId,QString)));
	//ui->tabWidget->addTab(new WebViewPagePanel(), "Home");
	WebTabContents * wtc = new WebTabContents(this);
	wtc->getWebView()->setUrl(QUrl("qrc:/rshtml/index.html"));
	ui->tabWidget->addTab(wtc,QString("WSRS"));

}
/*
 * void WebScriptDialog::onNewTabUrl(QString url)
{
    WebTabContents * wtc = new WebTabContents(this);
    wtc->getWebView()->setUrl(QUrl(url));
    ui->webTabs->addTab(wtc,QString("WSRS"));
    connect( wtc->getWebView(), SIGNAL(titleChanged(QString)),    this,   SLOT(onTitleChanged(QString)) );
    connect( wtc->getWebView(), SIGNAL(requestTabForHash(QString)),    this,   SLOT(onNewRsTab(QString)) );
}
*/

WebViewMainpage::~WebViewMainpage()
{
	delete ui;
}


void WebViewMainpage::on_pingAllButton_clicked()
{
	rsWebView->ping_all();
	NeMsgArrived(rsPeers->getOwnId(),"ping");
}


void WebViewMainpage::NeMsgArrived(const RsPeerId &peer_id, QString str)
{
	/*QJsonDocument jdoc = QJsonDocument::fromJson(str.toUtf8());
	QVariantMap vmap = jdoc.toVariant().toMap();
	std::cout << "GUI got Packet from: " << peer_id;
	std::cout << " saying " << str.toStdString();
	std::cout << std::endl;
	QString type = vmap.value("type").toString();
	if (type == "chat"){
		QString output = QString::fromStdString(rsPeers->getPeerName(peer_id));
		output+=": ";
		output+=vmap.value("message").toString();
		//ui->listWidget->addItem(output);
	}else{
		QString output = QString::fromStdString(rsPeers->getPeerName(peer_id));
		output+=": ";
		output+=str;
		//ui->listWidget->addItem(output);
	}

	{
		QString output = QString::fromStdString(rsPeers->getPeerName(peer_id));
		output+=": ";
		output+=str;
		//ui->netLogWidget->addItem(output);
	}*/
}

void WebViewMainpage::on_broadcastButton_clicked()
{
	/*rsWebView->msg_all(ui->msgInput->text().toStdString());
	NeMsgArrived(rsPeers->getOwnId(),ui->msgInput->text());
	ui->msgInput->clear();*/
}
