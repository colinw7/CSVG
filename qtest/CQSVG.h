#ifndef CQSVG_H
#define CQSVG_H

#include <QObject>
#include <QColor>
#include <CSVG.h>

class CQSVGWindow;
class QTimer;

class CQSVG : public QObject, public CSVG {
  Q_OBJECT

  Q_PROPERTY(QColor background   READ background     WRITE setBackground  )
  Q_PROPERTY(bool   animating    READ isAnimating)
  Q_PROPERTY(bool   checkerboard READ isCheckerboard WRITE setCheckerboard)

 public:
  CQSVG(CQSVGWindow *w);

 ~CQSVG();

  CQSVGWindow *window() const { return window_; }

  QColor background() const;
  void setBackground(const QColor &v);

  bool isAnimating() const { return animating_; }
  void setAnimating(bool b) { animating_ = b; }

  bool isCheckerboard() const { return checkerboard_; }
  void setCheckerboard(bool b);

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
  CQSVGWindow *window_       { 0 };
  QTimer      *timer_        { 0 };
  double       t_            { 0 };
  int          dt_           { 100 };
  bool         animating_    { false };
  bool         checkerboard_ { false };
};

#endif
