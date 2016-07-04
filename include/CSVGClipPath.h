#ifndef CSVGClipPath_H
#define CSVGClipPath_H

#include <CSVGObject.h>
#include <CSVGPathPart.h>
#include <CSVGTypes.h>

class CStrParse;
class CSVGPathPart;

class CSVGClipPath : public CSVGObject {
 public:
  CSVG_OBJECT_DEF("clipPath", CSVGObjTypeId::CLIP_PATH)

  CSVGClipPath(CSVG &svg);
  CSVGClipPath(const CSVGClipPath &path);

  CSVGClipPath *dup() const override;

  const CSVGPathPartList &getPartList() const { return parts_; }
  void setPartList(const CSVGPathPartList &parts) { parts_ = parts; }

  bool getUnitsValid() const { return clipPathUnits_.isValid(); }
  CSVGCoordUnits getUnits() const { return clipPathUnits_.getValue(CSVGCoordUnits::USER_SPACE); }
  void setUnits(CSVGCoordUnits units) { clipPathUnits_ = units; }

  bool processOption(const std::string &name, const std::string &value) override;

  bool canFlatten() const override { return false; }

  bool isDrawable() const override { return false; }

  void drawPath(CSVGObject* obj);

  void print(std::ostream &os, bool hier) const override;

  void printValues(std::ostream &os, bool flat=false) const override;

  friend std::ostream &operator<<(std::ostream &os, const CSVGClipPath &path);

 private:
  CSVGPathPartList         parts_;
  COptValT<CSVGCoordUnits> clipPathUnits_;
  COptString               markerEnd_;
  COptString               x_;
  COptString               y_;
  COptString               width_;
  COptString               height_;
};

#endif
