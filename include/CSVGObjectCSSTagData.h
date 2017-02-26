#ifndef CSVGObjectCSSTagData_H
#define CSVGObjectCSSTagData_H

#include <CCSS.h>
#include <CSVGObject.h>

class CSVGObjectCSSTagData : public CCSSTagData {
 public:
  typedef std::vector<CSVGObject *> Children;

 public:
  explicit CSVGObjectCSSTagData(CSVGObject *obj) :
   obj_(obj) {
  }

  CSVGObject *obj() const { return obj_; }

  bool isElement(const std::string &name) const override {
    return (name == obj_->getTagName());
  }

  bool isClass(const std::string &name) const override {
    for (const auto &c : obj_->getClasses()) {
      if (name == c)
        return true;
    }

    return false;
  }

  bool isId(const std::string &name) const override {
    return (name == obj_->getId());
  }

  bool hasAttribute(const std::string & /*name*/, CCSSAttributeOp /*op*/,
                    const std::string & /*value*/) const override {
#if 0
    if      (op == CCSSAttributeOp::NONE)
      return obj_->hasOptionName(name);
    else if (op == CCSSAttributeOp::EQUAL)
      return obj_->hasOptionNameValue(name, value);
    else if (op == CCSSAttributeOp::PARTIAL)
      return obj_->hasOptionNameSubValue(name, value);
    else if (op == CCSSAttributeOp::STARTS_WITH)
      return obj_->hasOptionNameStart(name, value);
    else
      return false;
#else
    return false;
#endif
  }

  bool isNthChild(int n) const override {
    if (! obj_->getParent())
      return false;

    const CSVGObject::ObjectList &children = obj_->getParent()->children();

    int i = 1;

    for (const auto &child : children) {
      if (child == obj_)
        return (i == n);

      ++i;
    }

    return false;
  }

  bool isInputValue(const std::string &) const override {
    //TODO
    return false;
  }

  CCSSTagDataP getParent() const override {
    if (! obj_->getParent())
      return CCSSTagDataP();

    return CCSSTagDataP(new CSVGObjectCSSTagData(obj_->getParent()));
  }

  void getChildren(TagDataArray &children) const override {
    const CSVGObject::ObjectList &children1 = obj_->children();

    for (const auto &child : children1) {
      CCSSTagDataP dataP(new CSVGObjectCSSTagData(child));

      children.push_back(dataP);
    }
  }

  CCSSTagDataP getPrevSibling() const override {
    if (! obj_->getParent())
      return CCSSTagDataP();

    const CSVGObject::ObjectList &children = obj_->getParent()->children();

    CSVGObject *lastChild = nullptr;

    for (const auto &child : children) {
      if (child == obj_) {
        if (lastChild)
          return CCSSTagDataP(new CSVGObjectCSSTagData(lastChild));
      }

      lastChild = child;
    }

    return CCSSTagDataP();
  }

  CCSSTagDataP getNextSibling() const override {
    if (! obj_->getParent())
      return CCSSTagDataP();

    bool nextChild = false;

    const CSVGObject::ObjectList &children = obj_->getParent()->children();

    for (const auto &child : children) {
      if (nextChild)
        return CCSSTagDataP(new CSVGObjectCSSTagData(child));

      if (child == obj_)
        nextChild = true; // return next child
    }

    return CCSSTagDataP();
  }

 private:
  CSVGObject *obj_ { nullptr };
};

#endif
