#ifndef CQInkscapeObjectProperties_H
#define CQInkscapeObjectProperties_H

#include <CQInkscapePalette.h>

class CSVGObject;

class QLineEdit;

namespace CQInkscape {

class ObjectProperties : public Palette {
  Q_OBJECT

 public:
  ObjectProperties(Window *window);

  void setObject(CSVGObject *obj);

 private slots:
  void setSlot();

 private:
  CSVGObject* object_    { nullptr };
  QLineEdit*  idEdit_    { nullptr };
  QLineEdit*  labelEdit_ { nullptr };
  QLineEdit*  titleEdit_ { nullptr };
};

}

#endif
