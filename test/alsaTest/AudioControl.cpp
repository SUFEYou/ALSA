#include "AudioControl.h"
#include "AudioCapture.h"
#include "AudioPlayback.h"
#include <QMutexLocker>
#include <QTimer>
#include <QDebug>


static AudioControl* AudioControl::m_instance = NULL;
static QMutex AudioControl::m_getMutex;

AudioControl::AudioControl(QObject *parent)
             : QObject(parent)
             , m_audioCapture(new AudioCapture)
             , m_audioPlayback(new AudioPlayback)
             , m_testTimer(new QTimer(this))
{
    m_audioDataList.clear();
    connect(m_testTimer, SIGNAL(timeout()), this, SLOT(test()));
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

bool AudioControl::audioControlInit(QString& status)
{
    if (!m_audioCapture->audioCaptureInit(status))
        return false;
    if (!m_audioPlayback->audioPlaybackInit(status))
        return false;

    m_audioCapture->start();
    m_audioPlayback->start();
    return true;
}

void AudioControl::test()
{

}

void AudioControl::addDataToList(const char *data, const unsigned int len)
{
     QMutexLocker locker(&m_dataMutex);
     if (len > 0)
     {
         AudioPeriodData* tmp = new AudioPeriodData;
         memcpy(tmp->data, data, len);
         tmp->dataLen = len;
         m_audioDataList.append(tmp);
     }

}

void AudioControl::popDataFromList(char *data, int &len)
{
    QMutexLocker locker(&m_dataMutex);
    if (!m_audioDataList.empty())
    {
        AudioPeriodData* tmp = m_audioDataList.front();
        m_audioDataList.pop_front();
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
