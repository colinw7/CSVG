#include <CCSS.h>
#include <CFile.h>
#include <CStrUtil.h>
#include <CStrParse.h>
#include <cassert>

CCSS::
CCSS() :
 styleData_()
{
}

bool
CCSS::
processFile(const std::string &filename)
{
  if (! CFile::exists(filename) || ! CFile::isRegular(filename))
    return false;

  CFile file(filename);

  std::string line;

  while (file.readLine(line)) {
    line = CStrUtil::stripSpaces(line);

    if (! line.empty())
      processLine(line);
  }

  return true;
}

bool
CCSS::
processLine(const std::string &line)
{
  return parse(line);
}

void
CCSS::
getIds(std::vector<std::string> &ids) const
{
  StyleDataMap::const_iterator p1, p2;

  for (p1 = styleData_.begin(), p2 = styleData_.end(); p1 != p2; ++p1) {
    const std::string &id = (*p1).first;

    ids.push_back(id);
  }
}

bool
CCSS::
parse(const std::string &str)
{
  CStrParse parse(str);

  std::string id;

  while (! parse.eof() && ! parse.isSpace() && ! parse.isChar('{')) {
    char c;

    parse.readChar(&c);

    id += c;
  }

  if (id.empty()) return false;

  //------

  StyleData &styleData = getStyleData(id);

  parse.skipSpace();

  if (parse.isChar('{')) {
    parse.skipChar();

    parse.skipSpace();

    std::string str1;

    while (! parse.eof() && ! parse.isChar('}')) {
      char c;

      parse.readChar(&c);

      str1 += c;
    }

    if (! parse.isChar('}'))
      return false;

    parse.skipChar();

    parse.skipSpace();

    if (! parseAttr(str1, styleData))
      return false;
  }

  return true;
}

bool
CCSS::
parseAttr(const std::string &str, StyleData &styleData)
{
  CStrParse parse(str);

  while (! parse.eof()) {
    std::string name;

    while (! parse.eof() && ! parse.isSpace() && ! parse.isChar(':')) {
      char c;

      parse.readChar(&c);

      name += c;
    }

    parse.skipSpace();

    std::string value;

    if (parse.isChar(':')) {
      parse.skipChar();

      parse.skipSpace();

      while (! parse.eof() && ! parse.isChar(';')) {
        char c;

        parse.readChar(&c);

        value += c;
      }

      if (parse.isChar(';')) {
        parse.skipChar();

        parse.skipSpace();
      }
    }

    if (name.empty())
      return false;

    styleData.addOption(name, value);
  }

  return true;
}

CCSS::StyleData &
CCSS::
getStyleData(const std::string &id)
{
  StyleDataMap::iterator p = styleData_.find(id);

  if (p == styleData_.end())
    p = styleData_.insert(p, StyleDataMap::value_type(id, StyleData(id)));

  StyleData &styleData = (*p).second;

  return styleData;
}

const CCSS::StyleData &
CCSS::
getStyleData(const std::string &id) const
{
  StyleDataMap::const_iterator p = styleData_.find(id);

  assert(p != styleData_.end());

  const StyleData &styleData = (*p).second;

  return styleData;
}

void
CCSS::
print(std::ostream &os) const
{
  StyleDataMap::const_iterator p1, p2;

  for (p1 = styleData_.begin(), p2 = styleData_.end(); p1 != p2; ++p1) {
    const StyleData &styleData = (*p1).second;

    styleData.print(os);

    os << std::endl;
  }
}

//----------

void
CCSS::StyleData::
print(std::ostream &os) const
{
  os << "<style class=\"" << id_ << "\"";

  OptionList::const_iterator p1, p2;

  for (p1 = options_.begin(), p2 = options_.end(); p1 != p2; ++p1) {
    const Option &option = *p1;

    os << " " << option.getName() << "=\"" << option.getValue() << "\"";
  }

  os << "/>";
}
