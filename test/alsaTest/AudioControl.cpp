#include "AudioControl.h"
#include "AudioCapture.h"
#include "AudioPlayback.h"
#include "G729Encoder.h"
#include "G729Decoder.h"
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
    m_mixerData.clear();
}

AudioControl::~AudioControl()
{

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
    G729Encoder::getInstance()->start();
    G729Decoder::getInstance()->start();

    return true;
}

void AudioControl::addToCaptureDataList(const char *data, const unsigned int len)
{
     QMutexLocker locker(&m_captureDataMutex);
     if (len > 0)
     {
         pAudioPeriodData tmp(new AudioPeriodData);
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
        static pAudioPeriodData tmp;
        tmp = m_captureDataList.front();
        m_captureDataList.pop_front();
        len = tmp->dataLen;
        memcpy(data, tmp->data, len);
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
        pAudioPeriodData tmp(new AudioPeriodData);
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
        static pAudioPeriodData tmp;
        tmp = m_playbackDataList.front();
        m_playbackDataList.pop_front();
        len = tmp->dataLen;
        memcpy(data, tmp->data, len);
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
        static char tmp[DATAMAXSIZE];
        static int len;
        memset(&tmp, 0, DATAMAXSIZE);
        popFromCaptureDataList(tmp, len);
        if (len > 0)
        {
            //addToPlaybackDataList(tmp, len);
            addToMixerData(1, tmp, len);
            //addToMixerData(2, tmp, len);
            //addToMixerData(3, tmp, len);
            G729Encoder::getInstance()->addEncoderData(tmp, len);
        }
    }
}

void AudioControl::dealPlaybackData()
{
    AudioPeriodDataList data;
    data.clear();
    {
        QMutexLocker locker(&m_mixerDataMutex);
        for(SoundMixerMap::iterator iter = m_mixerData.begin(); iter != m_mixerData.end(); ++iter)
        {
            pAudioPeriodDataList tmpList = iter.value();
            if (!tmpList->isEmpty())
            {
                pAudioPeriodData tmpData = tmpList->front();
                tmpList->pop_front();
                data.append(tmpData);
            }
        }
    }
    if (data.count() > 0)
    {
        //归一化混音
        static const int MAX=32767;
        static const int MIN=-32768;

        double f = 1;
        qint32 output;
        AudioPeriodData mixerData;
        memset(&mixerData, 0, sizeof(mixerData));
        int x = 2 * m_audioCapture->getFrameSize();
        for (int i = 0; i < x; ++i)
        {
           qint32 temp = 0;
           for (int j = 0; j < data.size(); ++j)
           {
               temp += *(qint16*)(&(data[j]->data[i*2]));
           }
           output = (qint32)(temp*f);
           if (output > MAX)
           {
              f = (double)MAX/(double)(output);
              output=MAX;
           }
           if (output < MIN)
           {
              f = (double)MIN/(double)(output);
              output = MIN;
           }
           if (f < 1)
           {
              f += ((double)1-f)/(double)32;
           }
           *(qint16*)(&(mixerData.data[i*2])) = (qint16)output;
           mixerData.dataLen += 2;
        }
        addToPlaybackDataList(mixerData.data, mixerData.dataLen);
    }
}

void AudioControl::addToMixerData(const uint8_t id, const char *data, const unsigned int len)
{
    QMutexLocker locker(&m_mixerDataMutex);
    SoundMixerMap::const_iterator iter = m_mixerData.find(id);
    if (iter == m_mixerData.constEnd())
    {
        pAudioPeriodDataList tmp(new AudioPeriodDataList);
        m_mixerData.insert(id, tmp);
    }
    pAudioPeriodDataList list = m_mixerData.value(id);
    if (len > 0)
    {
        pAudioPeriodData d(new AudioPeriodData);
        memcpy(d->data, data, len);
        d->dataLen = len;
        list->append(d);
    }
}

void AudioControl::up()
{
    m_soundMixer->up();
}

void AudioControl::down()
{
    m_soundMixer->down();
}
