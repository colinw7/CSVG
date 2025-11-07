#ifndef CSVGClipPath_H
#define CSVGClipPath_H

#include <CSVGObject.h>
#include <CSVGPathPart.h>
#include <CSVGTypes.h>

class CStrParse;
class CSVGPathPart;

class CSVGClipPath : public CSVGObject, public CSVGPrintBase<CSVGClipPath> {
 public:
  CSVG_OBJECT_DEF("clipPath", CSVGObjTypeId::CLIP_PATH)

  CSVGClipPath(CSVG &svg);
  CSVGClipPath(const CSVGClipPath &path);

  CSVGClipPath *dup() const override;

  const CSVGPathPartList &getPartList() const override { return parts_; }
  void setPartList(const CSVGPathPartList &parts) { parts_ = parts; }

  bool getUnitsValid() const { return !!clipPathUnits_; }
  CSVGCoordUnits getUnits() const { return clipPathUnits_.value_or(CSVGCoordUnits::USER_SPACE); }
  void setUnits(CSVGCoordUnits units) { clipPathUnits_ = units; }

  bool processOption(const std::string &name, const std::string &value) override;

  bool canFlatten() const override { return false; }

  bool isDrawable() const override { return false; }

  void drawPath(CSVGObject* obj);

  void print(std::ostream &os, bool hier=false) const override;

  void printValues(std::ostream &os, bool flat=false) const override;

  void accept(CSVGVisitor *visitor) override { visitor->visit(this); }

 private:
  CSVGPathPartList              parts_;
  std::optional<CSVGCoordUnits> clipPathUnits_;
  std::optional<std::string>    markerEnd_;
  std::optional<std::string>    x_;
  std::optional<std::string>    y_;
  std::optional<std::string>    width_;
  std::optional<std::string>    height_;
};

#endif
