#ifndef VIDEOUI_H
#define VIDEOUI_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class VideoUI;
}
QT_END_NAMESPACE

class VideoUI : public QMainWindow
{
    Q_OBJECT

public:
    VideoUI(QWidget *parent = nullptr);
    ~VideoUI();

private:
    Ui::VideoUI *ui;
};
#endif // VIDEOUI_H
