#ifndef CSVGAnimation_H
#define CSVGAnimation_H

#include <CSVGTypes.h>
#include <CMatrixStack2D.h>
#include <list>

class CSVGObject;

class CSVGAnimation {
 public:
  using Objects = std::list<CSVGObject *>;

 public:
  CSVGAnimation(CSVGObject *parent=nullptr);

 ~CSVGAnimation();

  CSVGObject *parent() const { return parent_; }
  void setParent(CSVGObject *parent) { parent_ = parent; }

  const Objects &objects() const { return objects_; }

  CMatrixStack2D getTransform() const { return t_.value_or(CMatrixStack2D()); }

  void addObject(CSVGObject *obj);

  void removeObject(CSVGObject *obj);

  CSVGObject *prevObject(CSVGObject *obj) const;

  void tick(double dt);

  void setTime(double t);

  void handleEvent(CSVGEventType type, const std::string &id, const std::string &data);

 private:
  CSVGObject*                   parent_ { nullptr };
  Objects                       objects_;
  std::optional<CMatrixStack2D> t_;
};

#endif
