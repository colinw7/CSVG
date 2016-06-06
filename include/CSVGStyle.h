#ifndef CSVGStyle_H
#define CSVGStyle_H

#include <CSVGObject.h>

class CSVGStyleData;

class CSVGStyle : public CSVGObject {
 public:
  CSVG_OBJECT_DEF("style", CSVGObjTypeId::STYLE)

  CSVGStyle(CSVG &svg);
  CSVGStyle(const CSVGStyle &style);

  CSVGStyle *dup() const override;

  bool processOption(const std::string &name, const std::string &value) override;

  void setText(const std::string &text) override;

  bool draw() override;

  void print(std::ostream &os, bool hier) const override;

  friend std::ostream &operator<<(std::ostream &os, const CSVGStyle &style);

 private:
  bool parseCSS(const std::string &str);

 private:
  CSVGStyleType type_;
};

#endif
