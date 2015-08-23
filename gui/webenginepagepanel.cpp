#include "webenginepagepanel.h"
#include "ui_webenginepagepanel.h"

WebViewPagePanel::WebViewPagePanel(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WebViewPagePanel)
{
    ui->setupUi(this);

}

WebViewPagePanel::~WebViewPagePanel()
{
    delete ui;
}
