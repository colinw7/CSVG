#include <CSVGI.h>
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
setColor(const string &color_def)
{
  CStrParse parse(color_def);

  parse.skipSpace();

  while (true) {
    string word;

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

    vector<string> match_strs;

    if      (word == "none") {
      setColor(CRGBA(0,0,0,0));

      setOpacity(0.0);
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

        if (! getOpacityValid())
          setOpacity(1.0);
      }
    }

    //------

    parse.skipSpace();
  }
}

double
CSVGFill::
getOpacity() const
{
  if (getOpacityValid())
    return opacity_.getValue();
  else
    return 1.0;
}

void
CSVGFill::
setOpacity(const string &opacity_def)
{
  double opacity = svg_.decodeOpacityString(opacity_def);

  setOpacity(opacity);
}

void
CSVGFill::
setRule(const string &rule_def)
{
  CFillType rule = svg_.decodeFillRuleString(rule_def);

  setRule(rule);
}

CFillType
CSVGFill::
getRule() const
{
  if (getRuleValid())
    return rule_.getValue();
  else
    return FILL_TYPE_EVEN_ODD;
}

bool
CSVGFill::
getFillObjectValid() const
{
  return url_.isValid() || fill_object_.isValid();
}

CSVGObject *
CSVGFill::
getFillObject() const
{
  if      (url_.isValid())
    return svg_.lookupObjectById(url_.getValue());
  else if (fill_object_.isValid())
    return fill_object_.getValue();
  else
    return NULL;
}

void
CSVGFill::
reset()
{
  color_      .setInvalid();
  opacity_    .setInvalid();
  rule_       .setInvalid();
  url_        .setInvalid();
  fill_object_.setInvalid();
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

  if (fill.fill_object_.isValid())
    setFillObject(fill.fill_object_.getValue());
}
