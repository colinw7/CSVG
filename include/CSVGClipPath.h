#ifndef CSVGClipPath_H
#define CSVGClipPath_H

#include <CSVGObject.h>

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

  bool processOption(const std::string &name, const std::string &value) override;

  void draw() override;

  void print(std::ostream &os, bool hier) const override;

  friend std::ostream &operator<<(std::ostream &os, const CSVGClipPath &path);

 private:
  PartList              parts_;
  COptValT<std::string> clipPathUnits_;
  COptValT<std::string> markerEnd_;
};

#endif
