/* This is the main page displayed by the plugin */
#ifndef NEMAINPAGE_H
#define NEMAINPAGE_H

#include <retroshare-gui/mainpage.h>
#include <retroshare/rsfiles.h>
#include <retroshare/rspeers.h>
#include "gui/WebViewNotify.h"



#include <QWidget>

namespace Ui {
class WebViewMainpage;
}

class WebViewMainpage : public MainPage
{
	Q_OBJECT

public:
	explicit WebViewMainpage(QWidget *parent, WebViewNotify *notify);
	~WebViewMainpage();

private slots:
	void on_pingAllButton_clicked();
	void NeMsgArrived(const RsPeerId &peer_id, QString str);

	void on_broadcastButton_clicked();
private:
	Ui::WebViewMainpage *ui;
	WebViewNotify *mNotify;
};

#endif // NEMAINPAGE_H
