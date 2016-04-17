#include <CSVGTitle.h>
#include <CSVG.h>

CSVGTitle::
CSVGTitle(CSVG &svg) :
 CSVGObject(svg)
{
}

CSVGTitle::
CSVGTitle(const CSVGTitle &title) :
 CSVGObject(title)
{
}

CSVGTitle *
CSVGTitle::
dup() const
{
  return new CSVGTitle(*this);
}

/* Attributes:
    <Core>
    <Style>
*/

bool
CSVGTitle::
processOption(const std::string &opt_name, const std::string &opt_value)
{
  return CSVGObject::processOption(opt_name, opt_value);
}
