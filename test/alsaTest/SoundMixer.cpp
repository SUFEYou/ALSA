#include "SoundMixer.h"
#include <QDebug>

//使用 amixer scontrols,中的选项
//char * sctrlstr = "Digital Input";
//char * sctrlstr = "PCM";

SoundMixer::SoundMixer()
           : m_handle(NULL)
           , m_elem(NULL)
           , m_volMin(0)
           , m_volMax(0)
           , m_val(20)
{

}

bool SoundMixer::soundMixerInit(QString& status)
{
    long leftVal = 0;
    long rightVal = 0;
    int err;

    snd_mixer_selem_id_t *sid;
    snd_mixer_selem_id_alloca(&sid);

    //打开混音器设备
    if ((err = snd_mixer_open(&m_handle, 0)) < 0)
    {
        status = QString("snd_mixer_open Err!");
        return false;
    }

    snd_mixer_attach(m_handle, "default");
    snd_mixer_selem_register(m_handle, NULL, NULL);
    snd_mixer_load(m_handle);

    //循环找到自己想要的element
    m_elem = snd_mixer_first_elem(m_handle);
    while(m_elem)
    {
        //比较element名字是否是我们想要设置的选项
        if (strcmp( "PCM", snd_mixer_selem_get_name (m_elem)) == 0)
        {
            qDebug() << "elem name: " << QString(snd_mixer_selem_get_name (m_elem));
            break;
        }
        //如果不是就继续寻找下一个
        //qDebug() << "In while, elem name: " << QString(snd_mixer_selem_get_name (m_elem));
        m_elem = snd_mixer_elem_next(m_elem);
    }

    if (!m_elem)
    {
        status = QString("snd_mixer_find_selem Err!");
        snd_mixer_close(m_handle);
        m_handle = NULL;
        return false;
    }
    //输出elem的名字
    qDebug() << "elem name : " << QString(snd_mixer_selem_get_name (m_elem));

    //snd_mixer_selem_get_capture_volume_range(m_elem, &m_volMin, &m_volMax);
    snd_mixer_selem_get_playback_volume_range(m_elem, &m_volMin, &m_volMax);
    qDebug() << "音量范围: " << m_volMin << " -- " << m_volMax;
    long dbl, dbh;
    snd_mixer_selem_get_playback_dB_range(m_elem, &dbl, &dbh);
    qDebug() << "DB范围: " << dbl << " -- " << dbh;

    snd_mixer_handle_events(m_handle);
    snd_mixer_selem_get_playback_volume(m_elem, SND_MIXER_SCHN_FRONT_LEFT, &leftVal);
    snd_mixer_selem_get_playback_volume(m_elem, SND_MIXER_SCHN_FRONT_RIGHT, &rightVal);
    qDebug() << "当前音量: leftVal = " << leftVal << ", rightVal = " << rightVal;

    //判断是不是单声道
    if( snd_mixer_selem_is_playback_mono(m_elem) )
    {
        //单声道
        snd_mixer_selem_set_playback_volume(m_elem,SND_MIXER_SCHN_FRONT_LEFT, m_val);
        qDebug() << "单声道: " << m_val;
    }
    else
    {
        //左音量
        snd_mixer_selem_set_playback_volume(m_elem,SND_MIXER_SCHN_FRONT_LEFT, m_val);
        //右音量
        snd_mixer_selem_set_playback_volume(m_elem,SND_MIXER_SCHN_FRONT_RIGHT, m_val);
        qDebug() << "双声道: " << m_val;
    }
    return true;
}

void SoundMixer::up()
{
    m_val += 1;
    if (m_val >= 31)
    m_val = 31;
    //判断是不是单声道
    if( snd_mixer_selem_is_playback_mono(m_elem) )
    {
        //单声道
        snd_mixer_selem_set_playback_volume(m_elem,SND_MIXER_SCHN_FRONT_LEFT, m_val);
        qDebug() << "单声道: " << m_val;
    }
    else
    {
        //左音量
        snd_mixer_selem_set_playback_volume(m_elem,SND_MIXER_SCHN_FRONT_LEFT, m_val);
        //右音量
        snd_mixer_selem_set_playback_volume(m_elem,SND_MIXER_SCHN_FRONT_RIGHT, m_val);
        qDebug() << "双声道: " << m_val;
    }
}
void SoundMixer::down()
{
    m_val -= 1;
    if (m_val <= 0)
    m_val = 0;
    //判断是不是单声道
    if( snd_mixer_selem_is_playback_mono(m_elem) )
    {
        //单声道
        snd_mixer_selem_set_playback_volume(m_elem,SND_MIXER_SCHN_FRONT_LEFT, m_val);
        qDebug() << "单声道: " << m_val;
    }
    else
    {
        //左音量
        snd_mixer_selem_set_playback_volume(m_elem,SND_MIXER_SCHN_FRONT_LEFT, m_val);
        //右音量
        snd_mixer_selem_set_playback_volume(m_elem,SND_MIXER_SCHN_FRONT_RIGHT, m_val);
        qDebug() << "双声道: " << m_val;
    }
}
