#ifndef CSVGFeImage_H
#define CSVGFeImage_H

#include <CSVGFilterBase.h>
#include <CSVGXLink.h>

class CSVGBuffer;

class CSVGFeImage : public CSVGFilterBase, public CSVGPrintBase<CSVGFeImage> {
 public:
  CSVG_OBJECT_DEF("feImage", CSVGObjTypeId::FE_IMAGE)

  CSVGFeImage(CSVG &svg);
  CSVGFeImage(const CSVGFeImage &image);

  CSVGFeImage *dup() const override;

  std::string getFilterOut() const;
  void setFilterOut(const std::string &s) { filterOut_ = s; }

  CSVGPreserveAspect getPreserveAspect() const {
    return preserveAspect_.getValue(CSVGPreserveAspect()); }
  void setPreserveAspect(const CSVGPreserveAspect &a) { preserveAspect_ = a; }

  bool processOption(const std::string &name, const std::string &value) override;

  bool drawElement() override;

  void print(std::ostream &os, bool hier=false) const override;

  void printValues(std::ostream &os, bool flat=false) const override;

  void accept(CSVGVisitor *visitor) override { visitor->visit(this); }

 private:
  COptString                   filterOut_;
  COptValT<CSVGPreserveAspect> preserveAspect_;
};

#endif
