#include <CSVGI.h>

CSVGFeTile::
CSVGFeTile(CSVG &svg) :
 CSVGFilter (svg),
 filter_in_ (),
 filter_out_()
{
  filter_in_  = "SourceGraphic";
  filter_out_ = "SourceGraphic";
}

CSVGFeTile::
CSVGFeTile(const CSVGFeTile &fe) :
 CSVGFilter (fe),
 filter_in_ (fe.filter_in_ ),
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
CSVGFeTile::
draw()
{
  CImagePtr src_image = svg_.getBufferImage(filter_in_);

  CImagePtr dst_image = filterImage(src_image);

  svg_.setBufferImage(filter_out_, dst_image);
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
print(ostream &os) const
{
  os << "feTile ";
}

ostream &
operator<<(ostream &os, const CSVGFeTile &fe)
{
  fe.print(os);

  return os;
}
