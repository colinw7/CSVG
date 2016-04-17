#ifndef CSVGClipPath_H
#define CSVGClipPath_H

#include <CSVGObject.h>
#include <CSVGTypes.h>

class CStrParse;
class CSVGPathPart;

class CSVGClipPath : public CSVGObject {
 public:
  typedef std::vector<CSVGPathPart *> PartList;

 public:
  CSVG_OBJECT_DEF("clipPath", CSVGObjTypeId::CLIP_PATH)

  CSVGClipPath(CSVG &svg);
  CSVGClipPath(const CSVGClipPath &path);

  CSVGClipPath *dup() const override;

  bool getUnitsValid() const { return clipPathUnits_.isValid(); }
  CSVGCoordUnits getUnits() const { return clipPathUnits_.getValue(CSVGCoordUnits::USER_SPACE); }
  void setUnits(CSVGCoordUnits units) { clipPathUnits_ = units; }

  bool processOption(const std::string &name, const std::string &value) override;

  bool isDrawable() const override { return false; }

  void drawPath(CSVGObject* obj);

  void print(std::ostream &os, bool hier) const override;

  friend std::ostream &operator<<(std::ostream &os, const CSVGClipPath &path);

 private:
  PartList                 parts_;
  COptValT<CSVGCoordUnits> clipPathUnits_;
  COptValT<std::string>    markerEnd_;
};

#endif
