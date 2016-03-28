#ifndef CSVGEventValue_H
#define CSVGEventValue_H

#include <CSVGTimeValue.h>

class CSVGEventValue {
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

 public:
  CSVGEventType type_;
  std::string   id_;
  CSVGTimeValue time_;
  std::string   args_;
};

#endif
