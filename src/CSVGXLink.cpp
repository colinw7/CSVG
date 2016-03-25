#include <CSVGXLink.h>
#include <CSVG.h>

CSVGXLink::
CSVGXLink(const CSVGXLink &xlink) :
 parent_  (xlink.parent_),
 resolved_(xlink.resolved_),
 str_     (xlink.str_),
 object_  (xlink.object_),
 image_   (xlink.image_)
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

  return *this;
}

void
CSVGXLink::
resolve() const
{
  if (resolved_) return;

  CSVGXLink *th = const_cast<CSVGXLink *>(this);

  parent_->decodeXLink(str_, &th->object_, th->image_);

  th->resolved_ = true;
}
