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
  if      (svg_.stringOption(opt_name, opt_value, "xmlns", str))
    ;
  else if (svg_.coordOption (opt_name, opt_value, "x", &real))
    x_.setValue(real);
  else if (svg_.coordOption (opt_name, opt_value, "y", &real))
    y_.setValue(real);
  else if (svg_.lengthOption(opt_name, opt_value, "width", length))
    width_.setValue(length);
  else if (svg_.lengthOption(opt_name, opt_value, "height", length))
    height_.setValue(length);
  else if (svg_.bboxOption  (opt_name, opt_value, "viewBox", &bbox))
    viewBox_ = bbox;
  else if (svg_.preserveAspectOption(opt_name, opt_value, "preserveAspectRatio", preserveAspect))
    preserveAspect_ = preserveAspect;
  else if (svg_.stringOption(opt_name, opt_value, "zoomAndPan", str))
    ;
  else if (svg_.stringOption(opt_name, opt_value, "version", str))
    ;
  else if (svg_.stringOption(opt_name, opt_value, "baseProfile", str))
    ;
  else if (svg_.stringOption(opt_name, opt_value, "contentScriptType", str))
    ;
  else if (svg_.stringOption(opt_name, opt_value, "contentStyleType", str))
    ;
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
  if      (width_.isValid())
    return width_.getValue().px().value();
  else if (height_.isValid())
    return height_.getValue().px().value();
  else
    return 400;
}

double
CSVGBlock::
getHeight() const
{
  if      (height_.isValid())
    return height_.getValue().px().value();
  else if (width_.isValid())
    return width_.getValue().px().value();
  else
    return 400;
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
  oldBuffer_ = svg_.getBuffer();
  xscale_    = svg_.xscale();
  yscale_    = svg_.yscale();

  CPoint2D bmin, bmax;

  svg_.viewMatrix().multiplyPoint(CPoint2D(this->getXMin(), this->getYMin()), bmin);
  svg_.viewMatrix().multiplyPoint(CPoint2D(this->getXMax(), this->getYMax()), bmax);

  double bw = this->getWidth ();
  double bh = this->getHeight();

  double xscale = bw/(bmax.x - bmin.x);
  double yscale = bh/(bmax.y - bmin.y);

  if (preserveAspect_.isValid()) {
    if (preserveAspect_.getValue().getScale() == CSVGScale::FIXED_MEET) {
      double scale = std::min(xscale, yscale);

      xscale = scale;
      yscale = scale;
    }
  }

  svg_.setXScale(xscale_*xscale);
  svg_.setYScale(yscale_*yscale);

  //------

  if (parent_) {
    CSVGBuffer *drawBuffer = svg_.getBuffer("_" + getUniqueName() + "_svg");

    svg_.setBuffer(drawBuffer);

    drawBuffer->clear();

    svg_.beginDrawBuffer(drawBuffer);
  }
}

void
CSVGBlock::
drawTerm()
{
  svg_.setXScale(xscale_);
  svg_.setYScale(yscale_);

  //------

  if (parent_) {
    CSVGBuffer *drawBuffer = svg_.getBuffer("_" + getUniqueName() + "_svg");

    svg_.endDrawBuffer(drawBuffer);

    //---

    CMatrixStack2D transform = oldBuffer_->transform();

    double bw = this->getWidth ();
    double bh = this->getHeight();

    double ix = 0, iy = 0;

    if (preserveAspect_.isValid()) {
      if      (preserveAspect_.getValue().getHAlign() == CHALIGN_TYPE_LEFT)
        ix = 0;
      else if (preserveAspect_.getValue().getHAlign() == CHALIGN_TYPE_CENTER)
        ix = (bw - std::min(bw, bh))/2;
      else if (preserveAspect_.getValue().getHAlign() == CHALIGN_TYPE_RIGHT)
        ix = bw - std::min(bw, bh);

      if      (preserveAspect_.getValue().getVAlign() == CVALIGN_TYPE_BOTTOM)
        iy = 0;
      else if (preserveAspect_.getValue().getVAlign() == CVALIGN_TYPE_CENTER)
        iy = (bh - std::min(bw, bh))/2;
      else if (preserveAspect_.getValue().getVAlign() == CVALIGN_TYPE_TOP)
        iy = bh - std::min(bw, bh);
    }

    double x = 0, y = 0;

    transform.multiplyPoint(ix, iy, &x, &y);

    double px, py;

    oldBuffer_->lengthToPixel(x, y, &px, &py);

    oldBuffer_->addBuffer(drawBuffer, px, py);

    //---

    svg_.setBuffer(oldBuffer_);
  }
}

void
CSVGBlock::
draw()
{
  if (svg_.getDebug())
    CSVGLog() << *this;
}

bool
CSVGBlock::
getBBox(CBBox2D &bbox) const
{
  if (! viewBox_.isValid())
    bbox = CBBox2D(getX(), getY(), getWidth(), getHeight());
  else
    bbox = getViewBox();

  return true;
}

void
CSVGBlock::
print(std::ostream &os, bool hier) const
{
  if (hier) {
    os << "<svg";

    CSVGObject::printValues(os);

    printNameValue (os, "x"     , x_     );
    printNameValue (os, "y"     , y_     );
    printNameLength(os, "width" , width_ );
    printNameLength(os, "height", height_);

    printNamePreserveAspect(os, "preserveAspectRatio", preserveAspect_);

    os << ">" << std::endl;

    printChildren(os, hier);

    os << "</svg>" << std::endl;
  }
  else
    os << "svg " << getWidth() << " " << getHeight();
}

std::ostream &
operator<<(std::ostream &os, const CSVGBlock &block)
{
  block.print(os, false);

  return os;
}
