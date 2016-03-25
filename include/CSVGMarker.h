#ifndef CSVGMarker_H
#define CSVGMarker_H

#include <CSVGObject.h>

class CSVGMarker : public CSVGObject {
 public:
  CSVG_OBJECT_DEF("marker", CSVGObjTypeId::MARKER)

  CSVGMarker(CSVG &svg);
  CSVGMarker(const CSVGMarker &marker);

  CSVGMarker *dup() const override;

  bool processOption(const std::string &name, const std::string &value) override;

  void draw() override;

  void drawMarker(double x, double y, double angle);

  void print(std::ostream &os, bool hier) const override;

  friend std::ostream &operator<<(std::ostream &os, const CSVGMarker &marker);

 private:
  COptValT<double>      refX_;
  COptValT<double>      refY_;
  COptValT<std::string> markerUnits_;
  COptValT<double>      markerWidth_;
  COptValT<double>      markerHeight_;
  COptValT<std::string> orient_;
};

#endif
