#ifndef CSVGBlock_H
#define CSVGBlock_H

#include <CSVGObject.h>
#include <CSVGTypes.h>

class CSVGBlock : public CSVGObject {
 public:
  CSVG_OBJECT_DEF("block", CSVGObjTypeId::BLOCK)

  CSVGBlock(CSVG &svg);
  CSVGBlock(const CSVGBlock &block);

  CSVGBlock *dup() const override;

  double getX() const;
  void setX(double x) { x_ = x; }

  double getY() const;
  void setY(double y) { y_ = y; }

  double getWidth() const;
  void setWidth(double w) { width_ = w; }

  double getHeight() const;
  void setHeight(double h) { height_ = h; }

  CSVGPreserveAspect preserveAspect() const {
    return preserveAspect_.getValue(CSVGPreserveAspect()); }
  void setPreserveAspect(const CSVGPreserveAspect &a) { preserveAspect_ = a; }

  CHAlignType getHAlign() const { return preserveAspect().getHAlign(); }
  CVAlignType getVAlign() const { return preserveAspect().getVAlign(); }
  CSVGScale   getScale () const { return preserveAspect().getScale (); }

  void setHAlign(const CHAlignType &a) {
    CSVGPreserveAspect preserveAspect = this->preserveAspect();
    preserveAspect.setHAlign(a);
    preserveAspect_ = preserveAspect;
  }

  void setVAlign(const CVAlignType &a) {
    CSVGPreserveAspect preserveAspect = this->preserveAspect();
    preserveAspect.setVAlign(a);
    preserveAspect_ = preserveAspect;
  }

  void setScale(const CSVGScale &s) {
    CSVGPreserveAspect preserveAspect = this->preserveAspect();
    preserveAspect.setScale(s);
    preserveAspect_ = preserveAspect;
  }

  void setSize(const CSize2D &size);

  bool processOption(const std::string &name, const std::string &value) override;

  void drawInit() override;

  void draw() override;

  void drawTerm() override;

  bool getBBox(CBBox2D &bbox) const override;

  void print(std::ostream &os, bool hier) const override;

  friend std::ostream &operator<<(std::ostream &os, const CSVGBlock &block);

 private:
  COptValT<double>             x_;
  COptValT<double>             y_;
  COptValT<CScreenUnits>       width_;
  COptValT<CScreenUnits>       height_;
  COptValT<CSVGPreserveAspect> preserveAspect_;
  mutable CSVGBuffer*          oldBuffer_ { 0 };
  mutable double               xscale_ { 1 };
  mutable double               yscale_ { 1 };
};

#endif
