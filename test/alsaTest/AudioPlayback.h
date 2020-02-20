#ifndef AUDIOPLAYBACK_H
#define AUDIOPLAYBACK_H

#include <QThread>
#include "alsa/asoundlib.h"


class AudioPlayback : public QThread
{
    Q_OBJECT
public:
    AudioPlayback();
    bool audioPlaybackInit(QString& status);

protected:
    virtual void run();

private:
    bool                    m_stop;
    snd_pcm_t               *m_handle;//pcm句柄
    snd_pcm_hw_params_t     *m_params;//pcm属性
    unsigned int            m_frameSize;

};

#endif // AUDIOPLAYBACK_H
