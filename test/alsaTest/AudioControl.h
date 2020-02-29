#ifndef AUDIOCONTROL_H
#define AUDIOCONTROL_H

#include <QThread>
#include <QMutex>
#include <QList>
#include <QMap>
#include <QSharedPointer>

#include "bcg729/encoder.h"
#include "bcg729/decoder.h"

class AudioCapture;
class AudioPlayback;
class SoundMixer;

#define DATAMAXSIZE 4096

typedef struct AUDIOPERIODDATA {
    char        data[DATAMAXSIZE];
    unsigned    dataLen;
} AudioPeriodData;

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
    void addToCaptureDataList(const char *data, const unsigned int len);
    void popFromCaptureDataList(char *data, int &len);
    void addToPlaybackDataList(const char *data, const unsigned int len);
    void popFromPlaybackDataList(char *data, int &len);
    void addToMixerData(const uint8_t id, const char *data, const unsigned int len);
    void up();
    void down();
    void decoder(const uint8_t id, uint8_t bitStream[], uint8_t bitStreamLength);

protected:
    virtual void run();

private:
    AudioControl();
    ~AudioControl();
    void dealCaptureData();
    void dealPlaybackData();

    void encoder(const char *data, const unsigned int len);

    void parametersBitStream2Array(uint8_t bitStream[], uint16_t parameters[]);
    void parametersArray2BitStream(uint16_t parameters[], uint8_t bitStream[]);

signals:
    void sendCaptureData(const char *data, const unsigned int len);
    void sendMixerData(const char *data, const unsigned int len);

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
    SoundMixer                              *m_soundMixer;
    bcg729EncoderChannelContextStruct       *m_encoderChannelContextStruct;
    bcg729DecoderChannelContextStruct       *m_decoderChannelContextStruct;
};

#endif // AUDIOCONTROL_H
