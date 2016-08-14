#include <CSVGAnimateBase.h>
#include <CSVG.h>
#include <CSVGLog.h>
#include <CSVGUtil.h>
#include <CStrUtil.h>

/* Attributes:
    <Core>
    <Conditional>
    animation event attributes
    xlink attributes
    animation attribute target attributes
    animation timing attributes
    animation value attributes
    animation addition attributes
    <External>
*/

CSVGAnimateBase::
CSVGAnimateBase(CSVG &svg) :
 CSVGObject(svg)
{
}

CSVGAnimateBase::
CSVGAnimateBase(const CSVGAnimateBase &animateBase) :
 CSVGObject(animateBase),
 attributeName_(animateBase.attributeName_),
 attributeType_(animateBase.attributeType_),
 begin_        (animateBase.begin_),
 end_          (animateBase.end_),
 dur_          (animateBase.dur_),
 from_         (animateBase.from_),
 to_           (animateBase.to_),
 repeatCount_  (animateBase.repeatCount_),
 repeatDur_    (animateBase.repeatDur_),
 fill_         (animateBase.fill_)
{
}

bool
CSVGAnimateBase::
processOption(const std::string &opt_name, const std::string &opt_value)
{
  if (processCoreOption           (opt_name, opt_value)) return true;
  if (processConditionalOption    (opt_name, opt_value)) return true;
  if (processExternalOption       (opt_name, opt_value)) return true;
  if (processAnimationEventsOption(opt_name, opt_value)) return true;

  std::string    str;
  CSVGTimeValue  time;
  CSVGEventValue event;

  if      (svg_.stringOption    (opt_name, opt_value, "attributeName", str))
    attributeName_ = str;
  else if (svg_.stringOption    (opt_name, opt_value, "attributeType", str))
    attributeType_ = str;
  else if (svg_.eventValueOption(opt_name, opt_value, "begin", event)) {
    begin_ = event;

    if (begin_.isValid()) {
      if (begin_.getValue().isTimeout()) {
        startTime_ = begin_.getValue().time().getSeconds();

        if (dur_.isValid())
          endTime_ = getStartTime() + dur_.getValue().getSeconds();
      }
      else if (begin_.getValue().type() == CSVGEventType::ANIMATE_BEGIN &&
               begin_.getValue().id() == "prev") {
        CSVGAnimateBase *prev = dynamic_cast<CSVGAnimateBase *>(
          getParent()->getAnimation().prevObject(this));

        if (prev) {
          startTime_ = prev->startTime_;

          if (dur_.isValid())
            endTime_ = getStartTime() + dur_.getValue().getSeconds();
        }
      }
    }
  }
  else if (svg_.eventValueOption(opt_name, opt_value, "end", event)) {
    end_ = event;

    if (end_.isValid() && end_.getValue().isTimeout())
      endTime_ = end_.getValue().time().getSeconds();
  }
  else if (svg_.timeValueOption (opt_name, opt_value, "dur", time)) {
    dur_ = time;

    if (dur_.isValid())
      endTime_ = getStartTime() + dur_.getValue().getSeconds();
  }
  else if (svg_.stringOption(opt_name, opt_value, "repeatCount", str)) {
    repeatCount_ = str;

    int n = 0;

    if      (str == "indefinite")
      repeatNum_ = -1;
    else if (CStrUtil::toInteger(str, &n))
      repeatNum_ = n;

    repeatInd_ = 0;
  }
  else if (svg_.timeValueOption(opt_name, opt_value, "repeatDur", time))
    repeatDur_ = time;
  else if (svg_.stringOption(opt_name, opt_value, "from", str))
    from_ = CStrUtil::stripSpaces(str);
  else if (svg_.stringOption(opt_name, opt_value, "to"  , str))
    to_ = CStrUtil::stripSpaces(str);
  else if (svg_.stringOption(opt_name, opt_value, "by"  , str))
    by_ = str;
  else if (svg_.stringOption(opt_name, opt_value, "fill", str))
    fill_ = str;
  else if (svg_.stringOption(opt_name, opt_value, "values", str))
    values_ = str;
  else if (svg_.stringOption(opt_name, opt_value, "calcMode", str))
    calcMode_ = str;
  else if (svg_.stringOption(opt_name, opt_value, "keyTimes", str))
    keyTimes_ = str;
  else if (svg_.stringOption(opt_name, opt_value, "keySplines", str))
    keySplines_ = str;
  else
    return false;

  return true;
}

void
CSVGAnimateBase::
tick(double dt)
{
  currentTime_ += dt;

  updateAnimation();
}

void
CSVGAnimateBase::
setTime(double t)
{
  currentTime_ = t;

  updateAnimation();
}

void
CSVGAnimateBase::
updateAnimation()
{
  if (! getParent())
    return;

  if (! isAnimating()) {
    if (startTime_.isValid()) {
      if (endTime_.isValid()) {
        if (currentTime_ >= getStartTime() && currentTime_ <= getEndTime())
          setAnimating(true);
      }
      else {
        if (currentTime_ >= getStartTime())
          setAnimating(true);
      }
    }
    else {
      if (! begin_.isValid() && endTime_.isValid()) {
        if (currentTime_ <= getEndTime())
          setAnimating(true);
      }
    }
  }
  else {
    if (endTime_.isValid() && currentTime_ > getEndTime()) {
      bool repeat = false;

      if      (repeatNum_.isValid()) {
        ++repeatInd_;

        if      (getRepeatNum() == -1)
          repeat = true;
        else if (getRepeatNum() >= repeatInd_)
          repeat = true;

        if (repeat && id_.isValid())
          getParent()->getSVG().sendEvent(CSVGEventType::ANIMATE_REPEAT, id_.getValue(),
                                          std::to_string(repeatInd_));
      }
      else if (repeatDur_.isValid()) {
        dur_ = repeatDur_;

        repeatDur_.setInvalid();

        endTime_ = getStartTime() + dur_.getValue().getSeconds();

        repeat = true;
      }

      if (repeat)
        currentTime_ = getStartTime();
      else
        setAnimating(false);
    }
  }

  //---

  if (! isAnimating())
    return;

  //---

  double t = CSVGUtil::map(currentTime_, getStartTime(), getEndTime(), 0, 1);

  animate(t);
}

void
CSVGAnimateBase::
handleEvent(CSVGEventType type, const std::string &id, const std::string &data,
            bool /*propagate*/)
{
  if (begin_.isValid() && begin_.getValue().isMatch(type, id, data)) {
    startTime_ = currentTime_ + begin_.getValue().time().getSeconds();

    if      (end_.isValid() && end_.getValue().isTimeout())
      endTime_ = end_.getValue().time().getSeconds();
    else if (dur_.isValid())
      endTime_ = getStartTime() + dur_.getValue().getSeconds();

    if (currentTime_ >= getStartTime() && currentTime_ <= getEndTime())
      setAnimating(true);
  }

  if (end_.isValid() && end_.getValue().isMatch(type, id, data))
    setAnimating(false);
}

void
CSVGAnimateBase::
setAnimating(bool b)
{
  if (b == animating_)
    return;

  if (b) {
    animating_ = true;

    if (id_.isValid())
      getParent()->getSVG().sendEvent(CSVGEventType::ANIMATE_BEGIN, id_.getValue());

    execEvent(CSVGEventType::BEGIN);
  }
  else {
    animating_ = false;

    if (id_.isValid())
      getParent()->getSVG().sendEvent(CSVGEventType::ANIMATE_END, id_.getValue());

    execEvent(CSVGEventType::END);
  }
}

void
CSVGAnimateBase::
printValues(std::ostream &os, bool /*flat*/) const
{
  printNameValue(os, "id", id_);

  printNameValue(os, "attributeName", attributeName_);
  printNameValue(os, "attributeType", attributeType_);
  printNameEvent(os, "begin"        , begin_        );
  printNameEvent(os, "end"          , end_          );
  printNameTime (os, "dur"          , dur_          );
  printNameValue(os, "from"         , from_         );
  printNameValue(os, "to"           , to_           );
  printNameValue(os, "by"           , by_           );
  printNameValue(os, "repeatCount"  , repeatCount_  );
  printNameTime (os, "repeatDur"    , repeatDur_    );
  printNameValue(os, "fill"         , fill_         );
  printNameValue(os, "values"       , values_       );
  printNameValue(os, "calcMode"     , calcMode_     );
  printNameValue(os, "keyTimes"     , keyTimes_     );
  printNameValue(os, "keySplines"   , keySplines_   );
}
