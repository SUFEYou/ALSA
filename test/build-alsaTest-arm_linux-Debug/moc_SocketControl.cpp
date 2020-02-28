/****************************************************************************
** Meta object code from reading C++ file 'SocketControl.h'
**
** Created: Fri Feb 28 09:39:19 2020
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../alsaTest/SocketControl.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'SocketControl.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_SocketControl[] = {

 // content:
       5,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      24,   15,   14,   14, 0x0a,
      51,   14,   14,   14, 0x0a,
      68,   14,   14,   14, 0x08,
      91,   79,   14,   14, 0x08,
     127,   14,   14,   14, 0x08,
     139,   14,   14,   14, 0x08,
     151,   14,   14,   14, 0x08,
     170,  166,   14,   14, 0x08,
     213,   14,   14,   14, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_SocketControl[] = {
    "SocketControl\0\0data,len\0"
    "sendData(const char*,uint)\0tcpClientStart()\0"
    "readData()\0socketError\0"
    "error(QAbstractSocket::SocketError)\0"
    "dealTimer()\0connected()\0disconnected()\0"
    "err\0displayError(QAbstractSocket::SocketError)\0"
    "readMessage()\0"
};

const QMetaObject SocketControl::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_SocketControl,
      qt_meta_data_SocketControl, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &SocketControl::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *SocketControl::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *SocketControl::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_SocketControl))
        return static_cast<void*>(const_cast< SocketControl*>(this));
    return QObject::qt_metacast(_clname);
}

int SocketControl::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: sendData((*reinterpret_cast< const char*(*)>(_a[1])),(*reinterpret_cast< const uint(*)>(_a[2]))); break;
        case 1: tcpClientStart(); break;
        case 2: readData(); break;
        case 3: error((*reinterpret_cast< QAbstractSocket::SocketError(*)>(_a[1]))); break;
        case 4: dealTimer(); break;
        case 5: connected(); break;
        case 6: disconnected(); break;
        case 7: displayError((*reinterpret_cast< QAbstractSocket::SocketError(*)>(_a[1]))); break;
        case 8: readMessage(); break;
        default: ;
        }
        _id -= 9;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
