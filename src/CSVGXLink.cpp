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
 xscale_  (xlink.xscale_),
 yscale_  (xlink.yscale_)
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
  xscale_   = xlink.xscale_;
  yscale_   = xlink.yscale_;

  return *this;
}

void
CSVGXLink::
resolve() const
{
  double xscale = parent_->getSVG().xscale();
  double yscale = parent_->getSVG().yscale();

  if (resolved_ && xscale_ == xscale && yscale_ == yscale)
    return;

  CSVGXLink *th = const_cast<CSVGXLink *>(this);

  if (str_ != "")
    parent_->decodeXLink(str_, &th->object_, &th->image_);

  th->xscale_   = xscale;
  th->yscale_   = yscale;
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
