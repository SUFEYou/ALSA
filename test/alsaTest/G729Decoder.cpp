#include "G729Decoder.h"
#include "AudioControl.h"
#include <QDebug>

G729Decoder* G729Decoder::m_instance = NULL;
QMutex G729Decoder::m_getMutex;

G729Decoder::G729Decoder()
    : m_stop(false)
    , m_decoderChannelContextStruct(NULL)
{

}

G729Decoder* G729Decoder::getInstance()
{
    if (m_instance == NULL)
    {
        QMutexLocker locker(&m_getMutex);
        if (m_instance == NULL)
        {
            m_instance = new G729Decoder;
        }
    }

    return m_instance;
}

void G729Decoder::run()
{
    m_decoderChannelContextStruct = initBcg729DecoderChannel();
    while(!m_stop)
    {
        decoder();
    }
    m_stop = false;
    closeBcg729DecoderChannel(m_decoderChannelContextStruct);
}

void G729Decoder::decoder()
{
    QMutexLocker locker(&m_decoderDataMutex);
    if (m_decoderDataList.empty())
        return;
    pG729DecoderData tmp(new G729DecoderData);
    tmp = m_decoderDataList.front();
    m_decoderDataList.pop_front();
    int16_t outputBuffer[INPUTBUFFERLEN]; /* output buffer: the reconstructed signal */
    bcg729Decoder(m_decoderChannelContextStruct, tmp->data, tmp->dataLen, 0, 0, 0, outputBuffer);

    char data[DATAMAXSIZE];
    for (int i = 0; i < INPUTBUFFERLEN; ++i)
    {
        memcpy(&data[2*i], &outputBuffer[i], sizeof(int16_t));
    }
    AudioControl::getInstance()->addToMixerData(5, data, 2*INPUTBUFFERLEN);
}

void G729Decoder::addDecoderData(const uint8_t *data, const uint8_t len)
{
    if (len != BITSTREAMLEN)
    {
        qDebug() << "In G729Decoder::addDecoderData len err, len = " << len;
        return;
    }
    pG729DecoderData tmp(new G729DecoderData);
    memcpy(tmp->data, data, len);
    tmp->dataLen = len;
    {
        QMutexLocker locker(&m_decoderDataMutex);
        m_decoderDataList.push_back(tmp);
    }
}
