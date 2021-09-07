#ifndef CQInkscapeFillStroke_H
#define CQInkscapeFillStroke_H

#include <CQInkscapePalette.h>

class CSVGObject;

class CQImageButton;
class CQColorChooser;
class CQRealSpin;
class CQLineDash;
class CQLineCap;
class CQLineJoin;

class QStackedWidget;
class QCheckBox;
class QComboBox;

namespace CQInkscape {

class Fill;
class StrokePaint;
class StrokeStyle;

class FillStroke : public Palette {
  Q_OBJECT

 public:
  FillStroke(Window *window);

  void setObject(CSVGObject *obj);

 private:
  Fill*        fill_        { nullptr };
  StrokePaint* strokePaint_ { nullptr };
  StrokeStyle* strokeStyle_ { nullptr };
};

//---

class Fill : public QFrame {
  Q_OBJECT

 public:
  Fill(Window *window);

  void setObject(CSVGObject *obj);

 private:
  Window* window_ { nullptr };

  // Mode Buttons
  CQImageButton *noneButton_    { nullptr };
  CQImageButton *solidButton_   { nullptr };
  CQImageButton *currentButton_ { nullptr };
  CQImageButton *inheritButton_ { nullptr };
  CQImageButton *lgradButton_   { nullptr };
  CQImageButton *rgradButton_   { nullptr };
  CQImageButton *imageButton_   { nullptr };

  QStackedWidget *stack_ { nullptr };

  // Flat Widgets
  CQColorChooser* colorChooser_ { nullptr };
  CQRealSpin*     opacityEdit_  { nullptr };
  QComboBox*      fillRule_     { nullptr };
  QCheckBox*      clipCheck_    { nullptr };
};

//---

class StrokePaint : public QFrame {
  Q_OBJECT

 public:
  StrokePaint(Window *window);

  void setObject(CSVGObject *obj);

 private:
  Window* window_ { nullptr };

  // Stroke Paint Widgets
  QCheckBox*      shownCheck_   { nullptr };
  CQColorChooser* colorChooser_ { nullptr };
  CQRealSpin*     opacityEdit_  { nullptr };
};

//---

class StrokeStyle : public QFrame {
  Q_OBJECT

 public:
  StrokeStyle(Window *window);

  void setObject(CSVGObject *obj);

 private:
  Window* window_ { nullptr };

  // Stroke Style Widgets
  CQRealSpin* widthEdit_ { nullptr };
  CQLineDash* dashEdit_  { nullptr };
  CQLineCap*  capEdit_   { nullptr };
  CQLineJoin* joinEdit_  { nullptr };
  CQRealSpin* mitreEdit_ { nullptr };
};

}

#endif
