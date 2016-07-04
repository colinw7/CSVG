#ifndef CSVGAnimateBase_H
#define CSVGAnimateBase_H

#include <CSVGObject.h>

class CSVGAnimateBase : public CSVGObject {
 public:
  CSVG_OBJECT_DEF("animateBase", CSVGObjTypeId::ANIMATE_BASE)

  CSVGAnimateBase(CSVG &svg);
  CSVGAnimateBase(const CSVGAnimateBase &animateBase);

  bool isAnimated() const override { return true; }

  std::string getAttributeName() const { return attributeName_.getValue(""); }
  void setAttributeName(const std::string &s) { attributeName_ = s; }

  std::string getAttributeType() const { return attributeType_.getValue(""); }
  void setAttributeType(const std::string &s) { attributeType_ = s; }

  CSVGEventValue getBegin() const { return begin_.getValue(CSVGEventValue()); }

  CSVGEventValue getEnd() const { return end_.getValue(CSVGEventValue()); }

  CSVGTimeValue getDur() const { return dur_.getValue(CSVGTimeValue()); }

  std::string getFrom() const { return from_.getValue(""); }
  void setFrom(const std::string &s) { from_ = s; }

  std::string getTo() const { return to_.getValue(""); }
  void setTo(const std::string &s) { to_ = s; }

  std::string getFill() const { return fill_.getValue(""); }
  void setFill(const std::string &s) { fill_ = s; }

  std::string getRepeatCount() const { return repeatCount_.getValue(""); }
  void setRepeatCount(const std::string &s) { repeatCount_ = s; }

  CSVGTimeValue getRepeatDur() const { return repeatDur_.getValue(CSVGTimeValue()); }

  bool isAnimating() const { return animating_; }
  void setAnimating(bool b);

  double getStartTime() const { return startTime_.getValue(0); }

  double getEndTime() const { return endTime_.getValue(0); }

  int getRepeatNum() const { return repeatNum_.getValue(0); }

  bool processOption(const std::string &name, const std::string &value) override;

  void tick(double dt) override;

  void setTime(double t) override;

  virtual void animate(double t) = 0;

  void updateAnimation();

  void handleEvent(CSVGEventType type, const std::string &id, const std::string &data,
                   bool propagate) override;

  void printValues(std::ostream &os, bool flat=false) const override;

 protected:
  COptString               attributeName_;
  COptString               attributeType_;
  COptValT<CSVGEventValue> begin_;
  COptValT<CSVGEventValue> end_;
  COptValT<CSVGTimeValue>  dur_;
  COptString               from_;
  COptString               to_;
  COptString               by_;
  COptString               values_;
  COptString               repeatCount_;
  COptValT<CSVGTimeValue>  repeatDur_;
  COptString               fill_;
  COptString               calcMode_;
  COptString               keyTimes_;
  COptString               keySplines_;
  double                   currentTime_ { 0.0 };
  bool                     animating_   { false };
  COptReal                 startTime_;
  COptReal                 endTime_;
  COptInt                  repeatNum_;
  int                      repeatInd_;
};

#endif
