#ifndef CSVGTitle_H
#define CSVGTitle_H

#include <CSVGObject.h>

class CSVGTitle : public CSVGObject {
 public:
  CSVG_OBJECT_DEF("title", CSVGObjTypeId::TITLE)

  CSVGTitle(CSVG &svg);
  CSVGTitle(const CSVGTitle &title);

  CSVGTitle *dup() const override;

  std::string getText() const override { return text_; }
  void setText(const std::string &text) override;

  bool processOption(const std::string &name, const std::string &value) override;

  void draw() override;

 private:
  std::string text_;
};

#endif
