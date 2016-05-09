#include <CSVGTextPath.h>
#include <CSVGText.h>
#include <CSVGPath.h>
#include <CSVGBuffer.h>
#include <CSVG.h>
#include <CSVGLog.h>
#include <CSVGUtil.h>
#include <CUtf8.h>

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
  CSVGLengthValue length;
  std::string     str;

  if      (svg_.percentOption(opt_name, opt_value, "startOffset", length))
    startOffset_ = length;
  else if (svg_.stringOption(opt_name, opt_value, "xlink:href", str))
    xlink_ = CSVGXLink(this, str);
  else
    return CSVGObject::processOption(opt_name, opt_value);

  return true;
}

void
CSVGTextPath::
draw()
{
  if (svg_.getDebug())
    CSVGLog() << *this;

  CSVGObject *obj = (xlink_.isValid() ? xlink_.getValue().getObject() : 0);
  if (! obj) return;

  CSVGPath *path = dynamic_cast<CSVGPath *>(obj);
  if (! path) return;

  CSVGText *parentText = getParentText();
  if (! parentText) return;

  //---

  CSVGBuffer *currentBuffer = svg_.getBuffer();

  CMatrixStack2D transform = currentBuffer->transform();

  CMatrixStack2D transform1(transform);

  transform1.append(path->getTransform());

  svg_.setTransform(transform1);

  //---

  std::string text   = getText();
  CHAlignType anchor = getFlatTextAnchor();
  CFontPtr    font   = getFont();

  const CSVGPathPartList &parts = path->getPartList();

  double l   = parts.getLength();
  int    len = text.length();

  double s1   = 0, s2   = getStartOffset().value();
  int    pos1 = 0, pos2 = 0;

  //---

  double w, a, d;

  svg_.textSize(text, font, &w, &a, &d);

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

    svg_.textSize(text1, font, &w1, &a1, &d1);

    s2 = s1 + w1/l;

    //---

    double xi1, yi1, ai1; int pi1;

    if (! parts.interp(s1, &xi1, &yi1, &ai1, &pi1))
      break;

    double xi2, yi2, ai2; int pi2;

    if (! parts.interp(s2, &xi2, &yi2, &ai2, &pi2))
      break;

    double ai = ai1;

    if (pi1 == pi2) {
      CSVGUtil::adjustAngles(ai1, ai2);

      ai = (ai1 + ai2)/2;
    }

    //---

    CFontPtr font1 = font;

    if (ai)
      font1 = font->dup(font->getFamily(), font->getStyle(), font->getSize(),
                        ai, font->getCharAngle(), font->getXRes(), font->getYRes());

    //---

    svg_.fillDrawText(xi1, yi1, text1, font1, anchor, svg_.isFilled(), svg_.isStroked());
  }

  //---

  svg_.setTransform(transform);
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

    printNameLength(os, "startOffset", startOffset_);
    printNameXLink (os, "xlink:href" , xlink_);

    CSVGObject::printValues(os);

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

std::ostream &
operator<<(std::ostream &os, const CSVGTextPath &text)
{
  text.print(os, false);

  return os;
}
