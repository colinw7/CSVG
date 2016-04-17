#ifndef CSVGFontFaceUri_H
#define CSVGFontFaceUri_H

#include <CSVGObject.h>
#include <CSVGXLink.h>

class CSVGFontFaceUri : public CSVGObject {
 public:
  CSVG_OBJECT_DEF("font-face-uri", CSVGObjTypeId::FONT_FACE_SRC)

  CSVGFontFaceUri(CSVG &svg);
  CSVGFontFaceUri(const CSVGFontFaceUri &font_face);

  CSVGFontFaceUri *dup() const override;

  bool processOption(const std::string &name, const std::string &value) override;

  void draw() override;

  void print(std::ostream &os, bool hier) const override;

  friend std::ostream &operator<<(std::ostream &os, const CSVGFontFaceUri &face);

 private:
  COptValT<CSVGXLink> xlink_;
};

#endif
