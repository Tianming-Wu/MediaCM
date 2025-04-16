#ifndef VIDEOUI_H
#define VIDEOUI_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QFile>
#include <QFileDialog>
#include <QGraphicsVideoItem>
#include <QTimer>

#include "configcm.h"

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

    bool isPlaying();
    bool isPaused();

public slots:
    void loadVideo(const QUrl& source);

    void Play();
    void Pause();
    void PausePlay();

    void PreviousFrame();
    void NextFrame();

protected slots:
    void showProgress(qint64 position);


protected:
    QString formatDuration(qint64 duration, bool ms = false);
    void updateDuraion(int duration);

    void showEvent(QShowEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    Ui::VideoUI *ui;

    QMediaPlayer *player;
    QGraphicsScene *scene;
    QGraphicsVideoItem *videoItem;
    QAudioOutput *audio;

    QTimer *pct;
    bool lastState;

    QUrl source;
    qreal pc;
    qint64 duration;
    QString durationstr;
    double frameRate;
    qint64 msPerFrame;

    ConfigCM config;
};
#endif // VIDEOUI_H
