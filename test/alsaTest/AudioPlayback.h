#ifndef AUDIOPLAYBACK_H
#define AUDIOPLAYBACK_H

#include <QThread>
#include "alsa/asoundlib.h"
#include "AudioControl.h"


class AudioPlayback : public QThread
{
    Q_OBJECT
public:
    AudioPlayback();
    bool audioPlaybackInit(QString& status);
    void audioPlaybackConnect();

protected:
    virtual void run();

private:
    void popFromPlaybackDataList(uint8_t *data, int &len);

private slots:
    void addPlaybackData(const uint8_t *data, const unsigned int len);

private:
    bool                    m_stop;
    snd_pcm_t               *m_handle;//pcm句柄
    snd_pcm_hw_params_t     *m_params;//pcm属性
    unsigned int            m_frameSize;

    AudioPeriodDataList     m_playbackDataList;
    QMutex                  m_playbackDataMutex;

};

#endif // AUDIOPLAYBACK_H
