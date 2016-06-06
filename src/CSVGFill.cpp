#include <CSVGFill.h>
#include <CSVG.h>
#include <CSVGLog.h>
#include <CRegExp.h>
#include <CStrParse.h>

bool
CSVGFill::
isFilled() const
{
  if (getUrlValid())
    return true;

  if (getFillObjectValid())
    return true;

  if (! getColorValid())
    return true;

  if (getColor().type() == CSVGColor::Type::NONE)
    return false;

  return true;
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

    resetColor     ();
  //resetUrl       ();
  //resetFillObject();

    std::vector<std::string> match_strs;

    if (CRegExpUtil::parse(word, "url(#\\(.*\\))", match_strs)) {
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

CSVGObject *
CSVGFill::
calcFillObject() const
{
  if      (getUrlValid())
    return svg_.lookupObjectById(getUrl());
  else if (getFillObjectValid())
    return getFillObject();
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
  // color
  if      (fill.getColorValid())
    setColor(fill.getColor());
  else if (svg_.styleObject()) {
    CSVGColor   color;
    CSVGCSSType colorType;

    if (svg_.getStyleFillColor(svg_.styleObject(), color, colorType))
      setColor(color);
  }

  // opacity
  if      (fill.getOpacityValid())
    setOpacity(fill.getOpacity());
  else if (svg_.styleObject()) {
    double a;

    if (svg_.getStyleFillOpacity(svg_.styleObject(), a))
      setOpacity(a);
  }

  // rule
  if (fill.getRuleValid())
    setRule(fill.getRule());

  // url
  if (fill.getUrlValid())
    setUrl(fill.getUrl());

  // fill object
  if (fill.getFillObjectValid())
    setFillObject(fill.getFillObject());

  //---

  if (getenv("CSVG_DEBUG_FILL")) {
    std::cerr << "Update Fill";
    if (svg_.styleObject() && svg_.styleObject()->getId() != "")
      std::cerr << "(" << svg_.styleObject()->getId() << ")";
    std::cerr << ":";
    print(std::cerr);
    std::cerr << std::endl;
  }
}

void
CSVGFill::
print(std::ostream &os) const
{
  std::stringstream ss;

  if (getColorValid())
    ss << "fill: " << getColor() << ";";

  if (getOpacityValid()) {
    if (ss.str() != "") ss << " ";

    ss << "fill-opacity: " << getOpacity() << ";";
  }

  if (getUrlValid())
    ss << "fill: url(#" << getUrl() << ");";

  os << ss.str();
}
