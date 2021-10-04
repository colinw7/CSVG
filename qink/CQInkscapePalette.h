#ifndef CQInkscapePalette_H
#define CQInkscapePalette_H

#include <QFrame>

namespace CQInkscape {

class Window;

class Palette : public QFrame {
  Q_OBJECT

 public:
  Palette(Window *window, const QString &name);

  Window *window() const { return window_; }

  QString name() const { return name_; }

 protected:
  Window*  window_ { nullptr };
  QString  name_;
};

}

#endif
