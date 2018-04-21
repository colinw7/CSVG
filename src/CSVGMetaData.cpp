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

bool
CSVGMetaData::
draw()
{
  return false;
}

void
CSVGMetaData::
print(std::ostream &, bool) const
{
}

std::ostream &
operator<<(std::ostream &os, const CSVGMetaData &data)
{
  data.print(os, false);

  return os;
}
