#ifndef CSVGStyle_H
#define CSVGStyle_H

#include <CSVGObject.h>

class CSVGStyleData;

class CSVGStyle : public CSVGObject, public CSVGPrintBase<CSVGStyle> {
 public:
  CSVG_OBJECT_DEF("style", CSVGObjTypeId::STYLE)

  CSVGStyle(CSVG &svg);
  CSVGStyle(const CSVGStyle &style);

  CSVGStyle *dup() const override;

  bool processOption(const std::string &name, const std::string &value) override;

  void setText(const std::string &text) override;

  bool draw() override;

  void print(std::ostream &os, bool hier=false) const override;

  void printValues(std::ostream &os, bool flat=false) const override;

  void accept(CSVGVisitor *visitor) override { visitor->visit(this); }

 private:
  bool parseCSS(const std::string &str);

 private:
  CSVGStyleType type_;
};

#endif
