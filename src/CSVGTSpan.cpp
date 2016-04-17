#include <CSVGTSpan.h>
#include <CSVGText.h>
#include <CSVG.h>
#include <CSVGLog.h>

/* Attributes:
    <Core>
    <Conditional>
    <GraphicalEvents>
    <Presentation>
    class
    style
    externalResourcesRequired
    x
    y
    dx
    dy
    rotate
    textLength
    lengthAdjust
*/

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
 dx_       (span.dx_),
 dy_       (span.dy_)
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
  double real;
  Reals  reals;

  if      (svg_.realOption(opt_name, opt_value, "x" , &real))
    x_ = real;
  else if (svg_.realOption(opt_name, opt_value, "y" , &real))
    y_ = real;
  else if (svg_.realListOption(opt_name, opt_value, "dx", reals))
    dx_ = reals;
  else if (svg_.realListOption(opt_name, opt_value, "dy", reals))
    dy_ = reals;
  else
    return CSVGObject::processOption(opt_name, opt_value);

  return true;
}

bool
CSVGTSpan::
getBBox(CBBox2D &bbox) const
{
  if (! viewBox_.isValid()) {
    double x = 0, y = 0;

    getDrawPos(x, y);

    double w = 8;
    double a = 10;
    double d = 2;

    svg_.textSize(getText(), getFont(), &w, &a, &d);

    bbox = CBBox2D(x, y + d, x + w, y - a);
  }
  else
    bbox = getViewBox();

  return true;
}

void
CSVGTSpan::
moveTo(const CPoint2D &p)
{
  x_ = p.x;
  y_ = p.y;
}

void
CSVGTSpan::
moveBy(const CVector2D &delta)
{
  x_ = getX() + delta.x();
  y_ = getY() + delta.y();
}

void
CSVGTSpan::
draw()
{
  if (svg_.getDebug())
    CSVGLog() << *this;

  double x = 0, y = 0;

  getDrawPos(x, y);

  if (svg_.isFilled() || svg_.isStroked()) {
    if (svg_.isFilled())
      svg_.fillText(x, y, getText(), getFont(), getTextAnchor());

    if (svg_.isStroked())
      svg_.drawText(x, y, getText(), getFont(), getTextAnchor());
  }
  else
    svg_.fillText(x, y, getText(), getFont(), getTextAnchor());

  double w, a, d;

  svg_.textSize(getText(), getFont(), &w, &a, &d);

  //---

  CSVGText *text = getParentText();

  if (text)
    text->setLastPos(CPoint2D(x + w, y));
}

void
CSVGTSpan::
getDrawPos(double &x, double &y) const
{
  CSVGText *text = getParentText();

  CPoint2D lastPos;

  if (text)
    lastPos = text->lastPos();

  //---

  x = lastPos.x;
  y = lastPos.y;

  if      (x_ .isValid())
    x = x_.getValue();
  else if (dx_.isValid()) {
    Reals reals = dx_.getValue();

    x += (! reals.empty() ? reals[0] : 0);
  }

  if      (y_ .isValid())
    y = y_.getValue();
  else if (dy_.isValid()) {
    Reals reals = dy_.getValue();

    y += (! reals.empty() ? reals[0] : 0);
  }
}

CSVGText *
CSVGTSpan::
getParentText() const
{
  CSVGObject *parent = getParent();

  while (parent && ! dynamic_cast<CSVGText *>(parent))
    parent = parent->getParent();

  return dynamic_cast<CSVGText *>(parent);
}

void
CSVGTSpan::
print(std::ostream &os, bool hier) const
{
  if (hier) {
    os << "<tspan";

    CSVGObject::printValues(os);

    printNameValue (os, "x" , x_ );
    printNameValue (os, "y" , y_ );
    printNameValues(os, "dx", dx_);
    printNameValues(os, "dy", dy_);

    os << ">";

    if (hasChildren()) {
      os << std::endl;

      if (hasText())
        os << getText() << std::endl;

      printChildren(os, hier);
    }
    else {
      os << getText();
    }

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
