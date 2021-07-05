#include <CQSVGEnum.h>
#include <CEnumConv.h>

namespace Conv {

using HAlignConv = CEnumConv<CHAlignType, CQSVGEnum::HAlignType>;

HAlignConv halignConv({
 { CHALIGN_TYPE_LEFT  , CQSVGEnum::AlignLeft    },
 { CHALIGN_TYPE_RIGHT , CQSVGEnum::AlignRight   },
 { CHALIGN_TYPE_CENTER, CQSVGEnum::AlignHCenter }
});

//---

using VAlignConv = CEnumConv<CVAlignType, CQSVGEnum::VAlignType>;

VAlignConv valignConv({
 { CVALIGN_TYPE_BOTTOM, CQSVGEnum::AlignBottom  },
 { CVALIGN_TYPE_TOP   , CQSVGEnum::AlignTop     },
 { CVALIGN_TYPE_CENTER, CQSVGEnum::AlignVCenter }
});

//---

using LineCapConv = CEnumConv<CLineCapType, CQSVGEnum::LineCapType>;

LineCapConv lineCapConv({
 { LINE_CAP_TYPE_BUTT  , CQSVGEnum::LineCapButt   },
 { LINE_CAP_TYPE_ROUND , CQSVGEnum::LineCapRound  },
 { LINE_CAP_TYPE_SQUARE, CQSVGEnum::LineCapSquare }
});

//---

using LineJoinConv = CEnumConv<CLineJoinType, CQSVGEnum::LineJoinType>;

LineJoinConv lineJoinConv({
 { LINE_JOIN_TYPE_MITRE, CQSVGEnum::LineJoinMitre },
 { LINE_JOIN_TYPE_ROUND, CQSVGEnum::LineJoinRound },
 { LINE_JOIN_TYPE_BEVEL, CQSVGEnum::LineJoinBevel }
});

//---

using FillTypeConv = CEnumConv<CFillType, CQSVGEnum::FillType>;

FillTypeConv fillTypeConv({
 { FILL_TYPE_NONE    , CQSVGEnum::FillTypeNone    },
 { FILL_TYPE_WINDING , CQSVGEnum::FillTypeWinding },
 { FILL_TYPE_EVEN_ODD, CQSVGEnum::FillTypeEvenOdd }
});

//---

using ColorMatrixTypeConv = CEnumConv<CSVGColorMatrixType, CQSVGEnum::ColorMatrixType>;

ColorMatrixTypeConv colorMatrixTypeConv({
 { CSVGColorMatrixType::MATRIX            , CQSVGEnum::ColorMatrixTypeMatrix           },
 { CSVGColorMatrixType::SATURATE          , CQSVGEnum::ColorMatrixTypeSaturate         },
 { CSVGColorMatrixType::HUE_ROTATE        , CQSVGEnum::ColorMatrixTypeHueRotate        },
 { CSVGColorMatrixType::LUMINANCE_TO_ALPHA, CQSVGEnum::ColorMatrixTypeLuminanceToAlpha }
});

//---

using CoordUnitsTypeConv = CEnumConv<CSVGCoordUnits, CQSVGEnum::CoordUnitsType>;

CoordUnitsTypeConv coordUnitsTypeConv({
 { CSVGCoordUnits::OBJECT_BBOX , CQSVGEnum::CoordUnitsObjectBBox  },
 { CSVGCoordUnits::USER_SPACE  , CQSVGEnum::CoordUnitsUserSpace   },
 { CSVGCoordUnits::STROKE_WIDTH, CQSVGEnum::CoordUnitsStrokeWidth }
});

//---

}

//---

CQSVGEnum::HAlignType
CQSVGEnum::
halignConv(const CHAlignType &type)
{
  return Conv::halignConv.conv(type);
}

CHAlignType
CQSVGEnum::
halignConv(const HAlignType &type)
{
  return Conv::halignConv.conv(type);
}

//---

CQSVGEnum::VAlignType
CQSVGEnum::
valignConv(const CVAlignType &type)
{
  return Conv::valignConv.conv(type);
}

CVAlignType
CQSVGEnum::
valignConv(const VAlignType &type)
{
  return Conv::valignConv.conv(type);
}

//---

CQSVGEnum::LineCapType
CQSVGEnum::
lineCapConv(const CLineCapType &type)
{
  return Conv::lineCapConv.conv(type);
}

CLineCapType
CQSVGEnum::
lineCapConv(const LineCapType &type)
{
  return Conv::lineCapConv.conv(type);
}

//---

CQSVGEnum::LineJoinType
CQSVGEnum::
lineJoinConv(const CLineJoinType &type)
{
  return Conv::lineJoinConv.conv(type);
}

CLineJoinType
CQSVGEnum::
lineJoinConv(const LineJoinType &type)
{
  return Conv::lineJoinConv.conv(type);
}

//---

CQSVGEnum::FillType
CQSVGEnum::
fillTypeConv(const CFillType &type)
{
  return Conv::fillTypeConv.conv(type);
}

CFillType
CQSVGEnum::
fillTypeConv(const FillType &type)
{
  return Conv::fillTypeConv.conv(type);
}

//---

CQSVGEnum::ColorMatrixType
CQSVGEnum::
colorMatrixTypeConv(const CSVGColorMatrixType &type)
{
  return Conv::colorMatrixTypeConv.conv(type);
}

CSVGColorMatrixType
CQSVGEnum::
colorMatrixTypeConv(const ColorMatrixType &type)
{
  return Conv::colorMatrixTypeConv.conv(type);
}

//---

CQSVGEnum::CoordUnitsType
CQSVGEnum::
coordUnitsTypeConv(const CSVGCoordUnits &type)
{
  return Conv::coordUnitsTypeConv.conv(type);
}

CSVGCoordUnits
CQSVGEnum::
coordUnitsTypeConv(const CoordUnitsType &type)
{
  return Conv::coordUnitsTypeConv.conv(type);
}
