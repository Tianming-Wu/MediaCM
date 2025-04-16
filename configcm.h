#ifndef CONFIGCM_H
#define CONFIGCM_H

#include <QObject>
#include <QSettings>

class ConfigCM : public QObject
{
    Q_OBJECT
public:
    explicit ConfigCM(QObject *parent = nullptr);
    QSettings settings;

    void writeDefault();

    inline bool DisplayMS() { return settings.value("UI/DisplayMS", false).toBool(); }

    inline bool AutoStart() { return settings.value("Player/AutoStart", true).toBool(); }
    inline int Volumn() { return settings.value("Player/Volumn", 100).toInt(); }
    inline float fVolumn() { return settings.value("Player/Volumn", 100).toInt() / 100.0; }
    inline qreal PlaybackRate() { return settings.value("Player/PlaybackRate").toReal(); }

public slots:
    inline void setDisplayMS(bool on) { settings.setValue("UI/DisplayMS", on); }

    inline void setAutoStart(bool on) { settings.setValue("Player/AutoStart", on); }
    inline void setVolumn(int vol) { settings.setValue("Player/Volumn", vol); }

signals:


};

#endif // CONFIGCM_H
