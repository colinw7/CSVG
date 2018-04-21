#ifndef CQSVGEnum_H
#define CQSVGEnum_H

#include <CAlignType.h>
#include <CLineCapType.h>
#include <CLineJoinType.h>
#include <CFillType.h>
#include <CSVGFeColorMatrix.h>
#include <QObject>

class CQSVGEnum : public QObject {
  Q_OBJECT

  Q_ENUMS(HAlignType)
  Q_ENUMS(VAlignType)
  Q_ENUMS(LineCapType)
  Q_ENUMS(LineJoinType)
  Q_ENUMS(FillType)
  Q_ENUMS(ColorMatrixType)
  Q_ENUMS(CoordUnitsType)

 public:
  enum HAlignType {
    AlignLeft,
    AlignRight,
    AlignHCenter
  };

  enum VAlignType {
    AlignTop,
    AlignBottom,
    AlignVCenter
  };

  enum LineCapType {
    LineCapButt,
    LineCapRound,
    LineCapSquare
  };

  enum LineJoinType {
    LineJoinMitre,
    LineJoinRound,
    LineJoinBevel
  };

  enum FillType {
    FillTypeNone,
    FillTypeWinding,
    FillTypeEvenOdd
  };

  enum ColorMatrixType {
    ColorMatrixTypeMatrix,
    ColorMatrixTypeSaturate,
    ColorMatrixTypeHueRotate,
    ColorMatrixTypeLuminanceToAlpha
  };

  enum CoordUnitsType {
    CoordUnitsObjectBBox,
    CoordUnitsUserSpace,
    CoordUnitsStrokeWidth
  };

 public:
  CQSVGEnum() { }

  //---

  const HAlignType &halignType() { return halignType_; }
  void setHAlignType(const HAlignType &v) { halignType_ = v; }

  static HAlignType halignConv(const CHAlignType &type);
  static CHAlignType halignConv(const HAlignType &type);

  //---

  const VAlignType &valignType() { return valignType_; }
  void setVAlignType(const VAlignType &v) { valignType_ = v; }

  static VAlignType valignConv(const CVAlignType &type);
  static CVAlignType valignConv(const VAlignType &type);

  //---

  const LineCapType &lineCapType() { return lineCapType_; }
  void setLineCapType(const LineCapType &v) { lineCapType_ = v; }

  static LineCapType lineCapConv(const CLineCapType &type);
  static CLineCapType lineCapConv(const LineCapType &type);

  //---

  const LineJoinType &lineJoinType() { return lineJoinType_; }
  void setLineJoinType(const LineJoinType &v) { lineJoinType_ = v; }

  static LineJoinType lineJoinConv(const CLineJoinType &type);
  static CLineJoinType lineJoinConv(const LineJoinType &type);

  //---

  const FillType &fillType() { return fillType_; }
  void setFillType(const FillType &v) { fillType_ = v; }

  static FillType fillTypeConv(const CFillType &type);
  static CFillType fillTypeConv(const FillType &type);

  //---

  const ColorMatrixType &colorMatrixType() { return colorMatrixType_; }
  void setColorMatrixType(const ColorMatrixType &type) { colorMatrixType_ = type; }

  static ColorMatrixType colorMatrixTypeConv(const CSVGColorMatrixType &type);
  static CSVGColorMatrixType colorMatrixTypeConv(const ColorMatrixType &type);

  //---

  const CoordUnitsType &coordUnitsType() { return coordUnitsType_; }
  void setCoordUnitsType(const CoordUnitsType &type) { coordUnitsType_ = type; }

  static CoordUnitsType coordUnitsTypeConv(const CSVGCoordUnits &type);
  static CSVGCoordUnits coordUnitsTypeConv(const CoordUnitsType &type);

  //---

 private:
  HAlignType      halignType_;
  VAlignType      valignType_;
  LineCapType     lineCapType_;
  LineJoinType    lineJoinType_;
  FillType        fillType_;
  ColorMatrixType colorMatrixType_;
  CoordUnitsType  coordUnitsType_;
};

#endif
