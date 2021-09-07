#include <CQInkscapeObjectProperties.h>

#include <CSVGObject.h>

#include <QVBoxLayout>

namespace CQInkscape {

ObjectProperties::
ObjectProperties(Window *window) :
 Palette(window, "Object Properties")
{
  //auto *layout = CQUtil::makeLayout<QVBoxLayout>(this, 2, 2);
}

void
ObjectProperties::
setObject(CSVGObject *obj)
{
  object_ = obj;
}

}
