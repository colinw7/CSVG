#ifndef CSVGFeMergeNode_H
#define CSVGFeMergeNode_H

#include <CSVGObject.h>

class CSVGFeMergeNode : public CSVGObject {
 public:
  CSVG_OBJECT_DEF("feMergeNode", CSVGObjTypeId::FE_MERGE_NODE)

  CSVGFeMergeNode(CSVG &svg);
  CSVGFeMergeNode(const CSVGFeMergeNode &merge);

  CSVGFeMergeNode *dup() const override;

  std::string getFilterIn () const { return filter_in_ .getValue("SourceGraphic"); }
  std::string getFilterOut() const { return filter_out_.getValue("SourceGraphic"); }

  bool processOption(const std::string &name, const std::string &value) override;

  bool isDrawable() const override { return false; }

  void draw() override;

  void print(std::ostream &os, bool hier) const override;

  friend std::ostream &operator<<(std::ostream &os, const CSVGFeMergeNode &filter);

 private:
  COptValT<std::string> filter_in_;
  COptValT<std::string> filter_out_;
};

#endif
