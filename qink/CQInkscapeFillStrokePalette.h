#ifndef CQInkscapeFillStrokePalette_H
#define CQInkscapeFillStrokePalette_H

#include <CQInkscapePalette.h>
#include <CLineJoinType.h>
#include <CLineCapType.h>

class CSVGObject;

class CQImageButton;
class CQColorChooser;
class CQRealSpin;
class CQLineDash;
class CQLineCap;
class CQLineJoin;
class CLineDash;

class QStackedWidget;
class QCheckBox;
class QComboBox;
class QGridLayout;

namespace CQInkscape {

class Fill;
class StrokePaint;
class StrokeStyle;

class FillStrokePalette : public Palette {
  Q_OBJECT

 public:
  FillStrokePalette(Window *window);

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

 public slots:
  void noneSlot();
  void solidSlot();
  void currentSlot();
  void inheritSlot();
  void lgradSlot();
  void rgradSlot();
  void imageSlot();

  void colorSlot(const QColor &c);
  void opacitySlot(double o);
  void fillRuleSlot(const QString &);
  void clipSlot();

 private:
  struct ModeData {
    CQImageButton* button { nullptr };
    int            ind    { -1 };
    QFrame*        panel  { nullptr };
    QGridLayout*   layout { nullptr };
  };

  using ModeDatas = std::vector<ModeData *>;

 private:
  Window* window_ { nullptr };

  CSVGObject *obj_ { nullptr };

  QStackedWidget *stack_ { nullptr };

  // Mode Data
  ModeData  noneData_;
  ModeData  solidData_;
  ModeData  currentData_;
  ModeData  inheritData_;
  ModeData  lgradData_;
  ModeData  rgradData_;
  ModeData  imageData_;
  ModeDatas modeDatas_;

  // Solid Widgets
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

 public slots:
  void noneSlot();
  void solidSlot();
  void currentSlot();
  void inheritSlot();
  void lgradSlot();
  void rgradSlot();
  void imageSlot();

  void colorSlot(const QColor &c);
  void opacitySlot(double o);

 private:
  struct ModeData {
    CQImageButton* button { nullptr };
    int            ind    { -1 };
    QFrame*        panel  { nullptr };
    QGridLayout*   layout { nullptr };
  };

  using ModeDatas = std::vector<ModeData *>;

 private:
  Window* window_ { nullptr };

  CSVGObject *obj_ { nullptr };

  QStackedWidget *stack_ { nullptr };

  // Mode Data
  ModeData  noneData_;
  ModeData  solidData_;
  ModeData  currentData_;
  ModeData  inheritData_;
  ModeData  lgradData_;
  ModeData  rgradData_;
  ModeData  imageData_;
  ModeDatas modeDatas_;

  // Solid Widgets
  CQColorChooser* colorChooser_ { nullptr };
  CQRealSpin*     opacityEdit_  { nullptr };
};

//---

class StrokeStyle : public QFrame {
  Q_OBJECT

 public:
  StrokeStyle(Window *window);

  void setObject(CSVGObject *obj);

 private slots:
  void widthSlot(double);
  void dashSlot(const CLineDash &);
  void capSlot(CLineCapType);
  void joinSlot(CLineJoinType);
  void mitreSlot(double);

 private:
  Window* window_ { nullptr };

  CSVGObject *obj_ { nullptr };

  // Stroke Style Widgets
  CQRealSpin* widthEdit_ { nullptr };
  CQLineDash* dashEdit_  { nullptr };
  CQLineCap*  capEdit_   { nullptr };
  CQLineJoin* joinEdit_  { nullptr };
  CQRealSpin* mitreEdit_ { nullptr };
};

}

#endif
