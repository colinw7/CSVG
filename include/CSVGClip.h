#ifndef CSVGClip_H
#define CSVGClip_H

#include <CSVGInheritVal.h>
#include <CFillType.h>
#include <optional>

class CSVG;

class CSVGClip {
 public:
  using FillType = CSVGInheritValT<CFillType>;

 public:
  CSVGClip(CSVG &svg) :
   svg_(svg), rule_() {
  }

  CSVGClip(const CSVGClip &clip) :
   svg_(clip.svg_), rule_(clip.rule_) {
  }

  CSVGClip &operator=(const CSVGClip &clip) {
    rule_ = clip.rule_;

    return *this;
  }

  void setRule(const std::string &rule_str);
  void setRule(const FillType &rule) { rule_ = rule; }

  bool getRuleValid() const { return !!rule_; }

  FillType getRule() const {
    if (rule_)
      return rule_.value();
    else
      return FillType(FILL_TYPE_EVEN_ODD);
  }

  void reset() {
    rule_.reset();
  }

 private:
  CSVG&                   svg_;
  std::optional<FillType> rule_;
};

#endif
