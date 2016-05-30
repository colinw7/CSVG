#include <CSVGStroke.h>
#include <CSVG.h>
#include <CSVGLog.h>
#include <CSVGUtil.h>
#include <CRegExp.h>
#include <CStrParse.h>

CRGBA
CSVGStroke::
getAlphaColor() const
{
  CRGBA rgba;

  if (color_.isValid()) {
    rgba = color_.getValue();

    if (getOpacityValid())
      rgba.setAlpha(getOpacity());
  }
  else
    rgba = CRGBA(0,0,0,0);

  return rgba;
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

    noColor_.setInvalid();

    resetColor();
  //resetUrl       ();
  //resetFillObject();

    std::vector<std::string> match_strs;

    if      (word == "none") {
      noColor_ = true;

      resetOpacity();
    }
    else if (word == "currentColor") {
      currentColor_ = true;
    }
    else if (CRegExpUtil::parse(word, "url(#\\(.*\\))", match_strs)) {
      setUrl(match_strs[0]);
    }
    else {
      CRGBA rgba;

      if (svg_.decodeColorString(word, rgba))
        setColor(rgba);
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

bool
CSVGStroke::
getFillObjectValid() const
{
  return url_.isValid() || fillObject_.isValid();
}

CSVGObject *
CSVGStroke::
getFillObject() const
{
  if      (url_.isValid())
    return svg_.lookupObjectById(url_.getValue());
  else if (fillObject_.isValid())
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
setDash(const std::string &dash_str)
{
  CLineDash dash;

  svg_.decodeDashString(dash_str, dash);

  setDash(dash);
}

void
CSVGStroke::
setDashOffset(const std::string &offset_str)
{
  CScreenUnits lvalue;

  if (! svg_.decodeLengthValue(offset_str, lvalue))
    return;

  setDashOffset(lvalue.px().value());
}

void
CSVGStroke::
setDashOffset(double offset)
{
  if (getDashValid())
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

  setMitreLimit(lvalue.px().value());
}

void
CSVGStroke::
reset()
{
  noColor_     .setInvalid();
  currentColor_.setInvalid();
  color_       .setInvalid();
  opacity_     .setInvalid();
  rule_        .setInvalid();
  url_         .setInvalid();
  fillObject_  .setInvalid();
  width_       .setInvalid();
  dash_        .setInvalid();
  cap_         .setInvalid();
  join_        .setInvalid();
  mlimit_      .setInvalid();
}

void
CSVGStroke::
update(const CSVGStroke &stroke)
{
  // TODO: return priority and use highest priority for fill (only use one fill)
  if (stroke.noColor_.isValid())
    noColor_ = stroke.noColor_;
  else {
    if (svg_.styleObject()) {
      bool        noColor;
      CSVGCSSType noColorType;

      if (svg_.getStyleStrokeNoColor(svg_.styleObject(), noColor, noColorType))
        noColor_ = noColor;
    }
  }

  if (stroke.currentColor_.isValid()) {
    currentColor_ = stroke.currentColor_;

    noColor_.setInvalid();
  }

  if (stroke.getColorValid()) {
    color_ = stroke.color_;

    noColor_.setInvalid();
  }
  else {
    if (svg_.styleObject()) {
      CRGBA       color;
      CSVGCSSType colorType;

      if (svg_.getStyleStrokeColor(svg_.styleObject(), color, colorType)) {
        color_ = color;

        noColor_.setInvalid();
      }
    }
  }

  COptReal ga;

  if (svg_.styleObject()) {
    if (svg_.styleObject()->getOpacityValid())
      ga = svg_.styleObject()->getOpacity();
  }

  if      (stroke.getOpacityValid())
    opacity_ = ga.getValue(1)*stroke.opacity_.getValue();
  else if (svg_.styleObject()) {
    double a;

    if      (svg_.getStyleStrokeOpacity(svg_.styleObject(), a))
      opacity_ = ga.getValue(1)*a;
    else if (ga.isValid())
      opacity_ = ga.getValue();
  }
  else if (ga.isValid())
    opacity_ = ga.getValue();

  if (stroke.getRuleValid())
    rule_ = stroke.rule_;

  if (stroke.getUrlValid()) {
    url_ = stroke.url_;

    noColor_.setInvalid();
  }

  if (stroke.fillObject_.isValid()) {
    fillObject_ = stroke.fillObject_;

    noColor_.setInvalid();
  }

  if (stroke.getWidthValid())
    setWidth(stroke.getWidth());
  else {
    if (svg_.styleObject()) {
      double w;

      if (svg_.getStyleStrokeWidth(svg_.styleObject(), w))
        setWidth(w);
    }
  }

  if (stroke.getDashValid())
    setDash(stroke.getDash());
  else {
    if (svg_.styleObject()) {
      CLineDash dash;

      if (svg_.getStyleStrokeDash(svg_.styleObject(), dash))
        setDash(dash);
    }
  }

  if (stroke.getLineCapValid())
    setLineCap(stroke.getLineCap());

  if (stroke.getLineJoinValid())
    setLineJoin(stroke.getLineJoin());

  if (stroke.getMitreLimitValid())
    setMitreLimit(stroke.getMitreLimit());
}

void
CSVGStroke::
print(std::ostream &os) const
{
  std::stringstream ss;

  if (noColor_.isValid())
    ss << "stroke: none;";

  if (color_.isValid())
    ss << "stroke: " << color_.getValue().getRGB().stringEncode() << ";";

  if (opacity_.isValid()) {
    if (ss.str() != "") ss << " ";

    ss << "stroke-opacity: " << opacity_.getValue() << ";";
  }

  if (url_.isValid())
    ss << "stroke: url(#" << url_.getValue() << ");";

  if (width_.isValid())
    ss << " stroke-width: " << width_.getValue() << ";";

  if (dash_.isValid()) {
    ss << " stroke-dasharray: " << dash_.getValue().toString() << ";"; // TODO: no offset, format

    if (dash_.getValue().getOffset() != 0)
      ss << " stroke-dashoffset: " << dash_.getValue().getOffset() << ";";
  }

  if (cap_.isValid()) {
    ss << " stroke-linecap: ";

    if      (cap_.getValue() == LINE_CAP_TYPE_BUTT  ) ss << "butt";
    else if (cap_.getValue() == LINE_CAP_TYPE_ROUND ) ss << "round";
    else if (cap_.getValue() == LINE_CAP_TYPE_SQUARE) ss << "square";
    else                                              ss << CSVGUtil::round(cap_.getValue());

    ss << ";";
  }

  if (join_.isValid()) {
    ss << " stroke-linejoin: ";

    if      (join_.getValue() == LINE_JOIN_TYPE_MITRE) ss << "miter";
    else if (join_.getValue() == LINE_JOIN_TYPE_ROUND) ss << "round";
    else if (join_.getValue() == LINE_JOIN_TYPE_BEVEL) ss << "bevel";
    else                                               ss << CSVGUtil::round(join_.getValue());

    ss << ";";
  }

  if (mlimit_.isValid())
    ss << " stroke-miterlimit: " << mlimit_.getValue() << ";";

  os << ss.str();
}
