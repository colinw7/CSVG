#ifndef CSVGAnimateBase_H
#define CSVGAnimateBase_H

#include <CSVGObject.h>

class CSVGAnimateBase : public CSVGObject {
 public:
  CSVG_OBJECT_DEF("animateBase", CSVGObjTypeId::ANIMATE_BASE)

  CSVGAnimateBase(CSVG &svg);
  CSVGAnimateBase(const CSVGAnimateBase &animateBase);

  bool isAnimated() const override { return true; }

  std::string getAttributeName() const { return attributeName_.value_or(""); }
  void setAttributeName(const std::string &s) { attributeName_ = s; }

  std::string getAttributeType() const { return attributeType_.value_or(""); }
  void setAttributeType(const std::string &s) { attributeType_ = s; }

  CSVGEventValue getBegin() const { return begin_.value_or(CSVGEventValue()); }

  CSVGEventValue getEnd() const { return end_.value_or(CSVGEventValue()); }

  CSVGTimeValue getDur() const { return dur_.value_or(CSVGTimeValue()); }

  std::string getFrom() const { return from_.value_or(""); }
  void setFrom(const std::string &s) { from_ = s; }

  std::string getTo() const { return to_.value_or(""); }
  void setTo(const std::string &s) { to_ = s; }

  std::string getBy() const { return by_.value_or(""); }
  void setBy(const std::string &s) { by_ = s; }

  std::string getValues() const { return values_.value_or(""); }
  void setValues(const std::string &s) { values_ = s; }

  std::string getFill() const { return fill_.value_or(""); }
  void setFill(const std::string &s) { fill_ = s; }

  std::string getRepeatCount() const { return repeatCount_.value_or(""); }
  void setRepeatCount(const std::string &s) { repeatCount_ = s; }

  CSVGTimeValue getRepeatDur() const { return repeatDur_.value_or(CSVGTimeValue()); }

  bool isAnimating() const { return animating_; }
  void setAnimating(bool b);

  double getStartTime() const { return startTime_.value_or(0); }

  double getEndTime() const { return endTime_.value_or(0); }

  int getRepeatNum() const { return repeatNum_.value_or(0); }

  bool processOption(const std::string &name, const std::string &value) override;

  void tick(double dt) override;

  void setTime(double t) override;

  virtual void animate(double t) = 0;

  void updateAnimation();

  void handleEvent(CSVGEventType type, const std::string &id, const std::string &data,
                   bool propagate) override;

  void printValues(std::ostream &os, bool flat=false) const override;

  void accept(CSVGVisitor *visitor) override { visitor->visit(this); }

 protected:
  std::optional<std::string>    attributeName_;
  std::optional<std::string>    attributeType_;
  std::optional<CSVGEventValue> begin_;
  std::optional<CSVGEventValue> end_;
  std::optional<CSVGTimeValue>  dur_;
  std::optional<std::string>    from_;
  std::optional<std::string>    to_;
  std::optional<std::string>    by_;
  std::optional<std::string>    values_;
  std::optional<std::string>    repeatCount_;
  std::optional<CSVGTimeValue>  repeatDur_;
  std::optional<std::string>    fill_;
  std::optional<std::string>    calcMode_;
  std::optional<std::string>    keyTimes_;
  std::optional<std::string>    keySplines_;
  double                        currentTime_ { 0.0 };
  bool                          animating_   { false };
  std::optional<double>         startTime_;
  std::optional<double>         endTime_;
  std::optional<int>            repeatNum_;
  int                           repeatInd_   { 0 };
};

#endif
