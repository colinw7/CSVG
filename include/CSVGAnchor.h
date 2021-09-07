#ifndef CSVGAnchor_H
#define CSVGAnchor_H

#include <CSVGObject.h>
#include <CSVGXLink.h>

class CSVGAnchor : public CSVGObject, public CSVGPrintBase<CSVGAnchor> {
 public:
  CSVG_OBJECT_DEF("a", CSVGObjTypeId::ANCHOR)

  CSVGAnchor(CSVG &svg);
  CSVGAnchor(const CSVGAnchor &use);

 ~CSVGAnchor();

  CSVGAnchor *dup() const override;

  const CSVGXLink &xlink() const { return xlink_.getValue(); }

  std::string getLinkName() const { return (xlink_.isValid() ? xlink_.getValue().str() : ""); }
  void setLinkName(const std::string &str);

  bool processOption(const std::string &name, const std::string &value) override;

  bool draw() override;

  void print(std::ostream &os, bool hier=false) const override;

  void printValues(std::ostream &os, bool flat=false) const override;

  void accept(CSVGVisitor *visitor) override { visitor->visit(this); }

 private:
  COptValT<CSVGXLink> xlink_;
  std::string         title_;
};

#endif
