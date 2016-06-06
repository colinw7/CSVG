#ifndef CSVGFontFaceSrc_H
#define CSVGFontFaceSrc_H

#include <CSVGObject.h>

class CSVGFontFaceSrc : public CSVGObject {
 public:
  CSVG_OBJECT_DEF("font-face-src", CSVGObjTypeId::FONT_FACE_SRC)

  CSVGFontFaceSrc(CSVG &svg);
  CSVGFontFaceSrc(const CSVGFontFaceSrc &font_face);

  CSVGFontFaceSrc *dup() const override;

  bool processOption(const std::string &name, const std::string &value) override;

  bool draw() override;

  void print(std::ostream &os, bool hier) const override;

  friend std::ostream &operator<<(std::ostream &os, const CSVGFontFaceSrc &face);
};

#endif
