#include <CSVGTextPath.h>
#include <CSVG.h>
#include <CSVGLog.h>

CSVGTextPath::
CSVGTextPath(CSVG &svg) :
 CSVGObject(svg),
 xlink_    (this)
{
}

CSVGTextPath::
CSVGTextPath(const CSVGTextPath &text) :
 CSVGObject  (text),
 startOffset_(text.startOffset_),
 xlink_      (text.xlink_)
{
}

CSVGTextPath *
CSVGTextPath::
dup() const
{
  return new CSVGTextPath(*this);
}

void
CSVGTextPath::
setLinkName(const std::string &str)
{
  xlink_ = CSVGXLink(this, str);
}

bool
CSVGTextPath::
processOption(const std::string &opt_name, const std::string &opt_value)
{
  CSVGLengthValue length;
  std::string     str;

  if      (svg_.percentOption(opt_name, opt_value, "startOffset", length))
    startOffset_ = length;
  else if (svg_.stringOption(opt_name, opt_value, "xlink:href", str))
    xlink_ = CSVGXLink(this, str);
  else
    return CSVGObject::processOption(opt_name, opt_value);

  return true;
}

void
CSVGTextPath::
draw()
{
  if (svg_.getDebug())
    CSVGLog() << *this;
}

void
CSVGTextPath::
print(std::ostream &os, bool hier) const
{
  if (hier) {
    os << "<textPath";

    printNameLength(os, "startOffset", startOffset_);
    printNameXLink (os, "xlink:href" , xlink_);

    CSVGObject::printValues(os);

    os << ">";

    if (hasChildren()) {
      os << std::endl;

      if (hasText())
        os << getText() << std::endl;

      printChildren(os, hier);
    }
    else {
      os << getText();
    }

    os << "</textPath>" << std::endl;
  }
  else
    os << "textPath ";
}

std::ostream &
operator<<(std::ostream &os, const CSVGTextPath &text)
{
  text.print(os, false);

  return os;
}
