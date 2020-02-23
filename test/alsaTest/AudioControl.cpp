#include "AudioControl.h"
#include "AudioCapture.h"
#include "AudioPlayback.h"
#include <QMutexLocker>
#include <QDebug>

#include <SoundMixer.h>


AudioControl* AudioControl::m_instance = NULL;
QMutex AudioControl::m_getMutex;

AudioControl::AudioControl()
             : m_stop(false)
             , m_audioCapture(new AudioCapture)
             , m_audioPlayback(new AudioPlayback)
             , m_soundMixer(new SoundMixer)
{
    m_captureDataList.clear();
    m_playbackDataList.clear();
}

AudioControl* AudioControl::getInstance()
{
    if (m_instance == NULL)
    {
        QMutexLocker locker(&m_getMutex);
        if (m_instance == NULL)
        {
            m_instance = new AudioControl;
        }
    }

    return m_instance;
}

void AudioControl::run()
{
    while(!m_stop)
    {
        dealCaptureData();
        dealPlaybackData();
    }
    m_stop = false;
}

bool AudioControl::audioControlInit(QString& status)
{
    if (!m_audioCapture->audioCaptureInit(status))
    {
        qDebug() << "Audio Capture Init err";
        return false;
    }
    if (!m_audioPlayback->audioPlaybackInit(status))
    {
        qDebug() << "Audio Playback Init err";
        return false;
    }
    if (!m_soundMixer->soundMixerInit(status))
    {
        qDebug() << "Sound Mixer Init err";
        return false;
    }

    m_audioCapture->start();
    m_audioPlayback->start();

    return true;
}

void AudioControl::addToCaptureDataList(const char *data, const unsigned int len)
{
     QMutexLocker locker(&m_captureDataMutex);
     if (len > 0)
     {
         AudioPeriodData* tmp = new AudioPeriodData;
         memcpy(tmp->data, data, len);
         tmp->dataLen = len;
         m_captureDataList.append(tmp);
     }

}

void AudioControl::popFromCaptureDataList(char *data, int &len)
{
    QMutexLocker locker(&m_captureDataMutex);
    if (!m_captureDataList.empty())
    {
        AudioPeriodData* tmp = m_captureDataList.front();
        m_captureDataList.pop_front();
        len = tmp->dataLen;
        memcpy(data, tmp->data, len);
        delete tmp;
        tmp = NULL;

    }
    else
    {
        len = -1;
    }

}

void AudioControl::addToPlaybackDataList(const char *data, const unsigned int len)
{
    QMutexLocker locker(&m_playbackDataMutex);
    if (len > 0)
    {
        AudioPeriodData* tmp = new AudioPeriodData;
        memcpy(tmp->data, data, len);
        tmp->dataLen = len;
        m_playbackDataList.append(tmp);
    }
}

void AudioControl::popFromPlaybackDataList(char *data, int &len)
{
    QMutexLocker locker(&m_playbackDataMutex);
    if (!m_playbackDataList.empty())
    {
        AudioPeriodData* tmp = m_playbackDataList.front();
        m_playbackDataList.pop_front();
        len = tmp->dataLen;
        memcpy(data, tmp->data, len);
        delete tmp;
        tmp = NULL;

    }
    else
    {
        len = -1;
    }
}

void AudioControl::dealCaptureData()
{
    if (!m_captureDataList.empty())
    {
        AudioPeriodData* tmp = m_captureDataList.front();
        m_captureDataList.pop_front();
        char a[4096];
        unsigned int len = 0;
        len = tmp->dataLen;
        memcpy(a, tmp->data, len);
        addToPlaybackDataList(a, len);
        delete tmp;
        tmp = NULL;
    }
}

void AudioControl::dealPlaybackData()
{

}

void AudioControl::up()
{
    m_soundMixer->up();
}
void AudioControl::down()
{
    m_soundMixer->down();
}
