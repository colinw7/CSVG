#ifndef CSVGTypes_H
#define CSVGTypes_H

enum class CSVGObjTypeId {
  ANCHOR=1,
  ANIMATE,
  ANIMATE_BASE,
  ANIMATE_COLOR,
  ANIMATE_MOTION,
  ANIMATE_TRANSFORM,
  BLOCK,
  CIRCLE,
  CLIP_PATH,
  COLOR_PROFILE,
  DEFS,
  DESC,
  ELLIPSE,
  FE_BLEND,
  FE_COLOR_MATRIX,
  FE_COMPONENT_TRANSFER,
  FE_COMPOSITE,
  FE_CONVOLVE_MATRIX,
  FE_DIFFUSE_LIGHTING,
  FE_DISPLACEMENT_MAP,
  FE_DISTANT_LIGHT,
  FE_FLOOD,
  FE_FUNC,
  FE_GAUSSIAN_BLUR,
  FE_IMAGE,
  FE_MERGE,
  FE_MERGE_NODE,
  FE_MORPHOLOGY,
  FE_OFFSET,
  FE_POINT_LIGHT,
  FE_SPECULAR_LIGHTING,
  FE_SPOT_LIGHT,
  FE_TILE,
  FE_TURBULENCE,
  FILTER,
  FONT,
  FONT_FACE,
  FONT_FACE_SRC,
  FONT_FACE_URI,
  GLYPH,
  GROUP,
  HKERN,
  IMAGE,
  LINEAR_GRADIENT,
  LINE,
  MARKER,
  MASK,
  METADATA,
  MISSING_GLYPH,
  MPATH,
  PATH,
  PATTERN,
  POLYGON,
  POLYLINE,
  RADIAL_GRADIENT,
  RECT,
  SCRIPT,
  SET,
  STOP,
  STYLE,
  SWITCH,
  SYMBOL,
  TEXT,
  TEXT_PATH,
  TITLE,
  TSPAN,
  USE
};

enum class CSVGScale {
  FREE,
  FIXED_MEET,
  FIXED_SLICE
};

enum class CSVGCoordUnits {
  OBJECT_BBOX,
  USER_SPACE,
  STROKE_WIDTH
};

enum class CSVGEventType {
  LOAD,
  CLICK,
  BEGIN,
  END,
  MOUSE_DOWN,
  MOUSE_UP,
  MOUSE_OVER,
  MOUSE_OUT,
  ANIMATE_BEGIN,
  ANIMATE_END,
  ANIMATE_REPEAT,
  TIMEOUT
};

enum class CSVGTextDecoration {
  NONE,
  UNDERLINE,
  OVERLINE,
  LINE_THROUGH
};

enum class CSVGColorMatrixType {
  MATRIX,
  SATURATE,
  HUE_ROTATE,
  LUMINANCE_TO_ALPHA
};

enum class CSVGBlendMode {
  NORMAL,
  MULTIPLY,
  SCREEN,
  DARKEN,
  LIGHTEN
};

enum class CSVGFilterFuncType {
  IDENTITY,
  LINEAR,
  GAMMA,
  TABLE,
  DISCRETE
};

enum class CSVGMorphologyOperator {
  ERODE,
  DILATE
};

enum class CSVGLengthType {
  UNKNOWN = 0,
  NUMBER = 1,
  PERCENTAGE = 2,
  EMS = 3,
  EXS = 4,
  PX = 5,
  CM = 6,
  MM = 7,
  IN = 8,
  PT = 9,
  PC = 10
};

enum class CSVGPathPartType {
  MOVE_TO,
  RMOVE_TO,
  LINE_TO,
  RLINE_TO,
  HLINE_TO,
  RHLINE_TO,
  VLINE_TO,
  RVLINE_TO,
  ARC_TO,
  RARC_TO,
  BEZIER2_TO,
  MBEZIER2_TO,
  RBEZIER2_TO,
  MRBEZIER2_TO,
  BEZIER3_TO,
  MBEZIER3_TO,
  RBEZIER3_TO,
  MRBEZIER3_TO,
  CLOSE_PATH
};

enum class CSVGStyleType {
  NONE,
  TEXT_CSS
};

enum class CSVGTimeValueType {
  NONE,
  HOURS,
  MINUTES,
  SECONDS,
  MILLISECONDS
};

enum class CSVGCSSType {
  NONE,
  GLOBAL,
  CLASS,
  TYPE,
  TYPE_CLASS,
  NAME
};

enum class CSVGEnableBackground {
  ACCUMULATE,
  NEW
};

enum class CSVGOverflowType {
  VISIBLE,
  HIDDEN,
  SCROLL,
  AUTO,
  INHERIT
};

enum class CSVGAngleType {
  UNKNOWN,
  UNSPECIFIED,
  DEG,
  RAD,
  GRAD
};

enum class CSVGPreserveAspectAlign {
  UNKNOWN  = 0,
  NONE     = 1,
  XMINYMIN = 2,
  XMIDYMIN = 3,
  XMAXYMIN = 4,
  XMINYMID = 5,
  XMIDYMID = 6,
  XMAXYMID = 7,
  XMINYMAX = 8,
  XMIDYMAX = 9,
  XMAXYMAX = 10
};

enum class CSVGPreserveAspectMeetOrSlice {
  UNKNOWN = 0,
  MEET    = 1,
  SLICE   = 2
};

//------

#include <COptVal.h>
#include <CScreenUnits.h>

typedef COptValT<CScreenUnits> COptScreenUnits;

#endif
