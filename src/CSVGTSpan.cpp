#include <CSVGTSpan.h>
#include <CSVG.h>
#include <CSVGLog.h>

CSVGTSpan::
CSVGTSpan(CSVG &svg) :
 CSVGObject(svg)
{
}

CSVGTSpan::
CSVGTSpan(const CSVGTSpan &span) :
 CSVGObject(span),
 x_        (span.x_),
 y_        (span.y_),
 text_     (span.text_)
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
processOption(const std::string &opt_name, const std::string &opt_value)
{
  std::string str;
  double      real;

  if      (svg_.stringOption(opt_name, opt_value, "x", str)) {
    if (CStrUtil::isReal(str) && CStrUtil::toReal(str, &real))
      x_ = real;
  }
  else if (svg_.stringOption(opt_name, opt_value, "y", str)) {
    if (CStrUtil::isReal(str) && CStrUtil::toReal(str, &real))
      y_ = real;
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
    svg_.fillText(getX(), getY(), getText(), getFont(), getTextAnchor());

  if (svg_.isStroked())
    svg_.drawText(getX(), getY(), getText(), getFont(), getTextAnchor());
}

void
CSVGTSpan::
print(std::ostream &os, bool hier) const
{
  if (hier) {
    os << "<tspan";

    CSVGObject::printValues(os);

    printNameValue(os, "x", x_);
    printNameValue(os, "y", y_);

    os << ">";

    os << getText();

    os << "</tspan>" << std::endl;
  }
  else
    os << "tspan " << getX() << getY() << " " << CStrUtil::single_quote(getText());
}

std::ostream &
operator<<(std::ostream &os, const CSVGTSpan &tspan)
{
  tspan.print(os, false);

  return os;
}
