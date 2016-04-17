#include <CSVGFeImage.h>
#include <CSVGFilter.h>
#include <CSVG.h>
#include <CSVGBuffer.h>

CSVGFeImage::
CSVGFeImage(CSVG &svg) :
 CSVGFilterBase(svg), xlink_(this)
{
}

CSVGFeImage::
CSVGFeImage(const CSVGFeImage &image) :
 CSVGFilterBase(image),
 xlink_    (image.xlink_),
 filterOut_(image.filterOut_)
{
}

CSVGFeImage *
CSVGFeImage::
dup() const
{
  return new CSVGFeImage(*this);
}

bool
CSVGFeImage::
processOption(const std::string &opt_name, const std::string &opt_value)
{
  std::string        str;
  CSVGPreserveAspect preserveAspect;

  if      (svg_.stringOption(opt_name, opt_value, "xlink:href", str))
    xlink_ = CSVGXLink(this, str);
  else if (svg_.stringOption(opt_name, opt_value, "result", str))
    filterOut_ = str;
  else if (svg_.preserveAspectOption(opt_name, opt_value, "preserveAspectRatio", preserveAspect))
    preserveAspect_ = preserveAspect;
  else
    return CSVGFilterBase::processOption(opt_name, opt_value);

  return true;
}

void
CSVGFeImage::
draw()
{
  CImagePtr src_image;

  if (xlink_.isValid()) {
    if      (xlink_.getValue().isImage())
      src_image = xlink_.getValue().getImage();
    else if (xlink_.getValue().isObject())
      src_image = xlink_.getValue().getObject()->toImage();
  }

  if (! src_image.isValid())
    return;

  CSVGBuffer *outBuffer = svg_.getBuffer(getFilterOut());

  if (svg_.getDebugFilter()) {
    std::string objectBufferName = "_" + getUniqueName();

    CSVGBuffer *buffer = svg_.getBuffer(objectBufferName + "_in");

    buffer->setImage(src_image);
  }

  filterImage(src_image, outBuffer);

  if (svg_.getDebugFilter()) {
    std::string objectBufferName = "_" + getUniqueName();

    CSVGBuffer *buffer = svg_.getBuffer(objectBufferName + "_out");

    buffer->setImage(outBuffer->getImage());
    buffer->setBBox (outBuffer->bbox());
  }
}

void
CSVGFeImage::
filterImage(CImagePtr src_image, CSVGBuffer *outBuffer)
{
  assert(src_image.isValid());

  CImagePtr dst_image;

  CBBox2D bbox;

  if (getParentBBox(bbox)) {
    // resize source image (to bbox pixel size)
    CImagePtr src_image1 = src_image->dup();

    //---

    CSVGCoordUnits punits = getParentFilter()->getPrimitiveUnits();

    if (x_.isValid()) {
      if (punits == CSVGCoordUnits::USER_SPACE)
        bbox.moveXTo(x_.getValue().value());
      else
        bbox.moveXTo(getXMin() + x_.getValue().value());
    }

    if (y_.isValid()) {
      if (punits == CSVGCoordUnits::USER_SPACE)
        bbox.moveYTo(y_.getValue().value());
      else
        bbox.moveYTo(getYMin() + y_.getValue().value());
    }

    if (width_.isValid())
      bbox.setWidth(width_.getValue().value());

    if (height_.isValid())
      bbox.setHeight(height_.getValue().value());

    if (xlink_.getValue().isImage()) {
      // TODO: is this always going to be 1->1
      double x1, y1, x2, y2;

      svg_.windowToPixel(bbox.getXMin(), bbox.getYMin(), &x1, &y1);
      svg_.windowToPixel(bbox.getXMax(), bbox.getYMax(), &x2, &y2);

      double pw = fabs(x2 - x1);
      double ph = fabs(y2 - y1);

      //---

      if (preserveAspect_.isValid()) {
        CSVGScale scale = preserveAspect_.getValue().getScale();

        //---

        if (scale == CSVGScale::FREE)
          src_image1->reshape(pw, ph);
        else
          src_image1->reshapeKeepAspect(pw, ph);
      }
      else {
        src_image1->reshape(pw, ph);
      }

      //---

      // create dest image
      CImageNoSrc src;

      dst_image = CImageMgrInst->createImage(src);

      dst_image->setDataSize(pw, ph);

      //---

      // add to dest image (centered)
      double dx = (pw - src_image1->getWidth ())/2;
      double dy = (ph - src_image1->getHeight())/2;

      src_image1->copyTo(dst_image, x1 + dx, y1 + dy);
    }
    else {
      dst_image = src_image->dup();
    }
  }
  else
    dst_image = src_image->dup();

  outBuffer->setImage(dst_image);
  outBuffer->setBBox(bbox);
}

void
CSVGFeImage::
print(std::ostream &os, bool hier) const
{
  if (hier) {
    os << "<feImage";

    CSVGObject::printValues(os);

    printNameXLink(os, "xlink:href", xlink_);
    printNameValue(os, "filter_out", filterOut_);

    printNamePreserveAspect(os, "preserveAspectRatio", preserveAspect_);

    os << "/>" << std::endl;
  }
  else
    os << "feImage ";
}

std::ostream &
operator<<(std::ostream &os, const CSVGFeImage &filter)
{
  filter.print(os, false);

  return os;
}
