#ifndef CSVGEventValue_H
#define CSVGEventValue_H

#include <CSVGTimeValue.h>

class CSVGEventValue : public CSVGPrintBase<CSVGEventValue> {
 public:
  CSVGEventValue(const CSVGTimeValue &time=CSVGTimeValue()) :
   type_(CSVGEventType::TIMEOUT), time_(time) {
  }

  CSVGEventValue(CSVGEventType type, const std::string &id="",
                 const CSVGTimeValue &time=CSVGTimeValue(), const std::string &args="") :
   type_(type), id_(id), time_(time), args_(args) {
  }

  CSVGEventType type() const { return type_ ; }

  bool isTimeout() const { return (type_ == CSVGEventType::TIMEOUT); }

  const CSVGTimeValue &time() const { return time_; }

  const std::string &id() const { return id_; }

  const std::string &args() const { return args_; }

  bool isMatch(CSVGEventType type, const std::string &id, const std::string &args) const {
    return (type_ == type && (id_ == "" || id_ == id) && (args_ == "" || args_ == args));
  }

  void print(std::ostream &os) const {
    if      (type() == CSVGEventType::TIMEOUT)
      os << time();
    else if (type() == CSVGEventType::CLICK)
      os << "click";
    else if (type() == CSVGEventType::MOUSE_DOWN)
      os << "mousedown";
    else if (type() == CSVGEventType::MOUSE_UP)
      os << "mouseup";
    else if (type() == CSVGEventType::MOUSE_OVER)
      os << "mouseover";
    else if (type() == CSVGEventType::MOUSE_OUT)
      os << "mouseout";
    else if (type() == CSVGEventType::ANIMATE_BEGIN)
      os << id() << ".begin";
    else if (type() == CSVGEventType::ANIMATE_END)
      os << id() << ".end";
    else if (type() == CSVGEventType::ANIMATE_REPEAT)
      os << id() << ".repeat";

    if (type() != CSVGEventType::TIMEOUT) {
      if (args() != "")
        os << "(" << args() << ")";

      if (time().type() != CSVGTimeValueType::NONE) {
        os << "+";

        os << time();
      }
    }
  }

 public:
  CSVGEventType type_;
  std::string   id_;
  CSVGTimeValue time_;
  std::string   args_;
};

#endif
