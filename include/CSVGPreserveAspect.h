#ifndef CSVGPreserveAspect_H
#define CSVGPreserveAspect_H

class CSVGPreserveAspect {
 public:
  CSVGPreserveAspect(const CHAlignType &halign=CHALIGN_TYPE_CENTER,
                     const CVAlignType &valign=CVALIGN_TYPE_CENTER,
                     const CSVGScale &scale=CSVGScale::FREE) :
   halign_(halign), valign_(valign), scale_(scale) {
  }

  const CHAlignType &getHAlign() const { return halign_; }
  void setHAlign(const CHAlignType &a) { halign_ = a; }

  const CVAlignType &getVAlign() const { return valign_; }
  void setVAlign(const CVAlignType &a) { valign_ = a; }

  const CSVGScale &getScale () const { return scale_ ; }
  void setScale(const CSVGScale &s) { scale_ = s; }

 public:
  CHAlignType halign_;
  CVAlignType valign_;
  CSVGScale   scale_;
};

#endif
