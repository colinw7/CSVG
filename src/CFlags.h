#ifndef CFLAGS_H
#define CFLAGS_H

#include <sys/types.h>

template<typename T>
class CFlags {
 public:
  typedef T enum_type;

 private:
  uint value_;

 public:
  // constructors
  CFlags() :
   value_(0) {
    reset();
  }

  CFlags(T value) :
   value_((int) value) {
  }

  // copy/assign
  CFlags(const CFlags &flags) :
   value_(flags.value_) {
  }

  CFlags &operator=(const CFlags &flags) {
    value_ = flags.value_;

    return *this;
  }

  // enum value
  T value() const { return (T) value_; }

  // conversion
  operator int() const { return value_; }

  //operator T() const { return value(); }

  // operators
  //CFlags &operator&=(int  flag) { value_ &= flag; return *this; }
  //CFlags &operator&=(uint flag) { value_ &= flag; return *this; }
  CFlags operator &=(CFlags flags) { value_ &= flags.value_; return *this; }
  CFlags operator &=(T      flag ) { value_ &= flag        ; return *this; }

  //CFlags operator &(int  flag) const { CFlags t(*this); t &= flag  ; return t; }
  //CFlags operator &(uint flag) const { CFlags t(*this); t &= flag  ; return t; }
  CFlags operator &(CFlags flags) const { CFlags t(*this); t &= flags; return t; }
  CFlags operator &(T      flag ) const { CFlags t(*this); t &= flag ; return t; }

  CFlags &operator|=(CFlags flags) { value_ |= flags.value_; return *this; }
  CFlags &operator|=(T      flag ) { value_ |= flag        ; return *this; }

  CFlags operator|(CFlags flags) const { CFlags t(*this); t |= flags; return t; }
  CFlags operator|(T      flag ) const { CFlags t(*this); t |= flag ; return t; }

  CFlags &operator^=(CFlags flags) { value_ ^= flags.value_; return *this; }
  CFlags &operator^=(T      flag ) { value_ ^= flag        ; return *this; }

  CFlags operator^(CFlags flags) const { CFlags t(*this); t ^= flags; return t; }
  CFlags operator^(T      flag ) const { CFlags t(*this); t ^= flag ; return t; }

  CFlags operator~() const { CFlags t; t.value_ = ~value_; return t; }

  //bool operator!() const { return ! isSet(); }

  // functions
  bool isSet() const { return (value_ != 0); }

  void set  () { value_ = ~0; }
  void reset() { value_ =  0; }

  void invert() { value_ = ~value_; }

  CFlags &add(CFlags flags) { *this |= flags; return *this; }
  CFlags &add(T      flag ) { *this |= flag ; return *this; }

  CFlags &remove(CFlags flags) { *this &= ~flags; return *this; }
  CFlags &remove(T      flag ) { *this &= ~flag ; return *this; }

  bool test(CFlags flags) const { return (value_ & flags.value_); }
  bool test(T      flag ) const { return (value_ & flag        ); }
};

#define CFLAGS_OPERATORS(T) \
inline CFlags<T> operator|(T f1, T f2) { return CFlags<T>(f1) | f2; } \
\
inline CFlags<T> operator|(T f1, CFlags<T> f2) { return f2 | f1; }

#endif
