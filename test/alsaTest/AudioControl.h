#ifndef AUDIOCONTROL_H
#define AUDIOCONTROL_H

#include <QObject>
#include <QMutex>
#include <QList>

class AudioCapture;
class AudioPlayback;
class QTimer;

typedef struct AUDIOPERIODDATA {
    char        data[4096];
    unsigned    dataLen;
} AudioPeriodData;

class AudioControl : public QObject
{
    Q_OBJECT
public:
    static AudioControl* getInstance();
    bool audioControlInit(QString& status);
    void addDataToList(const char *data, const unsigned int len);
    void popDataFromList(char *data, int &len);

private:
    AudioControl(QObject *parent=NULL);

private slots:
    void test();

private:
    static AudioControl     *m_instance;
    static QMutex           m_getMutex;
    AudioCapture            *m_audioCapture;
    AudioPlayback           *m_audioPlayback;
    QTimer                  *m_testTimer;
    QList<AudioPeriodData*>  m_audioDataList;
    QMutex                  m_dataMutex;
};

#endif // AUDIOCONTROL_H
