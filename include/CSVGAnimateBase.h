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

  std::string getFrom() const { return from_.getValue(""); }
  std::string getTo  () const { return to_  .getValue(""); }

  bool isAnimating() const { return animating_; }
  void setAnimating(bool b);

  double getStartTime() const { return startTime_.getValue(0); }

  double getEndTime() const { return endTime_.getValue(0); }

  int getRepeatNum() const { return repeatNum_.getValue(0); }

  bool processOption(const std::string &name, const std::string &value) override;

  void tick(double dt) override;

  virtual void animate(double t) = 0;

  void handleEvent(CSVGEventType type, const std::string &id, const std::string &data) override;

  void printValues(std::ostream &os) const;

 protected:
  COptValT<std::string>    attributeName_;
  COptValT<std::string>    attributeType_;
  COptValT<CSVGEventValue> begin_;
  COptValT<CSVGEventValue> end_;
  COptValT<CSVGTimeValue>  dur_;
  COptValT<std::string>    from_;
  COptValT<std::string>    to_;
  COptValT<std::string>    repeatCount_;
  COptValT<CSVGTimeValue>  repeatDur_;
  COptValT<std::string>    fill_;
  double                   currentTime_ { 0.0 };
  bool                     animating_   { false };
  COptValT<double>         startTime_;
  COptValT<double>         endTime_;
  COptValT<int>            repeatNum_;
  int                      repeatInd_;
};

#endif
