#ifndef CSVGPath_H
#define CSVGPath_H

#include <CSVGObject.h>

class CStrParse;
class CSVGPathPart;

class CSVGPath : public CSVGObject {
 public:
  typedef std::vector<CSVGPathPart *> PartList;

 public:
  CSVG_OBJECT_DEF("path", CSVGObjTypeId::PATH)

  CSVGPath(CSVG &svg);
  CSVGPath(const CSVGPath &path);

  CSVGPath *dup() const override;

  bool processOption(const std::string &name, const std::string &value) override;

  const PartList &getPartList() const { return parts_; }

  void draw() override;

  bool getBBox(CBBox2D &bbox) const override;

  void print(std::ostream &os, bool hier) const override;

  friend std::ostream &operator<<(std::ostream &os, const CSVGPath &path);

 protected:
  PartList parts_;
};

#endif
