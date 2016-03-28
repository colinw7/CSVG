#ifndef CSVGTypes_H
#define CSVGTypes_H

enum class CSVGScale {
  FREE,
  FIXED_MEET,
  FIXED_SLICE
};

enum class CSVGCoordUnits {
  OBJECT_BBOX,
  USER_SPACE
};

enum class CSVGEventType {
  CLICK,
  MOUSE_DOWN,
  MOUSE_UP,
  MOUSE_OVER,
  MOUSE_OUT,
  ANIMATE_BEGIN,
  ANIMATE_END,
  ANIMATE_REPEAT,
  TIMEOUT
};

#endif
