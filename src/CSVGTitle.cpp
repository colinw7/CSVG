#include "CSVGI.h"

CSVGTitle::
CSVGTitle(CSVG &svg) :
 CSVGObject(svg),
 text_     ()
{
}

CSVGTitle::
CSVGTitle(const CSVGTitle &title) :
 CSVGObject(title),
 text_     (title.text_)
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
processOption(const string &opt_name, const string &opt_value)
{
  return CSVGObject::processOption(opt_name, opt_value);
}

void
CSVGTitle::
draw()
{
}
