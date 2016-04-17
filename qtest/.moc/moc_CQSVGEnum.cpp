/****************************************************************************
** Meta object code from reading C++ file 'CQSVGEnum.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.2.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../CQSVGEnum.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'CQSVGEnum.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.2.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_CQSVGEnum_t {
    QByteArrayData data[26];
    char stringdata[380];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    offsetof(qt_meta_stringdata_CQSVGEnum_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData) \
    )
static const qt_meta_stringdata_CQSVGEnum_t qt_meta_stringdata_CQSVGEnum = {
    {
QT_MOC_LITERAL(0, 0, 9),
QT_MOC_LITERAL(1, 10, 10),
QT_MOC_LITERAL(2, 21, 9),
QT_MOC_LITERAL(3, 31, 10),
QT_MOC_LITERAL(4, 42, 12),
QT_MOC_LITERAL(5, 55, 10),
QT_MOC_LITERAL(6, 66, 8),
QT_MOC_LITERAL(7, 75, 11),
QT_MOC_LITERAL(8, 87, 12),
QT_MOC_LITERAL(9, 100, 11),
QT_MOC_LITERAL(10, 112, 11),
QT_MOC_LITERAL(11, 124, 12),
QT_MOC_LITERAL(12, 137, 13),
QT_MOC_LITERAL(13, 151, 12),
QT_MOC_LITERAL(14, 164, 13),
QT_MOC_LITERAL(15, 178, 13),
QT_MOC_LITERAL(16, 192, 13),
QT_MOC_LITERAL(17, 206, 8),
QT_MOC_LITERAL(18, 215, 12),
QT_MOC_LITERAL(19, 228, 15),
QT_MOC_LITERAL(20, 244, 15),
QT_MOC_LITERAL(21, 260, 15),
QT_MOC_LITERAL(22, 276, 21),
QT_MOC_LITERAL(23, 298, 23),
QT_MOC_LITERAL(24, 322, 24),
QT_MOC_LITERAL(25, 347, 31)
    },
    "CQSVGEnum\0HAlignType\0AlignLeft\0"
    "AlignRight\0AlignHCenter\0VAlignType\0"
    "AlignTop\0AlignBottom\0AlignVCenter\0"
    "LineCapType\0LineCapButt\0LineCapRound\0"
    "LineCapSquare\0LineJoinType\0LineJoinMitre\0"
    "LineJoinRound\0LineJoinBevel\0FillType\0"
    "FillTypeNone\0FillTypeWinding\0"
    "FillTypeEvenOdd\0ColorMatrixType\0"
    "ColorMatrixTypeMatrix\0ColorMatrixTypeSaturate\0"
    "ColorMatrixTypeHueRotate\0"
    "ColorMatrixTypeLuminanceToAlpha\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_CQSVGEnum[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       0,    0, // methods
       0,    0, // properties
       6,   14, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // enums: name, flags, count, data
       1, 0x0,    3,   38,
       5, 0x0,    3,   44,
       9, 0x0,    3,   50,
      13, 0x0,    3,   56,
      17, 0x0,    3,   62,
      21, 0x0,    4,   68,

 // enum data: key, value
       2, uint(CQSVGEnum::AlignLeft),
       3, uint(CQSVGEnum::AlignRight),
       4, uint(CQSVGEnum::AlignHCenter),
       6, uint(CQSVGEnum::AlignTop),
       7, uint(CQSVGEnum::AlignBottom),
       8, uint(CQSVGEnum::AlignVCenter),
      10, uint(CQSVGEnum::LineCapButt),
      11, uint(CQSVGEnum::LineCapRound),
      12, uint(CQSVGEnum::LineCapSquare),
      14, uint(CQSVGEnum::LineJoinMitre),
      15, uint(CQSVGEnum::LineJoinRound),
      16, uint(CQSVGEnum::LineJoinBevel),
      18, uint(CQSVGEnum::FillTypeNone),
      19, uint(CQSVGEnum::FillTypeWinding),
      20, uint(CQSVGEnum::FillTypeEvenOdd),
      22, uint(CQSVGEnum::ColorMatrixTypeMatrix),
      23, uint(CQSVGEnum::ColorMatrixTypeSaturate),
      24, uint(CQSVGEnum::ColorMatrixTypeHueRotate),
      25, uint(CQSVGEnum::ColorMatrixTypeLuminanceToAlpha),

       0        // eod
};

void CQSVGEnum::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    Q_UNUSED(_o);
    Q_UNUSED(_id);
    Q_UNUSED(_c);
    Q_UNUSED(_a);
}

const QMetaObject CQSVGEnum::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_CQSVGEnum.data,
      qt_meta_data_CQSVGEnum,  qt_static_metacall, 0, 0}
};


const QMetaObject *CQSVGEnum::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CQSVGEnum::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_CQSVGEnum.stringdata))
        return static_cast<void*>(const_cast< CQSVGEnum*>(this));
    return QObject::qt_metacast(_clname);
}

int CQSVGEnum::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    return _id;
}
QT_END_MOC_NAMESPACE
