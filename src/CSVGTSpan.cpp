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

void
CSVGTSpan::
setText(const std::string &text)
{
  text_ = text;
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
  if (! hasViewBox()) {
    double x = 0, y = 0;

    getDrawPos(x, y, 0);

    double w = 8;
    double a = 10;
    double d = 2;

    getFlatFontDef().textSize(getText(), &w, &a, &d);

    bbox = CBBox2D(x, y + d, x + w, y - a);
  }
  else
    bbox = getViewBox();

  return true;
}

void
CSVGTSpan::
moveDelta(const CVector2D &delta)
{
  x_ = getX() + delta.x();
  y_ = getY() + delta.y();
}

bool
CSVGTSpan::
draw()
{
  if (svg_.getDebug())
    CSVGLog() << *this;

  auto *parentText = getParentText();

  auto text    = getText();
  auto anchor  = getFlatTextAnchor();
  auto fontDef = getFlatFontDef();

  double x = 0, y = 0;

  if (getDX().size() > 1 || getDY().size() > 1) {
    for (uint i = 0; i < text.size(); ++i) {
      std::string text1 = text.substr(i, 1);

      //---

      double w, a, d;

      fontDef.textSize(text1, &w, &a, &d);

      //---

      getDrawPos(x, y, i);

      double y1 = y;

      if      (fontDef.isSubscript())
        y1 += a/2;
      else if (fontDef.isSuperscript())
        y1 -= a/2;

      svg_.fillDrawText(x, y1, text1, fontDef, anchor, svg_.isFilled(), svg_.isStroked());

      //---

      if (parentText)
        parentText->setLastPos(CPoint2D(x + w, y));
    }
  }
  else {
    double w, a, d;

    fontDef.textSize(text, &w, &a, &d);

    //---

    getDrawPos(x, y, 0);

    double y1 = y;

    if      (fontDef.isSubscript())
      y1 += a/2;
    else if (fontDef.isSuperscript())
      y1 -= a/2;

    svg_.fillDrawText(x, y1, text, fontDef, anchor, svg_.isFilled(), svg_.isStroked());

    //---

    if (parentText)
      parentText->setLastPos(CPoint2D(x + w, y));
  }

  return true;
}

void
CSVGTSpan::
getDrawPos(double &x, double &y, int i) const
{
  auto *parentText = getParentText();

  CPoint2D lastPos;

  if (parentText)
    lastPos = parentText->lastPos();

  //---

  x = lastPos.x;
  y = lastPos.y;

  if      (x_ .isValid())
    x = x_.getValue();
  else if (dx_.isValid()) {
    Reals reals = dx_.getValue();

    if (i < int(reals.size()))
      x += reals[i];
  }

  if      (y_ .isValid())
    y = y_.getValue();
  else if (dy_.isValid()) {
    Reals reals = dy_.getValue();

    if (i < int(reals.size()))
      y += reals[i];
  }
}

CSVGText *
CSVGTSpan::
getParentText() const
{
  auto *parent = getParent();

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

    printValues(os);

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

void
CSVGTSpan::
printValues(std::ostream &os, bool flat) const
{
  CSVGObject::printValues(os, flat);

  printNameValue (os, "x" , x_ );
  printNameValue (os, "y" , y_ );
  printNameValues(os, "dx", dx_);
  printNameValues(os, "dy", dy_);
}
