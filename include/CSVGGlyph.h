#ifndef CSVGGlyph_H
#define CSVGGlyph_H

#include <CSVGObject.h>

class CSVGPathPart;

class CSVGGlyph : public CSVGObject, public CSVGPrintBase<CSVGGlyph> {
 public:
  CSVG_OBJECT_DEF("glyph", CSVGObjTypeId::GLYPH)

  CSVGGlyph(CSVG &svg);
  CSVGGlyph(const CSVGGlyph &glyph);

  virtual ~CSVGGlyph() { }

  CSVGGlyph *dup() const override;

  std::string getUnicode() const { return unicode_  .value_or(""); }
  std::string getName   () const { return glyphName_.value_or(""); }
  int         getDx     () const { return horizAdvX_.value_or(0 ); }

  const CSVGPathPartList &getPartList() const override { return parts_; }
  void setPartList(const CSVGPathPartList &parts) { parts_ = parts; }

  virtual bool processOption(const std::string &name, const std::string &value) override;

  virtual bool draw() override;

  void print(std::ostream &os, bool hier=false) const override;

  void printValues(std::ostream &os, bool flat=false) const override;

  void accept(CSVGVisitor *visitor) override { visitor->visit(this); }

 protected:
  std::optional<std::string> unicode_;
  std::optional<std::string> glyphName_;
  CSVGPathPartList           parts_;
  std::optional<std::string> orientation_;
  std::optional<std::string> arabicForm_;
  std::optional<std::string> lang_;
  std::optional<int>         horizAdvX_;
  std::optional<int>         vertAdvY_;
  std::optional<int>         vertOriginX_;
  std::optional<int>         vertOriginY_;
};

#endif
