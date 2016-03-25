#include <CSVGFeTile.h>
#include <CSVG.h>

CSVGFeTile::
CSVGFeTile(CSVG &svg) :
 CSVGFilter(svg)
{
}

CSVGFeTile::
CSVGFeTile(const CSVGFeTile &fe) :
 CSVGFilter (fe),
 filter_in_ (fe.filter_in_),
 filter_out_(fe.filter_out_)
{
}

CSVGFeTile *
CSVGFeTile::
dup() const
{
  return new CSVGFeTile(*this);
}

bool
CSVGFeTile::
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
CSVGFeTile::
draw()
{
  CImagePtr src_image = svg_.getBufferImage(filter_in_.getValue("SourceGraphic"));

  CImagePtr dst_image = filterImage(src_image);

  svg_.setBufferImage(filter_out_.getValue("SourceGraphic"), dst_image);
}

CImagePtr
CSVGFeTile::
filterImage(CImagePtr src_image)
{
  CImagePtr dst_image = src_image->dup();

  return dst_image;
}

void
CSVGFeTile::
print(std::ostream &os, bool hier) const
{
  if (hier) {
    os << "<feTile";

    printNameValue(os, "id", id_);

    printNameValue(os, "in"    , filter_in_ );
    printNameValue(os, "result", filter_out_);

    os << "</>" << std::endl;
  }
  else
    os << "feTile ";
}

std::ostream &
operator<<(std::ostream &os, const CSVGFeTile &fe)
{
  fe.print(os, false);

  return os;
}
