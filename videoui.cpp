#include "videoui.h"
#include "./ui_videoui.h"

VideoUI::VideoUI(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::VideoUI)
{
    ui->setupUi(this);
}

VideoUI::~VideoUI()
{
    delete ui;
}
