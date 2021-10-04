#ifndef CQInkscapeTransformPalette_H
#define CQInkscapeTransformPalette_H

#include <CQInkscapePalette.h>
#include <CLineJoinType.h>
#include <CLineCapType.h>

class CSVGObject;

class CQTabWidget;
class CQRealSpin;
class CQAngleSpinBox;
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

 private slots:
  void clearSlot();
  void applySlot();

 private:
  MoveTransform*   move_      { nullptr };
  ScaleTransform*  scale_     { nullptr };
  RotateTransform* rotate_    { nullptr };
  SkewTransform*   skew_      { nullptr };
  MatrixTransform* matrix_    { nullptr };
  CQTabWidget*     tab_       { nullptr };
  QCheckBox*       eachCheck_ { nullptr };
};

//---

class TransformFrame : public QFrame {
  Q_OBJECT

 public:
  TransformFrame(Window *window);

  virtual void clear() = 0;
  virtual void apply() = 0;

 protected:
  Window*     window_ { nullptr };
  CSVGObject* object_ { nullptr };
};

//---

class MoveTransform : public TransformFrame {
  Q_OBJECT

 public:
  MoveTransform(Window *window);

  void setObject(CSVGObject *obj);

  void clear() override;
  void apply() override;

 private slots:
  void relSlot(int);

 private:
  void updateState();

 private:
  CQRealSpin* hEdit_    { nullptr };
  CQRealSpin* vEdit_    { nullptr };
  QCheckBox*  relCheck_ { nullptr };
};

//---

class ScaleTransform : public TransformFrame {
  Q_OBJECT

 public:
  ScaleTransform(Window *window);

  void setObject(CSVGObject *obj);

  void clear() override;
  void apply() override;

 private:
  void updateState();

 private:
  CQRealSpin* wEdit_     { nullptr };
  CQRealSpin* hEdit_     { nullptr };
  QCheckBox*  propCheck_ { nullptr };
};

//---

class RotateTransform : public TransformFrame {
  Q_OBJECT

 public:
  RotateTransform(Window *window);

  void setObject(CSVGObject *obj);

  void clear() override;
  void apply() override;

 private:
  CQAngleSpinBox* aEdit_  { nullptr };
};

//---

class SkewTransform : public TransformFrame {
  Q_OBJECT

 public:
  SkewTransform(Window *window);

  void setObject(CSVGObject *obj);

  void clear() override;
  void apply() override;

 private:
  CQRealSpin* hEdit_  { nullptr };
  CQRealSpin* vEdit_  { nullptr };
};

//---

class MatrixTransform : public TransformFrame {
  Q_OBJECT

 public:
  MatrixTransform(Window *window);

  void setObject(CSVGObject *obj);

  void clear() override;
  void apply() override;
};

}

#endif
