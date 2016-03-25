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
 position_ (span.position_),
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
print(std::ostream &os, bool hier) const
{
  if (hier) {
    os << "<tspan";

    os << " x=\"" << position_.x << "\"";
    os << " y=\"" << position_.y << "\"";

    if (hasFont())
      printFontDef(os);

    os << ">";

    os << text_;

    os << "</tspan>" << std::endl;
  }
  else
    os << "tspan " << position_ << " " << CStrUtil::single_quote(text_);
}

std::ostream &
operator<<(std::ostream &os, const CSVGTSpan &tspan)
{
  tspan.print(os, false);

  return os;
}
