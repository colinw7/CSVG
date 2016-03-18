#ifndef CLOG_H
#define CLOG_H

#include <CConv.h>

namespace CLogBuffer {
  std::string &msg();

  void init();
  void term();

  void add(const std::string &str);
}

class CLog {
 public:
  CLog() {
    CLogBuffer::init();
  }

  virtual ~CLog() {
    CLogBuffer::term();
  }

  template<typename T>
  const CLog &operator<<(const T &t) const {
    CLogBuffer::add(CConv::toString(t));

    return *this;
  }
};

#endif
