#include <CSVGFill.h>
#include <CSVG.h>
#include <CSVGLog.h>
#include <CRegExp.h>
#include <CStrParse.h>

CRGBA
CSVGFill::
getAlphaColor() const
{
  CRGBA rgba;

  if (color_.isValid()) {
    rgba = color_.getValue();

    if (getOpacityValid())
      rgba.setAlpha(getOpacity());
  }
  else
    rgba = CRGBA(0,0,0);

  return rgba;
}

void
CSVGFill::
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

    noColor_     .setInvalid();
    currentColor_.setInvalid();

    resetColor     ();
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
CSVGFill::
setOpacity(const std::string &opacity_def)
{
  double opacity = svg_.decodeOpacityString(opacity_def);

  setOpacity(opacity);
}

void
CSVGFill::
setRule(const std::string &rule_def)
{
  CFillType rule = svg_.decodeFillRuleString(rule_def);

  setRule(rule);
}

bool
CSVGFill::
getFillObjectValid() const
{
  return url_.isValid() || fillObject_.isValid();
}

CSVGObject *
CSVGFill::
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
CSVGFill::
reset()
{
  noColor_     .setInvalid();
  currentColor_.setInvalid();
  color_       .setInvalid();
  opacity_     .setInvalid();
  rule_        .setInvalid();
  url_         .setInvalid();
  fillObject_  .setInvalid();
}

void
CSVGFill::
update(const CSVGFill &fill)
{
  // TODO: return priority and use highest priority for fill (only use one fill)
  if (fill.noColor_.isValid())
    noColor_ = fill.noColor_;
  else {
    if (svg_.styleObject()) {
      bool b;

      if (svg_.getStyleFillNoColor(svg_.styleObject(), b))
        noColor_ = b;
    }
  }

  if (fill.currentColor_.isValid()) {
    currentColor_ = fill.currentColor_;

    noColor_.setInvalid();
  }

  if (fill.color_.isValid()) {
    color_ = fill.color_;

    noColor_.setInvalid();
  }
  else {
    if (svg_.styleObject()) {
      CRGBA c;

      if (svg_.getStyleFillColor(svg_.styleObject(), c)) {
        color_ = c;

        noColor_.setInvalid();
      }
    }
  }

  if (fill.opacity_.isValid())
    opacity_ = fill.opacity_;

  if (fill.rule_.isValid())
    rule_ = fill.rule_;

  if (fill.url_.isValid()) {
    url_ = fill.url_;

    noColor_.setInvalid();
  }

  if (fill.fillObject_.isValid()) {
    fillObject_ = fill.fillObject_;

    noColor_.setInvalid();
  }
}

void
CSVGFill::
print(std::ostream &os) const
{
  std::stringstream ss;

  if (noColor_.isValid())
    ss << "fill: none;";

  if (color_.isValid())
    ss << "fill: " << color_.getValue().getRGB().stringEncode() << ";";

  if (opacity_.isValid()) {
    if (ss.str() != "") ss << " ";

    ss << "fill-opacity: " << opacity_.getValue() << ";";
  }

  if (url_.isValid())
    ss << "fill: url(#" << url_.getValue() << ");";

  os << ss.str();
}
