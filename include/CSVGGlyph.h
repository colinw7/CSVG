#ifndef CSVGGlyph_H
#define CSVGGlyph_H

#include <CSVGObject.h>

class CSVGPathPart;

class CSVGGlyph : public CSVGObject {
 public:
  CSVG_OBJECT_DEF("glyph", CSVGObjTypeId::GLYPH)

  CSVGGlyph(CSVG &svg);
  CSVGGlyph(const CSVGGlyph &glyph);

  virtual ~CSVGGlyph() { }

  CSVGGlyph *dup() const override;

  std::string getUnicode() const { return unicode_  .getValue(""); }
  std::string getName   () const { return glyphName_.getValue(""); }
  int         getDx     () const { return horizAdvX_.getValue(0 ); }

  const CSVGPathPartList &getPartList() const { return parts_; }
  void setPartList(const CSVGPathPartList &parts) { parts_ = parts; }

  virtual bool processOption(const std::string &name, const std::string &value) override;

  virtual void draw() override;

  void print(std::ostream &os, bool hier) const override;

  friend std::ostream &operator<<(std::ostream &os, const CSVGGlyph &glyph);

 protected:
  COptValT<std::string> unicode_;
  COptValT<std::string> glyphName_;
  CSVGPathPartList      parts_;
  COptValT<std::string> orientation_;
  COptValT<std::string> arabicForm_;
  COptValT<std::string> lang_;
  COptValT<int>         horizAdvX_;
  COptValT<int>         vertAdvY_;
  COptValT<int>         vertOriginX_;
  COptValT<int>         vertOriginY_;
};

#endif
