#pragma once

#include <QWidget>
#include "themedbutton.h"

class QSlider;
class QAudioOutput;

class VolumeButton : public ThemedButton
{
    Q_OBJECT

public:
    enum State {Normal, SliderShown, Muted};
    Q_ENUM(State)

    VolumeButton(QWidget* parent = nullptr);

    inline State state() { return currentState; }
    inline int volume() { return vol; }
    inline float fVolume() { return vol /100.0; }
    inline bool muted() { return (currentState == Muted); }

public slots:
    void setState(State s);
    void bindSlider(QSlider* slider);
    void bindAudioOutput(QAudioOutput* audio);
    void NextState();

    void setVolume(int vol);
    void setMuted(bool isMuted);
    void updateVolumeIcon(int vol);

private slots:
    void updateVolume(int pos);

signals:
    void MuteChanged(bool isMuted);
    void VolumeChanged(int vol);
    void fVolumeChanged(float fvol);

private:
    State currentState;
    QSlider* slider = nullptr;
    QAudioOutput* audio = nullptr;
    int vol;
    QString lastIcon;
};
