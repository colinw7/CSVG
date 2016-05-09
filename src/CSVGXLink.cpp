#include <CSVGXLink.h>
#include <CSVGBuffer.h>
#include <CSVG.h>

CSVGXLink::
CSVGXLink(const CSVGXLink &xlink) :
 parent_  (xlink.parent_),
 resolved_(xlink.resolved_),
 str_     (xlink.str_),
 object_  (xlink.object_),
 image_   (xlink.image_),
 scale_   (xlink.scale_)
{
}

CSVGXLink &
CSVGXLink::
operator=(const CSVGXLink &xlink)
{
  parent_   = xlink.parent_;
  resolved_ = xlink.resolved_;
  str_      = xlink.str_;
  object_   = xlink.object_;
  image_    = xlink.image_;
  scale_    = xlink.scale_;

  return *this;
}

void
CSVGXLink::
resolve() const
{
  double scale = parent_->getSVG().scale();

  if (resolved_ && scale_ == scale)
    return;

  CSVGXLink *th = const_cast<CSVGXLink *>(this);

  if (str_ != "")
    parent_->decodeXLink(str_, &th->object_, &th->image_);

  th->scale_    = scale;
  th->resolved_ = true;
}

bool
CSVGXLink::
getImage(CSVGBuffer *buffer) const
{
  if      (isImage())
    buffer->setImage(getImage());
  else if (isObject())
    buffer->setImage(getObject()->toImage());
  else
    return false;

  return true;
}
