#include "CSVGI.h"
#include <CStrParse.h>
#include <CCSS.h>

CSVGStyle::
CSVGStyle(CSVG &svg) :
 CSVGObject(svg), type_(STYLE_NONE)
{
}

CSVGStyle::
CSVGStyle(const CSVGStyle &style) :
 CSVGObject(style), type_(style.type_)
{
}

CSVGStyle *
CSVGStyle::
dup() const
{
  return new CSVGStyle(*this);
}

/* Attributes:
    type <data>
*/
bool
CSVGStyle::
processOption(const string &opt_name, const string &opt_value)
{
  string str;

  if (svg_.stringOption(opt_name, opt_value, "type", str)) {
    if (str == "text/css")
      type_ = STYLE_TEXT_CSS;
    else
      std::cerr << "Invalid type " << str << std::endl;

    return true;
  }

  return CSVGObject::processOption(opt_name, opt_value);
}

void
CSVGStyle::
setText(const string &text)
{
  CStrParse parse(text);

  parse.skipSpace();

  if (parse.isString("<![CDATA[")) {
    parse.skipChars(9);

    parse.skipSpace();

    string str;
    bool   space = false;

    while (! parse.eof() && ! parse.isString("]]>")) {
      char c;

      parse.readChar(&c);

      if (! isspace(c)) {
        str += c;

        space = false;
      }
      else {
        if (! space) {
          str += " ";

          space = true;
        }
      }
    }

    if (! parseCSS(str))
      std::cerr << "Invalid Style: " << str << std::endl;
  }
  else
    std::cerr << "Invalid Text: " << text << std::endl;
}

bool
CSVGStyle::
parseCSS(const string &str)
{
  CCSS css;

  if (! css.processLine(str))
    return false;

  vector<string> ids;

  css.getIds(ids);

  uint num_ids = ids.size();

  for (uint i = 0; i < num_ids; ++i) {
    const CCSS::StyleData &styleData = css.getStyleData(ids[i]);

    CSVGStyleData &styleData1 = svg_.getStyleData(ids[i]);

    uint num_options = styleData.getNumOptions();

    for (uint j = 0; j < num_options; ++j) {
      const CCSS::Option &option = styleData.getOption(j);

      styleData1.setValue(option.getName(), option.getValue());
    }
  }

  return true;
}

void
CSVGStyle::
draw()
{
}

void
CSVGStyle::
print(ostream &os) const
{
  os << "style";
}

ostream &
operator<<(ostream &os, const CSVGStyle &style)
{
  style.print(os);

  return os;
}
