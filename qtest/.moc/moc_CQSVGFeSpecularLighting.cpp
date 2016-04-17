/****************************************************************************
** Meta object code from reading C++ file 'CQSVGFeSpecularLighting.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.2.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../CQSVGFeSpecularLighting.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CQSVGFeSpecularLighting.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.2.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_CQSVGFeSpecularLighting_t {
    QByteArrayData data[5];
    char stringdata[86];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_CQSVGFeSpecularLighting_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_CQSVGFeSpecularLighting_t qt_meta_stringdata_CQSVGFeSpecularLighting = {
    {
QT_MOC_LITERAL(0, 0, 23),
QT_MOC_LITERAL(1, 24, 13),
QT_MOC_LITERAL(2, 38, 16),
QT_MOC_LITERAL(3, 55, 16),
QT_MOC_LITERAL(4, 72, 12)
    },
    "CQSVGFeSpecularLighting\0lightingColor\0"
    "specularConstant\0specularExponent\0"
    "surfaceScale\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CQSVGFeSpecularLighting[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       4,   14, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // properties: name, type, flags
       1, QMetaType::QColor, 0x00095103,
       2, QMetaType::Double, 0x00095103,
       3, QMetaType::Double, 0x00095103,
       4, QMetaType::Double, 0x00095103,

       0        // eod
};

void CQSVGFeSpecularLighting::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject CQSVGFeSpecularLighting::staticMetaObject = {
    { &CQSVGObject::staticMetaObject, qt_meta_stringdata_CQSVGFeSpecularLighting.data,
      qt_meta_data_CQSVGFeSpecularLighting,  qt_static_metacall, 0, 0}
};


const QMetaObject *CQSVGFeSpecularLighting::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CQSVGFeSpecularLighting::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CQSVGFeSpecularLighting.stringdata))
        return static_cast<void*>(const_cast< CQSVGFeSpecularLighting*>(this));
    if (!strcmp(_clname, "CSVGFeSpecularLighting"))
        return static_cast< CSVGFeSpecularLighting*>(const_cast< CQSVGFeSpecularLighting*>(this));
    return CQSVGObject::qt_metacast(_clname);
}

int CQSVGFeSpecularLighting::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = CQSVGObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    
#ifndef QT_NO_PROPERTIES
     if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QColor*>(_v) = getLightingColor(); break;
        case 1: *reinterpret_cast< double*>(_v) = getSpecularConstant(); break;
        case 2: *reinterpret_cast< double*>(_v) = getSpecularExponent(); break;
        case 3: *reinterpret_cast< double*>(_v) = getSurfaceScale(); break;
        }
        _id -= 4;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setLightingColor(*reinterpret_cast< QColor*>(_v)); break;
        case 1: setSpecularConstant(*reinterpret_cast< double*>(_v)); break;
        case 2: setSpecularExponent(*reinterpret_cast< double*>(_v)); break;
        case 3: setSurfaceScale(*reinterpret_cast< double*>(_v)); break;
        }
        _id -= 4;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 4;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 4;
    } else if (_c == QMetaObject::RegisterPropertyMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}
QT_END_MOC_NAMESPACE
