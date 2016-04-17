#ifndef CSVGAnchor_H
#define CSVGAnchor_H

#include <CSVGObject.h>
#include <CSVGXLink.h>

class CSVGAnchor : public CSVGObject {
 public:
  CSVG_OBJECT_DEF("a", CSVGObjTypeId::ANCHOR)

  CSVGAnchor(CSVG &svg);
  CSVGAnchor(const CSVGAnchor &use);

 ~CSVGAnchor();

  CSVGAnchor *dup() const override;

  bool processOption(const std::string &name, const std::string &value) override;

  bool isDrawable() const override { return false; }

  void print(std::ostream &os, bool hier) const override;

  friend std::ostream &operator<<(std::ostream &os, const CSVGAnchor &use);

 private:
  COptValT<CSVGXLink> xlink_;
};

#endif
