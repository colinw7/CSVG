#ifndef CSVGMissingGlyph_H
#define CSVGMissingGlyph_H

#include <CSVGGlyph.h>

class CSVGMissingGlyph : public CSVGGlyph {
 public:
  CSVG_OBJECT_DEF("missing-glyph", CSVGObjTypeId::MISSING_GLYPH)

  CSVGMissingGlyph(CSVG &svg);

  bool processOption(const std::string &name, const std::string &value) override;

  bool draw() override;

  void print(std::ostream &os, bool hier=false) const override;

  friend std::ostream &operator<<(std::ostream &os, const CSVGMissingGlyph &glyph);

 private:
  COptInt hax_;
};

#endif
