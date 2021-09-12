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

  if (! getColorValid() || getColor().isInherit())
    return true;

  if (getColor().getValue().isNone())
    return false;

  return true;
}

// <paint> | inherit
void
CSVGFill::
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

    std::vector<std::string> match_strs;

    if (CRegExpUtil::parse(word, "url(#\\(.*\\))", match_strs)) {
      setUrl(match_strs[0]);
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
CSVGFill::
setOpacity(const std::string &opacityDef)
{
  double value;
  bool   inherit;

  if (! svg_.decodeOpacityString(opacityDef, value, inherit)) {
    CSVGLog() << "Bad opacity value " << opacityDef;
    return;
  }

  auto opacity = (! inherit ? Opacity(value) : Opacity::makeInherit());

  setOpacity(opacity);
}

// <fill-rule> | inherit
void
CSVGFill::
setRule(const std::string &ruleDef)
{
  auto rule = svg_.decodeFillRuleString(ruleDef);

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
    return nullptr;
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
  auto *styleObject = svg_.styleObject();

  // color
  if      (fill.getColorValid() && ! fill.getColor().isInherit())
    setColor(fill.getColor());
  else if (styleObject) {
    if (styleObject->getFillColorValid() && ! styleObject->getFillColor().isInherit())
      setColor(styleObject->getFlatFillColor().getValue());
  }

  // opacity
  if      (fill.getOpacityValid())
    setOpacity(fill.getOpacity());
  else if (styleObject) {
    if (styleObject->getFillOpacityValid() && ! styleObject->getFillOpacity().isInherit())
      setOpacity(styleObject->getFlatFillOpacity().getValue());
  }

  // rule
  if      (fill.getRuleValid())
    setRule(fill.getRule());
  else if (styleObject) {
    if (styleObject->getFillRuleValid())
      setRule(styleObject->getFlatFillRule().getValue());
  }

  // url
  if      (fill.getUrlValid())
    setUrl(fill.getUrl());
  else if (styleObject) {
    if (styleObject->getFillUrlValid())
      setUrl(styleObject->getFlatFillUrl().getValue());
  }

  // fill object
  if      (fill.getFillObjectValid())
    setFillObject(fill.getFillObject());
  else if (styleObject) {
    if (styleObject->getFillFillObjectValid())
      setFillObject(styleObject->getFlatFillFillObject().getValue());
  }

  //---

  if (getenv("CSVG_DEBUG_FILL")) {
    std::cerr << "Update Fill";
    if (styleObject && styleObject->getId() != "")
      std::cerr << "(" << styleObject->getId() << ")";
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
    ss << "fill:" << getColor() << ";";

  if (getOpacityValid()) {
    if (ss.str() != "") ss << " ";

    ss << "fill-opacity:" << getOpacity() << ";";
  }

  if (getRuleValid()) {
    if (ss.str() != "") ss << " ";

    ss << "fill-rule:" << svg_.encodeFillRuleString(getRule()) << ";";
  }

  if (getUrlValid()) {
    if (ss.str() != "") ss << " ";

    ss << "fill:url(#" << getUrl() << ");";
  }

  os << ss.str();
}
