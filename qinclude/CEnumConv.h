#ifndef CEnumConv_H
#define CEnumConv_H

#include <cassert>

//! class to convert between enum classes with equivalent values
template<typename E1, typename E2>
class CEnumConv {
 public:
  using E1E2 = std::pair<E1, E2>;

  CEnumConv(std::initializer_list<E1E2> l) {
    for (const auto &e : l)
      addValue(e);
  }

  void addValue(const E1E2 &e) {
    e1e2_[e.first ] = e.second;
    e2e1_[e.second] = e.first ;
  }

  // convert enum E1 to E2 (assert if not found)
  E2 conv(const E1 &e1) {
    auto p = e1e2_.find(e1);
    assert(p != e1e2_.end());

    return (*p).second;
  }

  // convert enum E1 to E2 (with default)
  E2 conv(const E1 &e1, const E2 &e2) {
    auto p = e1e2_.find(e1);

    return (p != e1e2_.end() ? (*p).second : e2);
  }

  // convert enum E2 to E1 (assert if not found)
  E1 conv(const E2 &e2) {
    auto p = e2e1_.find(e2);
    assert(p != e2e1_.end());

    return (*p).second;
  }

  // convert enum E2 to E1 (with default)
  E1 conv(const E2 &e2, const E1 &e1) {
    auto p = e2e1_.find(e2);

    return (p != e2e1_.end() ? (*p).second : e1);
  }

 private:
  using E1E2Map = std::map<E1, E2>;
  using E2E1Map = std::map<E2, E1>;

  E1E2Map e1e2_;
  E2E1Map e2e1_;
};

#endif
