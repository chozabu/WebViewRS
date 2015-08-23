#ifndef WEBENGINEPAGEPANEL_H
#define WEBENGINEPAGEPANEL_H

#include <QWidget>

namespace Ui {
class WebViewPagePanel;
}

class WebViewPagePanel : public QWidget
{
    Q_OBJECT

public:
    explicit WebViewPagePanel(QWidget *parent = 0);
    ~WebViewPagePanel();

private:
    Ui::WebViewPagePanel *ui;
};

#endif // WEBENGINEPAGEPANEL_H
