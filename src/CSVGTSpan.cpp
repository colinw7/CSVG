#include "CSVGI.h"

CSVGTSpan::
CSVGTSpan(CSVG &svg) :
 CSVGObject(svg),
 position_ (0, 0),
 text_     (),
 font_     (NULL)
{
}

CSVGTSpan::
CSVGTSpan(const CSVGTSpan &span) :
 CSVGObject(span),
 position_ (span.position_),
 text_     (span.text_),
 font_     (span.font_)
{
}

CSVGTSpan *
CSVGTSpan::
dup() const
{
  return new CSVGTSpan(*this);
}

bool
CSVGTSpan::
processOption(const string &opt_name, const string &opt_value)
{
  string str;
  double real;

  if      (svg_.stringOption(opt_name, opt_value, "x", str)) {
    if (CStrUtil::isReal(str) && CStrUtil::toReal(str, &real))
      position_.x = real;
  }
  else if (svg_.stringOption(opt_name, opt_value, "y", str)) {
    if (CStrUtil::isReal(str) && CStrUtil::toReal(str, &real))
      position_.y = real;
  }
  else
    return CSVGObject::processOption(opt_name, opt_value);

  return true;
}

void
CSVGTSpan::
draw()
{
  if (svg_.getDebug())
    CSVGLog() << *this;

  if (svg_.isFilled())
    svg_.fillText(position_.x, position_.y, text_, getFont(), getTextAnchor());

  if (svg_.isStroked())
    svg_.drawText(position_.x, position_.y, text_, getFont(), getTextAnchor());
}

void
CSVGTSpan::
print(ostream &os) const
{
  os << "tspan " << position_ << " " << CStrUtil::single_quote(text_);
}

ostream &
operator<<(ostream &os, const CSVGTSpan &tspan)
{
  tspan.print(os);

  return os;
}
