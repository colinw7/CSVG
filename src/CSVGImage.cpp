#include <CSVGImage.h>
#include <CSVGBuffer.h>
#include <CSVG.h>
#include <CSVGLog.h>

CSVGImage::
CSVGImage(CSVG &svg) :
 CSVGObject(svg),
 xlink_    (this)
{
  init();
}

CSVGImage::
CSVGImage(const CSVGImage &image) :
 CSVGObject(image),
 xlink_         (image.xlink_),
 x_             (image.x_),
 y_             (image.y_),
 w_             (image.w_),
 h_             (image.h_),
 preserveAspect_(image.preserveAspect_),
 colorProfile_  (image.colorProfile_)
{
  init();
}

CSVGImage::
~CSVGImage()
{
}

void
CSVGImage::
init()
{
  skipNames_.insert("x");
  skipNames_.insert("y");
  skipNames_.insert("width");
  skipNames_.insert("height");

  skipNames_.insert("preserveAspectRatio");

  skipNames_.insert("xlink:href");

  skipNames_.insert("color-profile");
}

CSVGImage *
CSVGImage::
dup() const
{
  return new CSVGImage(*this);
}

void
CSVGImage::
setLinkName(const std::string &str)
{
  xlink_ = CSVGXLink(this, str);
}

/* Atributes:
    <Core>
    <Conditional>
    <Style>
    <Viewport>
    <Color>
    <Opacity>
    <Graphics>
    <ColorProfile>
    <Clip>
    <Mask>
    <Filter>
    <GraphicalEvents>
    <Cursor>
    <XLinkEmbed>
    <External>
    x <Coordinate>
    y <Coordinate>
    width <Length>
    height <Length>
    preserveAspectRatio <PreserveAspectRatioSpec>
    transform <TransformList>
*/
bool
CSVGImage::
processOption(const std::string &opt_name, const std::string &opt_value)
{
  std::string        str;
  double             real;
  CScreenUnits       length;
  CSVGPreserveAspect preserveAspect;

  if      (svg_.coordOption (opt_name, opt_value, "x", &real))
    x_ = real;
  else if (svg_.coordOption (opt_name, opt_value, "y", &real))
    y_ = real;
  else if (svg_.lengthOption(opt_name, opt_value, "width", length))
    w_ = length;
  else if (svg_.lengthOption(opt_name, opt_value, "height", length))
    h_ = length;
  else if (svg_.preserveAspectOption(opt_name, opt_value, "preserveAspectRatio", preserveAspect))
    preserveAspect_ = preserveAspect;
  else if (svg_.stringOption(opt_name, opt_value, "xlink:href", str))
    xlink_ = CSVGXLink(this, str);
  // type = "<content-type>"
  // focusable = "true" | "false" | "auto" 
  else if (svg_.stringOption(opt_name, opt_value, "color-profile", str))
    colorProfile_ = str;
  else
    return CSVGObject::processOption(opt_name, opt_value);

  return true;
}

bool
CSVGImage::
getSize(CSize2D &size) const
{
  size = CSize2D(getWidth(), getHeight());

  return true;
}

void
CSVGImage::
setOrigin(const CPoint2D &pos)
{
  x_ = pos.x;
  y_ = pos.y;
}

void
CSVGImage::
setSize(const CSize2D &size)
{
  w_ = size.getWidth ();
  h_ = size.getHeight();
}

bool
CSVGImage::
draw()
{
  if (svg_.getDebug())
    CSVGLog() << *this;

  if (! initImage())
    return false;

  //---

  auto *oldBuffer = svg_.getCurrentBuffer();

  //---

  // get bbox
  CBBox2D bbox;

  getBBox(bbox);

  if (svg_.isCheckViewBox()) {
    auto dbbox = getDrawBBox();

    if (! bbox.overlaps(dbbox))
      CSVGLog() << "Outside viewbox : " << *this;
  }

  // resize image to bbox
  double x1, y1, x2, y2;

  svg_.windowToPixel(bbox.getXMin(), bbox.getYMin(), &x1, &y1);
  svg_.windowToPixel(bbox.getXMax(), bbox.getYMax(), &x2, &y2);

  double pw = fabs(x2 - x1);
  double ph = fabs(y2 - y1);

  oldBuffer->addReshapeImage(getImageBuffer(), x1, y1, int(pw), int(ph));

  return true;
}

bool
CSVGImage::
initImage() const
{
  if (! xlink_)
    return false;

  auto *imageBuffer = getImageBuffer();

  if      (xlink_.value().isObject()) {
    auto *object = xlink_.value().getObject();

    auto *objImageBuffer = object->toBufferImage();

    // TODO: don't cache image as could be rescaled
    //th->xlink_.value().setObject(0);

    if (objImageBuffer)
      imageBuffer->setImageBuffer(objImageBuffer);

    return true;
  }
  else if (xlink_.value().isImage()) {
    imageBuffer->setImageBuffer(xlink_.value().getImageBuffer());

    return true;
  }
  else
    return false;
}

CSVGObject *
CSVGImage::
getObject() const
{
  if (! xlink_)
    return nullptr;

  if (! xlink_.value().isObject())
    return nullptr;

  return xlink_.value().getObject();
}

CSVGBuffer *
CSVGImage::
getImageBuffer() const
{
  std::string imageBufferName = "image_" + getUniqueName();

  return svg_.getBuffer(imageBufferName);
}

bool
CSVGImage::
getBBox(CBBox2D &bbox) const
{
  if (! hasViewBox())
    bbox = CBBox2D(getPosition(), getPosition() + getSizeInternal());
  else
    bbox = getViewBox();

  return true;
}

void
CSVGImage::
moveDelta(const CVector2D &delta)
{
  auto p = getPosition();

  p += delta;

  x_ = p.x;
  y_ = p.y;
}

void
CSVGImage::
resizeTo(const CSize2D &size)
{
  setSize(size);
}

void
CSVGImage::
print(std::ostream &os, bool hier) const
{
  if (hier) {
    os << "<image";

    printValues(os);

    os << "/>\n";
  }
  else
    os << "image " << getPosition() << " " << getSizeInternal();
}

void
CSVGImage::
printValues(std::ostream &os, bool flat) const
{
  CSVGObject::printValues(os, flat);

  printNameValue (os, "x"     , x_);
  printNameValue (os, "y"     , y_);
  printNameLength(os, "width" , w_);
  printNameLength(os, "height", h_);

  printNamePreserveAspect(os, "preserveAspectRatio", preserveAspect_);

  if (! xlink_.value().isNull())
    printNameXLink(os, "xlink:href", xlink_);

  printNameValue(os, "color-profile", colorProfile_);
}
