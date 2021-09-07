#ifndef CQInkscapeObjectProperties_H
#define CQInkscapeObjectProperties_H

#include <CQInkscapePalette.h>

class CSVGObject;

namespace CQInkscape {

class ObjectProperties : public Palette {
  Q_OBJECT

 public:
  ObjectProperties(Window *window);

  void setObject(CSVGObject *obj);

 private:
  CSVGObject *object_ { nullptr };
};

}

#endif
