#include "AudioControl.h"
#include "AudioCapture.h"
#include "AudioPlayback.h"
#include "SoundMixer.h"
#include <QMutexLocker>
#include <QDebug>
#include <QTime>

//0:Opus 1:G729
#define ENCODER_DECODER_TYPE 0

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

#if ENCODER_DECODER_TYPE
    m_encoderChannelContextStruct = initBcg729EncoderChannel(0);
    m_decoderChannelContextStruct = initBcg729DecoderChannel();
#else
    m_encoder = opus_encoder_create(8000, 1, OPUS_APPLICATION_AUDIO, &m_err);
    if (!m_encoder || m_err < 0)
    {
        qDebug() << "failed to create encoder: " << QString(opus_strerror(m_err));
        if (!m_encoder)
        {
            opus_encoder_destroy(m_encoder);
        }
    }

    m_decoder = opus_decoder_create(8000, 1, &m_err);
    if (!m_decoder || m_err < 0)
    {
        qDebug() << "failed to create decoder: " << QString(opus_strerror(m_err));
        if (!m_decoder)
        {
            opus_decoder_destroy(m_decoder);
        }
    }
#endif
}

AudioControl::~AudioControl()
{
#if ENCODER_DECODER_TYPE
    closeBcg729EncoderChannel(m_encoderChannelContextStruct);
    closeBcg729DecoderChannel(m_decoderChannelContextStruct);
#else
    if (!m_encoder)
    {
        opus_encoder_destroy(m_encoder);
    }
    if (!m_decoder)
    {
        opus_decoder_destroy(m_decoder);
    }
#endif
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

    m_audioPlayback->audioPlaybackConnect();
    m_audioCapture->start();
    m_audioPlayback->start();

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
        if (m_playbackDataList.length() > 100)
        {
            AudioPeriodDataList::iterator iter = m_playbackDataList.begin();
            m_playbackDataList.erase(iter, iter+20);
        }
        m_playbackDataList.append(tmp);
    }
}

void AudioControl::popFromPlaybackDataList(char *data, int &len)
{
    QMutexLocker locker(&m_playbackDataMutex);
    //qDebug() << "PlaybackDataList length = " << m_playbackDataList.length();
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
            //addToMixerData(1, tmp, len);
            //emit sendCaptureData(tmp, len);
            encoder(tmp, len);
            //addToMixerData(2, tmp, len);
            //addToMixerData(3, tmp, len);
        }
    }
}

void AudioControl::dealPlaybackData()
{
    QTime t;
    t.start();

    AudioPeriodDataList data;
    data.clear();
    {
        QMutexLocker locker(&m_mixerDataMutex);
        //qDebug() << "Mixer Map Size: " << m_mixerData.size();
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

        AudioPeriodData mixerData;
        memset(&mixerData, 0, sizeof(mixerData));
        int x = 2 * m_audioCapture->getFrameSize();
#if 1
        double f = 1;
        qint32 output;
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
#else
        for (int i = 0; i < x; ++i)
        {
           qint32 temp = 0;
           for (int j = 0; j < data.size(); ++j)
           {
               temp += *(qint16*)(&(data[j]->data[i*2]));
           }
           if (temp > MAX)
           {
              temp=MAX;
           }
           if (temp < MIN)
           {
              temp = MIN;
           }
           *(qint16*)(&(mixerData.data[i*2])) = (qint16)temp;
           mixerData.dataLen += 2;
        }
#endif
        addToPlaybackDataList(mixerData.data, mixerData.dataLen);
        //emit sendMixerData(mixerData.data, mixerData.dataLen);
    }

    //qDebug() << "Mixer Time elapsed: " << t.elapsed();
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
        //qDebug() << "List size = " << list->length();
        if (list->length() > 200)
        {
            //list->pop_front();
            AudioPeriodDataList::iterator iter = list->begin();
            list->erase(iter, iter+100);
        }
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

void AudioControl::encoder(const char *data, const unsigned int len)
{
    QTime t;
    t.start();
    //qDebug() << "In AudioControl::encoder, len = " << len;
    if (len < 160)
        return;
    int16_t inputBuffer[80];
    for (int i = 0; i < 80; ++i)
    {
        memcpy(&inputBuffer[i], &data[i*2], sizeof(int16_t));
    }

#if ENCODER_DECODER_TYPE
    uint8_t bitStream[10];
    uint8_t bitStreamLength = 0;
    bcg729Encoder(m_encoderChannelContextStruct, inputBuffer, bitStream, &bitStreamLength);

    /* convert bitStream output in an array for easier debug */
    if (bitStreamLength == 10)
    {
        //qDebug() << "bitStreamLength == 10";
        //decoder(0, bitStream, bitStreamLength);
        emit sendCaptureData(bitStream, bitStreamLength);
    }
    else if (bitStreamLength == 2)
    {
        /* write the output to the output file */
        qDebug() << "bitStreamLength == 2";
    }
    else
    {
        /* bitstream to 0, un transmitted frame */
        qDebug() << "bitStreamLength == 0";
    }
#else
    unsigned char cbits[DATAMAXSIZE];
    int nbBytes = opus_encode(m_encoder, inputBuffer, 80, cbits, DATAMAXSIZE);
    if (nbBytes > 0)
    {
        //qDebug() << "encoder data size: " << nbBytes;
        //decoder(0, cbits, nbBytes);
        emit sendCaptureData(cbits, nbBytes);
    }

#endif

    //qDebug() << "Encoder Time elapsed: " << t.elapsed();
}

void AudioControl::decoder(const uint8_t id, const char *bitStream, uint8_t bitStreamLength)
{
    QTime t;
    t.start();

    int16_t outputBuffer[80]; /* output buffer: the reconstructed signal */
    char tmp[DATAMAXSIZE];
#if ENCODER_DECODER_TYPE
    bcg729Decoder(m_decoderChannelContextStruct, bitStream, bitStreamLength, 0, 0, 0, outputBuffer);
    for (int i = 0; i < 80; ++i)
    {
        memcpy(&tmp[2*i], &outputBuffer[i], sizeof(int16_t));
    }
#else

    int frame_size = opus_decode(m_decoder, bitStream, bitStreamLength, outputBuffer, 80, 0);
    if (frame_size > 0 && frame_size <= 80)
    {
        //qDebug() << "decoder data size : " << frame_size;
        for (int i = 0; i < frame_size; ++i)
        {
            memcpy(&tmp[2*i], &outputBuffer[i], sizeof(int16_t));
        }
    }

#endif

    //addToPlaybackDataList(tmp, 160);
    addToMixerData(id, tmp, 160);

    //qDebug() << "Decoder Time elapsed: " << t.elapsed();
}

/*****************************************************************************/
/* parametersArray2BitStream : convert bitStream to an array of parameters   */
/*             reverse operation of previous funtion                         */
/*    parameters:                                                            */
/*      -(i) bitStream : the 16 values streamed on 80 bits in a              */
/*           10*8bits values array                                           */
/*      -(o) parameters : 16 values parameters array                         */
/*                                                                           */
/*****************************************************************************/
void AudioControl::parametersBitStream2Array(uint8_t bitStream[], uint16_t parameters[])
{
    parameters[0] = (bitStream[0]>>7)&(uint16_t)0x1;
    parameters[1] = bitStream[0]&(uint16_t)0x7f;
    parameters[2] = (bitStream[1]>>3)&(uint16_t)0x1f;
    parameters[3] = (((uint16_t)bitStream[1]&(uint16_t)0x7)<<2) | ((bitStream[2]>>6)&(uint16_t)0x3);
    parameters[4] = (((uint16_t)bitStream[2])&(uint16_t)0x3f)<<2 | ((bitStream[3]>>6)&(uint16_t)0x3);;
    parameters[5] = (bitStream[3]>>5)&(uint16_t)0x1;
    parameters[6] = (((uint16_t)(bitStream[3]&(uint16_t)0x1f))<<8)| bitStream[4];
    parameters[7] = (bitStream[5]>>4)&(uint16_t)0xf;
    parameters[8] = (bitStream[5]>>1)&(uint16_t)0x7;
    parameters[9] = (((uint16_t)bitStream[5]&(uint16_t)0x1)<<3)|((bitStream[6]>>5)&(uint16_t)0x7);
    parameters[10]= (uint16_t)bitStream[6]&(uint16_t)0x1f;
    parameters[11]= (((uint16_t)bitStream[7])<<5)|((bitStream[8]>>3)&(uint16_t)0x1f);
    parameters[12]= ((bitStream[8]&(uint16_t)0x7)<<1) | ((bitStream[9]>>7)&(uint16_t)0x1);
    parameters[13]= (bitStream[9]>>4)&(uint16_t)0x7;
    parameters[14]= bitStream[9]&(uint16_t)0xf;

    return;
}

/*** bitStream to parameters Array conversions functions ***/
/* Note: these functions are in utils.c because used by test source code too */

/*****************************************************************************/
/* parametersArray2BitStream : convert array of parameters to bitStream      */
/*      according to spec 4 - Table 8 and following mapping of values        */
/*               0 -> L0 (1 bit)                                             */
/*               1 -> L1 (7 bits)                                            */
/*               2 -> L2 (5 bits)                                            */
/*               3 -> L3 (5 bits)                                            */
/*               4 -> P1 (8 bit)                                             */
/*               5 -> P0 (1 bits)                                            */
/*               6 -> C1 (13 bits)                                           */
/*               7 -> S1 (4 bits)                                            */
/*               8 -> GA1(3 bits)                                            */
/*               9 -> GB1(4 bits)                                            */
/*               10 -> P2 (5 bits)                                           */
/*               11 -> C2 (13 bits)                                          */
/*               12 -> S2 (4 bits)                                           */
/*               13 -> GA2(3 bits)                                           */
/*               14 -> GB2(4 bits)                                           */
/*    parameters:                                                            */
/*      -(i) parameters : 16 values parameters array                         */
/*      -(o) bitStream : the 16 values streamed on 80 bits in a              */
/*           10*8bits values array                                           */
/*                                                                           */
/*****************************************************************************/
void AudioControl::parametersArray2BitStream(uint16_t parameters[], uint8_t bitStream[])
{
    bitStream[0] = ((parameters[0]&((uint16_t) 0x1))<<7) |
            (parameters[1]&((uint16_t) 0x7f));

    bitStream[1] = ((parameters[2]&((uint16_t) 0x1f))<<3) |
            ((parameters[3]>>2)&((uint16_t) 0x7));

    bitStream[2] = ((parameters[3]&((uint16_t) 0x3))<<6) |
            ((parameters[4]>>2)&((uint16_t) 0x3f));

    bitStream[3] = ((parameters[4]&((uint16_t) 0x3))<<6) |
            ((parameters[5]&((uint16_t) 0x1))<<5) |
            ((parameters[6]>>8)&((uint16_t) 0x1f));

    bitStream[4] = ((parameters[6])&((uint16_t) 0xff));

    bitStream[5] = ((parameters[7]&((uint16_t) 0xf))<<4) |
            ((parameters[8]&((uint16_t) 0x7))<<1) |
            ((parameters[9]>>3)&((uint16_t) 0x1));

    bitStream[6] = ((parameters[9]&((uint16_t) 0x7))<<5) |
            (parameters[10]&((uint16_t) 0x1f));

    bitStream[7] = ((parameters[11]>>5)&((uint16_t) 0xff));

    bitStream[8] = ((parameters[11]&((uint16_t) 0x1f))<<3) |
            ((parameters[12]>>1)&((uint16_t) 0x7));

    bitStream[9] = ((parameters[12]&((uint16_t) 0x1))<<7) |
            ((parameters[13]&((uint16_t) 0x7))<<4) |
            (parameters[14]&((uint16_t) 0xf));

    return;
}
