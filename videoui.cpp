#include "videoui.h"
#include "./ui_videoui.h"

#include <QShowEvent>
#include <QCloseEvent>
#include <QMediaMetaData>

VideoUI::VideoUI(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::VideoUI)
    , player(new QMediaPlayer(this)), scene(new QGraphicsScene(this)), videoItem(new QGraphicsVideoItem)
    , audio(new QAudioOutput(this))
    , pct(new QTimer(this))
{
    ui->setupUi(this);

    // Initialize
    pct->setInterval(1000);

    ui->volumnBar->hide();

    player->setPlaybackRate(config.PlaybackRate());
    audio->setVolume(config.fVolumn());

    // UI Initialize
    player->setVideoOutput(videoItem);
    player->setAudioOutput(audio);
    scene->addItem(videoItem);
    ui->graphicsView->setScene(scene);

    videoItem->setSize(ui->graphicsView->size());

    // UI Properties

    // UI Slots

    // UI Signals
    connect(ui->progressBar, &QSlider::sliderPressed, this, [&]{ lastState = isPlaying(); Pause(); pct->start(); });
    connect(ui->progressBar, &QSlider::sliderReleased, this, [&]{ pct->stop(); if(lastState) Play(); });
    connect(pct, &QTimer::timeout, this, [&]{ player->setPosition(ui->progressBar->value() * pc); });

    connect(ui->buttonPlay, &QPushButton::clicked, this, &VideoUI::PausePlay);
    connect(ui->buttonPreviousFrame, &QPushButton::clicked, this, &VideoUI::PreviousFrame);
    connect(ui->buttonNextFrame, &QPushButton::clicked, this, &VideoUI::NextFrame);
    connect(ui->buttonQuit, &QPushButton::clicked, this, &VideoUI::close);

    connect(ui->buttonVolumn, &QPushButton::toggled, this, [&](bool b){ ui->volumnBar->setVisible(b); });
    connect(ui->volumnBar, &QSlider::sliderMoved, this, [&](int pos) { config.setVolumn(pos); audio->setVolume(pos /100.0); });

    // MediaPlayer
    connect(player, &QMediaPlayer::positionChanged, this, &VideoUI::showProgress);
    connect(player, &QMediaPlayer::durationChanged, this, &VideoUI::updateDuraion);

    connect(player, &QMediaPlayer::mediaStatusChanged, this, [&](QMediaPlayer::MediaStatus status) {
        if(status == QMediaPlayer::LoadedMedia && this->isVisible() && config.AutoStart())
            Play();
        else
            Pause();
    });

    connect(player, &QMediaPlayer::metaDataChanged, this, [this](){
        QVariant frameRateVar = player->metaData().value(QMediaMetaData::VideoFrameRate);
        if (frameRateVar.isValid()) {
            frameRate = frameRateVar.toDouble();
        } else {
            frameRate = 30.0;
        }
        msPerFrame = static_cast<qint64>(1000.0 / frameRate);

        qDebug() << frameRate << msPerFrame;
    });


    QTimer::singleShot(0, this, [&]{
        this->resizeEvent(nullptr);
    });
}

VideoUI::~VideoUI()
{
    delete ui;
}

void VideoUI::showProgress(qint64 position)
{
    ui->progressBar->setValue(position/pc);
    static bool displayms = config.DisplayMS();
    ui->progressLabel->setText(formatDuration(position, displayms) + "/" + durationstr);
}

bool VideoUI::isPlaying() { return (player->playbackState() == QMediaPlayer::PlayingState); }
bool VideoUI::isPaused() { return (player->playbackState() == QMediaPlayer::PausedState); }

void VideoUI::loadVideo(const QUrl &source)
{
    this->source = source;
    player->setSource(this->source);
}

void VideoUI::Play()
{
    player->play();
}

void VideoUI::Pause()
{
    player->pause();
}

void VideoUI::PausePlay()
{
    if(player->playbackState() == QMediaPlayer::PlayingState)
        Pause();
    else
        Play();
}

void VideoUI::PreviousFrame()
{
    if(!isPaused()) Pause();

    qint64 position = player->position();
    qint64 targetPos = position - msPerFrame;
    if(targetPos >= 0)
        player->setPosition(targetPos);
}

void VideoUI::NextFrame()
{
    if(!isPaused()) Pause();

    qint64 position = player->position();
    qint64 targetPos = position + msPerFrame;
    if(targetPos <= duration)
        player->setPosition(targetPos);
}

QString VideoUI::formatDuration(qint64 duration, bool ms)
{
    const qint64 hours = duration / 3600000;
    qint64 remaining = duration % 3600000;
    const qint64 minutes = remaining / 60000;
    remaining %= 60000;
    const qint64 seconds = remaining / 1000;
    const qint64 milliseconds = remaining % 1000;

    constexpr QLatin1Char fill = QLatin1Char('0');

    QString result;
    if(hours > 0) result = QString("%1:").arg(hours, 2, 10, fill);
    result += QString("%1:%2").arg(minutes, 2, 10, fill).arg(seconds, 2, 10, fill);
    if(ms) result += QString(".%1").arg(milliseconds, 3, 10, fill);

    return result;
}

void VideoUI::updateDuraion(int duration)
{
    this->duration = duration;
    durationstr = formatDuration(duration, config.DisplayMS());
    pc = duration / 10000.0;
}

void VideoUI::showEvent(QShowEvent *event)
{
    if(player->mediaStatus() == QMediaPlayer::LoadedMedia && config.AutoStart()) Play();
    QMainWindow::showEvent(event);
}

void VideoUI::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);

    QSize size = this->size();
    QSize panelSize = ui->panel->size();

    ui->graphicsView->setGeometry(QRect(QPoint(0,0), this->size()));
    ui->panel->move(size.width() / 2 - (panelSize.width() / 2), size.height() - panelSize.height() - 15 );

    if(videoItem && scene) {
        videoItem->setSize(ui->graphicsView->size());
        scene->setSceneRect(0, 0, ui->graphicsView->width(), ui->graphicsView->height());
        ui->graphicsView->viewport()->update();
    }
}
