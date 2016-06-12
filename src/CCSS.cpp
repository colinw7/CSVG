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
  if (! CFile::exists(filename) || ! CFile::isRegular(filename)) {
    if (isDebug())
      std::cerr << "Invalid file '" << filename << "'" << std::endl;
    return false;
  }

  CFile file(filename);

  std::string str;

  std::string line;

  while (file.readLine(line)) {
    line = CStrUtil::stripSpaces(line);

    if (line.empty())
      continue;

    if (! str.empty())
      str += "\n";

    str += line;
  }

  processLine(str);

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

  while (! parse.eof()) {
    parse.skipSpace();

    if (parse.isString("/*")) {
      skipComment(parse);

      parse.skipSpace();
    }

    // get id
    std::string id;

    if (! readId(parse, id)) {
      if (isDebug())
        std::cerr << "Empty id : '" << parse.stateStr() << "'" << std::endl;
      return false;
    }

    //------

    // get values
    StyleData &styleData = getStyleData(id);

    if (parse.isChar('{')) {
      std::string str1;

      // still parse text with missing end brace, just exit loop
      bool rc = readBracedString(parse, str1);

      if (! parseAttr(str1, styleData))
        return false;

      if (! rc)
        break;
    }
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

    if (name.empty()) {
      if (isDebug())
        std::cerr << "Empty name : '" << parse.stateStr() << "'" << std::endl;
      return false;
    }

    styleData.addOption(name, value);
  }

  return true;
}

bool
CCSS::
readId(CStrParse &parse, std::string &id) const
{
  id = "";

  parse.skipSpace();

  while (! parse.eof() && ! parse.isSpace() && ! parse.isChar('{')) {
    char c;

    parse.readChar(&c);

    id += c;
  }

  parse.skipSpace();

  return ! id.empty();
}

bool
CCSS::
readBracedString(CStrParse &parse, std::string &str) const
{
  str = "";

  parse.skipChar();

  parse.skipSpace();

  while (! parse.eof() && ! parse.isChar('}')) {
    char c;

    parse.readChar(&c);

    str += c;
  }

  if (! parse.isChar('}')) {
    if (isDebug())
      std::cerr << "Missing close brace : '" << parse.stateStr() << "'" << std::endl;
    return false;
  }

  parse.skipChar();

  parse.skipSpace();

  return true;
}

bool
CCSS::
skipComment(CStrParse &parse) const
{
  parse.skipChars(2);

  while (! parse.eof()) {
    if (parse.isString("*/")) {
      parse.skipChars(2);
      return true;
    }

    parse.skipChar();
  }

  if (isDebug())
    std::cerr << "Unterminated commend : '" << parse.stateStr() << "'" << std::endl;

  return false;
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
