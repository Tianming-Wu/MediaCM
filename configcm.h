#ifndef CONFIGCM_H
#define CONFIGCM_H

#include <QObject>
#include <QSettings>

class ConfigCM : public QObject
{
    Q_OBJECT

    Q_DISABLE_COPY(ConfigCM)

    Q_PROPERTY(bool DisplayMS READ DisplayMS WRITE setDisplayMS STORED false)
    Q_PROPERTY(int PanelShowTimer READ PanelShowTimer WRITE setPanelShowTimer STORED false)
    Q_PROPERTY(bool TouchSupport READ TouchSupport WRITE setTouchSupport STORED false)
    Q_PROPERTY(bool AutoStart READ AutoStart WRITE setAutoStart STORED false)
    Q_PROPERTY(bool Muted READ Muted WRITE setMuted STORED false)
    Q_PROPERTY(int Volume READ Volume WRITE setVolume STORED false)
    Q_PROPERTY(float fVolume READ fVolume STORED false)
    Q_PROPERTY(qreal PlaybackRate READ PlaybackRate WRITE setPlaybackRate STORED false)

public:
    explicit ConfigCM(QObject *parent = nullptr);
    QSettings settings;

    void writeDefault();

    inline bool DisplayMS() { return settings.value("UI/DisplayMS", false).toBool(); }
    inline int PanelShowTimer() { return settings.value("UI/PanelShowTimer", 3000).toInt(); }
    inline bool TouchSupport() { return settings.value("UI/TouchSupport", false).toBool(); }

    inline bool AutoStart() { return settings.value("Player/AutoStart", true).toBool(); }
    inline bool Muted() { return settings.value("Player/Muted", false).toBool(); }
    inline int Volume() { return settings.value("Player/Volume", 100).toInt(); }
    inline float fVolume() { return settings.value("Player/Volume", 100).toInt() / 100.0; }
    inline qreal PlaybackRate() { return settings.value("Player/PlaybackRate").toReal(); }

public slots:
    inline void setDisplayMS(bool on) { settings.setValue("UI/DisplayMS", on); }
    inline void setPanelShowTimer(int t) { settings.setValue("UI/PanelShowTimer", t); }
    inline void setTouchSupport(bool on) { settings.setValue("UI/TouchSupport", on); }

    inline void setAutoStart(bool on) { settings.setValue("Player/AutoStart", on); }
    inline void setVolume(int vol) { settings.setValue("Player/Volume", vol); }
    inline void setMuted(bool on) { settings.setValue("Player/Muted", on); }
    inline void setPlaybackRate(qreal rate) { settings.setValue("Player/PlaybackRate", rate); }

signals:
    // I didn't make this because settings ui doesn't exist lol

};

#endif // CONFIGCM_H
