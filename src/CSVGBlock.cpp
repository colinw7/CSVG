#include <CSVGBlock.h>
#include <CSVG.h>
#include <CSVGBuffer.h>
#include <CSVGLog.h>
#include <CRegExp.h>

CSVGBlock::
CSVGBlock(CSVG &svg) :
 CSVGObject(svg)
{
}

CSVGBlock::
CSVGBlock(const CSVGBlock &block) :
 CSVGObject(block),
 x_             (block.x_),
 y_             (block.y_),
 width_         (block.width_),
 height_        (block.height_),
 preserveAspect_(block.preserveAspect_)
{
}

CSVGBlock *
CSVGBlock::
dup() const
{
  return new CSVGBlock(*this);
}

/* Attributes:
    <xmlns>
    <Core>
    <Conditional>
    <Style>
    <Presentation>
    <DocumentEvents>
    <GraphicalEvents>
    <External>
    x <Coordinate>
    y <Coordinate>
    width <Length>
    height <Length>
    viewBox <ViewBoxSpec>
    preserveAspectRatio <PreserveAspectRatioSpec>
    zoomAndPan ( disable | magnify ) 'magnify'
    version <Number>
    baseProfile <Text>
    contentScriptType <ContentType>
    contentStyleType <ContentType>
*/
bool
CSVGBlock::
processOption(const std::string &opt_name, const std::string &opt_value)
{
  std::string        str;
  double             real;
  CBBox2D            bbox;
  CScreenUnits       length;
  CSVGPreserveAspect preserveAspect;

  if (processCoreOption           (opt_name, opt_value)) return true;
  if (processConditionalOption    (opt_name, opt_value)) return true;
  if (processStyleOption          (opt_name, opt_value)) return true;
  if (processPresentationOption   (opt_name, opt_value)) return true;
  if (processDocumentEventsOption (opt_name, opt_value)) return true;
  if (processGraphicalEventsOption(opt_name, opt_value)) return true;
  if (processExternalOption       (opt_name, opt_value)) return true;

  // TODO: xmlns:<prefix>
  if      (svg_.stringOption(opt_name, opt_value, "xmlns", str)) {
    //notHandled(opt_name, opt_value);
  }
  else if (svg_.stringOption(opt_name, opt_value, "version", str)) {
    // specify SVG version - ignore
    //  version = "1.0" | "1.1" | "1.2"
    //notHandled(opt_name, opt_value);
  }
  else if (svg_.stringOption(opt_name, opt_value, "baseProfile", str)) {
    // specify minimum SVG language profile - ignore
    //  baseProfile = "none" | "full" | "basic" | "tiny"
    //notHandled(opt_name, opt_value);
  }
  else if (svg_.coordOption (opt_name, opt_value, "x", &real))
    x_.setValue(real);
  else if (svg_.coordOption (opt_name, opt_value, "y", &real))
    y_.setValue(real);
  else if (svg_.lengthOption(opt_name, opt_value, "width", length))
    width_.setValue(length);
  else if (svg_.lengthOption(opt_name, opt_value, "height", length))
    height_.setValue(length);
  else if (svg_.bboxOption  (opt_name, opt_value, "viewBox", bbox))
    viewBox_ = bbox;
  else if (svg_.preserveAspectOption(opt_name, opt_value, "preserveAspectRatio", preserveAspect))
    preserveAspect_ = preserveAspect;
  // "snapshotTime" <clock-value> | "none"
  // "timelineBegin" "onLoad" | "onStart"
  // "playbackOrder" "forwardOnly" | "all"
  else if (svg_.stringOption(opt_name, opt_value, "contentScriptType", str)) {
    // <content-type>
    //setNameValue("contentScriptType", str);
    //notHandled(opt_name, opt_value);
  }
  else if (svg_.stringOption(opt_name, opt_value, "zoomAndPan", str)) {
    // "magnify" | "disable"
    // allow disabled zoom and pan - ignore
    //notHandled(opt_name, opt_value);
  }
  // "focusable" "true" | "false" | "auto"
  else if (svg_.stringOption(opt_name, opt_value, "contentStyleType", str)) {
    notHandled(opt_name, opt_value);
  }
  else if (CRegExpUtil::parse(opt_name, "xmlns:.*")) {
    //notHandled(opt_name, opt_value);
  }
  else if (CRegExpUtil::parse(opt_name, "sodipodi:.*")) {
    //notHandled(opt_name, opt_value);
  }
  else if (CRegExpUtil::parse(opt_name, "inkscape:.*")) {
    //notHandled(opt_name, opt_value);
  }
  else
    return false;

  return true;
}

COptString
CSVGBlock::
getNameValue(const std::string &name) const
{
  COptString s;

  if (name == "preserveAspectRatio") {
    s = preserveAspect().toString();
  }
  else
    s = CSVGObject::getNameValue(name);

  return s;
}

double
CSVGBlock::
getX() const
{
  return (x_.isValid() ? x_.getValue() : 0.0);
}

double
CSVGBlock::
getY() const
{
  return (y_.isValid() ? y_.getValue() : 0.0);
}

double
CSVGBlock::
getWidth() const
{
  double w = getScreenWidth();

  // auto bbox = getViewBox();
  //if (bbox.isSet()) w = bbox.getWidth();

  if      (width_.isValid())
    return width_.getValue().px(CScreenUnits(w)).value();
  else if (height_.isValid())
    return height_.getValue().px(CScreenUnits(w)).value();
  else
    return w;
}

double
CSVGBlock::
getHeight() const
{
  double h = getScreenHeight();

  //auto bbox = getViewBox();
  //if (bbox.isSet()) h = bbox.getHeight();

  if      (height_.isValid())
    return height_.getValue().px(CScreenUnits(h)).value();
  else if (width_.isValid())
    return width_.getValue().px(CScreenUnits(h)).value();
  else
    return h;
}

void
CSVGBlock::
setSize(const CSize2D &size)
{
  width_ .setValue(CScreenUnits(size.getWidth ()));
  height_.setValue(CScreenUnits(size.getHeight()));
}

void
CSVGBlock::
drawInit()
{
  oldBuffer_ = svg_.getCurrentBuffer();

  // save current block data values
  blockData_ = svg_.blockData();

  // pixel and view box
  auto pbbox = calcPixelBox();
  auto vbbox = calcViewBox ();

  //auto bmin = vbbox.getLL();
  //auto bmax = vbbox.getUR();

  //svg_.viewMatrix().multiplyPoint(vbbox.getLL(), bmin);
  //svg_.viewMatrix().multiplyPoint(vbbox.getUR(), bmax);

  //double bw = pbbox.getWidth ();
  //double bh = pbbox.getHeight();

  double xscale = pbbox.getWidth ()/vbbox.getWidth ();
  double yscale = pbbox.getHeight()/vbbox.getHeight();

  if (hasPreserveAspect()) {
    auto scale = getScale();

    if      (scale == CSVGScale::FIXED_MEET) {
      double minScale = std::min(xscale, yscale);

      xscale = minScale;
      yscale = minScale;
    }
    else if (scale == CSVGScale::FIXED_SLICE) {
      double maxScale = std::max(xscale, yscale);

      xscale = maxScale;
      yscale = maxScale;
    }
  }

  // TODO: adjust for scale
  //svg_.setOffset(CPoint2D(pbbox.getXMin(), pbbox.getYMin()));

  //svg_.setXScale(blockData_.xscale()*xscale);
  //svg_.setYScale(blockData_.yscale()*yscale);
  //svg_.setXScale(xscale);
  //svg_.setYScale(yscale);

  //svg_.setPreserveAspect(preserveAspect());

  //------

  if (parent_) {
    auto *drawBuffer = svg_.pushBuffer("_" + getUniqueName() + "_svg");

    drawBuffer->clear();

    svg_.beginDrawBuffer(drawBuffer, pbbox, vbbox, svg_.offset(), xscale, yscale,
                         preserveAspect());
  }
}

void
CSVGBlock::
drawTerm()
{
  //svg_.setOffset        (blockData_.offset());
  //svg_.setXScale        (blockData_.xscale());
  //svg_.setYScale        (blockData_.yscale());
  //svg_.setPreserveAspect(blockData_.preserveAspect());

  //------

  if (parent_) {
    auto *drawBuffer = svg_.getBuffer("_" + getUniqueName() + "_svg");

    svg_.endDrawBuffer(drawBuffer);

    //---

    auto transform = oldBuffer_->transform();

    //---

    // pixel and view box
    auto pbbox = calcPixelBox();
    //auto vbbox = calcViewBox ();

    //auto bmin = vbbox.getLL();
    //auto bmax = vbbox.getUR();

    //svg_.viewMatrix().multiplyPoint(vbbox.getLL(), bmin);
    //svg_.viewMatrix().multiplyPoint(vbbox.getUR(), bmax);

    double bw = pbbox.getWidth ();
    double bh = pbbox.getHeight();

    //double xscale = pbbox.getWidth ()/vbbox.getWidth ();
    //double yscale = pbbox.getHeight()/vbbox.getHeight();

    //---

    bool   clipped = false;
    double ix      = 0;
    double iy      = 0;
    double s       = 0;

    if (hasPreserveAspect()) {
      auto scale  = getScale();
      auto halign = getHAlign();
      auto valign = getVAlign();

      if      (scale == CSVGScale::FIXED_MEET) {
        s = std::min(bw, bh);

        if      (halign == CHALIGN_TYPE_LEFT)
          ix = 0;
        else if (halign == CHALIGN_TYPE_CENTER)
          ix = (bw - s)/2;
        else if (halign == CHALIGN_TYPE_RIGHT)
          ix = bw - s;

        if      (valign == CVALIGN_TYPE_BOTTOM)
          iy = 0;
        else if (valign == CVALIGN_TYPE_CENTER)
          iy = (bh - s)/2;
        else if (valign == CVALIGN_TYPE_TOP)
          iy = bh - s;
      }
      else if (scale == CSVGScale::FIXED_SLICE) {
        s = std::max(bw, bh);

        if      (halign == CHALIGN_TYPE_LEFT)
          ix = 0;
        else if (halign == CHALIGN_TYPE_CENTER)
          ix = (bw - s)/2;
        else if (halign == CHALIGN_TYPE_RIGHT)
          ix = bw - s;

        if      (valign == CVALIGN_TYPE_BOTTOM)
          iy = 0;
        else if (valign == CVALIGN_TYPE_CENTER)
          iy = (bh - s)/2;
        else if (valign == CVALIGN_TYPE_TOP)
          iy = bh - std::max(bw, bh);

        clipped = true;
      }
    }

    ix += getX();
    iy += getY();

    double x, y;

    transform.multiplyPoint(ix, iy, &x, &y);

    if (clipped) {
      //double px1, py1, px2, py2;

      //drawBuffer->windowToPixel(-ix     , -iy     , &px1, &py1);
      //drawBuffer->windowToPixel(-ix + bw, -iy + bh, &px2, &py2);

      double xs = svg_.flatXScale();
      double ys = svg_.flatYScale();

      auto *clipBuffer = svg_.getBuffer("_" + getUniqueName() + "_svg_clip");

      double px1 = -ix*xs;
      double py1 = -iy*ys;
      double px2 = (-ix + bw)*xs;
      double py2 = (-iy + bh)*ys;

      clipBuffer->setClippedBuffer(drawBuffer, px1, py1, px2, py2);

      //---

      double px, py;

      svg_.lengthToPixel(x, y, &px, &py);

      bool oldDrawing = oldBuffer_->isDrawing();

      if (oldDrawing)
        oldBuffer_->stopDraw();

      oldBuffer_->addImageBuffer(px, py, clipBuffer);

      if (oldDrawing)
        oldBuffer_->startDraw();
    }
    else {
      double px, py;

      svg_.lengthToPixel(x, y, &px, &py);

      bool oldDrawing = oldBuffer_->isDrawing();

      if (oldDrawing)
        oldBuffer_->stopDraw();

      oldBuffer_->addBuffer(drawBuffer, px, py);

      if (oldDrawing)
        oldBuffer_->startDraw();
    }

    //---

    svg_.popBuffer();
  }
}

bool
CSVGBlock::
draw()
{
  if (svg_.getDebug())
    CSVGLog() << *this;

  return false;
}

CBBox2D
CSVGBlock::
calcPixelBox() const
{
  return CBBox2D(getX(), getY(), getX() + getWidth(), getY() + getHeight());
}

CBBox2D
CSVGBlock::
calcViewBox() const
{
  if (hasViewBox())
    return getViewBox();

  return calcPixelBox();
}

bool
CSVGBlock::
getBBox(CBBox2D &bbox) const
{
  // default view box (if not specified) to pixel range
  if (hasViewBox())
    bbox = getViewBox();
  else
    bbox = calcPixelBox();

  return true;
}

void
CSVGBlock::
printRoot(std::ostream &os, const std::vector<CCSS> &cssList, bool hier) const
{
  if (hier) {
    os << "<svg";

    printValues(os);

    os << ">" << std::endl;

    for (const auto &css : cssList) {
      if (css.hasStyleData()) {
        os << "<style type=\"text/css\"><![CDATA[" << std::endl;

        css.print(os);

        os << "]]></style>" << std::endl;
      }
    }

    printChildren(os, hier);

    os << "</svg>" << std::endl;
  }
  else
    os << "svg " << getWidth() << " " << getHeight();
}

void
CSVGBlock::
print(std::ostream &os, bool hier) const
{
  if (hier) {
    os << "<svg";

    printValues(os);

    os << ">" << std::endl;

    printChildren(os, hier);

    os << "</svg>" << std::endl;
  }
  else
    os << "svg " << getWidth() << " " << getHeight();
}

void
CSVGBlock::
printValues(std::ostream &os, bool flat) const
{
  CSVGObject::printValues(os, flat);

  printNameValue (os, "x"     , x_     );
  printNameValue (os, "y"     , y_     );
  printNameLength(os, "width" , width_ );
  printNameLength(os, "height", height_);

  printNamePreserveAspect(os, "preserveAspectRatio", preserveAspect_);
}
