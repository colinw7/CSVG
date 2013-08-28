#include <CLog.h>
#include <iostream>

std::string &
CLogBuffer::
msg()
{
 static std::string msg_;

 return msg_;
}

void
CLogBuffer::
init()
{
  msg() = "";
}

void
CLogBuffer::
term()
{
  std::cerr << msg() << std::endl;
}

void
CLogBuffer::
add(const std::string &str)
{
  msg() += str;
}
