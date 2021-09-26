#ifndef CQInkscapeTransformPalette_H
#define CQInkscapeTransformPalette_H

#include <CQInkscapePalette.h>
#include <CLineJoinType.h>
#include <CLineCapType.h>

class CSVGObject;

class CQRealSpin;
class QCheckBox;

namespace CQInkscape {

class MoveTransform;
class ScaleTransform;
class RotateTransform;
class SkewTransform;
class MatrixTransform;

class TransformPalette : public Palette {
  Q_OBJECT

 public:
  TransformPalette(Window *window);

  void setObject(CSVGObject *obj);

 private:
  MoveTransform*   move_   { nullptr };
  ScaleTransform*  scale_  { nullptr };
  RotateTransform* rotate_ { nullptr };
  SkewTransform*   skew_   { nullptr };
  MatrixTransform* matrix_ { nullptr };
};

//---

class MoveTransform : public QFrame {
  Q_OBJECT

 public:
  MoveTransform(Window *window);

  void setObject(CSVGObject *obj);

 private:
  Window*     window_   { nullptr };
  CQRealSpin* hEdit_    { nullptr };
  CQRealSpin* vEdit_    { nullptr };
  QCheckBox*  relCheck_ { nullptr };
};

//---

class ScaleTransform : public QFrame {
  Q_OBJECT

 public:
  ScaleTransform(Window *window);

  void setObject(CSVGObject *obj);

 private:
  Window*     window_    { nullptr };
  CQRealSpin* wEdit_     { nullptr };
  CQRealSpin* hEdit_     { nullptr };
  QCheckBox*  propCheck_ { nullptr };
};

//---

class RotateTransform : public QFrame {
  Q_OBJECT

 public:
  RotateTransform(Window *window);

  void setObject(CSVGObject *obj);

 private:
  Window*     window_ { nullptr };
  CQRealSpin* aEdit_  { nullptr };
};

//---

class SkewTransform : public QFrame {
  Q_OBJECT

 public:
  SkewTransform(Window *window);

  void setObject(CSVGObject *obj);

 private:
  Window*     window_ { nullptr };
  CQRealSpin* hEdit_  { nullptr };
  CQRealSpin* vEdit_  { nullptr };
};

//---

class MatrixTransform : public QFrame {
  Q_OBJECT

 public:
  MatrixTransform(Window *window);

  void setObject(CSVGObject *obj);

 private:
  Window* window_ { nullptr };
};

}

#endif
