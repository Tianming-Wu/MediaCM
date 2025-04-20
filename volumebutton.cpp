#include "volumebutton.h"

#include <QSlider>
#include <QAudioOutput>

VolumeButton::VolumeButton(QWidget *parent)
    : ThemedButton(parent), currentState(Normal), vol(100)
{
    connect(this, &QPushButton::clicked, this, &VolumeButton::NextState);
    connect(this, &VolumeButton::VolumeChanged, this, &VolumeButton::updateVolumeIcon);
}

void VolumeButton::setState(State s)
{
    currentState = s;
    switch(currentState) {
    case Normal: {
        slider->setVisible(false);
        audio->setMuted(false);
        emit MuteChanged(false);
        break;
    }
    case SliderShown: {
        slider->setVisible(true);
        audio->setMuted(false);
        emit MuteChanged(false);
        break;
    }
    case Muted: {
        slider->setVisible(false);
        audio->setMuted(true);
        emit MuteChanged(true);
        break;
    }
    default:
        setState(Normal);
    }
    updateVolumeIcon(vol);
}

void VolumeButton::bindSlider(QSlider *slider)
{
    if(this->slider) {
        disconnect(this->slider, &QSlider::sliderMoved, this, &VolumeButton::updateVolume);
    }

    this->slider = slider;

    connect(slider, &QSlider::sliderMoved, this, &VolumeButton::updateVolume);

    slider->setValue(vol);

    if(currentState == State::SliderShown) this->slider->setVisible(true);
    else this->slider->setVisible(false);
}

void VolumeButton::bindAudioOutput(QAudioOutput *audio)
{
    if(this->audio) {
        disconnect(this, &VolumeButton::fVolumeChanged, this->audio, &QAudioOutput::setVolume);
        disconnect(this, &VolumeButton::MuteChanged, this->audio, &QAudioOutput::setMuted);
    }

    this->audio = audio;

    connect(this, &VolumeButton::fVolumeChanged, audio, &QAudioOutput::setVolume);
    connect(this, &VolumeButton::MuteChanged, audio, &QAudioOutput::setMuted);

    audio->setVolume(fVolume());
    audio->setMuted(muted());
}

void VolumeButton::NextState()
{
    setState(static_cast<State>(currentState+1));
}

void VolumeButton::setVolume(int vol)
{
    this->vol = vol;
    slider->setValue(vol);
    audio->setVolume(vol /100.0);
    updateVolumeIcon(vol);
}

void VolumeButton::setMuted(bool isMuted)
{
    isMuted?
        setState(Muted):
        setState(Normal);
    audio->setMuted(isMuted);
    updateVolumeIcon(vol);
}

void VolumeButton::updateVolumeIcon(int vol)
{
    this->vol = vol;
    if( (vol < 0) || (vol > 100) )
        throw std::runtime_error("Invalid volume value");
    QString res = ":/ui/vol/";
    if(currentState == Muted) res += "nosound";
    else if(vol == 0) res += "muted";
    else if(vol <= 33) res += "lowvolume";
    else if(vol <= 66) res += "mediumvolume";
    else res += "highvolume";

    res += ".png";

    if(res != lastIcon) {
        ThemedButton::setIcon(QIcon(res));
        lastIcon = res;
    }
}

void VolumeButton::updateVolume(int pos)
{
    this->vol = pos;
    float fvol = pos /100.0;
    audio->setVolume(fvol);
    updateVolumeIcon(pos);

    emit VolumeChanged(pos);
    emit fVolumeChanged(fvol);
}
