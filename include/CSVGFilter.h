#ifndef CSVG_FILTER_H
#define CSVG_FILTER_H

#include <CSVGObject.h>

class CSVGFilter : public CSVGObject {
 public:
  CSVG_OBJECT_DEF("filter", CSVGObjTypeId::FILTER)

  CSVGFilter(CSVG &svg);
  CSVGFilter(const CSVGFilter &filter);

  CSVGFilter *dup() const override;

  bool processOption(const std::string &name, const std::string &value) override;

  void setObject(CSVGObject *object) { object_ = object; }

  CSVGObject *getObject() const { return object_; }

  void draw() override;

  virtual CImagePtr filterImage(CImagePtr src_image);

  void print(std::ostream &os, bool hier) const override;

  friend std::ostream &operator<<(std::ostream &os, const CSVGFilter &filter);

 private:
  CSVGFilter &operator=(const CSVGFilter &rhs);

 private:
  CSVGObject*           object_ { 0 };
  COptValT<std::string> units_;
  COptValT<std::string> primitiveUnits_;
  COptValT<std::string> x_;
  COptValT<std::string> y_;
  COptValT<std::string> width_;
  COptValT<std::string> height_;
  COptValT<std::string> filterRes_;
  COptValT<std::string> colorInterFilters_;
  COptValT<std::string> xlink_;
};

#endif
