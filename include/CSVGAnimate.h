#ifndef CSVGAnimate_H
#define CSVGAnimate_H

#include <CSVGAnimateBase.h>

class CSVGAnimate : public CSVGAnimateBase, public CSVGPrintBase<CSVGAnimate> {
 public:
  CSVG_OBJECT_DEF("animate", CSVGObjTypeId::ANIMATE)

  CSVGAnimate(CSVG &svg);
  CSVGAnimate(const CSVGAnimate &animate);

  CSVGAnimate *dup() const override;

  bool processOption(const std::string &name, const std::string &value) override;

  void animate(double t) override;

  void print(std::ostream &os, bool hier=false) const override;

  void accept(CSVGVisitor *visitor) override { visitor->visit(this); }
};

#endif
