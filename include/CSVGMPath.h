#ifndef CSVGMPath_H
#define CSVGMPath_H

#include <CSVGObject.h>
#include <CSVGXLink.h>

class CStrParse;
class CSVGPathPart;

class CSVGMPath : public CSVGObject, public CSVGPrintBase<CSVGMPath> {
 public:
  CSVG_OBJECT_DEF("mpath", CSVGObjTypeId::MPATH)

  CSVGMPath(CSVG &svg);
  CSVGMPath(const CSVGMPath &path);

  CSVGMPath *dup() const override;

  bool processOption(const std::string &name, const std::string &value) override;

  const CSVGPathPartList &getPartList() const override { return parts_; }
  void setPartList(const CSVGPathPartList &parts) { parts_ = parts; }

  const CSVGXLink &xlink() const { return xlink_.value(); }

  std::string getLinkName() const { return (xlink_ ? xlink_.value().str() : ""); }
  void setLinkName(const std::string &str);

  bool draw() override;

  bool getBBox(CBBox2D &bbox) const override;

  void print(std::ostream &os, bool hier=false) const override;

  void printValues(std::ostream &os, bool flat=false) const override;

  void accept(CSVGVisitor *visitor) override { visitor->visit(this); }

 protected:
  CSVGPathPartList         parts_;
  std::optional<CSVGXLink> xlink_;
};

#endif
