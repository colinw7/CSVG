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
  double opacity;

  if (! svg_.decodeOpacityString(opacity_def, opacity)) {
    CSVGLog() << "Bad opacity value " << opacity_def;
    return;
  }

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
    return nullptr;
}

void
CSVGStroke::
setWidth(const std::string &width_def)
{
  double width;

  if (! svg_.decodeWidthString(width_def, width)) {
    CSVGLog() << "Illegal width value " << width_def;
    return;
  }

  setWidth(width);
}

void
CSVGStroke::
setDashArray(const std::string &dash_str)
{
  CSVGStrokeDash::Dashes dashes;
  bool                   solid;

  if (! svg_.decodeDashString(dash_str, dashes, solid)) {
    CSVGLog() << "Bad dash string " << dash_str;
    return;
  }

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
  COptValT<CScreenUnits> lvalue = svg_.decodeLengthValue(offset_str);

  if (! lvalue.isValid())
    return;

  setDashOffset(lvalue.getValue());
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
  COptValT<CScreenUnits> lvalue = svg_.decodeLengthValue(limit_str);

  if (! lvalue.isValid())
    return;

  setMitreLimit(lvalue.getValue().pxValue(1));
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
    if (svg_.styleObject()->getStrokeColorValid())
      setColor(svg_.styleObject()->getFlatStrokeColor().getValue());
  }

  // opacity
  if      (stroke.getOpacityValid())
    setOpacity(stroke.getOpacity());
  else if (svg_.styleObject()) {
    if (svg_.styleObject()->getStrokeOpacityValid())
      opacity_ = svg_.styleObject()->getFlatStrokeOpacity().getValue();
  }

  // rule
  if      (stroke.getRuleValid())
    setRule(stroke.getRule());
  else if (svg_.styleObject()) {
    if (svg_.styleObject()->getStrokeRuleValid())
      rule_ = svg_.styleObject()->getFlatStrokeRule().getValue();
  }

  // url
  if      (stroke.getUrlValid())
    setUrl(stroke.getUrl());
  else if (svg_.styleObject()) {
    if (svg_.styleObject()->getStrokeUrlValid())
      url_ = svg_.styleObject()->getFlatStrokeUrl().getValue();
  }

  // fill object
  if      (stroke.getFillObjectValid())
    setFillObject(stroke.getFillObject());
  else if (svg_.styleObject()) {
    if (svg_.styleObject()->getStrokeFillObjectValid())
      fillObject_ = svg_.styleObject()->getFlatStrokeFillObject().getValue();
  }

  // width
  if      (stroke.getWidthValid())
    setWidth(stroke.getWidth());
  else if (svg_.styleObject()) {
    if (svg_.styleObject()->getStrokeWidthValid())
      setWidth(svg_.styleObject()->getFlatStrokeWidth().getValue());
  }

  // dash
  if      (stroke.getDashValid())
    setDash(stroke.getDash());
  else if (svg_.styleObject()) {
    if (svg_.styleObject()->getStrokeDashValid())
      setDash(svg_.styleObject()->getFlatStrokeLineDash().getValue());
  }

  // line cap
  if      (stroke.getLineCapValid())
    setLineCap(stroke.getLineCap());
  else if (svg_.styleObject()) {
    if (svg_.styleObject()->getStrokeLineCapValid())
      setLineCap(svg_.styleObject()->getFlatStrokeLineCap().getValue());
  }

  // line join
  if      (stroke.getLineJoinValid())
    setLineJoin(stroke.getLineJoin());
  else if (svg_.styleObject()) {
    if (svg_.styleObject()->getStrokeLineJoinValid())
      setLineJoin(svg_.styleObject()->getFlatStrokeLineJoin().getValue());
  }

  // mitre limit
  if      (stroke.getMitreLimitValid())
    setMitreLimit(stroke.getMitreLimit());
  else if (svg_.styleObject()) {
    if (svg_.styleObject()->getStrokeMitreLimitValid())
      setMitreLimit(svg_.styleObject()->getFlatStrokeMitreLimit().getValue());
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
  bool              output = false;

  if (getColorValid()) {
    ss << "stroke: " << getColor() << ";";

    output = true;
  }

  if (getOpacityValid()) {
    if (output) ss << " ";

    ss << "stroke-opacity: " << getOpacity() << ";";

    output = true;
  }

  if (getUrlValid()) {
    if (output) ss << " ";

    ss << "stroke: url(#" << getUrl() << ");";

    output = true;
  }

  if (getWidthValid()) {
    if (output) ss << " ";

    ss << "stroke-width: " << getWidth() << ";";

    output = true;
  }

  if (getDashValid()) {
    if (output) ss << " ";

    ss << "stroke-dasharray: ";

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

    output = true;
  }

  if (getLineCapValid()) {
    if (output) ss << " ";

    ss << "stroke-linecap: ";

    if      (getLineCap() == LINE_CAP_TYPE_BUTT  ) ss << "butt";
    else if (getLineCap() == LINE_CAP_TYPE_ROUND ) ss << "round";
    else if (getLineCap() == LINE_CAP_TYPE_SQUARE) ss << "square";
    else                                           ss << CSVGUtil::round(getLineCap());

    ss << ";";

    output = true;
  }

  if (getLineJoinValid()) {
    if (output) ss << " ";

    ss << "stroke-linejoin: ";

    if      (getLineJoin() == LINE_JOIN_TYPE_MITRE) ss << "miter";
    else if (getLineJoin() == LINE_JOIN_TYPE_ROUND) ss << "round";
    else if (getLineJoin() == LINE_JOIN_TYPE_BEVEL) ss << "bevel";
    else                                            ss << CSVGUtil::round(getLineJoin());

    ss << ";";

    output = true;
  }

  if (getMitreLimitValid()) {
    if (output) ss << " ";

    ss << "stroke-miterlimit: " << getMitreLimit() << ";";

    output = true;
  }

  os << ss.str();
}
