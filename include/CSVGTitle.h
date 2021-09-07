#ifndef CSVGTitle_H
#define CSVGTitle_H

#include <CSVGObject.h>

class CSVGTitle : public CSVGObject, public CSVGPrintBase<CSVGTitle> {
 public:
  CSVG_OBJECT_DEF("title", CSVGObjTypeId::TITLE)

  CSVGTitle(CSVG &svg);
  CSVGTitle(const CSVGTitle &title);

  CSVGTitle *dup() const override;

  bool getBBox(CBBox2D &) const override { return false; }

  bool processOption(const std::string &name, const std::string &value) override;

  bool isDrawable() const override { return false; }

  bool isToolTip() const { return tooltip_; }

  void accept(CSVGVisitor *visitor) override { visitor->visit(this); }

 private:
  bool tooltip_ { false };
};

#endif
