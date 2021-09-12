#include <CSVGStroke.h>
#include <CSVG.h>
#include <CSVGLog.h>
#include <CRegExp.h>
#include <CStrParse.h>
#include <CMathRound.h>

bool
CSVGStroke::
isStroked() const
{
  if (getUrlValid())
    return true;

  if (getFillObjectValid())
    return true;

  if (! getColorValid() || getColor().isInherit())
    return false;

  if (getColor().getValue().isNone())
    return false;

  return true;
}

// <paint> | inherit
void
CSVGStroke::
setColor(const std::string &colorDef)
{
  CStrParse parse(colorDef);

  parse.skipSpace();

  while (true) {
    std::string word;

    if (parse.isChar('#')) {
      word += '#';

      parse.skipChar();

      char c(0);

      while (parse.readChar(&c) && ! isspace(c))
        word += c;
    }
    else {
      if (! parse.readIdentifier(word))
        break;

      if (parse.isChar('(')) {
        word += '(';

        parse.skipChar();

        char c(0);

        while (parse.readChar(&c) && c != ')')
          word += c;

        if (c == ')')
          word += c;
      }
    }

    //------

    resetColor     ();
  //resetUrl       ();
  //resetFillObject();

    std::vector<std::string> matchStrs;

    if      (CRegExpUtil::parse(word, "url(#\\(.*\\))", matchStrs)) {
      setUrl(matchStrs[0]);
    }
    else {
      CSVGColor color;
      bool      inherit;

      if (svg_.decodeColorString(word, color, inherit))
        setColor(! inherit ? Color(color) : Color::makeInherit());
    }

    //------

    parse.skipSpace();
  }
}

// <opacity-value> | inherit
void
CSVGStroke::
setOpacity(const std::string &opacityDef)
{
  double opacity;
  bool   inherit;

  if (! svg_.decodeOpacityString(opacityDef, opacity, inherit)) {
    CSVGLog() << "Bad opacity value " << opacityDef;
    return;
  }

  setOpacity(! inherit ? Opacity(opacity) : Opacity::makeInherit());
}

// <fill-rule> | inherit
void
CSVGStroke::
setRule(const std::string &ruleDef)
{
  auto rule = svg_.decodeFillRuleString(ruleDef);

  setRule(rule);
}

CSVGObject *
CSVGStroke::
calcFillObject() const
{
  if      (getUrlValid())
    return svg_.lookupObjectById(url_.getValue());
  else if (getFillObjectValid())
    return fillObject_.getValue();
  else
    return nullptr;
}

// <length> | inherit
void
CSVGStroke::
setWidth(const std::string &widthDef)
{
  double width;
  bool   inherit;

  if (! svg_.decodeWidthString(widthDef, width, inherit)) {
    CSVGLog() << "Illegal width value " << widthDef;
    return;
  }

  setWidth(! inherit ? Width(width) : Width::makeInherit());
}

// none | <list-of-lengths> | inherit
void
CSVGStroke::
setDashArray(const std::string &dashStr)
{
  if (dashStr == "inherit") {
    lineDash_ = DashArray::makeInherit();
  }
  else {
    CSVGLineDash::Dashes dashes;
    bool                 solid;

    if (! svg_.decodeDashString(dashStr, dashes, solid)) {
      CSVGLog() << "Bad dash string " << dashStr;
      return;
    }

    if      (solid)
      lineDash_ = DashArray(CSVGLineDash::makeSolid());
    else if (dashes.empty())
      lineDash_ = DashArray(CSVGLineDash::makeNone());
    else
      lineDash_ = DashArray(CSVGLineDash(dashes));
  }
}

// <length> | inherit
void
CSVGStroke::
setDashOffset(const std::string &offsetStr)
{
  if (offsetStr == "inherit") {
    lineOffset_ = DashOffset::makeInherit();
  }
  else {
    auto lvalue = svg_.decodeLengthValue(offsetStr);

    if (! lvalue.isValid())
      return;

    lineOffset_ = DashOffset(lvalue.getValue());
  }
}

// butt | round | square | inherit
void
CSVGStroke::
setLineCap(const std::string &capStr)
{
  CLineCapType cap     = LINE_CAP_TYPE_NONE;
  bool         inherit = false;

  if      (capStr == "butt")
    cap = LINE_CAP_TYPE_BUTT;
  else if (capStr == "round")
    cap = LINE_CAP_TYPE_ROUND;
  else if (capStr == "square")
    cap = LINE_CAP_TYPE_SQUARE;
  else if (capStr == "inherit")
    inherit = true;
  else if (capStr == "none")
    cap = LINE_CAP_TYPE_NONE;
  else {
    CSVGLog() << "Illegal line_cap " << capStr;
    return;
  }

  setLineCap(! inherit ? LineCap(cap) : LineCap::makeInherit());
}

// miter | round | bevel | inherit
void
CSVGStroke::
setLineJoin(const std::string &joinStr)
{
  CLineJoinType join    = LINE_JOIN_TYPE_NONE;
  bool          inherit = false;

  if      (joinStr == "mitre" || joinStr == "miter")
    join = LINE_JOIN_TYPE_MITRE;
  else if (joinStr == "round")
    join = LINE_JOIN_TYPE_ROUND;
  else if (joinStr == "bevel")
    join = LINE_JOIN_TYPE_BEVEL;
  else if (joinStr == "inherit")
    inherit = true;
  else if (joinStr == "none")
    join = LINE_JOIN_TYPE_NONE;
  else {
    CSVGLog() << "Illegal line_join " << joinStr;
    return;
  }

  setLineJoin(! inherit ? LineJoin(join) : LineJoin::makeInherit());
}

// <miterlimit> | inherit
void
CSVGStroke::
setMiterLimit(const std::string &limitStr)
{
  double miterlimit = 0.0;
  bool   inherit    = false;

  if (limitStr == "inherit")
    inherit = true;
  else {
    auto lvalue = svg_.decodeLengthValue(limitStr);

    if (! lvalue.isValid())
      return;

    miterlimit = lvalue.getValue().pxValue(1);
  }

  setMiterLimit(! inherit ? MiterLimit(miterlimit) : MiterLimit::makeInherit());
}

void
CSVGStroke::
reset()
{
  color_     .setInvalid();
  opacity_   .setInvalid();
  rule_      .setInvalid();
  url_       .setInvalid();
  fillObject_.setInvalid();
  width_     .setInvalid();
  lineDash_  .setInvalid();
  lineOffset_.setInvalid();
  lineCap_   .setInvalid();
  lineJoin_  .setInvalid();
  mlimit_    .setInvalid();
}

void
CSVGStroke::
update(const CSVGStroke &stroke)
{
  auto *styleObject = svg_.styleObject();

  // color
  if      (stroke.getColorValid() && ! stroke.getColor().isInherit())
    setColor(stroke.getColor());
  else if (styleObject) {
    if (styleObject->getStrokeColorValid() && ! styleObject->getStrokeColor().isInherit())
      setColor(styleObject->getFlatStrokeColor().getValue());
  }

  // opacity
  if      (stroke.getOpacityValid())
    setOpacity(stroke.getOpacity());
  else if (styleObject) {
    if (styleObject->getStrokeOpacityValid() && ! styleObject->getStrokeOpacity().isInherit())
      opacity_ = styleObject->getFlatStrokeOpacity().getValue();
  }

  // rule
  if      (stroke.getRuleValid())
    setRule(stroke.getRule());
  else if (styleObject) {
    if (styleObject->getStrokeRuleValid())
      rule_ = styleObject->getFlatStrokeRule().getValue();
  }

  // url
  if      (stroke.getUrlValid())
    setUrl(stroke.getUrl());
  else if (styleObject) {
    if (styleObject->getStrokeUrlValid())
      url_ = styleObject->getFlatStrokeUrl().getValue();
  }

  // fill object
  if      (stroke.getFillObjectValid())
    setFillObject(stroke.getFillObject());
  else if (styleObject) {
    if (styleObject->getStrokeFillObjectValid())
      fillObject_ = styleObject->getFlatStrokeFillObject().getValue();
  }

  // width
  if      (stroke.getWidthValid())
    setWidth(stroke.getWidth());
  else if (styleObject) {
    if (styleObject->getStrokeWidthValid())
      setWidth(styleObject->getFlatStrokeWidth().getValue());
  }

  // dash array
  if      (stroke.getDashArrayValid())
    setDashArray(stroke.getDashArray());
  else if (styleObject) {
    if (styleObject->getStrokeDashArrayValid())
      setDashArray(styleObject->getFlatStrokeDashArray().getValue());
  }

  // dash offset
  if      (stroke.getDashOffsetValid())
    setDashOffset(stroke.getDashOffset());
  else if (styleObject) {
    if (styleObject->getStrokeDashOffsetValid())
      setDashOffset(styleObject->getFlatStrokeDashOffset().getValue());
  }

  // line cap
  if      (stroke.getLineCapValid())
    setLineCap(stroke.getLineCap());
  else if (styleObject) {
    if (styleObject->getStrokeLineCapValid())
      setLineCap(styleObject->getFlatStrokeLineCap().getValue());
  }

  // line join
  if      (stroke.getLineJoinValid())
    setLineJoin(stroke.getLineJoin());
  else if (styleObject) {
    if (styleObject->getStrokeLineJoinValid())
      setLineJoin(styleObject->getFlatStrokeLineJoin().getValue());
  }

  // miter limit
  if      (stroke.getMiterLimitValid())
    setMiterLimit(stroke.getMiterLimit());
  else if (styleObject) {
    if (styleObject->getStrokeMiterLimitValid())
      setMiterLimit(styleObject->getFlatStrokeMiterLimit().getValue());
  }

  //---

  if (getenv("CSVG_DEBUG_STROKE")) {
    std::cerr << "Update Stroke";
    if (styleObject && styleObject->getId() != "")
      std::cerr << "(" << styleObject->getId() << ")";
    std::cerr << ":";
    print(std::cerr);
    std::cerr << std::endl;
  }
}

void
CSVGStroke::
print(std::ostream &os) const
{
  std::stringstream ss;

  if (getColorValid())
    ss << "stroke:" << getColor() << ";";

  if (getOpacityValid()) {
    if (ss.str() != "") ss << " ";

    ss << "stroke-opacity:" << getOpacity() << ";";
  }

  if (getUrlValid()) {
    if (ss.str() != "") ss << " ";

    ss << "stroke:url(#" << getUrl() << ");";
  }

  if (getWidthValid()) {
    if (ss.str() != "") ss << " ";

    ss << "stroke-width:" << getWidth() << ";";
  }

  if (getDashArrayValid()) {
    if (ss.str() != "") ss << " ";

    ss << "stroke-dasharray:";

    if (getDashArray().isInherit())
      ss << "inherit";
    else
      getDashArray().getValue().print(ss);

    ss << ";";
  }

  if (getDashOffsetValid()) {
    if (ss.str() != "") ss << " ";

    ss << "stroke-dashoffset:";

    if (getDashOffset().isInherit())
      ss << "inherit";
    else
      getDashOffset().getValue().print(ss);

    ss << ";";
  }

  if (getLineCapValid()) {
    if (ss.str() != "") ss << " ";

    ss << "stroke-linecap:";

    printLineCap(ss, getLineCap());

    ss << ";";
  }

  if (getLineJoinValid()) {
    if (ss.str() != "") ss << " ";

    ss << "stroke-linejoin:";

    printLineJoin(ss, getLineJoin());

    ss << ";";
  }

  if (getMiterLimitValid()) {
    if (ss.str() != "") ss << " ";

    ss << "stroke-miterlimit:" << getMiterLimit() << ";";
  }

  os << ss.str();
}

void
CSVGStroke::
printLineCap(std::ostream &ss, const LineCap &lineCap) const
{
  if (lineCap.isInherit())
    ss << "inherit";
  else
    printLineCapType(ss, lineCap.getValue());
}

void
CSVGStroke::
printLineCapType(std::ostream &ss, const CLineCapType &lineCap) const
{
  if      (lineCap == LINE_CAP_TYPE_BUTT  ) ss << "butt";
  else if (lineCap == LINE_CAP_TYPE_ROUND ) ss << "round";
  else if (lineCap == LINE_CAP_TYPE_SQUARE) ss << "square";
  else                                      ss << CMathRound::RoundNearest(lineCap);
}

void
CSVGStroke::
printLineJoin(std::ostream &ss, const LineJoin &lineCap) const
{
  if (lineCap.isInherit())
    ss << "inherit";
  else
    printLineJoinType(ss, lineCap.getValue());
}

void
CSVGStroke::
printLineJoinType(std::ostream &ss, const CLineJoinType &lineJoin) const
{
  if      (lineJoin == LINE_JOIN_TYPE_MITRE) ss << "miter";
  else if (lineJoin == LINE_JOIN_TYPE_ROUND) ss << "round";
  else if (lineJoin == LINE_JOIN_TYPE_BEVEL) ss << "bevel";
  else                                       ss << CMathRound::RoundNearest(lineJoin);
}
