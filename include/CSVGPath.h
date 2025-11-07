#ifndef CSVGPath_H
#define CSVGPath_H

#include <CSVGObject.h>

class CStrParse;
class CSVGPathPart;

class CSVGPath : public CSVGObject, public CSVGPrintBase<CSVGPath> {
 public:
  CSVG_OBJECT_DEF("path", CSVGObjTypeId::PATH)

  CSVGPath(CSVG &svg);
  CSVGPath(const CSVGPath &path);

  CSVGPath *dup() const override;

  bool processOption(const std::string &name, const std::string &value) override;

  const CSVGPathPartList &getPartList() const override { return parts_; }

  void setPartList(const CSVGPathPartList &parts) { parts_ = parts; }

  bool hasPathLength() const { return !!pathLength_; }
  double getPathLength() const { return pathLength_.value_or(0); }
  void setPathLength(double r) { pathLength_ = r; }

  bool draw() override;

  bool getBBox(CBBox2D &bbox) const override;

  void print(std::ostream &os, bool hier=false) const override;

  void printValues(std::ostream &os, bool flat=false) const override;

  void accept(CSVGVisitor *visitor) override { visitor->visit(this); }

 protected:
  bool drawZeroLength();

  void moveDelta(const CVector2D &d) override;

 protected:
  CSVGPathPartList      parts_;
  std::optional<double> pathLength_;
};

#endif
