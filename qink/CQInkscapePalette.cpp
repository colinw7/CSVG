#include <CQInkscapePalette.h>
#include <CQInkscapeWindow.h>

namespace CQInkscape {

Palette::
Palette(Window *window, const QString &name) :
 QFrame(window), window_(window), name_(name)
{
}

}
