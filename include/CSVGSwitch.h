#ifndef CSVGSwitch_H
#define CSVGSwitch_H

#include <CSVGObject.h>
#include <CSVGXLink.h>

class CSVGSwitch : public CSVGObject, public CSVGPrintBase<CSVGSwitch> {
 public:
  CSVG_OBJECT_DEF("switch", CSVGObjTypeId::SWITCH)

  CSVGSwitch(CSVG &svg);
  CSVGSwitch(const CSVGSwitch &use);

 ~CSVGSwitch();

  CSVGSwitch *dup() const override;

  bool processOption(const std::string &name, const std::string &value) override;

  bool draw() override;

  void print(std::ostream &os, bool hier=false) const override;

  void printValues(std::ostream &os, bool flat=false) const override;

  void accept(CSVGVisitor *visitor) override { visitor->visit(this); }
};

#endif
