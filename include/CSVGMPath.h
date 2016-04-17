#ifndef CSVGMPath_H
#define CSVGMPath_H

#include <CSVGObject.h>
#include <CSVGXLink.h>

class CStrParse;
class CSVGPathPart;

class CSVGMPath : public CSVGObject {
 public:
  typedef std::vector<CSVGPathPart *> PartList;

 public:
  CSVG_OBJECT_DEF("mpath", CSVGObjTypeId::MPATH)

  CSVGMPath(CSVG &svg);
  CSVGMPath(const CSVGMPath &path);

  CSVGMPath *dup() const override;

  bool processOption(const std::string &name, const std::string &value) override;

  const PartList &getPartList() const { return parts_; }

  const CSVGXLink &xlink() const { return xlink_.getValue(); }

  void draw() override;

  bool getBBox(CBBox2D &bbox) const override;

  void print(std::ostream &os, bool hier) const override;

  friend std::ostream &operator<<(std::ostream &os, const CSVGMPath &path);

 protected:
  PartList            parts_;
  COptValT<CSVGXLink> xlink_;
};

#endif
