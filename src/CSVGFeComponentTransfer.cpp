#include <CSVGFeComponentTransfer.h>
#include <CSVGFeFunc.h>
#include <CSVG.h>

CSVGFeComponentTransfer::
CSVGFeComponentTransfer(CSVG &svg) :
 CSVGFilter(svg)
{
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
processOption(const std::string &opt_name, const std::string &opt_value)
{
  std::string str;

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
  CImagePtr src_image = svg_.getBufferImage(filter_in_.getValue("SourceGraphic"));

  CImagePtr dst_image = filterImage(src_image);

  svg_.setBufferImage(filter_out_.getValue("SourceGraphic"), dst_image);
}

CImagePtr
CSVGFeComponentTransfer::
filterImage(CImagePtr src_image)
{
  CImagePtr dst_image = src_image->dup();

  for (const auto &c : children()) {
    CSVGFeFunc *func = dynamic_cast<CSVGFeFunc *>(c);

    if (func)
      dst_image = func->filterImage(dst_image);
  }

  return dst_image;
}

void
CSVGFeComponentTransfer::
print(std::ostream &os, bool hier) const
{
  if (hier) {
    os << "<feComponentTransfer";

    CSVGObject::printValues(os);

    printNameValue(os, "in"    , filter_in_ );
    printNameValue(os, "result", filter_out_);

    os << ">" << std::endl;

    printChildren(os, hier);

    os << "</feComponentTransfer>" << std::endl;
  }
  else
    os << "feComponentTransfer ";
}

std::ostream &
operator<<(std::ostream &os, const CSVGFeComponentTransfer &fe)
{
  fe.print(os, false);

  return os;
}
