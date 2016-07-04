#include <CQSVG.h>
#include <CQSVGAnchor.h>
#include <CQSVGAnimateColor.h>
#include <CQSVGAnimate.h>
#include <CQSVGAnimateMotion.h>
#include <CQSVGAnimateTransform.h>
#include <CQSVGBlock.h>
#include <CQSVGCircle.h>
#include <CQSVGClipPath.h>
#include <CQSVGDefs.h>
#include <CQSVGDesc.h>
#include <CQSVGEllipse.h>
#include <CQSVGFeBlend.h>
#include <CQSVGFeColorMatrix.h>
#include <CQSVGFeComposite.h>
#include <CQSVGFeDiffuseLighting.h>
#include <CQSVGFeDistantLight.h>
#include <CQSVGFeDisplacementMap.h>
#include <CQSVGFeFlood.h>
#include <CQSVGFeGaussianBlur.h>
#include <CQSVGFeImage.h>
#include <CQSVGFeMerge.h>
#include <CQSVGFeMergeNode.h>
#include <CQSVGFeMorphology.h>
#include <CQSVGFeOffset.h>
#include <CQSVGFePointLight.h>
#include <CQSVGFeSpecularLighting.h>
#include <CQSVGFeSpotLight.h>
#include <CQSVGFeTurbulence.h>
#include <CQSVGFeTile.h>
#include <CQSVGFilter.h>
#include <CQSVGGroup.h>
#include <CQSVGImage.h>
#include <CQSVGImageData.h>
#include <CQSVGLine.h>
#include <CQSVGLinearGradient.h>
#include <CQSVGMarker.h>
#include <CQSVGMask.h>
#include <CQSVGMPath.h>
#include <CQSVGPath.h>
#include <CQSVGPattern.h>
#include <CQSVGPolyLine.h>
#include <CQSVGPolygon.h>
#include <CQSVGRadialGradient.h>
#include <CQSVGRect.h>
#include <CQSVGSet.h>
#include <CQSVGStop.h>
#include <CQSVGSymbol.h>
#include <CQSVGText.h>
#include <CQSVGTextPath.h>
#include <CQSVGTitle.h>
#include <CQSVGTSpan.h>
#include <CQSVGUse.h>
#include <CQSVGWindow.h>
#include <CQSVGPathPart.h>
#include <CQSVGFontObj.h>
#include <QTimer>

CQSVG::
CQSVG(CQSVGWindow *window) :
 window_(window)
{
  timer_ = new QTimer;

  connect(timer_, SIGNAL(timeout()), this, SLOT(tickSlot()));
}

CQSVG::
~CQSVG()
{
  delete timer_;
}

CSVG *
CQSVG::
dup() const
{
  return new CQSVG(window_);
}

QColor
CQSVG::
background() const
{
  return CQUtil::rgbaToColor(CSVG::background());
}

void
CQSVG::
setBackground(const QColor &c)
{
  CSVG::setBackground(CQUtil::colorToRGBA(c));
}

void
CQSVG::
setCheckerboard(bool b)
{
  checkerboard_ = b;
}

void
CQSVG::
updateBusy()
{
  window_->updateBusy();
}

CSVGBlock *
CQSVG::
createBlock()
{
  return new CQSVGBlock(this);
}

CSVGAnchor *
CQSVG::
createAnchor()
{
  return new CQSVGAnchor(this);
}

CSVGAnimate *
CQSVG::
createAnimate()
{
  return new CQSVGAnimate(this);
}

CSVGAnimateColor *
CQSVG::
createAnimateColor()
{
  return new CQSVGAnimateColor(this);
}

CSVGAnimateMotion *
CQSVG::
createAnimateMotion()
{
  return new CQSVGAnimateMotion(this);
}

CSVGAnimateTransform *
CQSVG::
createAnimateTransform()
{
  return new CQSVGAnimateTransform(this);
}

CSVGCircle *
CQSVG::
createCircle()
{
  return new CQSVGCircle(this);
}

CSVGClipPath *
CQSVG::
createClipPath()
{
  return new CQSVGClipPath(this);
}

CSVGDefs *
CQSVG::
createDefs()
{
  return new CQSVGDefs(this);
}

CSVGDesc *
CQSVG::
createDesc()
{
  return new CQSVGDesc(this);
}

CSVGEllipse *
CQSVG::
createEllipse()
{
  return new CQSVGEllipse(this);
}

CSVGFeBlend *
CQSVG::
createFeBlend()
{
  return new CQSVGFeBlend(this);
}

CSVGFeColorMatrix *
CQSVG::
createFeColorMatrix()
{
  return new CQSVGFeColorMatrix(this);
}

CSVGFeComposite *
CQSVG::
createFeComposite()
{
  return new CQSVGFeComposite(this);
}

CSVGFeDiffuseLighting *
CQSVG::
createFeDiffuseLighting()
{
  return new CQSVGFeDiffuseLighting(this);
}

CSVGFeDisplacementMap *
CQSVG::
createFeDisplacementMap()
{
  return new CQSVGFeDisplacementMap(this);
}

CSVGFeDistantLight *
CQSVG::
createFeDistantLight()
{
  return new CQSVGFeDistantLight(this);
}

CSVGFeFlood *
CQSVG::
createFeFlood()
{
  return new CQSVGFeFlood(this);
}

CSVGFeGaussianBlur *
CQSVG::
createFeGaussianBlur()
{
  return new CQSVGFeGaussianBlur(this);
}

CSVGFeImage *
CQSVG::
createFeImage()
{
  return new CQSVGFeImage(this);
}

CSVGFeMerge *
CQSVG::
createFeMerge()
{
  return new CQSVGFeMerge(this);
}

CSVGFeMergeNode *
CQSVG::
createFeMergeNode()
{
  return new CQSVGFeMergeNode(this);
}

CSVGFeMorphology *
CQSVG::
createFeMorphology()
{
  return new CQSVGFeMorphology(this);
}

CSVGFeOffset *
CQSVG::
createFeOffset()
{
  return new CQSVGFeOffset(this);
}

CSVGFePointLight *
CQSVG::
createFePointLight()
{
  return new CQSVGFePointLight(this);
}

CSVGFeSpecularLighting *
CQSVG::
createFeSpecularLighting()
{
  return new CQSVGFeSpecularLighting(this);
}

CSVGFeSpotLight *
CQSVG::
createFeSpotLight()
{
  return new CQSVGFeSpotLight(this);
}

CSVGFeTile *
CQSVG::
createFeTile()
{
  return new CQSVGFeTile(this);
}

CSVGFeTurbulence *
CQSVG::
createFeTurbulence()
{
  return new CQSVGFeTurbulence(this);
}

CSVGFilter *
CQSVG::
createFilter()
{
  return new CQSVGFilter(this);
}

CSVGGroup *
CQSVG::
createGroup()
{
  return new CQSVGGroup(this);
}

CSVGImage *
CQSVG::
createImage()
{
  return new CQSVGImage(this);
}

CSVGLine *
CQSVG::
createLine()
{
  return new CQSVGLine(this);
}

CSVGLinearGradient *
CQSVG::
createLinearGradient()
{
  return new CQSVGLinearGradient(this);
}

CSVGMarker *
CQSVG::
createMarker()
{
  return new CQSVGMarker(this);
}

CSVGMask *
CQSVG::
createMask()
{
  return new CQSVGMask(this);
}

CSVGMPath *
CQSVG::
createMPath()
{
  return new CQSVGMPath(this);
}

CSVGPath *
CQSVG::
createPath()
{
  return new CQSVGPath(this);
}

CSVGPattern *
CQSVG::
createPattern()
{
  return new CQSVGPattern(this);
}

CSVGPolygon *
CQSVG::
createPolygon()
{
  return new CQSVGPolygon(this);
}

CSVGPolyLine *
CQSVG::
createPolyLine()
{
  return new CQSVGPolyLine(this);
}

CSVGRadialGradient *
CQSVG::
createRadialGradient()
{
  return new CQSVGRadialGradient(this);
}

CSVGRect *
CQSVG::
createRect()
{
  return new CQSVGRect(this);
}

CSVGSet *
CQSVG::
createSet()
{
  return new CQSVGSet(this);
}

CSVGStop *
CQSVG::
createStop()
{
  return new CQSVGStop(this);
}

CSVGSymbol *
CQSVG::
createSymbol()
{
  return new CQSVGSymbol(this);
}

CSVGText *
CQSVG::
createText()
{
  return new CQSVGText(this);
}

CSVGTextPath *
CQSVG::
createTextPath()
{
  return new CQSVGTextPath(this);
}

CSVGTitle *
CQSVG::
createTitle()
{
  return new CQSVGTitle(this);
}

CSVGTSpan *
CQSVG::
createTSpan()
{
  return new CQSVGTSpan(this);
}

CSVGUse *
CQSVG::
createUse()
{
  return new CQSVGUse(this);
}

//------

CSVGImageData *
CQSVG::
createImageData()
{
  return new CQSVGImageData;
}

CSVGFontObj *
CQSVG::
createFontObj(const CSVGFontDef &def)
{
  return new CQSVGFontObj(def);
}

//------

CSVGPathMoveTo *
CQSVG::
createPathMoveTo(double x, double y)
{
  return new CQSVGPathMoveTo(*this, x, y);
}

CSVGPathRMoveTo *
CQSVG::
createPathRMoveTo(double x, double y)
{
  return new CQSVGPathRMoveTo(*this, x, y);
}

CSVGPathLineTo *
CQSVG::
createPathLineTo(double x, double y)
{
  return new CQSVGPathLineTo(*this, x, y);
}

CSVGPathRLineTo *
CQSVG::
createPathRLineTo(double x, double y)
{
  return new CQSVGPathRLineTo(*this, x, y);
}

CSVGPathHLineTo *
CQSVG::
createPathHLineTo(double x)
{
  return new CQSVGPathHLineTo(*this, x);
}

CSVGPathRHLineTo *
CQSVG::
createPathRHLineTo(double x)
{
  return new CQSVGPathRHLineTo(*this, x);
}

CSVGPathVLineTo *
CQSVG::
createPathVLineTo(double y)
{
  return new CQSVGPathVLineTo(*this, y);
}

CSVGPathRVLineTo *
CQSVG::
createPathRVLineTo(double y)
{
  return new CQSVGPathRVLineTo(*this, y);
}

CSVGPathArcTo *
CQSVG::
createPathArcTo(double rx, double ry, double xa, double fa, double fs, double x2, double y2)
{
  return new CQSVGPathArcTo(*this, rx, ry, xa, fa, fs, x2, y2);
}

CSVGPathRArcTo *
CQSVG::
createPathRArcTo(double rx, double ry, double xa, double fa, double fs, double x2, double y2)
{
  return new CQSVGPathRArcTo(*this, rx, ry, xa, fa, fs, x2, y2);
}

CSVGPathBezier2To *
CQSVG::
createPathBezier2To(double x1, double y1, double x2, double y2)
{
  return new CQSVGPathBezier2To(*this, x1, y1, x2, y2);
}

CSVGPathMBezier2To *
CQSVG::
createPathMBezier2To(double x2, double y2)
{
  return new CQSVGPathMBezier2To(*this, x2, y2);
}

CSVGPathRBezier2To *
CQSVG::
createPathRBezier2To(double x1, double y1, double x2, double y2)
{
  return new CQSVGPathRBezier2To(*this, x1, y1, x2, y2);
}

CSVGPathMRBezier2To *
CQSVG::
createPathMRBezier2To(double x2, double y2)
{
  return new CQSVGPathMRBezier2To(*this, x2, y2);
}

CSVGPathBezier3To *
CQSVG::
createPathBezier3To(double x1, double y1, double x2, double y2, double x3, double y3)
{
  return new CQSVGPathBezier3To(*this, x1, y1, x2, y2, x3, y3);
}

CSVGPathMBezier3To *
CQSVG::
createPathMBezier3To(double x2, double y2, double x3, double y3)
{
  return new CQSVGPathMBezier3To(*this, x2, y2, x3, y3);
}

CSVGPathRBezier3To *
CQSVG::
createPathRBezier3To (double x1, double y1, double x2, double y2, double x3, double y3)
{
  return new CQSVGPathRBezier3To(*this, x1, y1, x2, y2, x3, y3);
}

CSVGPathMRBezier3To *
CQSVG::
createPathMRBezier3To(double x2, double y2, double x3, double y3)
{
  return new CQSVGPathMRBezier3To(*this, x2, y2, x3, y3);
}

CSVGPathClosePath *
CQSVG::
createPathClosePath(bool relative)
{
  return new CQSVGPathClosePath(*this, relative);
}

//------

void
CQSVG::
startTimer()
{
  if (! isAnimating()) {
    timer_->start(dt_);

    setAnimating(true);
  }
}

void
CQSVG::
stopTimer()
{
  if (isAnimating()) {
    timer_->stop();

    setAnimating(false);
  }
}

void
CQSVG::
stepTimer()
{
  tickStep(1);
}

void
CQSVG::
bstepTimer()
{
  tickStep(-1);
}

void
CQSVG::
tickSlot()
{
  tickStep(1);
}

void
CQSVG::
setTime(double t)
{
  t_ = t;

  getRoot()->setTime(t_);

  window_->setTime(t_);
}

void
CQSVG::
tickStep(int n)
{
  double dt = n*dt_/1000.0;

  t_ += dt;

  getRoot()->tick(dt);

  window_->setTime(t_);
}

void
CQSVG::
redraw()
{
  window_->redraw();
}
