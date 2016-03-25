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

    std::vector<std::string> match_strs;

    if      (word == "none") {
      noColor_ = true;

      resetColor();

      resetOpacity();
    }
    else if (word == "currentColor") {
      CSVGLog() << "currentColor not supported";
    }
    else if (CRegExpUtil::parse(word, "url(#\\(.*\\))", match_strs))
      setUrl(match_strs[0]);
    else {
      CRGBA rgba;

      if (svg_.decodeColorString(word, rgba)) {
        setColor(rgba);

        resetOpacity();
      }
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
  color_     .setInvalid();
  opacity_   .setInvalid();
  rule_      .setInvalid();
  url_       .setInvalid();
  fillObject_.setInvalid();
}

void
CSVGFill::
update(const CSVGFill &fill)
{
  if (fill.getColorValid())
    setColor(fill.getColor());

  if (fill.getOpacityValid())
    setOpacity(fill.getOpacity());

  if (fill.getRuleValid())
    setRule(fill.getRule());

  if (fill.url_.isValid())
    setUrl(fill.url_.getValue());

  if (fill.fillObject_.isValid())
    setFillObject(fill.fillObject_.getValue());
}

void
CSVGFill::
print(std::ostream &os) const
{
  if (noColor_)
    os << "fill: none;";

  if (color_.isValid())
    os << "fill: " << color_.getValue().getRGB().stringEncode() << ";";

  if (opacity_.isValid())
    os << " fill-opacity: " << opacity_.getValue() << ";";

  if (url_.isValid())
    os << "fill: url(#" << url_.getValue() << ");";
}
