#include <CSVGTextPath.h>
#include <CSVGText.h>
#include <CSVGPath.h>
#include <CSVGBuffer.h>
#include <CSVG.h>
#include <CSVGLog.h>
#include <CUtf8.h>

#if 0
namespace {

inline int angleQuadrant(double a) {
  while (a <  0     ) a += 2*M_PI;
  while (a >= 2*M_PI) a -= 2*M_PI;

  if (a <   M_PI/2) return 0;
  if (a <   M_PI  ) return 1;
  if (a < 3*M_PI/2) return 2;

  return 3;
}

inline void adjustAngles(double &a1, double &a2) {
  int seg1 = angleQuadrant(a1);
  int seg2 = angleQuadrant(a2);

  if      (seg1 == 0 && seg2 == 3)
    a2 -= 2*M_PI;
  else if (seg1 == 3 && seg2 == 0)
    a2 += 2*M_PI;
}

}
#endif

//---

CSVGTextPath::
CSVGTextPath(CSVG &svg) :
 CSVGObject(svg),
 xlink_    (this)
{
}

CSVGTextPath::
CSVGTextPath(const CSVGTextPath &text) :
 CSVGObject  (text),
 startOffset_(text.startOffset_),
 xlink_      (text.xlink_)
{
}

CSVGTextPath *
CSVGTextPath::
dup() const
{
  return new CSVGTextPath(*this);
}

void
CSVGTextPath::
setLinkName(const std::string &str)
{
  xlink_ = CSVGXLink(this, str);
}

bool
CSVGTextPath::
processOption(const std::string &opt_name, const std::string &opt_value)
{
  std::string  str;
  CScreenUnits length;

  if      (svg_.percentOption(opt_name, opt_value, "startOffset", length)) {
    if (length.units() != CScreenUnits::Units::PERCENT)
      startOffset_ = length.ratioValue(1);
    else
      startOffset_ = length;
  }
  else if (svg_.stringOption(opt_name, opt_value, "xlink:href", str))
    xlink_ = CSVGXLink(this, str);
  else
    return CSVGObject::processOption(opt_name, opt_value);

  return true;
}

bool
CSVGTextPath::
draw()
{
  if (svg_.getDebug())
    CSVGLog() << *this;

  CSVGObject *obj = (xlink_.isValid() ? xlink_.getValue().getObject() : nullptr);
  if (! obj) return false;

  auto *path = dynamic_cast<CSVGPath *>(obj);
  if (! path) return false;

  CSVGText *parentText = getParentText();
  if (! parentText) return false;

  //---

  auto *currentBuffer = svg_.getCurrentBuffer();

  auto transform = currentBuffer->transform();

  auto transform1 = transform;

  transform1.append(path->getTransform());

  svg_.setTransform(transform1);

  //---

  std::string text    = getText();
  CHAlignType anchor  = getFlatTextAnchor();
  CSVGFontDef fontDef = getFlatFontDef();

  const CSVGPathPartList &parts = path->getPartList();

  double l   = parts.getLength();
  int    len = text.length();

  double s1   = 0, s2   = 0;
  int    pos1 = 0, pos2 = 0;

  if (getStartOffset().units() == CScreenUnits::Units::PERCENT)
    s2 = getStartOffset().ratioValue(l);
  else {
    s2 = getStartOffset().pxValue();

    if (path->hasPathLength())
      s2 /= path->getPathLength();
    else
      s2 /= l;
  }

  //---

  double w, a, d;

  fontDef.textSize(text, &w, &a, &d);

  CHAlignType textAnchor = this->getFlatTextAnchor();

  if (textAnchor == CHALIGN_TYPE_CENTER)
    s2 -= w/(2*l);
  if (textAnchor == CHALIGN_TYPE_RIGHT)
    s2 -= w/l;

  //---

  while (pos1 < len) {
    s1   = s2;
    pos1 = pos2;

    (void) CUtf8::readNextChar(text, pos2);

    std::string text1 = text.substr(pos1, pos2 - pos1);

    //---

    double w1, a1, d1;

    fontDef.textSize(text1, &w1, &a1, &d1);

    s2 = s1 + w1/l;

    //---

    double xi1, yi1, ai1; int pi1;

    if (! parts.interp(s1, &xi1, &yi1, &ai1, &pi1))
      break;

    double xi2, yi2, ai2; int pi2;

    if (! parts.interp(s2, &xi2, &yi2, &ai2, &pi2))
      break;

    double ai = ai1;

#if 0
    if (pi1 == pi2) {
std::cerr << "Adjust angles " << ai1 << " " << ai2 << std::endl;
      adjustAngles(ai1, ai2);

      ai = (ai1 + ai2)/2;
std::cerr << "New angles " << ai1 << " " << ai2 << std::endl;
    }
#endif

    //---

    CSVGFontDef fontDef1 = fontDef;

    if (ai) {
      fontDef1.setFamily   (fontDef.getFamily());
      fontDef1.setStyle    (fontDef.getStyle ());
      fontDef1.setSize     (fontDef.getSize  ()),
      fontDef1.setAngle    (ai);
    //fontDef1.setCharAngle(fontDef.getCharAngle());
    }

    //---

    svg_.fillDrawText(xi1, yi1, text1, fontDef1, anchor, svg_.isFilled(), svg_.isStroked());
  }

  //---

  svg_.setTransform(transform);

  return true;
}

CSVGText *
CSVGTextPath::
getParentText() const
{
  CSVGObject *parent = getParent();

  while (parent && ! dynamic_cast<CSVGText *>(parent))
    parent = parent->getParent();

  return dynamic_cast<CSVGText *>(parent);
}

void
CSVGTextPath::
print(std::ostream &os, bool hier) const
{
  if (hier) {
    os << "<textPath";

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

    os << "</textPath>" << std::endl;
  }
  else
    os << "textPath ";
}

void
CSVGTextPath::
printValues(std::ostream &os, bool flat) const
{
  printNamePercent(os, "startOffset", startOffset_);

  if (! xlink_.getValue().isNull())
    printNameXLink (os, "xlink:href", xlink_);

  CSVGObject::printValues(os, flat);
}

std::ostream &
operator<<(std::ostream &os, const CSVGTextPath &text)
{
  text.print(os, false);

  return os;
}
