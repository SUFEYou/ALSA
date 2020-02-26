#ifndef G729ENCODER_H
#define G729ENCODER_H

#include <QThread>
#include <QMutex>
#include <QList>
#include "bcg729/encoder.h"
#include "G729Common.h"

class G729Encoder : public QThread
{
    Q_OBJECT
public:
    static G729Encoder* getInstance();
    void addEncoderData(const char *data, const unsigned int len);

protected:
    virtual void run();

private:
    G729Encoder();
    void encoder();

private:
    static G729Encoder                      *m_instance;
    static QMutex                           m_getMutex;
    bool                                    m_stop;
    bcg729EncoderChannelContextStruct       *m_encoderChannelContextStruct;
    QList<pG729EncoderData>                 m_encoderDataList;
    QMutex                                  m_encoderDataMutex;
};

#endif // G729ENCODER_H
