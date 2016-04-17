#ifndef CEnumConv_H
#define CEnumConv_H

#include <cassert>

template<typename E1, typename E2>
class CEnumConv {
 public:
  typedef std::pair<E1,E2> E1E2;

  CEnumConv(std::initializer_list<E1E2> l) {
    for (const auto &e : l)
      addValue(e);
  }

  void addValue(const E1E2 &e) {
    e1e2_[e.first ] = e.second;
    e2e1_[e.second] = e.first ;
  }

  E2 conv(const E1 &e1) {
    auto p = e1e2_.find(e1);

    assert(p != e1e2_.end());

    return (*p).second;
  }

  E2 conv(const E1 &e1, const E2 &e2) {
    auto p = e1e2_.find(e1);

    return (p != e1e2_.end() ? (*p).second : e2);
  }

  E1 conv(const E2 &e2) {
    auto p = e2e1_.find(e2);

    assert(p != e2e1_.end());

    return (*p).second;
  }

  E1 conv(const E2 &e2, const E1 &e1) {
    auto p = e2e1_.find(e2);

    return (p != e2e1_.end() ? (*p).second : e1);
  }

 private:
  typedef std::map<E1,E2> E1E2Map;
  typedef std::map<E2,E1> E2E1Map;

  E1E2Map e1e2_;
  E2E1Map e2e1_;
};

#endif
