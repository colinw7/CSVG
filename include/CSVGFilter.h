#ifndef CSVG_FILTER_H
#define CSVG_FILTER_H

class CSVGFilter : public CSVGObject {
 private:
  CSVGObject *object_;

 public:
  CSVG_OBJECT_DEF("filter", CSVG_OBJ_TYPE_FILTER)

  CSVGFilter(CSVG &svg);
  CSVGFilter(const CSVGFilter &filter);

  CSVGFilter *dup() const;

  bool processOption(const std::string &name, const std::string &value);

  void setObject(CSVGObject *object) { object_ = object; }

  CSVGObject *getObject() const { return object_; }

  void draw();

  virtual CImagePtr filterImage(CImagePtr src_image);

  void print(std::ostream &os) const;

  friend std::ostream &operator<<(std::ostream &os, const CSVGFilter &filter);

 private:
  CSVGFilter &operator=(const CSVGFilter &rhs);
};

#endif
