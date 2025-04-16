#include "configcm.h"

#include <QApplication>

ConfigCM::ConfigCM(QObject *parent)
    : QObject{parent}
    , settings(QApplication::applicationDirPath() + "/MediaCM.ini", QSettings::IniFormat)
{}

void ConfigCM::writeDefault()
{
    setAutoStart(AutoStart());

    setDisplayMS(DisplayMS());
}
