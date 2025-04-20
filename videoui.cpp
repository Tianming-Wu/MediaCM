#include "videoui.h"
#include "./ui_videoui.h"

#include <QShowEvent>
#include <QCloseEvent>
#include <QMouseEvent>
#include <QMediaMetaData>
#include <QAudioDevice>

VideoUI::VideoUI(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::VideoUI), theme(this)
    , player(new QMediaPlayer(this)), scene(new QGraphicsScene(this)), videoItem(new QGraphicsVideoItem)
    , audio(new QAudioOutput(this))
    , pct(new QTimer(this)), panelShowTimer(new QTimer(this))
{
    ui->setupUi(this);

    // Initialize
    pct->setInterval(1000);

    panelShowTimer->setInterval(config.PanelShowTimer());
    panelShowTimer->setSingleShot(true);

    panelSize = ui->panel->size();

    player->setPlaybackRate(config.PlaybackRate());

    ui->buttonVolume->bindSlider(ui->volumeBar);
    ui->buttonVolume->bindAudioOutput(audio);
    ui->buttonVolume->setVolume(config.Volume());
    ui->buttonVolume->setMuted(config.Muted());

    pa_show = new QPropertyAnimation(ui->panel, "pos", this);
    pa_show->setDuration(200);
    pa_show->setEasingCurve(QEasingCurve::OutCubic);

    // UI Initialize
    player->setVideoOutput(videoItem);
    player->setAudioOutput(audio);
    scene->addItem(videoItem);
    ui->graphicsView->setScene(scene);

    videoItem->setSize(ui->graphicsView->size());

    // UI Properties
    ui->graphicsView->setAttribute(Qt::WA_TransparentForMouseEvents);
    this->setStyleSheet(theme.generate());
    connect(&theme, &ThemeCM::ThemeChanged, this, [&]{ this->setStyleSheet(theme.generate()); });
    // theme.generate();

    // UI Slots
    connect(panelShowTimer, &QTimer::timeout, this, [&]{ showPanel(false); });

    // UI Signals
    connect(ui->progressBar, &QSlider::sliderPressed, this, [&]{ lastState = isPlaying(); Pause(); pct->start(); });
    connect(ui->progressBar, &QSlider::sliderReleased, this, [&]{ pct->stop(); if(lastState) Play(); });
    connect(pct, &QTimer::timeout, this, [&]{ player->setPosition(ui->progressBar->value() * pc); });

    connect(ui->buttonPlay, &QPushButton::clicked, this, &VideoUI::PausePlay);
    connect(ui->buttonPreviousFrame, &QPushButton::clicked, this, &VideoUI::PreviousFrame);
    connect(ui->buttonNextFrame, &QPushButton::clicked, this, &VideoUI::NextFrame);
    connect(ui->buttonQuit, &QPushButton::clicked, this, &VideoUI::close);

    // Volume Control
    connect(ui->buttonVolume, &VolumeButton::VolumeChanged, &config, &ConfigCM::setVolume);
    connect(ui->buttonVolume, &VolumeButton::MuteChanged, &config, &ConfigCM::setMuted);

    // Control Panel Show / Hide
    connect(ui->progressBar, &QSlider::sliderPressed, panelShowTimer, &QTimer::stop);
    connect(ui->progressBar, &QSlider::sliderReleased, this, &VideoUI::tryStartPanelHideTimer);

    connect(ui->buttonPlay, &QPushButton::clicked, this, &VideoUI::tryStartPanelHideTimer);
    connect(ui->buttonPreviousFrame, &QPushButton::clicked, this, &VideoUI::tryStartPanelHideTimer);
    connect(ui->buttonNextFrame, &QPushButton::clicked, this, &VideoUI::tryStartPanelHideTimer);

    connect(ui->buttonVolume, &QPushButton::toggled, this, &VideoUI::tryStartPanelHideTimer);
    connect(ui->volumeBar, &QSlider::sliderPressed, panelShowTimer, &QTimer::stop);
    connect(ui->volumeBar, &QSlider::sliderReleased, this, &VideoUI::tryStartPanelHideTimer);

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
    });


    QTimer::singleShot(0, this, [&]{
        this->resizeEvent(nullptr);
        // panelShowTimer->start();
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

void VideoUI::tryStartPanelHideTimer() { if(isPlaying()) panelShowTimer->start(); }

bool VideoUI::isPlaying() { return (player->playbackState() == QMediaPlayer::PlayingState); }
bool VideoUI::isPaused() { return (player->playbackState() == QMediaPlayer::PausedState); }

bool VideoUI::isPanelVisible() { return (panelState == VUI::NormalState); }

void VideoUI::loadVideo(const QUrl &source)
{
    this->source = source;
    player->setSource(this->source);
}

void VideoUI::Play()
{
    player->play();
    ui->buttonPlay->setIcon(QIcon(":/ui/pause.png"));
    if(isPanelVisible()) panelShowTimer->start();
}

void VideoUI::Pause()
{
    player->pause();
    ui->buttonPlay->setIcon(QIcon(":/ui/play.png"));
    if(isPanelVisible()) panelShowTimer->stop();
}

void VideoUI::PausePlay()
{
    if(isPlaying())
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

void VideoUI::showPanel(bool v)
{
    if(isPanelVisible() == v) return;
    if(v) {
        pa_show->setDirection(QPropertyAnimation::Forward);
        panelState = VUI::NormalState;
    } else {
        pa_show->setDirection(QPropertyAnimation::Backward);
        panelState = VUI::HiddenState;
    }
    pa_show->start();
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

    ui->graphicsView->setGeometry(QRect(QPoint(0,0), this->size()));
    panelPos = QPoint(size.width() / 2 - (panelSize.width() / 2), size.height() - panelSize.height() - 15);
    panelHiddenPos = QPoint(panelPos.x(), size.height() + 5);
    pa_show->setStartValue(panelHiddenPos); pa_show->setEndValue(panelPos);
    ui->panel->move(isPanelVisible()?panelPos:panelHiddenPos);

    if(videoItem && scene) {
        videoItem->setSize(ui->graphicsView->size());
        scene->setSceneRect(0, 0, ui->graphicsView->width(), ui->graphicsView->height());
        ui->graphicsView->viewport()->update();
    }
}

void VideoUI::mousePressEvent(QMouseEvent *event)
{
    if(!dbgclickmark) {
        if(panelState == VUI::HiddenState) {
            showPanel(true);
            tryStartPanelHideTimer();
        } else {
            showPanel(false);
            panelShowTimer->stop();
        }
    } else dbgclickmark = false;
    QMainWindow::mousePressEvent(event);
}

void VideoUI::mouseReleaseEvent(QMouseEvent *event)
{
    QMainWindow::mouseReleaseEvent(event);
}

void VideoUI::mouseDoubleClickEvent(QMouseEvent *event)
{
    dbgclickmark = true;
    QMainWindow::mouseDoubleClickEvent(event);
    PausePlay();
}
