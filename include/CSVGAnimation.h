#ifndef CSVGAnimation_H
#define CSVGAnimation_H

#include <CSVGTypes.h>
#include <CMatrix2D.h>
#include <COptVal.h>
#include <list>

class CSVGObject;

class CSVGAnimation {
 public:
  typedef std::list<CSVGObject *> Objects;

 public:
  CSVGAnimation(CSVGObject *parent=0);

 ~CSVGAnimation();

  CSVGObject *parent() const { return parent_; }
  void setParent(CSVGObject *parent) { parent_ = parent; }

  const Objects &objects() const { return objects_; }

  const CMatrix2D &getTransform() const { return t_.getValue(CMatrix2D()); }

  void addObject(CSVGObject *obj);

  void removeObject(CSVGObject *obj);

  CSVGObject *prevObject(CSVGObject *obj) const;

  void tick(double dt);

  void handleEvent(CSVGEventType type, const std::string &id, const std::string &data);

 private:
  CSVGObject          *parent_ { 0 };
  Objects              objects_;
  COptValT<CMatrix2D>  t_;
};

#endif
