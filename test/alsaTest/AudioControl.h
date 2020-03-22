#ifndef AUDIOCONTROL_H
#define AUDIOCONTROL_H

#include <QThread>
#include <QMutex>
#include <QList>
#include <QMap>
#include <QSharedPointer>
#include <QMetaType>

#include "bcg729/encoder.h"
#include "bcg729/decoder.h"

#include "opus/opus.h"
#include "opus/opus_types.h"

class AudioCapture;
class AudioPlayback;
class SoundMixer;

#define DATAMAXSIZE 512

typedef struct AUDIOPERIODDATA {
    uint8_t     data[DATAMAXSIZE];
    uint16_t     dataLen;
} AudioPeriodData;

Q_DECLARE_METATYPE(AudioPeriodData);

typedef QSharedPointer<AudioPeriodData>         pAudioPeriodData;
typedef QList<pAudioPeriodData>                 AudioPeriodDataList;
typedef QSharedPointer<AudioPeriodDataList>     pAudioPeriodDataList;
typedef QMap<uint8_t, pAudioPeriodDataList>     SoundMixerMap;



class AudioControl : public QThread
{
    Q_OBJECT
public:
    static AudioControl* getInstance();
    bool audioControlInit(QString& status);
    void addToCaptureDataList(const uint8_t *data, const unsigned int len);
    void popFromCaptureDataList(uint8_t *data, int &len);
    void addToPlaybackDataList(const uint8_t *data, const unsigned int len);
    void popFromPlaybackDataList(uint8_t *data, int &len);
    void popFromPlaybackDataList(uint8_t *data, int &len, bool flag);
    void addToMixerData(const uint8_t id, uint8_t *data, const unsigned int len);
    void up();
    void down();
    void decoder(const uint8_t id, uint8_t *bitStream, uint8_t bitStreamLength);

protected:
    virtual void run();

private:
    AudioControl();
    ~AudioControl();
    void dealCaptureData();
    void dealPlaybackData();

    void encoder(const uint8_t *data, const unsigned int len);

    void parametersBitStream2Array(uint8_t bitStream[], uint16_t parameters[]);
    void parametersArray2BitStream(uint16_t parameters[], uint8_t bitStream[]);

signals:
    //void sendCaptureData(uint8_t *data, const unsigned int len);
    void sendCaptureData(AudioPeriodData data);
    void sendMixerData(const uint8_t *data, const unsigned int len);

private slots:

private:
    static AudioControl                     *m_instance;
    static QMutex                           m_getMutex;
    bool                                    m_stop;
    AudioCapture                            *m_audioCapture;
    AudioPlayback                           *m_audioPlayback;
    AudioPeriodDataList                     m_captureDataList;
    QMutex                                  m_captureDataMutex;
    AudioPeriodDataList                     m_playbackDataList;
    QMutex                                  m_playbackDataMutex;
    SoundMixerMap                           m_mixerData;
    QMutex                                  m_mixerDataMutex;
    QMutex                                  m_decoderMutex;
    SoundMixer                              *m_soundMixer;
    bcg729EncoderChannelContextStruct       *m_encoderChannelContextStruct;
    bcg729DecoderChannelContextStruct       *m_decoderChannelContextStruct;

    OpusEncoder                             *m_encoder;
    OpusDecoder                             *m_decoder;
    int                                     m_err;

};

#endif // AUDIOCONTROL_H
