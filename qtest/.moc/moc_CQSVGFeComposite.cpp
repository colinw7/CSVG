/****************************************************************************
** Meta object code from reading C++ file 'CQSVGFeComposite.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.2.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../CQSVGFeComposite.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CQSVGFeComposite.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.2.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_CQSVGFeComposite_t {
    QByteArrayData data[5];
    char stringdata[30];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_CQSVGFeComposite_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_CQSVGFeComposite_t qt_meta_stringdata_CQSVGFeComposite = {
    {
QT_MOC_LITERAL(0, 0, 16),
QT_MOC_LITERAL(1, 17, 2),
QT_MOC_LITERAL(2, 20, 2),
QT_MOC_LITERAL(3, 23, 2),
QT_MOC_LITERAL(4, 26, 2)
    },
    "CQSVGFeComposite\0k1\0k2\0k3\0k4\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CQSVGFeComposite[] = {

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
       1, QMetaType::Double, 0x00095103,
       2, QMetaType::Double, 0x00095103,
       3, QMetaType::Double, 0x00095103,
       4, QMetaType::Double, 0x00095103,

       0        // eod
};

void CQSVGFeComposite::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject CQSVGFeComposite::staticMetaObject = {
    { &CQSVGObject::staticMetaObject, qt_meta_stringdata_CQSVGFeComposite.data,
      qt_meta_data_CQSVGFeComposite,  qt_static_metacall, 0, 0}
};


const QMetaObject *CQSVGFeComposite::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CQSVGFeComposite::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CQSVGFeComposite.stringdata))
        return static_cast<void*>(const_cast< CQSVGFeComposite*>(this));
    if (!strcmp(_clname, "CSVGFeComposite"))
        return static_cast< CSVGFeComposite*>(const_cast< CQSVGFeComposite*>(this));
    return CQSVGObject::qt_metacast(_clname);
}

int CQSVGFeComposite::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = CQSVGObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    
#ifndef QT_NO_PROPERTIES
     if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< double*>(_v) = getK1(); break;
        case 1: *reinterpret_cast< double*>(_v) = getK2(); break;
        case 2: *reinterpret_cast< double*>(_v) = getK3(); break;
        case 3: *reinterpret_cast< double*>(_v) = getK4(); break;
        }
        _id -= 4;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setK1(*reinterpret_cast< double*>(_v)); break;
        case 1: setK2(*reinterpret_cast< double*>(_v)); break;
        case 2: setK3(*reinterpret_cast< double*>(_v)); break;
        case 3: setK4(*reinterpret_cast< double*>(_v)); break;
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
