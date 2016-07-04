#ifndef CSVGMPath_H
#define CSVGMPath_H

#include <CSVGObject.h>
#include <CSVGXLink.h>

class CStrParse;
class CSVGPathPart;

class CSVGMPath : public CSVGObject {
 public:
  CSVG_OBJECT_DEF("mpath", CSVGObjTypeId::MPATH)

  CSVGMPath(CSVG &svg);
  CSVGMPath(const CSVGMPath &path);

  CSVGMPath *dup() const override;

  bool processOption(const std::string &name, const std::string &value) override;

  const CSVGPathPartList &getPartList() const { return parts_; }
  void setPartList(const CSVGPathPartList &parts) { parts_ = parts; }

  const CSVGXLink &xlink() const { return xlink_.getValue(); }

  std::string getLinkName() const { return (xlink_.isValid() ? xlink_.getValue().str() : ""); }
  void setLinkName(const std::string &str);

  bool draw() override;

  bool getBBox(CBBox2D &bbox) const override;

  void print(std::ostream &os, bool hier) const override;

  void printValues(std::ostream &os, bool flat=false) const override;

  friend std::ostream &operator<<(std::ostream &os, const CSVGMPath &path);

 protected:
  CSVGPathPartList    parts_;
  COptValT<CSVGXLink> xlink_;
};

#endif
