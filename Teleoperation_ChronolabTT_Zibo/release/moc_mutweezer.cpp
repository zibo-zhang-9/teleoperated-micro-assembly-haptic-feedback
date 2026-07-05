/****************************************************************************
** Meta object code from reading C++ file 'mutweezer.h'
*****************************************************************************/

#include "../mutweezer.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mutweezer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MuTweezer_t {
    QByteArrayData data[5];
    char stringdata0[38];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MuTweezer_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MuTweezer_t qt_meta_stringdata_MuTweezer = {
    {
QT_MOC_LITERAL(0, 0, 9), // "MuTweezer"
QT_MOC_LITERAL(1, 10, 12), // "dataReceived"
QT_MOC_LITERAL(2, 23, 0), // ""
QT_MOC_LITERAL(3, 24, 11), // "sendMessage"
QT_MOC_LITERAL(4, 36, 1) // "m"

    },
    "MuTweezer\0dataReceived\0\0sendMessage\0"
    "m"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MuTweezer[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   24,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       3,    1,   25,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    4,

       0        // eod
};

void MuTweezer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MuTweezer *_t = static_cast<MuTweezer *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->dataReceived(); break;
        case 1: _t->sendMessage((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (MuTweezer::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&MuTweezer::dataReceived)) {
                *result = 0;
                return;
            }
        }
    }
}

const QMetaObject MuTweezer::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_MuTweezer.data,
      qt_meta_data_MuTweezer,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *MuTweezer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MuTweezer::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MuTweezer.stringdata0))
        return static_cast<void*>(const_cast< MuTweezer*>(this));
    return QThread::qt_metacast(_clname);
}

int MuTweezer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 2)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 2;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 2)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 2;
    }
    return _id;
}

// SIGNAL 0
void MuTweezer::dataReceived()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
