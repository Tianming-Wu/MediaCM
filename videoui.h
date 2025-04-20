#ifndef VIDEOUI_H
#define VIDEOUI_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QFile>
#include <QFileDialog>
#include <QGraphicsVideoItem>
#include <QTimer>
#include <QPropertyAnimation>

#include "configcm.h"
#include "themecm.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class VideoUI;
}
QT_END_NAMESPACE

namespace VUI {
enum PanelState {
    NormalState, HiddenState
};
}

class VideoUI : public QMainWindow
{
    Q_OBJECT

public:
    VideoUI(QWidget *parent = nullptr);
    ~VideoUI();

    bool isPlaying();
    bool isPaused();

    bool isPanelVisible();

public slots:
    void loadVideo(const QUrl& source);

    void Play();
    void Pause();
    void PausePlay();

    void PreviousFrame();
    void NextFrame();

    void showPanel(bool v);
    void showPanel() {
        showPanel(!isPanelVisible());
    }

protected slots:
    void showProgress(qint64 position);

    void tryStartPanelHideTimer();

protected:
    QString formatDuration(qint64 duration, bool ms = false);
    void updateDuraion(int duration);

    void showEvent(QShowEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;

private:
    Ui::VideoUI *ui;

    QMediaPlayer *player;
    QGraphicsScene *scene;
    QGraphicsVideoItem *videoItem;
    QAudioOutput *audio;

    QUrl source;
    qreal pc;
    qint64 duration;
    QString durationstr;
    double frameRate;
    qint64 msPerFrame;

    QTimer *pct;
    bool lastState;

    bool dbgclickmark;

    QSize panelSize;
    QPoint panelPos, panelHiddenPos;
    VUI::PanelState panelState = VUI::NormalState;
    QTimer *panelShowTimer;
    QPropertyAnimation *pa_show;

    ConfigCM config;
    ThemeCM theme;
};
#endif // VIDEOUI_H
