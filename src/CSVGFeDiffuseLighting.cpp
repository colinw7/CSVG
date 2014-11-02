#include <CSVGI.h>

CSVGFeDiffuseLighting::
CSVGFeDiffuseLighting(CSVG &svg) :
 CSVGFilter (svg),
 filter_in_ (),
 filter_out_()
{
  filter_in_  = "SourceGraphic";
  filter_out_ = "SourceGraphic";
}

CSVGFeDiffuseLighting::
CSVGFeDiffuseLighting(const CSVGFeDiffuseLighting &fe) :
 CSVGFilter (fe),
 filter_in_ (fe.filter_in_ ),
 filter_out_(fe.filter_out_)
{
}

CSVGFeDiffuseLighting *
CSVGFeDiffuseLighting::
dup() const
{
  return new CSVGFeDiffuseLighting(*this);
}

bool
CSVGFeDiffuseLighting::
processOption(const string &opt_name, const string &opt_value)
{
  string str;

  if      (svg_.stringOption(opt_name, opt_value, "in", str))
    filter_in_ = str;
  else if (svg_.stringOption(opt_name, opt_value, "result", str))
    filter_out_ = str;
  else if (svg_.stringOption(opt_name, opt_value, "lighting-color", str))
    ;
  else if (svg_.stringOption(opt_name, opt_value, "surfaceScale", str))
    ;
  else if (svg_.stringOption(opt_name, opt_value, "diffuseConstant", str))
    ;
  else
    return CSVGObject::processOption(opt_name, opt_value);

  return true;
}

void
CSVGFeDiffuseLighting::
draw()
{
  CImagePtr src_image = svg_.getBufferImage(filter_in_);

  CImagePtr dst_image = filterImage(src_image);

  svg_.setBufferImage(filter_out_, dst_image);
}

CImagePtr
CSVGFeDiffuseLighting::
filterImage(CImagePtr src_image)
{
  CImagePtr dst_image = src_image->dup();

  return dst_image;
}

void
CSVGFeDiffuseLighting::
print(ostream &os) const
{
  os << "feDiffuseLighting ";
}

ostream &
operator<<(ostream &os, const CSVGFeDiffuseLighting &fe)
{
  fe.print(os);

  return os;
}
