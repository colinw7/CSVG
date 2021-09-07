#ifndef CQInkscapeSettings_H
#define CQInkscapeSettings_H

#include <QDialog>

class QCheckBox;

namespace CQInkscape {

class Window;

//---

class Settings : public QDialog {
  Q_OBJECT

 public:
  Settings(Window *window);

  void updateState();

  void accept() override;

 private slots:
  void doneSlot();

 private:
  Window* window_ { nullptr };

  QCheckBox *checkerboardCheck_ { nullptr };
  QCheckBox *viewBoxCheck_      { nullptr };
  QCheckBox *consoleCheck_      { nullptr };
  QCheckBox *bufferViewCheck_   { nullptr };
};

}

#endif
