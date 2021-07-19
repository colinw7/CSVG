#include <CSVGText.h>
#include <CSVG.h>
#include <CSVGLog.h>

/* Attributes:
    <Core>
    <Conditional>
    <Style>
    <Text>
    <TextContent>
    <Font>
    <Paint>
    <Color>
    <Opacity>
    <Graphics>
    <Clip>
    <Mask>
    <Filter>
    <GraphicalEvents>
    <Cursor>
    <External>
    x <Coordinates>
    y <Coordinates>
    dx <Lengths>
    dy <Lengths>
    rotate <Numbers>
    textLength <Length>
    lengthAdjust ( spacing | spacingAndGlyphs ) #IMPLIED
    transform <TransformList>
*/
CSVGText::
CSVGText(CSVG &svg) :
 CSVGObject(svg)
{
}

CSVGText::
CSVGText(const CSVGText &text) :
 CSVGObject(text),
 x_           (text.x_),
 y_           (text.y_),
 dx_          (text.dx_),
 dy_          (text.dy_),
 rotate_      (text.rotate_),
 textLength_  (text.textLength_),
 lengthAdjust_(text.lengthAdjust_)
{
}

CSVGText *
CSVGText::
dup() const
{
  return new CSVGText(*this);
}

bool
CSVGText::
processOption(const std::string &opt_name, const std::string &opt_value)
{
  if (processCoreOption           (opt_name, opt_value)) return true;
  if (processConditionalOption    (opt_name, opt_value)) return true;
  if (processStyleOption          (opt_name, opt_value)) return true;
  if (processTextOption           (opt_name, opt_value)) return true;
  if (processTextContentOption    (opt_name, opt_value)) return true;
  if (processFontOption           (opt_name, opt_value)) return true;
  if (processPaintOption          (opt_name, opt_value)) return true;
  if (processColorOption          (opt_name, opt_value)) return true;
  if (processOpacityOption        (opt_name, opt_value)) return true;
  if (processGraphicsOption       (opt_name, opt_value)) return true;
  if (processClipOption           (opt_name, opt_value)) return true;
  if (processMaskOption           (opt_name, opt_value)) return true;
  if (processFilterOption         (opt_name, opt_value)) return true;
  if (processGraphicalEventsOption(opt_name, opt_value)) return true;
  if (processCursorOption         (opt_name, opt_value)) return true;
  if (processExternalOption       (opt_name, opt_value)) return true;

  std::string    str;
  Reals          reals;
  Coords         coords;
  Lengths        lengths;
  CMatrixStack2D transform;

  if      (svg_.coordListOption (opt_name, opt_value, "x", coords))
    x_ = coords;
  else if (svg_.coordListOption (opt_name, opt_value, "y", coords))
    y_ = coords;
  else if (svg_.lengthListOption(opt_name, opt_value, "dx", lengths))
    dx_ = lengths;
  else if (svg_.lengthListOption(opt_name, opt_value, "dy", lengths))
    dy_ = lengths;
  // editable = "none" | "simple"
  else if (svg_.realListOption(opt_name, opt_value, "rotate", reals))
    rotate_ = reals;
  // focusable = "true" | "false" | "auto"
  else if (svg_.stringOption(opt_name, opt_value, "textLength", str))
    textLength_ = str;
  else if (svg_.stringOption(opt_name, opt_value, "lengthAdjust", str))
    lengthAdjust_ = str;
  else if (svg_.transformOption(opt_name, opt_value, "transform", transform))
    setTransform(transform);
  else
    return false;

  return true;
}

CScreenUnits
CSVGText::
getX() const
{
  if (! x_.isValid() || x_.getValue().empty())
    return 0;

  return x_.getValue().front();
}

void
CSVGText::
setX(const CScreenUnits &x)
{
  Coords coords { x };

  x_ = coords;
}

CScreenUnits
CSVGText::
getY() const
{
  if (! y_.isValid() || y_.getValue().empty())
    return 0;

  return y_.getValue().front();
}

void
CSVGText::
setY(const CScreenUnits &y)
{
  Coords coords { y };

  y_ = coords;
}

CScreenUnits
CSVGText::
getDX() const
{
  if (! dx_.isValid() || dx_.getValue().empty())
    return 0;

  return dx_.getValue().front();
}

void
CSVGText::
setDX(const CScreenUnits &x)
{
  Lengths lengths { x };

  dx_ = lengths;
}

CScreenUnits
CSVGText::
getDY() const
{
  if (! dy_.isValid() || dy_.getValue().empty())
    return 0;

  return dy_.getValue().front();
}

void
CSVGText::
setDY(const CScreenUnits &y)
{
  Lengths lengths { y };

  dy_ = lengths;
}

CPoint2D
CSVGText::
getPosition() const
{
  double x = getX().pxValue(1);
  double y = getY().pxValue(1);

  return CPoint2D(x, y);
}

bool
CSVGText::
getBBox(CBBox2D &bbox) const
{
  if (! hasViewBox()) {
    double x = getX().pxValue(1);
    double y = getY().pxValue(1);
    double w = 8;
    double a = 10;
    double d = 2;

    getFlatFontDef().textSize(getText(), &w, &a, &d);

    bbox = CBBox2D(x, y + d, x + w, y - a);

    CBBox2D bbox1;

    if (getChildrenBBox(bbox1))
      bbox += bbox1;
  }
  else
    bbox = getViewBox();

  return true;
}

void
CSVGText::
moveTo(const CPoint2D &p)
{
  setX(p.x);
  setY(p.y);
}

void
CSVGText::
moveBy(const CVector2D &delta)
{
  double x = getX().pxValue(1);
  double y = getY().pxValue(1);

  setX(x + delta.x());
  setY(y + delta.y());
}

COptString
CSVGText::
getNameValue(const std::string &name) const
{
  COptString str;

  if      (name == "x") {
    if (x_.isValid())
      str = valuesToString(x_.getValue());
  }
  else if (name == "y") {
    if (y_.isValid())
      str = valuesToString(y_.getValue());
  }
  else if (name == "dx") {
    if (dx_.isValid())
      str = valuesToString(dx_.getValue());
  }
  else if (name == "dy") {
    if (dy_.isValid())
      str = valuesToString(dy_.getValue());
  }
  else if (name == "rotate") {
    if (rotate_.isValid())
      str = valuesToString(rotate_.getValue());
  }
  else if (name == "lengthAdjust") {
    str = CStrUtil::toString(getLengthAdjustValue());
  }
  else
    str = CSVGObject::getNameValue(name);

  return str;
}

bool
CSVGText::
draw()
{
  if (svg_.getDebug())
    CSVGLog() << *this;

  auto text    = getText();
  auto anchor  = getFlatTextAnchor();
  auto fontDef = getFlatFontDef();

  bool topBottom = (getWritingMode() == "tb");

  double x = getX().pxValue(1);
  double y = getY().pxValue(1);

  bool renderChars = false;

  if (hasWordSpacing() || hasLetterSpacing())
    renderChars = true;

  //---

  if      (textLength_.isValid()) {
    double w, a, d;

    fontDef.textSize(text, &w, &a, &d);

    //---

    bool scaleFont = (getLengthAdjust() == "spacingAndGlyphs");

    //---

    COptReal tl;
    double   r;

    if (CStrUtil::toReal(getTextLength(), &r))
      tl = r;

    //---

    auto fontDef1 = fontDef;

    double dx = 0;
    double fs = 1;

    if (tl.isValid()) {
      if (scaleFont) {
        fs = tl.getValue()/w;

        fontDef1.setFamily   (fontDef.getFamily());
        fontDef1.setStyle    (fontDef.getStyle());
        fontDef1.setSize     (CSVGFontDef::FontSize(fontDef.getSize().getValue()*fs)),
        fontDef1.setAngle    (fontDef.getAngle()),
      //fontDef1.setCharAngle(fontDef.getCharAngle());

        fontDef1.textSize(text, &w, &a, &d);
      }

      if (text.size() > 1)
        dx = (tl.getValue() - w)/(text.size() - 1);
    }

    //---

    for (uint i = 0; i < text.size(); ++i) {
      auto text1 = text.substr(i, 1);

      //---

      double w1, a1, d1;

      fontDef1.textSize(text1, &w1, &a1, &d1);

      //---

      if (i > 0)
        x += dx;

      //---

      svg_.fillDrawText(x, y, text1, fontDef1, anchor, svg_.isFilled(), svg_.isStroked());

      //---

      if (i < text.size() - 1) {
        auto text2 = text.substr(i + 1, 1);

        x += w1;
      }

      setLastPos(CPoint2D(x, y));
    }
  }
  else if (! topBottom && renderChars) {
    double ws = 0;

    if (hasWordSpacing()) {
      ws = getWordSpacing().pxValue(1);
    }

    double ls = 0;

    if (hasLetterSpacing()) {
      ls = getLetterSpacing().pxValue(1);
    }

    //---

    for (uint i = 0; i < text.size(); ++i) {
      auto text1 = text.substr(i, 1);

      //---

      double w1, a1, d1;

      fontDef.textSize(text1, &w1, &a1, &d1);

      //---

      svg_.fillDrawText(x, y, text1, fontDef, anchor, svg_.isFilled(), svg_.isStroked());

      //---

      x += w1;

      if (i < text.size() - 1) {
        auto text3 = text.substr(i + 1, 1);

#if 0
        auto text2 = text.substr(i, 2);

        double w2, a2, d2;
        double w3, a3, d3;

        fontDef.textSize(text2, &w2, &a2, &d2);
        fontDef.textSize(text3, &w3, &a3, &d3);

        double dl = w2 - w1 - w3;

        x += dl;
#endif

        if (isspace(text3[0]))
          x += ws;
        else
          x += ls;
      }

      setLastPos(CPoint2D(x, y));
    }
  }
  else if (topBottom) {
    COptReal fa;
    double   r;

    if      (getVGlyphOrient() == "auto")
      fa = CMathGen::DegToRad(90);
    else if (CStrUtil::toReal(getVGlyphOrient(), &r))
      fa = r;

    auto fontDef1 = fontDef;

    if (fa.isValid()) {
      fontDef1.setFamily   (fontDef.getFamily());
      fontDef1.setStyle    (fontDef.getStyle ());
      fontDef1.setSize     (fontDef.getSize  ()),
      fontDef1.setAngle    (fa.getValue());
    //fontDef1.setCharAngle(fontDef.getCharAngle());
    }

    for (uint i = 0; i < text.size(); ++i) {
      auto text1 = text.substr(i, 1);

      //---

      double w1, a1, d1;

      fontDef1.textSize(text1, &w1, &a1, &d1);

      //---

      svg_.fillDrawText(x, y, text1, fontDef1, anchor, svg_.isFilled(), svg_.isStroked());

      //---

      double dy = w1*sin(fa.getValue()) + (a1 + d1)*cos(fa.getValue());

      y += dy;

      setLastPos(CPoint2D(x, y));
    }
  }
  else {
    double w, a, d;

    fontDef.textSize(text, &w, &a, &d);

    //---

    svg_.fillDrawText(x, y, text, fontDef, anchor, svg_.isFilled(), svg_.isStroked());

    //---

    setLastPos(CPoint2D(x + w, y));
  }

  return true;
}

void
CSVGText::
print(std::ostream &os, bool hier) const
{
  if (hier) {
    os << "<text";

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

    os << "</text>" << std::endl;
  }
  else
    os << "text " << getPosition()  << " " << CStrUtil::single_quote(getText());
}

void
CSVGText::
printValues(std::ostream &os, bool flat) const
{
  CSVGObject::printValues(os, flat);

  printNameValues(os, "x" , x_ );
  printNameValues(os, "y" , y_ );
  printNameValues(os, "dx", dx_);
  printNameValues(os, "dy", dy_);

  printNameValues(os, "rotate", rotate_);

  printNameValue(os, "textLength"  , textLength_  );
  printNameValue(os, "lengthAdjust", lengthAdjust_);
}

std::ostream &
operator<<(std::ostream &os, const CSVGText &text)
{
  text.print(os, false);

  return os;
}
