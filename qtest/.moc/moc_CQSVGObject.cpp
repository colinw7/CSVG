/****************************************************************************
** Meta object code from reading C++ file 'CQSVGObject.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.2.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../CQSVGObject.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CQSVGObject.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.2.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_CQSVGObject_t {
    QByteArrayData data[22];
    char stringdata[271];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_CQSVGObject_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_CQSVGObject_t qt_meta_stringdata_CQSVGObject = {
    {
QT_MOC_LITERAL(0, 0, 11),
QT_MOC_LITERAL(1, 12, 2),
QT_MOC_LITERAL(2, 15, 8),
QT_MOC_LITERAL(3, 24, 8),
QT_MOC_LITERAL(4, 33, 7),
QT_MOC_LITERAL(5, 41, 9),
QT_MOC_LITERAL(6, 51, 11),
QT_MOC_LITERAL(7, 63, 13),
QT_MOC_LITERAL(8, 77, 11),
QT_MOC_LITERAL(9, 89, 10),
QT_MOC_LITERAL(10, 100, 9),
QT_MOC_LITERAL(11, 110, 9),
QT_MOC_LITERAL(12, 120, 22),
QT_MOC_LITERAL(13, 143, 10),
QT_MOC_LITERAL(14, 154, 23),
QT_MOC_LITERAL(15, 178, 13),
QT_MOC_LITERAL(16, 192, 18),
QT_MOC_LITERAL(17, 211, 9),
QT_MOC_LITERAL(18, 221, 11),
QT_MOC_LITERAL(19, 233, 8),
QT_MOC_LITERAL(20, 242, 19),
QT_MOC_LITERAL(21, 262, 7)
    },
    "CQSVGObject\0id\0selected\0filtered\0"
    "visible\0transform\0strokeColor\0"
    "strokeOpacity\0strokeWidth\0strokeDash\0"
    "CLineDash\0strokeCap\0CQSVGEnum::LineCapType\0"
    "strokeJoin\0CQSVGEnum::LineJoinType\0"
    "fillIsNoColor\0fillIsCurrentColor\0"
    "fillColor\0fillOpacity\0fillRule\0"
    "CQSVGEnum::FillType\0fillUrl\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CQSVGObject[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
      17,   14, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // properties: name, type, flags
       1, QMetaType::QString, 0x00095103,
       2, QMetaType::Bool, 0x00095103,
       3, QMetaType::Bool, 0x00095103,
       4, QMetaType::Bool, 0x00095103,
       5, QMetaType::QString, 0x00095001,
       6, QMetaType::QColor, 0x00095103,
       7, QMetaType::Double, 0x00095103,
       8, QMetaType::Double, 0x00095103,
       9, 0x80000000 | 10, 0x0009510b,
      11, 0x80000000 | 12, 0x0009510b,
      13, 0x80000000 | 14, 0x0009510b,
      15, QMetaType::Bool, 0x00095103,
      16, QMetaType::Bool, 0x00095103,
      17, QMetaType::QColor, 0x00095103,
      18, QMetaType::Double, 0x00095103,
      19, 0x80000000 | 20, 0x0009510b,
      21, QMetaType::QString, 0x00095103,

       0        // eod
};

void CQSVGObject::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::RegisterPropertyMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 8:
            *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< CLineDash >(); break;
        }
    }

    Q_UNUSED(_o);
}

static const QMetaObject *qt_meta_extradata_CQSVGObject[] = {
        &CQSVGEnum::staticMetaObject,
    0
};

const QMetaObject CQSVGObject::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_CQSVGObject.data,
      qt_meta_data_CQSVGObject,  qt_static_metacall, qt_meta_extradata_CQSVGObject, 0}
};


const QMetaObject *CQSVGObject::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CQSVGObject::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CQSVGObject.stringdata))
        return static_cast<void*>(const_cast< CQSVGObject*>(this));
    return QObject::qt_metacast(_clname);
}

int CQSVGObject::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    
#ifndef QT_NO_PROPERTIES
     if (_c == QMetaObject::ReadProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QString*>(_v) = id(); break;
        case 1: *reinterpret_cast< bool*>(_v) = isSelected(); break;
        case 2: *reinterpret_cast< bool*>(_v) = isFiltered(); break;
        case 3: *reinterpret_cast< bool*>(_v) = isVisible(); break;
        case 4: *reinterpret_cast< QString*>(_v) = transformStr(); break;
        case 5: *reinterpret_cast< QColor*>(_v) = strokeColor(); break;
        case 6: *reinterpret_cast< double*>(_v) = strokeOpacity(); break;
        case 7: *reinterpret_cast< double*>(_v) = strokeWidth(); break;
        case 8: *reinterpret_cast< CLineDash*>(_v) = strokeDash(); break;
        case 9: *reinterpret_cast< CQSVGEnum::LineCapType*>(_v) = strokeCap(); break;
        case 10: *reinterpret_cast< CQSVGEnum::LineJoinType*>(_v) = strokeJoin(); break;
        case 11: *reinterpret_cast< bool*>(_v) = fillIsNoColor(); break;
        case 12: *reinterpret_cast< bool*>(_v) = fillIsCurrentColor(); break;
        case 13: *reinterpret_cast< QColor*>(_v) = fillColor(); break;
        case 14: *reinterpret_cast< double*>(_v) = fillOpacity(); break;
        case 15: *reinterpret_cast< CQSVGEnum::FillType*>(_v) = fillRule(); break;
        case 16: *reinterpret_cast< QString*>(_v) = fillUrl(); break;
        }
        _id -= 17;
    } else if (_c == QMetaObject::WriteProperty) {
        void *_v = _a[0];
        switch (_id) {
        case 0: setId(*reinterpret_cast< QString*>(_v)); break;
        case 1: setSelected(*reinterpret_cast< bool*>(_v)); break;
        case 2: setFiltered(*reinterpret_cast< bool*>(_v)); break;
        case 3: setVisible(*reinterpret_cast< bool*>(_v)); break;
        case 5: setStrokeColor(*reinterpret_cast< QColor*>(_v)); break;
        case 6: setStrokeOpacity(*reinterpret_cast< double*>(_v)); break;
        case 7: setStrokeWidth(*reinterpret_cast< double*>(_v)); break;
        case 8: setStrokeDash(*reinterpret_cast< CLineDash*>(_v)); break;
        case 9: setStrokeCap(*reinterpret_cast< CQSVGEnum::LineCapType*>(_v)); break;
        case 10: setStrokeJoin(*reinterpret_cast< CQSVGEnum::LineJoinType*>(_v)); break;
        case 11: setFillIsNoColor(*reinterpret_cast< bool*>(_v)); break;
        case 12: setFillIsCurrentColor(*reinterpret_cast< bool*>(_v)); break;
        case 13: setFillColor(*reinterpret_cast< QColor*>(_v)); break;
        case 14: setFillOpacity(*reinterpret_cast< double*>(_v)); break;
        case 15: setFillRule(*reinterpret_cast< CQSVGEnum::FillType*>(_v)); break;
        case 16: setFillUrl(*reinterpret_cast< QString*>(_v)); break;
        }
        _id -= 17;
    } else if (_c == QMetaObject::ResetProperty) {
        _id -= 17;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 17;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 17;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 17;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 17;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 17;
    } else if (_c == QMetaObject::RegisterPropertyMetaType) {
        if (_id < 17)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 17;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}
QT_END_MOC_NAMESPACE
