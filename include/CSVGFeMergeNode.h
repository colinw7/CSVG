#ifndef CSVGFeMergeNode_H
#define CSVGFeMergeNode_H

#include <CSVGObject.h>

class CSVGFeMergeNode : public CSVGObject {
 public:
  CSVG_OBJECT_DEF("feMergeNode", CSVGObjTypeId::FE_MERGE_NODE)

  CSVGFeMergeNode(CSVG &svg);
  CSVGFeMergeNode(const CSVGFeMergeNode &merge);

  CSVGFeMergeNode *dup() const override;

  std::string getFilterIn() const;
  void setFilterIn(const std::string &s) { filterIn_ = s; }

  std::string getFilterOut() const;
  void setFilterOut(const std::string &s) { filterOut_ = s; }

  bool processOption(const std::string &name, const std::string &value) override;

  bool isDrawable() const override { return false; }

  CSVGFilter *getParentFilter() const;

  void print(std::ostream &os, bool hier) const override;

  friend std::ostream &operator<<(std::ostream &os, const CSVGFeMergeNode &filter);

 private:
  COptString filterIn_;
  COptString filterOut_;
};

#endif
