#ifndef CSVGSwitch_H
#define CSVGSwitch_H

#include <CSVGObject.h>
#include <CSVGXLink.h>

class CSVGSwitch : public CSVGObject {
 public:
  CSVG_OBJECT_DEF("switch", CSVGObjTypeId::SWITCH)

  CSVGSwitch(CSVG &svg);
  CSVGSwitch(const CSVGSwitch &use);

 ~CSVGSwitch();

  CSVGSwitch *dup() const override;

  bool processOption(const std::string &name, const std::string &value) override;

  bool draw() override;

  void print(std::ostream &os, bool hier) const override;

  void printValues(std::ostream &os, bool flat=false) const override;

  friend std::ostream &operator<<(std::ostream &os, const CSVGSwitch &swtch);
};

#endif
