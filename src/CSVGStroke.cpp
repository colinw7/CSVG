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
        setColor(! inherit ? Color(color) : Color::inherit());
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

  setOpacity(! inherit ? Opacity(opacity) : Opacity::inherit());
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

  setWidth( inherit ? Width(width) : Width::inherit());
}

// none | <list-of-lengths> | inherit
void
CSVGStroke::
setDashArray(const std::string &dashStr)
{
  if (dashStr == "inherit") {
    dash_ = LineDash::inherit();
  }
  else {
    CSVGStrokeDash::Dashes dashes;
    bool                   solid;

    if (! svg_.decodeDashString(dashStr, dashes, solid)) {
      CSVGLog() << "Bad dash string " << dashStr;
      return;
    }

    CSVGStrokeDash dash;

    if (getDashValid())
      dash = getDash().getValue();

    dash.setDashes(dashes);
    dash.setSolid (solid);

    dash_ = LineDash(dash);
  }
}

void
CSVGStroke::
setDashArray(const std::vector<CScreenUnits> &array)
{
  CSVGStrokeDash dash;

  if (getDashValid())
    dash = getDash().getValue();

  dash.setDashes(array);

  dash_ = LineDash(dash);
}

// <length> | inherit
void
CSVGStroke::
setDashOffset(const std::string &offsetStr)
{
  if (offsetStr == "inherit") {
    // TODO
  }
  else {
    COptValT<CScreenUnits> lvalue = svg_.decodeLengthValue(offsetStr);

    if (! lvalue.isValid())
      return;

    setDashOffset(lvalue.getValue());
  }
}

void
CSVGStroke::
setDashOffset(const CScreenUnits &offset)
{
  CSVGStrokeDash dash;

  if (getDashValid())
    dash = getDash().getValue();

  dash.setOffset(offset);

  dash_ = LineDash(dash);
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

  setLineCap(! inherit ? LineCap(cap) : LineCap::inherit());
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

  setLineJoin(! inherit ? LineJoin(join) : LineJoin::inherit());
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

  setMiterLimit(! inherit ? MiterLimit(miterlimit) : MiterLimit::inherit());
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
  dash_      .setInvalid();
  cap_       .setInvalid();
  join_      .setInvalid();
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

  // dash
  if      (stroke.getDashValid())
    setDash(stroke.getDash());
  else if (styleObject) {
    if (styleObject->getStrokeDashValid())
      setDash(styleObject->getFlatStrokeLineDash().getValue());
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
    ss << "stroke: " << getColor() << ";";

  if (getOpacityValid()) {
    if (ss.str() != "") ss << " ";

    ss << "stroke-opacity: " << getOpacity() << ";";
  }

  if (getUrlValid()) {
    if (ss.str() != "") ss << " ";

    ss << "stroke: url(#" << getUrl() << ");";
  }

  if (getWidthValid()) {
    if (ss.str() != "") ss << " ";

    ss << "stroke-width: " << getWidth() << ";";
  }

  if (getDashValid()) {
    if (ss.str() != "") ss << " ";

    ss << "stroke-dasharray: ";

    if (getDash().isInherit())
      ss << "inherit";
    else
      getDash().getValue().printType(ss);

    ss << ";";

    if (! getDash().isInherit()) {
      if (getDash().getValue().hasOffset()) {
        ss << " stroke-dashoffset: ";

        getDash().getValue().printOffset(ss);

        ss << ";";
      }
    }
  }

  if (getLineCapValid()) {
    if (ss.str() != "") ss << " ";

    ss << "stroke-linecap: ";

    printLineCap(ss, getLineCap());

    ss << ";";
  }

  if (getLineJoinValid()) {
    if (ss.str() != "") ss << " ";

    ss << "stroke-linejoin: ";

    printLineJoin(ss, getLineJoin());

    ss << ";";
  }

  if (getMiterLimitValid()) {
    if (ss.str() != "") ss << " ";

    ss << "stroke-miterlimit: " << getMiterLimit() << ";";
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
