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

  virtual bool draw() override;

  void print(std::ostream &os, bool hier) const override;

  friend std::ostream &operator<<(std::ostream &os, const CSVGGlyph &glyph);

 protected:
  COptString       unicode_;
  COptString       glyphName_;
  CSVGPathPartList parts_;
  COptString       orientation_;
  COptString       arabicForm_;
  COptString       lang_;
  COptInt          horizAdvX_;
  COptInt          vertAdvY_;
  COptInt          vertOriginX_;
  COptInt          vertOriginY_;
};

#endif
