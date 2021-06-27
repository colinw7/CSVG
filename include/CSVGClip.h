#ifndef CSVGClip_H
#define CSVGClip_H

#include <COptVal.h>
#include <CFillType.h>

class CSVG;

class CSVGClip {
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

  void setRule(CFillType rule) { rule_.setValue(rule); }

  bool getRuleValid() const {
    return rule_.isValid();
  }

  CFillType getRule() const {
    if (rule_.isValid())
      return rule_.getValue();
    else
      return FILL_TYPE_EVEN_ODD;
  }

  void reset() {
    rule_.setInvalid();
  }

 private:
  CSVG                &svg_;
  COptValT<CFillType>  rule_;
};

#endif
