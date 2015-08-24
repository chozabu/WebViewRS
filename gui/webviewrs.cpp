#include "webviewrs.h"
#include <QDebug>
#include <QWebFrame>
#include "webbridgers.h"
#include "gui/RetroShareLink.h"
#include "gui/notifyqt.h"
#include "retroshare/rsfiles.h"

WebViewRS::WebViewRS(QWidget *parent) :
    QWebView(parent)
{
    connect( this->page()->mainFrame(), SIGNAL(javaScriptWindowObjectCleared()),    this,   SLOT(onJavaScriptWindowObjectCleared()) );
    this->page()->setLinkDelegationPolicy(QWebPage::DelegateExternalLinks);
    connect( this->page(), SIGNAL(linkClicked(const QUrl & )),    this,   SLOT(onLinkClicked(const QUrl & )) );
    settings()->enablePersistentStorage();

    connect(NotifyQt::getInstance(), SIGNAL(downloadComplete(QString)), this, SLOT(onDownloadComplete(QString)));


}
WebViewRS::~WebViewRS()
{
    //this->close();
}
void WebViewRS::onDownloadComplete(QString hash){
    if (hash == hashque){

        FileInfo fi;
        // look up path
        if (rsFiles->alreadyHaveFile(RsFileHash(hash.toStdString()), fi)){
            QString qpath(fi.path.c_str());
            QString name(fi.fname.c_str());
            if(!qpath.endsWith(name)){
                qpath.append("/");
                qpath.append(name);
            }
            QUrl newurl(qpath);
            this->setUrl(newurl);
            return;
        }
    }
}

void WebViewRS::onLinkClicked(const QUrl & url){
    std::cout << "processing webkut url: " << url.toString().toStdString().c_str() << std::endl;
    if(url.toString().startsWith("retroshare")){
        //retroshare://file?name=Distro_guide.png&size=946336&hash=68282a3813fa284ab3c997c9702599b44c6db568
        RetroShareLink rsl(url);
        if (rsl.name().endsWith(".png") || rsl.name().endsWith(".gif") || rsl.name().endsWith(".jpg") || rsl.name().endsWith(".txt")  ||
                rsl.name().endsWith(".html") || rsl.name().endsWith(".htm") || rsl.name().endsWith(".mp3")){
                    loadRSFile(rsl.name(), rsl.hash(), rsl.size());
                    return;
        }
        QStringList urls;
        urls.push_back(url.toString());
        RetroShareLink::process(urls, RetroShareLink::TYPE_UNKNOWN, RSLINK_PROCESS_NOTIFY_ALL);
    } else {
        this->setUrl(url);
    }
}

void WebViewRS::loadRSFile(QString qname, QString qhash, int qsize){

    FileInfo fi;
    //look up path
    if (rsFiles->alreadyHaveFile(RsFileHash(qhash.toStdString()), fi)){
        QString qpath(fi.path.c_str());
        QString name(fi.fname.c_str());
        if(!qpath.endsWith(name)){
            qpath.append("/");
            qpath.append(name);
        }
        QUrl newurl(qpath);
        this->setUrl(newurl);
        return;
    }
    //hashque = qhash;
    emit requestTabForHash(qhash);
    // Get a list of available direct sources, in case the file is browsable only.
    //
    FileInfo finfo ;
    rsFiles->FileDetails(RsFileHash(qhash.toStdString()), RS_FILE_HINTS_REMOTE, finfo) ;

    std::list<RsPeerId> srcIds;
    for(std::list<TransferInfo>::const_iterator it(finfo.peers.begin());it!=finfo.peers.end();++it)
    {
    #ifdef DEBUG_RSLINK
        std::cerr << "  adding peerid " << (*it).peerId << std::endl ;
    #endif
        srcIds.push_back((*it).peerId) ;
    }

    if (rsFiles->FileRequest(qname.toStdString(), RsFileHash(qhash.toStdString()), qsize, "", RS_FILE_REQ_ANONYMOUS_ROUTING, srcIds)) {
    } else {
    }
}

void WebViewRS::onJavaScriptWindowObjectCleared()
{
    QWebFrame *frame = this->page()->mainFrame();
    //frame->setZoomFactor(4);
    frame->addToJavaScriptWindowObject("webview", this);
    frame->addToJavaScriptWindowObject("bridge", eBridge);
}


void WebViewRS::go2page(QString url)
{
    this->setUrl(QUrl(url));

}

void WebViewRS::onChange()
{
    this->setUrl(QUrl("canvas_interactive_particles.html"));// << "Browser::onChange()";
    qDebug() << "Browser::onChange()";
}

void WebViewRS::setHashQue(QString hash)
{
    hashque = hash;
}
