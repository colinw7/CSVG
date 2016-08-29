#include <CSVGScript.h>
#include <CSVG.h>
#include <CSVGLog.h>
#include <CStrParse.h>

CSVGScript::
CSVGScript(CSVG &svg) :
 CSVGObject(svg),
 xlink_    (this)
{
}

CSVGScript::
CSVGScript(const CSVGScript &use) :
 CSVGObject(use),
 type_     (use.type_),
 xlink_    (use.xlink_)
{
}

CSVGScript::
~CSVGScript()
{
}

CSVGScript *
CSVGScript::
dup() const
{
  return new CSVGScript(*this);
}

/* Attributes:
    <Core>
    <Conditional>
    <Style>
    <Presentation>
    <GraphicalEvents>
    <XLinkEmbed>
    <External>
*/

bool
CSVGScript::
processOption(const std::string &opt_name, const std::string &opt_value)
{
  std::string str;

  if       (svg_.stringOption(opt_name, opt_value, "type", str)) {
    type_ = str;

    if (type_ == "text/ecmascript") {
      std::string text = getText();

      svg_.setScript(text);

      setText("");
    }
  }
  else if (svg_.stringOption(opt_name, opt_value, "xlink:href", str)) {
    xlink_ = CSVGXLink(this, str);

    if (xlink_.isValid())
      (void) xlink_.getValue().isObject(); // resolve
  }
  else
    return CSVGObject::processOption(opt_name, opt_value);

  return true;
}

void
CSVGScript::
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

      if (! isspace(c) || c == '\n') {
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

    svg_.setScript(str);
  }
  else {
    CSVGObject::setText(text);
  }
}

bool
CSVGScript::
draw()
{
  if (svg_.getDebug())
    CSVGLog() << *this;

  return false;
}

void
CSVGScript::
print(std::ostream &os, bool hier) const
{
  if (hier) {
    os << "<script";

    printValues(os);

    if (hasText() || hasChildren()) {
      os << ">" << std::endl;

      if (hasText())
        os << getText() << std::endl;

      printChildren(os, hier);

      os << "</script>" << std::endl;
    }
    else
      os << "/>" << std::endl;
  }
  else
    os << "script";
}

void
CSVGScript::
printValues(std::ostream &os, bool flat) const
{
  CSVGObject::printValues(os, flat);

  printNameValue(os, "type", type_);
}

std::ostream &
operator<<(std::ostream &os, const CSVGScript &use)
{
  use.print(os, false);

  return os;
}
