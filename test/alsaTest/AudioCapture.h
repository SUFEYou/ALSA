#ifndef AUDIOCAPTURE_H
#define AUDIOCAPTURE_H

#include <QThread>
#include "alsa/asoundlib.h"

class AudioCapture: public QThread
{
    Q_OBJECT
public:
    AudioCapture();
    bool audioCaptureInit(QString& status);

protected:
    virtual void run();

private:
    bool                    m_stop;
    snd_pcm_t               *m_handle;//pcm句柄
    snd_pcm_hw_params_t     *m_params;//pcm属性
    unsigned int            m_frameSize;
};

#endif // AUDIOCAPTURE_H
