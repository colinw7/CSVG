#ifndef CSVGLineDash_H
#define CSVGLineDash_H

#include <CSVGTmpl.h>
#include <CScreenUnits.h>
#include <vector>

class CSVGLineDash : public CSVGEqBase<CSVGLineDash>, public CSVGPrintBase<CSVGLineDash> {
 public:
  enum class Type {
    NONE,
    SOLID,
    DASHED
  };

 public:
  using Dashes = std::vector<CScreenUnits>;

 public:
  static CSVGLineDash makeNone() {
    return CSVGLineDash(Type::NONE);
  }

  static CSVGLineDash makeSolid() {
    return CSVGLineDash(Type::SOLID);
  }

 public:
  CSVGLineDash() { }

  CSVGLineDash(const Dashes &dashes) :
   type_(Type::DASHED), dashes_(dashes) {
  }

  const Type &type() const { return type_; }
  void setType(const Type &v) { type_ = v; }

  const Dashes &dashes() const { assert(type_ == Type::DASHED); return dashes_; }
  void setDashes(const Dashes &d) { type_ = Type::DASHED; dashes_ = d; }

  bool isNone  () const { return type_ == Type::NONE;   }
  bool isSolid () const { return type_ == Type::SOLID;  }
  bool isDashed() const { return type_ == Type::DASHED; }

  int numDashes() const { return dashes_.size(); }

  const CScreenUnits &dash(int i) const {
    assert(i >= 0 && i < int(dashes_.size()));
    return dashes_[i];
  }

  friend bool operator==(const CSVGLineDash &lhs, const CSVGLineDash &rhs) {
    if (lhs.type() != rhs.type()) return false;

    int nl = lhs.dashes().size();
    int nr = rhs.dashes().size();
    if (nl != nr) return false;

    for (int i = 0; i < nl; ++i)
      if (lhs.dash(i) != rhs.dash(i))
        return false;

    return true;
  }

  void print(std::ostream &os) const {
    if      (isNone  ()) os << "none";
    else if (isSolid ()) os << "solid";
    else {
      int n = dashes().size();

      for (int i = 0; i < n; ++i) {
        if (i > 0) os << " ";

        os << dash(i);
      }
    }
  }

  std::string toString() const {
    std::stringstream ss;

    ss << *this;

    return ss.str();
  }

 private:
  CSVGLineDash(Type type) :
   type_(type) {
  }

 private:
  Type   type_ { Type::NONE };
  Dashes dashes_;
};

#endif
