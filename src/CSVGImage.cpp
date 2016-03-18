#include <CSVGI.h>

CSVGImage::
CSVGImage(CSVG &svg) :
 CSVGObject(svg),
 xlink_    (this),
 pos_      (0, 0),
 size_     (100, 100)
{
}

CSVGImage::
CSVGImage(const CSVGImage &image) :
 CSVGObject(image),
 xlink_    (image.xlink_),
 pos_      (image.pos_),
 size_     (image.size_)
{
}

CSVGImage::
~CSVGImage()
{
}

CSVGImage *
CSVGImage::
dup() const
{
  return new CSVGImage(*this);
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
  std::string str;
  double      real;

  if      (svg_.coordOption (opt_name, opt_value, "x", &real))
    pos_.x = real;
  else if (svg_.coordOption (opt_name, opt_value, "y", &real))
    pos_.y = real;
  else if (svg_.lengthOption(opt_name, opt_value, "width", &real))
    size_.width = real;
  else if (svg_.lengthOption(opt_name, opt_value, "height", &real))
    size_.height = real;
  else if (svg_.stringOption(opt_name, opt_value, "xlink:href", str))
    xlink_ = CSVGXLink(this, str);
  else
    return CSVGObject::processOption(opt_name, opt_value);

  return true;
}

bool
CSVGImage::
getSize(CSize2D &size) const
{
  size = size_;

  return true;
}

CImagePtr
CSVGImage::
getImage() const
{
  if (xlink_.isObject()) {
    CSVGImage *th = const_cast<CSVGImage *>(this);

    CImagePtr image = xlink_.getObject()->toImage();

    th->xlink_.setObject(0);
    th->xlink_.setImage (image);
  }

  if (xlink_.isImage())
    return xlink_.getImage();

  return CImagePtr();
}

void
CSVGImage::
setOrigin(const CPoint2D &pos)
{
  pos_ = pos;
}

void
CSVGImage::
setSize(const CSize2D &size)
{
  size_ = size;
}

void
CSVGImage::
setImage(const std::string &filename)
{
  CImageFileSrc src(filename);

  CImagePtr image = CImageMgrInst->lookupImage(src);

  xlink_.setImage(image);
}

void
CSVGImage::
draw()
{
  if (svg_.getDebug())
    CSVGLog() << *this;

  CImagePtr image = getImage();

  if (image.isValid()) {
    CBBox2D bbox;

    getBBox(bbox);

    svg_.drawImage(bbox, image);
  }
}

bool
CSVGImage::
getBBox(CBBox2D &bbox) const
{
  if (! view_box_.isSet())
    bbox = CBBox2D(pos_, pos_ + size_);
  else
    bbox = view_box_;

  return true;
}

void
CSVGImage::
moveBy(const CVector2D &delta)
{
  pos_ += delta;
}

void
CSVGImage::
resizeTo(const CSize2D &size)
{
  size_ = size;
}

void
CSVGImage::
print(std::ostream &os) const
{
  os << "image " << pos_ << " " << size_;
}

std::ostream &
operator<<(std::ostream &os, const CSVGImage &image)
{
  image.print(os);

  return os;
}
