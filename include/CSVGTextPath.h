#ifndef CSVGTextPath_H
#define CSVGTextPath_H

#include <CSVGObject.h>
#include <CSVGXLink.h>

class CSVGText;

class CSVGTextPath : public CSVGObject, public CSVGPrintBase<CSVGTextPath> {
 public:
  CSVG_OBJECT_DEF("textPath", CSVGObjTypeId::TEXT_PATH)

  CSVGTextPath(CSVG &svg);
  CSVGTextPath(const CSVGTextPath &text);

  CSVGTextPath *dup() const override;

  CScreenUnits getStartOffset() const { return startOffset_.getValue(CScreenUnits(0)); }
  void setStartOffset(const CScreenUnits &l) { startOffset_ = l; }

  const CSVGXLink &xlink() const { return xlink_.getValue(); }

  std::string getLinkName() const { return (xlink_.isValid() ? xlink_.getValue().str() : ""); }
  void setLinkName(const std::string &str);

  bool processOption(const std::string &name, const std::string &value) override;

  bool draw() override;

  void print(std::ostream &os, bool hier=false) const override;

  void printValues(std::ostream &os, bool flat=false) const override;

  void accept(CSVGVisitor *visitor) override { visitor->visit(this); }

 private:
  CSVGText *getParentText() const;

 private:
  COptValT<CScreenUnits> startOffset_;
  COptValT<CSVGXLink>    xlink_;
};

#endif
