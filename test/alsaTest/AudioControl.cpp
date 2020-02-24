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
        //addToPlaybackDataList(tmp->data, tmp->dataLen);
        addToMixerData(1, tmp->data, tmp->dataLen);
        addToMixerData(2, tmp->data, tmp->dataLen);
        addToMixerData(3, tmp->data, tmp->dataLen);
        delete tmp;
        tmp = NULL;
    }
}

void AudioControl::dealPlaybackData()
{
    QList<AudioPeriodData*> data;
    data.clear();
    for(QMap<uint8_t,QList<AudioPeriodData*>*>::iterator iter = m_mixerData.begin(); iter != m_mixerData.end(); ++iter)
    {
        QList<AudioPeriodData*>* tmpList = iter.value();
        if (!tmpList->isEmpty())
        {
            AudioPeriodData* tmpData = tmpList->front();
            tmpList->pop_front();
            data.append(tmpData);
        }
        else
        {
            m_mixerData.erase(iter);
            delete tmpList;
            tmpList = NULL;
        }
    }
    if (data.count() > 0)
    {
        //归一化混音
        int const MAX=32767;
        int const MIN=-32768;

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

    for (int i = 0; i < data.size(); ++i)
    {
        AudioPeriodData* tmp = data.front();
        data.pop_front();
        delete tmp;
        tmp = NULL;
    }
}

void AudioControl::addToMixerData(const uint8_t id, const char *data, const unsigned int len)
{
    QMap<uint8_t,QList<AudioPeriodData*>*>::const_iterator iter = m_mixerData.find(id);
    if (iter == m_mixerData.constEnd())
    {
        QList<AudioPeriodData*>* tmp = new QList<AudioPeriodData*>;
        m_mixerData.insert(id, tmp);
    }
    QList<AudioPeriodData*>* list = m_mixerData.value(id);
    if (len > 0)
    {
        AudioPeriodData* d = new AudioPeriodData;
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
