#include <CSVGStroke.h>
#include <CSVG.h>
#include <CSVGLog.h>
#include <CSVGUtil.h>
#include <CRegExp.h>
#include <CStrParse.h>

bool
CSVGStroke::
isStroked() const
{
  if (getUrlValid())
    return true;

  if (getFillObjectValid())
    return true;

  if (! getColorValid())
    return false;

  if (getColor().type() == CSVGColor::Type::NONE)
    return false;

  return true;
}

void
CSVGStroke::
setColor(const std::string &color_def)
{
  CStrParse parse(color_def);

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

    std::vector<std::string> match_strs;

    if      (CRegExpUtil::parse(word, "url(#\\(.*\\))", match_strs)) {
      setUrl(match_strs[0]);
    }
    else {
      CSVGColor color;

      if (svg_.decodeColorString(word, color))
        setColor(color);
    }

    //------

    parse.skipSpace();
  }
}

void
CSVGStroke::
setOpacity(const std::string &opacity_def)
{
  double opacity = svg_.decodeOpacityString(opacity_def);

  setOpacity(opacity);
}

void
CSVGStroke::
setRule(const std::string &rule_def)
{
  CFillType rule = svg_.decodeFillRuleString(rule_def);

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
    return 0;
}

void
CSVGStroke::
setWidth(const std::string &width_def)
{
  double width = svg_.decodeWidthString(width_def);

  setWidth(width);
}

void
CSVGStroke::
setDashArray(const std::string &dash_str)
{
  CSVGStrokeDash::Dashes dashes;
  bool                   solid;

  svg_.decodeDashString(dash_str, dashes, solid);

  if (! getDashValid())
    dash_ = CSVGStrokeDash();

  dash_.getValue().setDashes(dashes);
  dash_.getValue().setSolid (solid);
}

void
CSVGStroke::
setDashArray(const std::vector<CScreenUnits> &array)
{
  if (! getDashValid())
    dash_ = CSVGStrokeDash();

  dash_.getValue().setDashes(array);
}

void
CSVGStroke::
setDashOffset(const std::string &offset_str)
{
  CScreenUnits lvalue;

  if (! svg_.decodeLengthValue(offset_str, lvalue))
    return;

  setDashOffset(lvalue);
}

void
CSVGStroke::
setDashOffset(const CScreenUnits &offset)
{
  if (! getDashValid())
    dash_ = CSVGStrokeDash();

  dash_.getValue().setOffset(offset);
}

void
CSVGStroke::
setLineCap(const std::string &cap_str)
{
  CLineCapType cap = LINE_CAP_TYPE_NONE;

  if      (cap_str == "butt")
    cap = LINE_CAP_TYPE_BUTT;
  else if (cap_str == "round")
    cap = LINE_CAP_TYPE_ROUND;
  else if (cap_str == "square")
    cap = LINE_CAP_TYPE_SQUARE;
  else {
    CSVGLog() << "Illegal line_cap " << cap_str;
    return;
  }

  setLineCap(cap);
}

void
CSVGStroke::
setLineJoin(const std::string &join_str)
{
  CLineJoinType join = LINE_JOIN_TYPE_NONE;

  if      (join_str == "mitre" || join_str == "miter")
    join = LINE_JOIN_TYPE_MITRE;
  else if (join_str == "round")
    join = LINE_JOIN_TYPE_ROUND;
  else if (join_str == "bevel")
    join = LINE_JOIN_TYPE_BEVEL;
  else {
    CSVGLog() << "Illegal line_join " << join_str;
    return;
  }

  setLineJoin(join);
}

void
CSVGStroke::
setMitreLimit(const std::string &limit_str)
{
  CScreenUnits lvalue;

  if (! svg_.decodeLengthValue(limit_str, lvalue))
    return;

  setMitreLimit(lvalue.pxValue(0));
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
  // color
  if      (stroke.getColorValid())
    setColor(stroke.getColor());
  else if (svg_.styleObject()) {
    CSVGColor   color;
    CSVGCSSType colorType;

    if (svg_.getStyleStrokeColor(svg_.styleObject(), color, colorType))
      setColor(color);
  }

  // opacity
  if      (stroke.getOpacityValid())
    setOpacity(stroke.getOpacity());
  else if (svg_.styleObject()) {
    double      opacity;
    CSVGCSSType opacityType;

    if (svg_.getStyleStrokeOpacity(svg_.styleObject(), opacity, opacityType))
      opacity_ = opacity;
  }

  // rule
  if      (stroke.getRuleValid())
    setRule(stroke.getRule());
  else if (svg_.styleObject()) {
    CFillType   rule;
    CSVGCSSType ruleType;

    if (svg_.getStyleStrokeRule(svg_.styleObject(), rule, ruleType))
      rule_ = rule;
  }

  // url
  if      (stroke.getUrlValid())
    setUrl(stroke.getUrl());
  else if (svg_.styleObject()) {
    std::string url;
    CSVGCSSType urlType;

    if (svg_.getStyleStrokeUrl(svg_.styleObject(), url, urlType))
      url_ = url;
  }

  // fill object
  if      (stroke.getFillObjectValid())
    setFillObject(stroke.getFillObject());
  else if (svg_.styleObject()) {
    CSVGObject* fillObject;
    CSVGCSSType fillObjectType;

    if (svg_.getStyleStrokeFillObject(svg_.styleObject(), fillObject, fillObjectType))
      fillObject_ = fillObject;
  }

  // width
  if      (stroke.getWidthValid())
    setWidth(stroke.getWidth());
  else if (svg_.styleObject()) {
    double      width;
    CSVGCSSType widthType;

    if (svg_.getStyleStrokeWidth(svg_.styleObject(), width, widthType))
      setWidth(width);
  }

  // dash
  if      (stroke.getDashValid())
    setDash(stroke.getDash());
  else if (svg_.styleObject()) {
    CSVGStrokeDash dash;
    CSVGCSSType    dashType;

    if (svg_.getStyleStrokeDash(svg_.styleObject(), dash, dashType))
      setDash(dash);
  }

  // line cap
  if      (stroke.getLineCapValid())
    setLineCap(stroke.getLineCap());
  else if (svg_.styleObject()) {
    CLineCapType lineCap;
    CSVGCSSType  lineCapType;

    if (svg_.getStyleStrokeCap(svg_.styleObject(), lineCap, lineCapType))
      setLineCap(lineCap);
  }

  // line join
  if      (stroke.getLineJoinValid())
    setLineJoin(stroke.getLineJoin());
  else if (svg_.styleObject()) {
    CLineJoinType lineJoin;
    CSVGCSSType   lineJoinType;

    if (svg_.getStyleStrokeJoin(svg_.styleObject(), lineJoin, lineJoinType))
      setLineJoin(lineJoin);
  }

  // mitre limit
  if      (stroke.getMitreLimitValid())
    setMitreLimit(stroke.getMitreLimit());
  else if (svg_.styleObject()) {
    double      mitreLimit;
    CSVGCSSType mitreLimitType;

    if (svg_.getStyleStrokeMitreLimit(svg_.styleObject(), mitreLimit, mitreLimitType))
      setMitreLimit(mitreLimit);
  }

  //---

  if (getenv("CSVG_DEBUG_STROKE")) {
    std::cerr << "Update Stroke";
    if (svg_.styleObject() && svg_.styleObject()->getId() != "")
      std::cerr << "(" << svg_.styleObject()->getId() << ")";
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

  if (getUrlValid())
    ss << "stroke: url(#" << getUrl() << ");";

  if (getWidthValid())
    ss << " stroke-width: " << getWidth() << ";";

  if (getDashValid()) {
    ss << " stroke-dasharray: ";

    if      (getDash().isSolid())
      ss << "solid";
    else if (getDash().isNone())
      ss << "none";
    else
      getDash().printDashes(ss);

    ss << ";";

    if (getDash().hasOffset()) {
      ss << " stroke-dashoffset: " << getDash().offset() << ";";
    }
  }

  if (getLineCapValid()) {
    ss << " stroke-linecap: ";

    if      (getLineCap() == LINE_CAP_TYPE_BUTT  ) ss << "butt";
    else if (getLineCap() == LINE_CAP_TYPE_ROUND ) ss << "round";
    else if (getLineCap() == LINE_CAP_TYPE_SQUARE) ss << "square";
    else                                           ss << CSVGUtil::round(getLineCap());

    ss << ";";
  }

  if (getLineJoinValid()) {
    ss << " stroke-linejoin: ";

    if      (getLineJoin() == LINE_JOIN_TYPE_MITRE) ss << "miter";
    else if (getLineJoin() == LINE_JOIN_TYPE_ROUND) ss << "round";
    else if (getLineJoin() == LINE_JOIN_TYPE_BEVEL) ss << "bevel";
    else                                            ss << CSVGUtil::round(getLineJoin());

    ss << ";";
  }

  if (getMitreLimitValid())
    ss << " stroke-miterlimit: " << getMitreLimit() << ";";

  os << ss.str();
}
