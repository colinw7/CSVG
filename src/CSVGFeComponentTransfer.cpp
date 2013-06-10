#include "CSVGI.h"

CSVGFeComponentTransfer::
CSVGFeComponentTransfer(CSVG &svg) :
 CSVGFilter (svg),
 filter_in_ (),
 filter_out_()
{
  filter_in_  = "SourceGraphic";
  filter_out_ = "SourceGraphic";
}

CSVGFeComponentTransfer::
CSVGFeComponentTransfer(const CSVGFeComponentTransfer &fe) :
 CSVGFilter (fe),
 filter_in_ (fe.filter_in_),
 filter_out_(fe.filter_out_)
{
}

CSVGFeComponentTransfer *
CSVGFeComponentTransfer::
dup() const
{
  return new CSVGFeComponentTransfer(*this);
}

bool
CSVGFeComponentTransfer::
processOption(const string &opt_name, const string &opt_value)
{
  string str;

  if      (svg_.stringOption(opt_name, opt_value, "in", str))
    filter_in_ = str;
  else if (svg_.stringOption(opt_name, opt_value, "result", str))
    filter_out_ = str;
  else
    return CSVGObject::processOption(opt_name, opt_value);

  return true;
}

void
CSVGFeComponentTransfer::
draw()
{
  CImagePtr src_image = svg_.getBufferImage(filter_in_);

  CImagePtr dst_image = filterImage(src_image);

  svg_.setBufferImage(filter_out_, dst_image);
}

CImagePtr
CSVGFeComponentTransfer::
filterImage(CImagePtr src_image)
{
  CImagePtr dst_image = src_image->dup();

  ObjectList::const_iterator p1, p2;

  for (p1 = childrenBegin(), p2 = childrenEnd(); p1 != p2; ++p1) {
    CSVGFeFunc *func = dynamic_cast<CSVGFeFunc *>(*p1);

    if (func)
      dst_image = func->filterImage(dst_image);
  }

  return dst_image;
}

void
CSVGFeComponentTransfer::
print(ostream &os) const
{
  os << "feComponentTransfer ";
}

ostream &
operator<<(ostream &os, const CSVGFeComponentTransfer &fe)
{
  fe.print(os);

  return os;
}
