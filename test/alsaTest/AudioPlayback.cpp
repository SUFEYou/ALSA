#include "AudioPlayback.h"
#include <QDebug>

AudioPlayback::AudioPlayback()
              : m_stop(false)
              , m_handle(NULL)
              , m_params(NULL)
              , m_frameSize(80)
{

}

void AudioPlayback::audioPlaybackConnect()
{
    connect(AudioControl::getInstance(), SIGNAL(sendMixerData(const char*,uint)), this, SLOT(addPlaybackData(const char*,uint)));
}

void AudioPlayback::run()
{
    static char a[4096];
    static int rc = 0;
    while (!m_stop)
    {
        //memset(a, 0, sizeof(a));
        AudioControl::getInstance()->popFromPlaybackDataList(a, rc);
        //popFromPlaybackDataList(a, rc);
        if (rc <= 0)
        {
            //qDebug() << "Do Not Have Enough Data To Play!";
            continue;
        }
        else if ((rc%m_frameSize) != 0)
        {
            qDebug() << "Play Frame Size Err!";
            continue;
        }
        rc = snd_pcm_writei(m_handle, a, m_frameSize);
        if (rc == -EPIPE)
        {
              /* EPIPE means underrun */
              //qDebug() << "underrun occurred";
              snd_pcm_prepare(m_handle);
        }
        else if (rc < 0)
        {
            qDebug() << "error from writei: " << snd_strerror(rc);
        }
        else if (rc != (int)m_frameSize)
        {
            qDebug() << "short write, write " << rc << "frames";
        }
        //qDebug() << "write " << rc << "frames";
    }
    m_stop = false;
}

bool AudioPlayback::audioPlaybackInit(QString& status)
{
    //打开设备
    int r = snd_pcm_open(&m_handle, "default", SND_PCM_STREAM_PLAYBACK, 0);
    if(r < 0)
    {
        status = QString("snd pcm open fail");
        return false;
    }
    //设置参数
    //初始化pcm属性
    snd_pcm_hw_params_alloca(&m_params);
    snd_pcm_hw_params_any(m_handle, m_params);
    //交错模式
    snd_pcm_hw_params_set_access(m_handle, m_params, SND_PCM_ACCESS_RW_INTERLEAVED);
    //设置双声道，小端格式，16位
    snd_pcm_hw_params_set_format(m_handle, m_params, SND_PCM_FORMAT_S16_LE);
    //snd_pcm_hw_params_set_channels(m_handle, m_params, 2);
    snd_pcm_hw_params_set_channels(m_handle, m_params, 1);
    //snd_pcm_hw_params_set_format(m_handle, m_params, SND_PCM_FORMAT_S8);
    //snd_pcm_hw_params_set_channels(m_handle, m_params, 1);
    //设置采样率（44100标准MP3采样频率）
    //unsigned int val = 44100;
    unsigned int val = 8000;
    //snd_pcm_hw_params_set_rate_near(m_handle, m_params, &val, 0);
    snd_pcm_hw_params_set_rate(m_handle, m_params, val, 0);
    //设在采样周期,（最好是让系统自动设置，这一步可以省略）
    //snd_pcm_hw_params_set_period_size_near(m_handle, m_params, (snd_pcm_uframes_t*)&m_frameSize, 0);
    snd_pcm_hw_params_set_period_size(m_handle, m_params, m_frameSize, 0);
    //设置好的参数回写设备
    r = snd_pcm_hw_params(m_handle, m_params);
    if(r < 0)
    {
        status = QString("snd pcm open fail");
        return false;
    }
    //16--2--（一帧数据4个字节）
    //获取一个周期有多少帧数据，一个周期一个周期方式处理音频数据。
    //snd_pcm_hw_params_get_period_size(m_params, (snd_pcm_uframes_t*)&m_frameSize,0);
    //unsigned char *buffer = malloc(4*frames);//由于双通道，16bit，每个通道2个字节，一个周期所需要的空间为4个字节*帧数
    status = QString("snd pcm open succeed");
    return true;
}

void AudioPlayback::addPlaybackData(const char *data, const unsigned int len)
{
    //QMutexLocker locker(&m_playbackDataMutex);
    if (len > 0)
    {
        pAudioPeriodData tmp(new AudioPeriodData);
        memcpy(tmp->data, data, len);
        tmp->dataLen = len;
        if (m_playbackDataList.length() > 100)
        {
            AudioPeriodDataList::iterator iter = m_playbackDataList.begin();
            m_playbackDataList.erase(iter, iter+20);
        }
        m_playbackDataList.append(tmp);
    }
}

void AudioPlayback::popFromPlaybackDataList(char *data, int &len)
{
    //QMutexLocker locker(&m_playbackDataMutex);
    qDebug() << "########################PlaybackDataList length = " << m_playbackDataList.length();
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
