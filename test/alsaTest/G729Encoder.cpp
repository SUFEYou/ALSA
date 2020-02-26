#include "G729Encoder.h"
#include "G729Decoder.h"
#include <QDebug>

G729Encoder* G729Encoder::m_instance = NULL;
QMutex G729Encoder::m_getMutex;

G729Encoder::G729Encoder()
              : m_stop(false)
              , m_encoderChannelContextStruct(NULL)
{

}

G729Encoder* G729Encoder::getInstance()
{
    if (m_instance == NULL)
    {
        QMutexLocker locker(&m_getMutex);
        if (m_instance == NULL)
        {
            m_instance = new G729Encoder;
        }
    }

    return m_instance;
}

void G729Encoder::run()
{
    m_encoderChannelContextStruct = initBcg729EncoderChannel(0);
    while(!m_stop)
    {
        encoder();
    }
    m_stop = false;
    closeBcg729EncoderChannel(m_encoderChannelContextStruct);
}

void G729Encoder::encoder()
{
    QMutexLocker locker(&m_encoderDataMutex);
    if (m_encoderDataList.empty())
        return;

    pG729EncoderData tmp(new G729EncoderData);
    tmp = m_encoderDataList.front();
    m_encoderDataList.pop_front();

    uint8_t bitStream[BITSTREAMLEN];
    uint8_t bitStreamLength;

    bcg729Encoder(m_encoderChannelContextStruct, tmp->data, bitStream, &bitStreamLength);

    /* convert bitStream output in an array for easier debug */
    if (bitStreamLength == 10)
    {
        //qDebug() << "bitStreamLength == 10";
        //decoder(bitStream, bitStreamLength);
        G729Decoder::getInstance()->addDecoderData(bitStream, bitStreamLength);
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
}

void G729Encoder::addEncoderData(const char *data, const unsigned int len)
{
    if (len < 2*INPUTBUFFERLEN)
    {
        qDebug() << "In G729Encoder::addEncoderData len do not enough. len = " << len;
        return;
    }
    pG729EncoderData tmp(new G729EncoderData);
    for (int i = 0; i < INPUTBUFFERLEN; ++i)
    {
        memcpy(&(tmp->data[i]), &data[i*2], sizeof(int16_t));
    }
    tmp->dataLen = INPUTBUFFERLEN;
    {
        QMutexLocker locker(&m_encoderDataMutex);
        m_encoderDataList.push_back(tmp);
    }
}
