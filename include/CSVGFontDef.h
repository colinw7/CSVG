#include <CFont.h>

class CSVG;

class CSVGFontDef {
 private:
  CSVG                  &svg_;
  COptValT<std::string>  family_;
  COptValT<double>       size_;
  COptValT<CFontStyles>  style_;

 public:
  CSVGFontDef(CSVG &svg);
  CSVGFontDef(const CSVGFontDef &font_def);

  const CSVGFontDef &operator=(const CSVGFontDef &font_def);

  void reset();

  void setFamily(const std::string &family);
  void setSize  (double size);
  void setWeight(const std::string &weight_def);
  void setStyle (const std::string &style_def);

  bool hasFamily() const { return family_.isValid(); }
  bool hasSize  () const { return size_  .isValid(); }
  bool hasStyle () const { return style_ .isValid(); }

  std::string getFamily() const;
  double      getSize  () const;
  CFontStyles getStyle () const;
};
