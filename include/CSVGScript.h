#ifndef CSVGScript_H
#define CSVGScript_H

#include <CSVGObject.h>
#include <CSVGXLink.h>

class CSVGScript : public CSVGObject {
 public:
  CSVG_OBJECT_DEF("script", CSVGObjTypeId::SCRIPT)

  CSVGScript(CSVG &svg);
  CSVGScript(const CSVGScript &use);

 ~CSVGScript();

  CSVGScript *dup() const override;

  bool processOption(const std::string &name, const std::string &value) override;

  bool draw() override;

  void print(std::ostream &os, bool hier) const override;

  friend std::ostream &operator<<(std::ostream &os, const CSVGScript &use);

 private:
  COptString          type_;
  COptValT<CSVGXLink> xlink_;
};

#endif
