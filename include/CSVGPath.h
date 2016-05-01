#ifndef CSVGPath_H
#define CSVGPath_H

#include <CSVGObject.h>

class CStrParse;
class CSVGPathPart;

class CSVGPath : public CSVGObject {
 public:
  CSVG_OBJECT_DEF("path", CSVGObjTypeId::PATH)

  CSVGPath(CSVG &svg);
  CSVGPath(const CSVGPath &path);

  CSVGPath *dup() const override;

  bool processOption(const std::string &name, const std::string &value) override;

  const CSVGPathPartList &getPartList() const { return parts_; }
  void setPartList(const CSVGPathPartList &parts) { parts_ = parts; }

  void moveBy(const CVector2D &d);

  void draw() override;

  bool getBBox(CBBox2D &bbox) const override;

  void print(std::ostream &os, bool hier) const override;

  friend std::ostream &operator<<(std::ostream &os, const CSVGPath &path);

 protected:
  CSVGPathPartList parts_;
};

#endif
