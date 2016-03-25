#ifndef CSVGDesc_H
#define CSVGDesc_H

#include <CSVGObject.h>

class CSVGDesc : public CSVGObject {
 public:
  CSVG_OBJECT_DEF("desc", CSVGObjTypeId::DESC)

  CSVGDesc(CSVG &svg);
  CSVGDesc(const CSVGDesc &desc);

  CSVGDesc *dup() const override;

  std::string getText() const override { return text_.getValue(""); }
  void setText(const std::string &text) override;

  bool processOption(const std::string &name, const std::string &value) override;

  bool isDrawable() const override { return false; }

  void draw() override;

  void print(std::ostream &os, bool hier) const override;

  friend std::ostream &operator<<(std::ostream &os, const CSVGDesc &desc);

 private:
  COptValT<std::string> text_;
};

#endif
