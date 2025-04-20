#include "configcm.h"

#include <QApplication>

ConfigCM::ConfigCM(QObject *parent)
    : QObject{parent}
    , settings(QApplication::applicationDirPath() + "/MediaCM.ini", QSettings::IniFormat)
{}

void ConfigCM::writeDefault()
{
    // UI
    setDisplayMS(DisplayMS());
    setPanelShowTimer(PanelShowTimer());

    // Player
    setAutoStart(AutoStart());
    setVolume(Volume());
    setMuted(Muted());
    setPlaybackRate(PlaybackRate());
}
