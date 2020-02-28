/****************************************************************************
** Meta object code from reading C++ file 'RadioControl.h'
**
** Created: Fri Feb 28 09:39:20 2020
**      by: The Qt Meta Object Compiler version 62 (Qt 4.7.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../alsaTest/RadioControl.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'RadioControl.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.7.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_RadioControl[] = {

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
      14,   13,   13,   13, 0x08,
      26,   13,   13,   13, 0x08,
      45,   41,   13,   13, 0x08,
      88,   13,   13,   13, 0x08,
     102,   41,   13,   13, 0x08,
     141,   13,   13,   13, 0x08,
     158,   13,   13,   13, 0x08,
     170,   13,   13,   13, 0x08,
     177,   13,   13,   13, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_RadioControl[] = {
    "RadioControl\0\0connected()\0disconnected()\0"
    "err\0displayError(QAbstractSocket::SocketError)\0"
    "readMessage()\0udpError(QAbstractSocket::SocketError)\0"
    "udpReadMessage()\0dealTimer()\0test()\0"
    "udpStartTimerDeal()\0"
};

const QMetaObject RadioControl::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_RadioControl,
      qt_meta_data_RadioControl, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &RadioControl::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *RadioControl::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *RadioControl::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_RadioControl))
        return static_cast<void*>(const_cast< RadioControl*>(this));
    return QObject::qt_metacast(_clname);
}

int RadioControl::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: connected(); break;
        case 1: disconnected(); break;
        case 2: displayError((*reinterpret_cast< QAbstractSocket::SocketError(*)>(_a[1]))); break;
        case 3: readMessage(); break;
        case 4: udpError((*reinterpret_cast< QAbstractSocket::SocketError(*)>(_a[1]))); break;
        case 5: udpReadMessage(); break;
        case 6: dealTimer(); break;
        case 7: test(); break;
        case 8: udpStartTimerDeal(); break;
        default: ;
        }
        _id -= 9;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
