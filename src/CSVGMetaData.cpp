#include <CSVGMetaData.h>
#include <CSVG.h>

CSVGMetaData::
CSVGMetaData(CSVG &svg) :
 CSVGObject(svg)
{
}

bool
CSVGMetaData::
processOption(const std::string &, const std::string &)
{
  return true;
}

void
CSVGMetaData::
print(std::ostream &, bool) const
{
}
