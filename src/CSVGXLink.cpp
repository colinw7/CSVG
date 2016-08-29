#include <CSVGXLink.h>
#include <CSVGBuffer.h>
#include <CSVG.h>

CSVGXLink::
CSVGXLink(const CSVGXLink &xlink) :
 parent_  (xlink.parent_),
 resolved_(xlink.resolved_),
 str_     (xlink.str_),
 object_  (xlink.object_),
 isImage_ (xlink.isImage_),
 xscale_  (xlink.xscale_),
 yscale_  (xlink.yscale_)
{
  if (xlink.imageBuffer_) {
    initImageBuffer();

    imageBuffer_->setImageBuffer(xlink.imageBuffer_);
  }
}

CSVGXLink &
CSVGXLink::
operator=(const CSVGXLink &xlink)
{
  parent_   = xlink.parent_;
  resolved_ = xlink.resolved_;
  str_      = xlink.str_;
  object_   = xlink.object_;
  isImage_  = xlink.isImage_;
  xscale_   = xlink.xscale_;
  yscale_   = xlink.yscale_;

  if (xlink.imageBuffer_) {
    initImageBuffer();

    imageBuffer_->setImageBuffer(xlink.imageBuffer_);
  }

  return *this;
}

bool
CSVGXLink::
isObject() const
{
  resolve();

  return (object_ != nullptr);
}

CSVGObject *
CSVGXLink::
getObject() const
{
  resolve();

  return object_;
}

void
CSVGXLink::
setObject(CSVGObject *object)
{
  object_   = object;
  resolved_ = true;
}

bool
CSVGXLink::
isImage() const
{
  resolve();

  return isImage_;
}

CSVGBuffer *
CSVGXLink::
getImageBuffer() const
{
  resolve();

  return imageBuffer_;
}

void
CSVGXLink::
setImage(CSVGBuffer *buffer)
{
  initImageBuffer();

  imageBuffer_->setImageBuffer(buffer);

  isImage_  = true;
  resolved_ = true;
}

bool
CSVGXLink::
getImage(CSVGBuffer *buffer) const
{
  if      (isImage()) {
    buffer->setImageBuffer(getImageBuffer());
  }
  else if (isObject()) {
    CSVGBuffer *imageBuffer = getObject()->toBufferImage();

    if (! imageBuffer)
      return false;

    buffer->setImageBuffer(imageBuffer);
  //buffer->setImage(getObject()->toImage());
  }
  else
    return false;

  return true;
}

bool
CSVGXLink::
isNull() const
{
  resolve();

  return (str_.empty() && ! object_);
}

void
CSVGXLink::
resolve() const
{
  CSVGXLink *th = const_cast<CSVGXLink *>(this);

  CSVG &svg = parent_->getSVG();

  double xscale = svg.flatXScale();
  double yscale = svg.flatYScale();

  if (resolved_ && xscale_ == xscale && yscale_ == yscale)
    return;

  if (str_ != "") {
    CSVGBuffer *buffer;

    parent_->decodeXLink(str_, &th->object_, &buffer);

    if (buffer) {
      th->initImageBuffer();

      th->isImage_     = true;
      th->imageBuffer_ = svg.getBuffer(parent_->getUniqueName() + "_xlink_image");

      th->imageBuffer_->setImageBuffer(buffer);
    }
  }

  th->xscale_   = xscale;
  th->yscale_   = yscale;
  th->resolved_ = true;
}

void
CSVGXLink::
initImageBuffer()
{
  CSVG &svg = parent_->getSVG();

  imageBuffer_ = svg.getBuffer(parent_->getUniqueName() + "_xlink_image");
}
