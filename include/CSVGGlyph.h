#ifndef CSVGGlyph_H
#define CSVGGlyph_H

#include <CSVGObject.h>

class CSVGPathPart;

class CSVGGlyph : public CSVGObject {
 public:
  typedef std::vector<CSVGPathPart *> PartList;

 public:
  CSVG_OBJECT_DEF("glyph", CSVGObjTypeId::GLYPH)

  CSVGGlyph(CSVG &svg);
  CSVGGlyph(const CSVGGlyph &glyph);

  virtual ~CSVGGlyph() { }

  CSVGGlyph *dup() const override;

  std::string getUnicode() const { return unicode_.getValue(""); }
  std::string getName   () const { return name_   .getValue(""); }
  int         getDx     () const { return dx_     .getValue(0 ); }

  virtual bool processOption(const std::string &name, const std::string &value) override;

  virtual void draw() override;

  void print(std::ostream &os, bool hier) const override;

  friend std::ostream &operator<<(std::ostream &os, const CSVGGlyph &glyph);

 protected:
  COptValT<std::string> unicode_;
  COptValT<std::string> name_;
  COptValT<int>         dx_;
  PartList              parts_;
};

#endif
