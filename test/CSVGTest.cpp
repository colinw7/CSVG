#include <CSVGLib.h>
#include <CSVGRenderer.h>
#include <CImageRenderer2D.h>

class CISVGRenderer : public CSVGRenderer {
 public:
  CISVGRenderer(CImageRenderer2D *renderer) :
   renderer_(renderer) {
  }

  CISVGRenderer *dup() const {
    return new CISVGRenderer(renderer_);
  }

  void setSize(int width, int height) {
    renderer_->updateSize(width, height);
  }

  void getSize(int *width, int *height) const {
    *width  = renderer_->getPixelRenderer()->getWidth ();
    *height = renderer_->getPixelRenderer()->getHeight();
  }

  void setDataRange(double xmin, double ymin, double xmax, double ymax) {
    renderer_->setDataRange(xmin, ymin, xmax, ymax);
  }

  void getDataRange(double *xmin, double *ymin, double *xmax, double *ymax) const {
    renderer_->getDataRange(xmin, ymin, xmax, ymax);
  }

  void setAntiAlias(bool) { }

  void setEqualScale(bool) { }

  void setScaleMin(bool) { }

  void beginDraw() { }
  void endDraw  () { }

  void setViewMatrix(const CMatrix2D &) { }

  void clear(const CRGBA &) { }

  void pathInit() { }
  void pathMoveTo(const CPoint2D &) { }
  void pathRMoveTo(const CPoint2D &) { }
  void pathLineTo(const CPoint2D &) { }
  void pathRLineTo(const CPoint2D &) { }
  void pathCurveTo(const CPoint2D &, const CPoint2D &) { }
  void pathRCurveTo(const CPoint2D &, const CPoint2D &) { }
  void pathCurveTo(const CPoint2D &, const CPoint2D &, const CPoint2D &) { }
  void pathRCurveTo(const CPoint2D &, const CPoint2D &, const CPoint2D &) { }
  void pathArcTo(const CPoint2D &, double, double, double, double) { }
  void pathText(const std::string &) { }
  void pathClose() { }

  bool pathGetCurrentPoint(CPoint2D &) {
    return false;
  }

  void pathStroke() { }
  void pathFill() { }
  void pathClip() { }
  void pathEoclip() { }

  void initClip() { }

  void pathBBox(CBBox2D &) { }

  void drawImage(const CPoint2D &, CImagePtr) { }

  void setFont(CFontPtr) { }

  void setStrokeColor(const CRGBA &) { }
  void setLineWidth(double) { }
  void setLineDash(const CLineDash &) { }
  void setLineCap(const CLineCapType &) { }
  void setLineJoin(const CLineJoinType &) { }
  void setMitreLimit(double) { }

  void setFillType(CFillType) { }

  void setFillColor(const CRGBA &) { }
  void setFillGradient(CGenGradient *) { }
  void setFillImage(CImagePtr) { }

  void setAlign(CHAlignType, CVAlignType) { }

  void windowToPixel(const CPoint2D &, CPoint2D &) { }

  void textBounds(const std::string &, CBBox2D &) { }

  CImagePtr getImage() const {
    return renderer_->getImage();
  }

  void setImage(CImagePtr) { }

 private:
  CImageRenderer2D *renderer_;
};

//-------

int
main(int argc, char **argv)
{
  bool debug = false;

  std::vector<std::string> files;

  for (int i = 1; i < argc; i++) {
    if (argv[i][0] == '-') {
      if (strcmp(&argv[i][1], "debug") == 0)
        debug = true;
      else
        std::cerr << "Invalid option: " << argv[i] << std::endl;
    }
    else
      files.push_back(argv[i]);
  }

  if (files.empty()) {
    std::cerr << "No files specified" << std::endl;
    exit(1);
  }

  uint num_files = files.size();

  for (uint i = 0; i < num_files; ++i) {
    CImageNoSrc src;

    CImagePtr image = CImageMgrInst->createImage(src);

    CSVG svg;

    svg.setDebug(debug);

    svg.init();

    svg.read(files[i]);

    image->setDataSize(svg.getIWidth(), svg.getIHeight());

    CImageRenderer2D renderer(image);

    CISVGRenderer irenderer(&renderer);

    svg.setRenderer(&irenderer);

    svg.draw();

    std::string name = CStrUtil::strprintf("svg_%s.png", files[i].c_str());

    renderer.getImage()->write(name, CFILE_TYPE_IMAGE_PNG);
  }

  return 0;
}
