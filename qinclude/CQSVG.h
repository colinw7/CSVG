#ifndef CQSVG_H
#define CQSVG_H

#include <QObject>
#include <QColor>
#include <CSVG.h>

class CQSVGWindow;
class QTimer;

class CQSVG : public QObject, public CSVG {
  Q_OBJECT

  Q_PROPERTY(QColor background    READ background      WRITE setBackground  )
  Q_PROPERTY(bool   animating     READ isAnimating)
  Q_PROPERTY(bool   checkerboard  READ isCheckerboard  WRITE setCheckerboard)
  Q_PROPERTY(bool   showGradient  READ isShowGradient  WRITE setShowGradient)
  Q_PROPERTY(bool   showFilterBox READ isShowFilterBox WRITE setShowFilterBox)
  Q_PROPERTY(bool   ignoreFilter  READ getIgnoreFilter WRITE setIgnoreFilter)

 public:
  CQSVG(CQSVGWindow *w=nullptr);

 ~CQSVG();

  CSVG *dup() const override;

  //---

  CQSVGWindow *window() const { return window_; }

  QColor background() const;
  void setBackground(const QColor &v);

  bool isAnimating() const { return animating_; }
  void setAnimating(bool b) { animating_ = b; }

  bool isCheckerboard() const { return checkerboard_; }
  void setCheckerboard(bool b);

  bool isShowGradient() const { return showGradient_; }
  void setShowGradient(bool b) { showGradient_ = b; }

  bool isShowFilterBox() const { return showFilterBox_; }
  void setShowFilterBox(bool b) { showFilterBox_ = b; }

  void updateBusy() override;

  //---

  CSVGBlock              *createBlock() override;
  CSVGAnchor             *createAnchor() override;
  CSVGAnimate            *createAnimate() override;
  CSVGAnimateColor       *createAnimateColor() override;
  CSVGAnimateMotion      *createAnimateMotion() override;
  CSVGAnimateTransform   *createAnimateTransform() override;
  CSVGCircle             *createCircle() override;
  CSVGClipPath           *createClipPath() override;
  CSVGDefs               *createDefs() override;
  CSVGDesc               *createDesc() override;
  CSVGEllipse            *createEllipse() override;
  CSVGFeBlend            *createFeBlend() override;
  CSVGFeColorMatrix      *createFeColorMatrix() override;
  CSVGFeComposite        *createFeComposite() override;
  CSVGFeDiffuseLighting  *createFeDiffuseLighting() override;
  CSVGFeDistantLight     *createFeDistantLight() override;
  CSVGFeDisplacementMap  *createFeDisplacementMap() override;
  CSVGFeFlood            *createFeFlood() override;
  CSVGFeGaussianBlur     *createFeGaussianBlur() override;
  CSVGFeImage            *createFeImage() override;
  CSVGFeMerge            *createFeMerge() override;
  CSVGFeMergeNode        *createFeMergeNode() override;
  CSVGFeMorphology       *createFeMorphology() override;
  CSVGFeOffset           *createFeOffset() override;
  CSVGFePointLight       *createFePointLight() override;
  CSVGFeSpecularLighting *createFeSpecularLighting() override;
  CSVGFeSpotLight        *createFeSpotLight() override;
  CSVGFeTile             *createFeTile() override;
  CSVGFeTurbulence       *createFeTurbulence() override;
  CSVGFilter             *createFilter() override;
  CSVGGroup              *createGroup() override;
  CSVGImage              *createImage() override;
  CSVGLine               *createLine() override;
  CSVGLinearGradient     *createLinearGradient() override;
  CSVGMarker             *createMarker() override;
  CSVGMask               *createMask() override;
  CSVGMPath              *createMPath() override;
  CSVGPath               *createPath() override;
  CSVGPattern            *createPattern() override;
  CSVGPolygon            *createPolygon() override;
  CSVGPolyLine           *createPolyLine() override;
  CSVGRadialGradient     *createRadialGradient() override;
  CSVGRect               *createRect() override;
  CSVGSet                *createSet() override;
  CSVGStop               *createStop() override;
  CSVGSymbol             *createSymbol() override;
  CSVGText               *createText() override;
  CSVGTextPath           *createTextPath() override;
  CSVGTitle              *createTitle() override;
  CSVGTSpan              *createTSpan() override;
  CSVGUse                *createUse() override;

  CSVGImageData *createImageData() override;
  CSVGFontObj*   createFontObj(const CSVGFontDef &def) override;

  CSVGPathMoveTo   *createPathMoveTo  (double x, double y) override;
  CSVGPathRMoveTo  *createPathRMoveTo (double x, double y) override;
  CSVGPathLineTo   *createPathLineTo  (double x, double y) override;
  CSVGPathRLineTo  *createPathRLineTo (double x, double y) override;
  CSVGPathHLineTo  *createPathHLineTo (double x) override;
  CSVGPathRHLineTo *createPathRHLineTo(double x) override;
  CSVGPathVLineTo  *createPathVLineTo (double y) override;
  CSVGPathRVLineTo *createPathRVLineTo(double y) override;

  CSVGPathArcTo  *createPathArcTo (double rx, double ry, double xa, double fa,
                                   double fs, double x2, double y2) override;
  CSVGPathRArcTo *createPathRArcTo(double rx, double ry, double xa, double fa,
                                   double fs, double x2, double y2) override;

  CSVGPathBezier2To   *createPathBezier2To  (double x1, double y1, double x2, double y2) override;
  CSVGPathMBezier2To  *createPathMBezier2To (double x2, double y2) override;
  CSVGPathRBezier2To  *createPathRBezier2To (double x1, double y1, double x2, double y2) override;
  CSVGPathMRBezier2To *createPathMRBezier2To(double x2, double y2) override;
  CSVGPathBezier3To   *createPathBezier3To  (double x1, double y1, double x2, double y2,
                                                     double x3, double y3) override;
  CSVGPathMBezier3To  *createPathMBezier3To (double x2, double y2, double x3, double y3) override;
  CSVGPathRBezier3To  *createPathRBezier3To (double x1, double y1, double x2, double y2,
                                                     double x3, double y3) override;
  CSVGPathMRBezier3To *createPathMRBezier3To(double x2, double y2, double x3, double y3) override;

  CSVGPathClosePath *createPathClosePath(bool relative) override;

  //---

  void startTimer();
  void stopTimer ();
  void stepTimer ();
  void bstepTimer();

  void setTime(double t);

  void tickStep(int n=1);

  void redraw() override;

 private slots:
  void tickSlot();

 private:
  CQSVGWindow *window_        { 0 };
  QTimer      *timer_         { 0 };
  double       t_             { 0 };
  int          dt_            { 100 };
  bool         animating_     { false };
  bool         checkerboard_  { false };
  bool         showGradient_  { true };
  bool         showFilterBox_ { true };
};

#endif
