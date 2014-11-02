#include <CSVGI.h>

CSVGFeFunc::
CSVGFeFunc(CSVG &svg, CColorComponent component) :
 CSVGObject (svg),
 component_ (component),
 type_      (LINEAR),
 slope_     (1),
 intercept_ (0),
 amplitude_ (1),
 exponent_  (1),
 offset_    (0),
 table_     ()
{
}

CSVGFeFunc::
CSVGFeFunc(const CSVGFeFunc &fe) :
 CSVGObject (fe),
 component_ (fe.component_),
 type_      (fe.type_),
 slope_     (fe.slope_),
 intercept_ (fe.intercept_),
 amplitude_ (fe.amplitude_),
 exponent_  (fe.exponent_ ),
 offset_    (fe.offset_   ),
 table_     (fe.table_    )
{
}

CSVGFeFunc *
CSVGFeFunc::
dup() const
{
  return new CSVGFeFunc(*this);
}

bool
CSVGFeFunc::
processOption(const string &opt_name, const string &opt_value)
{
  string         str;
  double         real;
  vector<double> reals;

  if      (svg_.stringOption(opt_name, opt_value, "type", str)) {
    if      (str == "identity") type_ = IDENTITY;
    else if (str == "linear"  ) type_ = LINEAR;
    else if (str == "gamma"   ) type_ = GAMMA;
    else if (str == "table"   ) type_ = TABLE;
    else if (str == "discrete") type_ = DISCRETE;
    else std::cerr << "Unrecognised type " << str << std::endl;
  }
  else if (svg_.realOption(opt_name, opt_value, "slope", &real))
    slope_ = real;
  else if (svg_.realOption(opt_name, opt_value, "intercept", &real))
    intercept_ = real;
  else if (svg_.realOption(opt_name, opt_value, "amplitude", &real))
    amplitude_ = real;
  else if (svg_.realOption(opt_name, opt_value, "exponent", &real))
    exponent_ = real;
  else if (svg_.realOption(opt_name, opt_value, "offset", &real))
    offset_ = real;
  else if (svg_.realListOption(opt_name, opt_value, "tableValues", reals))
    table_ = reals;
  else
    return CSVGObject::processOption(opt_name, opt_value);

  return true;
}

void
CSVGFeFunc::
draw()
{
}

CImagePtr
CSVGFeFunc::
filterImage(CImagePtr src_image)
{
  CImagePtr dst_image = src_image->dup();

  if      (type_ == IDENTITY) {
  }
  else if (type_ == LINEAR)
    dst_image->linearFunc(component_, slope_, intercept_);
  else if (type_ == GAMMA)
    dst_image->gammaFunc(component_, amplitude_, exponent_, offset_);
  else if (type_ == TABLE)
    dst_image->tableFunc(component_, table_);
  else if (type_ == DISCRETE)
    dst_image->discreteFunc(component_, table_);

  return dst_image;
}

void
CSVGFeFunc::
print(ostream &os) const
{
  os << "feFunc";
}

ostream &
operator<<(ostream &os, const CSVGFeFunc &filter)
{
  filter.print(os);

  return os;
}
