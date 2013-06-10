#ifndef CREGEXP_H
#define CREGEXP_H

#include <string>
#include <vector>

#define RE_UINT   "[0-9][0-9]*"
#define RE_INT    "[+-]*[0-9][0-9]*"
#define RE_REAL   "[+-]*[0-9][0-9]*"                 "|" \
                  ".[0-9][0-9]*"                     "|" \
                  ".[0-9][0-9]*[Ee][+-]*[0-9][0-9]*" "|" \
                  "[+-]*[1-9]*[0-9].[0-9][0-9]*"     "|" \
                  "[+-]*[1-9]*[0-9].[0-9][0-9]*[Ee][+-]*[0-9][0-9]*"
#define RE_SPACE  "[ \\t\\n,]"
#define RE_HEXDEC "[0-9a-fA-F]"
#define RE_OCTAL  "[0-7]"

struct CRegExpMatch {
  int start;
  int end;

  CRegExpMatch(int start, int end) :
   start(start), end(end) {
  }
};

class CRegExpImpl;

class CRegExp {
 private:
  CRegExpImpl *impl_;

 public:
  CRegExp(const std::string &pattern="");
  CRegExp(const CRegExp &regexp);
 ~CRegExp();

  const CRegExp &operator=(const CRegExp &regexp);

  void setPattern(const std::string &pattern);
  const std::string &getPattern() const;

  void setCaseSensitive(bool flag);
  void setExtended(bool flag);
  void setMatchBOL(bool flag);
  void setMatchEOL(bool flag);

  bool find(const std::string &str) const;

  std::string replace(const std::string &str, bool global=false) const;

  bool isMatchAll() const;

  bool getMatchRange(int *start, int *end) const;

  bool getMatchRange(int pos, int *start, int *end) const;

  void getMatches(std::vector<CRegExpMatch> &matches) const;
  void getMatches(std::vector<std::string> &matches) const;

  int         getNumMatches() const;
  std::string getMatchString(int i) const;

  bool isValid() const;

  std::string getErrorMsg() const;

  static bool find(const std::string &pattern, const std::string &str);

  static std::string replace(const std::string &pattern, const std::string &str, bool global=false);
};

namespace CRegExpUtil {
  bool parse(const std::string &str, const std::string &pattern);
  bool parse(const std::string &str, const CRegExp &pattern);
  bool parse(const std::string &str, const std::string &pattern,
             std::vector<std::string> &match_strs);
  bool parse(const std::string &str, const CRegExp &pattern,
             std::vector<std::string> &match_strs);

  int  gregsub(const std::string &istr, const CRegExp &pattern,
               const std::string &rstr, std::string &ostr);
  bool regsub (const std::string &istr, const CRegExp &pattern,
               const std::string &rstr, std::string &ostr);
}

#endif
