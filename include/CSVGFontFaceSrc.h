#ifndef CSVGFontFaceSrc_H
#define CSVGFontFaceSrc_H

#include <CSVGObject.h>

class CSVGFontFaceSrc : public CSVGObject, public CSVGPrintBase<CSVGFontFaceSrc> {
 public:
  CSVG_OBJECT_DEF("font-face-src", CSVGObjTypeId::FONT_FACE_SRC)

  CSVGFontFaceSrc(CSVG &svg);
  CSVGFontFaceSrc(const CSVGFontFaceSrc &font_face);

  CSVGFontFaceSrc *dup() const override;

  bool processOption(const std::string &name, const std::string &value) override;

  bool draw() override;

  void print(std::ostream &os, bool hier=false) const override;

  void printValues(std::ostream &os, bool flat=false) const override;

  void accept(CSVGVisitor *visitor) override { visitor->visit(this); }
};

#endif
