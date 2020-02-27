/****************************************************************************
** Meta object code from reading C++ file 'AudioControl.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.6.3)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../alsaTest/AudioControl.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'AudioControl.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.6.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_AudioControl_t {
    QByteArrayData data[6];
    char stringdata0[51];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_AudioControl_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_AudioControl_t qt_meta_stringdata_AudioControl = {
    {
QT_MOC_LITERAL(0, 0, 12), // "AudioControl"
QT_MOC_LITERAL(1, 13, 15), // "sendCaptureData"
QT_MOC_LITERAL(2, 29, 0), // ""
QT_MOC_LITERAL(3, 30, 11), // "const char*"
QT_MOC_LITERAL(4, 42, 4), // "data"
QT_MOC_LITERAL(5, 47, 3) // "len"

    },
    "AudioControl\0sendCaptureData\0\0const char*\0"
    "data\0len"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_AudioControl[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   19,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3, QMetaType::UInt,    4,    5,

       0        // eod
};

void AudioControl::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        AudioControl *_t = static_cast<AudioControl *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->sendCaptureData((*reinterpret_cast< const char*(*)>(_a[1])),(*reinterpret_cast< const uint(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (AudioControl::*_t)(const char * , const unsigned int );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&AudioControl::sendCaptureData)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject AudioControl::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_AudioControl.data,
      qt_meta_data_AudioControl,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *AudioControl::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AudioControl::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_AudioControl.stringdata0))
        return static_cast<void*>(const_cast< AudioControl*>(this));
    return QThread::qt_metacast(_clname);
}

int AudioControl::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}

// SIGNAL 0
void AudioControl::sendCaptureData(const char * _t1, const unsigned int _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE
