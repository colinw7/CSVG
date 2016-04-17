#ifndef CSVGUse_H
#define CSVGUse_H

#include <CSVGObject.h>
#include <CSVGXLink.h>

class CSVGUse : public CSVGObject {
 public:
  CSVG_OBJECT_DEF("use", CSVGObjTypeId::USE)

  CSVGUse(CSVG &svg);
  CSVGUse(const CSVGUse &use);

 ~CSVGUse();

  CSVGUse *dup() const override;

  CSVGObject *getObject() const;

  bool processOption(const std::string &name, const std::string &value) override;

  void termParse();

  bool getBBox(CBBox2D &bbox) const override;

  void moveBy(const CVector2D &delta) override;

  void draw() override;

  void print(std::ostream &os, bool hier) const override;

  friend std::ostream &operator<<(std::ostream &os, const CSVGUse &use);

 private:
  COptValT<CSVGXLink> xlink_;
  COptValT<double>    x_;
  COptValT<double>    y_;
  COptValT<double>    width_;
  COptValT<double>    height_;
};

#endif
