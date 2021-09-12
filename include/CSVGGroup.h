#ifndef CSVGGroup_H
#define CSVGGroup_H

#include <CSVGObject.h>

class CSVGGroup : public CSVGObject, public CSVGPrintBase<CSVGGroup> {
 public:
  CSVG_OBJECT_DEF("group", CSVGObjTypeId::GROUP)

  CSVGGroup(CSVG &svg);
  CSVGGroup(const CSVGGroup &group);

  CSVGGroup *dup() const override;

  std::string getTagName() const override { return "g"; }

  bool processOption(const std::string &name, const std::string &value) override;

  bool draw() override;

  void resizeTo(const CSize2D &size) override;

  void rotateBy(double da) override;

  //bool isDrawable() const override { return false; }

  void print(std::ostream &os, bool hier=false) const override;

  void printValues(std::ostream &os, bool flat=false) const override;

  void accept(CSVGVisitor *visitor) override { visitor->visit(this); }

 protected:
  void moveDelta(const CVector2D &delta) override;
};

#endif
