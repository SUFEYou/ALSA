#ifndef G729DECODER_H
#define G729DECODER_H

#include <QThread>
#include <QMutex>
#include <QList>
#include "bcg729/decoder.h"
#include "G729Common.h"

class G729Decoder: public QThread
{
    Q_OBJECT
public:
    static G729Decoder* getInstance();
    void addDecoderData(const uint8_t *data, const uint8_t len);

protected:
    virtual void run();
    void decoder();

private:
    G729Decoder();

private:
    static G729Decoder                      *m_instance;
    static QMutex                           m_getMutex;
    bool                                    m_stop;
    bcg729DecoderChannelContextStruct       *m_decoderChannelContextStruct;
    QList<pG729DecoderData>                 m_decoderDataList;
    QMutex                                  m_decoderDataMutex;
};

#endif // G729DECODER_H
