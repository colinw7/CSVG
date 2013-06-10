#ifndef CCSS_H
#define CCSS_H

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <sys/types.h>

class CCSS {
 public:
  class Option {
   private:
    std::string name_;
    std::string value_;

   public:
    Option(const std::string &name, const std::string &value) :
     name_(name), value_(value) {
    }

    const std::string &getName () const { return name_ ; }
    const std::string &getValue() const { return value_; }
  };

  typedef std::vector<Option> OptionList;

  class StyleData {
   private:
    std::string id_;
    OptionList  options_;

   public:
    StyleData(const std::string &id) :
     id_(id), options_() {
    }

    void addOption(const std::string &name, const std::string &value) {
      options_.push_back(Option(name, value));
    }

    uint getNumOptions() const { return options_.size(); }

    const Option &getOption(uint i) const { return options_[i]; }

    void print(std::ostream &os) const;
  };

  typedef std::map<std::string, StyleData> StyleDataMap;

  typedef StyleDataMap::const_iterator style_const_iterator;

 public:
  CCSS();

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

 private:
  StyleDataMap styleData_;
};

#endif
