#ifndef CSVGFontFaceUri_H
#define CSVGFontFaceUri_H

#include <CSVGObject.h>
#include <CSVGXLink.h>

class CSVGFontFaceUri : public CSVGObject {
 public:
  CSVG_OBJECT_DEF("font-face-uri", CSVGObjTypeId::FONT_FACE_URI)

  CSVGFontFaceUri(CSVG &svg);
  CSVGFontFaceUri(const CSVGFontFaceUri &font_face);

  CSVGFontFaceUri *dup() const override;

  const CSVGXLink &xlink() const { return xlink_.getValue(); }

  std::string getLinkName() const { return (xlink_.isValid() ? xlink_.getValue().str() : ""); }
  void setLinkName(const std::string &str);

  bool processOption(const std::string &name, const std::string &value) override;

  bool draw() override;

  void print(std::ostream &os, bool hier) const override;

  void printValues(std::ostream &os, bool flat=false) const override;

  friend std::ostream &operator<<(std::ostream &os, const CSVGFontFaceUri &face);

 private:
  COptValT<CSVGXLink> xlink_;
};

#endif
