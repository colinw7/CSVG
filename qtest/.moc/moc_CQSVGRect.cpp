/****************************************************************************
** Meta object code from reading C++ file 'CQSVGRect.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.2.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../CQSVGRect.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CQSVGRect.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.2.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_CQSVGRect_t {
    QByteArrayData data[7];
    char stringdata[25];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_CQSVGRect_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_CQSVGRect_t qt_meta_stringdata_CQSVGRect = {
    {
QT_MOC_LITERAL(0, 0, 9),
QT_MOC_LITERAL(1, 10, 1),
QT_MOC_LITERAL(2, 12, 1),
QT_MOC_LITERAL(3, 14, 1),
QT_MOC_LITERAL(4, 16, 1),
QT_MOC_LITERAL(5, 18, 2),
QT_MOC_LITERAL(6, 21, 2)
    },
    "CQSVGRect\0x\0y\0w\0h\0rx\0ry\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CQSVGRect[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       6,   14, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // properties: name, type, flags
       1, QMetaType::Double, 0x00095103,
       2, QMetaType::Double, 0x00095103,
       3, QMetaType::Double, 0x00095003,
       4, QMetaType::Double, 0x00095003,
       5, QMetaType::Double, 0x00095003,
       6, QMetaType::Double, 0x00095003,

       0        // eod
};

void CQSVGRect::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject CQSVGRect::staticMetaObject = {
    { &CQSVGObject::staticMetaObject, qt_meta_stringdata_CQSVGRect.data,
      qt_meta_data_CQSVGRect,  qt_static_metacall, 0, 0}
};


const QMetaObject *CQSVGRect::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CQSVGRect::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CQSVGRect.stringdata))
        return static_cast<void*>(const_cast< CQSVGRect*>(this));
    if (!strcmp(_clname, "CSVGRect"))
        return static_cast< CSVGRect*>(const_cast< CQSVGRect*>(this));
    return CQSVGObject::qt_metacast(_clname);
}

int CQSVGRect::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = CQSVGObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    
#ifndef QT_NO_PROPERTIES
     if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< double*>(_v) = getX(); break;
        case 1: *reinterpret_cast< double*>(_v) = getY(); break;
        case 2: *reinterpret_cast< double*>(_v) = getWidth(); break;
        case 3: *reinterpret_cast< double*>(_v) = getHeight(); break;
        case 4: *reinterpret_cast< double*>(_v) = getRX(); break;
        case 5: *reinterpret_cast< double*>(_v) = getRY(); break;
        }
        _id -= 6;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setX(*reinterpret_cast< double*>(_v)); break;
        case 1: setY(*reinterpret_cast< double*>(_v)); break;
        case 2: setWidth(*reinterpret_cast< double*>(_v)); break;
        case 3: setHeight(*reinterpret_cast< double*>(_v)); break;
        case 4: setRX(*reinterpret_cast< double*>(_v)); break;
        case 5: setRY(*reinterpret_cast< double*>(_v)); break;
        }
        _id -= 6;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 6;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 6;
    } else if (_c == QMetaObject::RegisterPropertyMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}
QT_END_MOC_NAMESPACE
