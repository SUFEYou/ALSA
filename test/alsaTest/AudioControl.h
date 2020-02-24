#ifndef AUDIOCONTROL_H
#define AUDIOCONTROL_H

#include <QThread>
#include <QMutex>
#include <QList>
#include <QMap>
#include <QSharedPointer>

class AudioCapture;
class AudioPlayback;
class SoundMixer;

#define DATAMAXSIZE 4096

typedef struct AUDIOPERIODDATA {
    char        data[DATAMAXSIZE];
    unsigned    dataLen;
} AudioPeriodData;

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

protected:
    virtual void run();

private:
    AudioControl();
    void dealCaptureData();
    void dealPlaybackData();

signals:


private slots:

private:
    static AudioControl                     *m_instance;
    static QMutex                           m_getMutex;
    bool                                    m_stop;
    AudioCapture                            *m_audioCapture;
    AudioPlayback                           *m_audioPlayback;
    QList<AudioPeriodData*>                 m_captureDataList;
    QMutex                                  m_captureDataMutex;
    QList<AudioPeriodData*>                 m_playbackDataList;
    QMutex                                  m_playbackDataMutex;
    QMap<uint8_t,QList<AudioPeriodData*>*>  m_mixerData;

    SoundMixer                              *m_soundMixer;
};

#endif // AUDIOCONTROL_H
