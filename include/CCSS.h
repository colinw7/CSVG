#ifndef CCSS_H
#define CCSS_H

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <sys/types.h>

class CStrParse;

class CCSS {
 public:
  class Option {
   public:
    Option(const std::string &name, const std::string &value) :
     name_(name), value_(value) {
    }

    const std::string &getName () const { return name_ ; }
    const std::string &getValue() const { return value_; }

   private:
    std::string name_;
    std::string value_;
  };

  typedef std::vector<Option> OptionList;

  //---

  class StyleData {
   public:
    StyleData(const std::string &id) :
     id_(id), options_() {
    }

    const std::string &getId() const { return id_; }

    const OptionList &getOptions() const { return options_; }

    uint getNumOptions() const { return options_.size(); }

    const Option &getOption(uint i) const { return options_[i]; }

    void addOption(const std::string &name, const std::string &value) {
      options_.push_back(Option(name, value));
    }

    void print(std::ostream &os) const;

   private:
    std::string id_;
    OptionList  options_;
  };

  typedef std::map<std::string, StyleData> StyleDataMap;

  //---

 public:
  CCSS();

  bool isDebug() const { return debug_; }
  void setDebug(bool b) { debug_ = b; }

  bool processFile(const std::string &fileName);

  bool processLine(const std::string &line);

  void getIds(std::vector<std::string> &ids) const;

  StyleData &getStyleData(const std::string &id);

  const StyleData &getStyleData(const std::string &id) const;

  void print(std::ostream &os) const;

  friend std::ostream &operator<<(std::ostream &os, const CCSS &css) {
    css.print(os);

    return os;
  }

 private:
  bool parse(const std::string &str);
  bool parseAttr(const std::string &str, StyleData &styleData);

  bool readId(CStrParse &parse, std::string &id) const;

  bool readBracedString(CStrParse &parse, std::string &str) const;

  bool skipComment(CStrParse &parse) const;

 private:
  bool         debug_ { false };
  StyleDataMap styleData_;
};

#endif
