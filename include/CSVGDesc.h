#ifndef CSVGDesc_H
#define CSVGDesc_H

#include <CSVGObject.h>

class CSVGDesc : public CSVGObject, public CSVGPrintBase<CSVGDesc> {
 public:
  CSVG_OBJECT_DEF("desc", CSVGObjTypeId::DESC)

  CSVGDesc(CSVG &svg);
  CSVGDesc(const CSVGDesc &desc);

  CSVGDesc *dup() const override;

  bool processOption(const std::string &name, const std::string &value) override;

  bool isDrawable() const override { return false; }

  bool isToolTip() const { return tooltip_; }

  bool draw() override;

  void print(std::ostream &os, bool hier=false) const override;

  void printValues(std::ostream &os, bool flat=false) const override;

  void accept(CSVGVisitor *visitor) override { visitor->visit(this); }

 private:
  bool tooltip_ { false };
};

#endif
