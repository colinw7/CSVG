#include <CCSS.h>
#include <CFile.h>
#include <CStrUtil.h>
#include <CStrParse.h>
#include <cassert>

CCSS::
CCSS()
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
  for (const auto &d : styleData_) {
    const std::string &id = d.first;

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
  auto p = styleData_.find(id);

  if (p == styleData_.end())
    p = styleData_.insert(p, StyleDataMap::value_type(id, StyleData(id)));

  StyleData &styleData = (*p).second;

  return styleData;
}

const CCSS::StyleData &
CCSS::
getStyleData(const std::string &id) const
{
  auto p = styleData_.find(id);

  assert(p != styleData_.end());

  const StyleData &styleData = (*p).second;

  return styleData;
}

void
CCSS::
print(std::ostream &os) const
{
  for (const auto &d : styleData_) {
    const StyleData &styleData = d.second;

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

  for (const auto &o : options_)
    os << " " << o.getName() << "=\"" << o.getValue() << "\"";

  os << "/>";
}
