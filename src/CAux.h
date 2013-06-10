#ifndef CAUX_H
#define CAUX_H

#include <algorithm>
#include <vector>
#include <list>

namespace CAux {
  template <class Return, class Type>
  class mem_fun_t {
    typedef Return (Type::*Func)();

   public:
    explicit mem_fun_t(Func func) : func_(func) { }

    Return operator()(Type *obj) const {
      return (obj->*func_)();
    }

    Return operator()(Type &obj) const {
      return (obj.*func_)();
    }

   private:
    Func func_;
  };
}

namespace CAux {
  template <class Return, class Type>
  class const_mem_fun_t {
    typedef Return (Type::*Func)();

   public:
    explicit const_mem_fun_t(Func func) : func_(func) {}

    Return operator()(const Type *obj) const {
      return (obj->*func_)();
    }

    Return operator()(const Type &obj) const {
      return (obj.*func_)();
    }

   private:
    Func func_;
  };
}

namespace CAux {
  template <class Iterator, class Function>
  inline
  Function
  for_each(Iterator first, Iterator last, Function func) {
    return std::for_each(first, last, func);
  }

  template <class Iterator, class Return, class Class>
  inline
  mem_fun_t<Return, Class>
  for_each(Iterator first, Iterator last, Return (Class::*func)()) {
    return std::for_each(first, last, mem_fun_t<Return, Class>(func));
  }

  template <class Iterator, class Return, class Class>
  inline
  const_mem_fun_t<Return, Class>
  for_each(Iterator first, Iterator last, Return (Class::*func)() const) {
    return std::for_each(first, last, const_mem_fun_t<Return, Class>(func));
  }

  template <class T, class Function>
  inline
  Function
  for_each(const std::vector<T> &v, Function func) {
    return std::for_each(v.begin(), v.end(), func);
  }

  template <class T, class Return, class Class>
  inline
  mem_fun_t<Return, Class>
  for_each(const std::vector<T> &v, Return (Class::*func)()) {
    return std::for_each(v.begin(), v.end(), mem_fun_t<Return, Class>(func));
  }

  template <class T, class Return, class Class>
  inline
  const_mem_fun_t<Return, Class>
  for_each(const std::vector<T> &v, Return (Class::*func)() const) {
    return std::for_each(v.begin(), v.end(), const_mem_fun_t<Return, Class>(func));
  }

  template <class T, class Function>
  inline
  Function
  for_each(const std::list<T> &v, Function func) {
    return std::for_each(v.begin(), v.end(), func);
  }

  template <class T, class Return, class Class>
  inline
  mem_fun_t<Return, Class>
  for_each(const std::list<T> &v, Return (Class::*func)()) {
    return std::for_each(v.begin(), v.end(), mem_fun_t<Return, Class>(func));
  }

  template <class T, class Return, class Class>
  inline
  const_mem_fun_t<Return, Class>
  for_each(const std::list<T> &v, Return (Class::*func)() const) {
    return std::for_each(v.begin(), v.end(), const_mem_fun_t<Return, Class>(func));
  }
}

namespace CAux {
  template <class T>
  inline
  T pop_front(std::vector<T> &v) {
    typename std::vector<T>::iterator p1 = v.begin(), p2 = v.end();

    T t = *p1;

    std::vector<T> v1;

    std::copy(++p1, p2, std::back_inserter(v1));

    v = v1;

    return t;
  }
};

#endif
