#include <CSVGStyle.h>
#include <CSVG.h>
#include <CSVGStyleData.h>

#include <CStrParse.h>
#include <CCSS.h>

CSVGStyle::
CSVGStyle(CSVG &svg) :
 CSVGObject(svg), type_(CSVGStyleType::NONE)
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
processOption(const std::string &opt_name, const std::string &opt_value)
{
  std::string str;

  if (svg_.stringOption(opt_name, opt_value, "type", str)) {
    if (str == "text/css")
      type_ = CSVGStyleType::TEXT_CSS;
    else
      std::cerr << "Invalid type " << str << std::endl;

    return true;
  }

  return CSVGObject::processOption(opt_name, opt_value);
}

void
CSVGStyle::
setText(const std::string &text)
{
  CStrParse parse(text);

  parse.skipSpace();

  if (parse.isString("<![CDATA[")) {
    parse.skipChars(9);

    parse.skipSpace();

    std::string str;
    bool        space = false;

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

    if (! svg_.readCSSString(str))
      std::cerr << "Invalid Style: " << str << std::endl;
  }
  else {
    if (! svg_.readCSSString(text))
      std::cerr << "Invalid Style Text: " << text << std::endl;

    //std::cerr << "Invalid Text: " << text << std::endl;
  }

  text_ = text;
}

bool
CSVGStyle::
draw()
{
  return false;
}

void
CSVGStyle::
print(std::ostream &os, bool hier) const
{
  if (hier) {
    os << "<style";

    printValues(os);

    os << ">" << std::endl;

    os << getText();

    printChildren(os, hier);

    os << "</style>" << std::endl;
  }
  else
    os << "style";
}

void
CSVGStyle::
printValues(std::ostream &os, bool flat) const
{
  CSVGObject::printValues(os, flat);

  if (type_ == CSVGStyleType::TEXT_CSS)
    os << " type=\"text/css\"";
}

std::ostream &
operator<<(std::ostream &os, const CSVGStyle &style)
{
  style.print(os, false);

  return os;
}
