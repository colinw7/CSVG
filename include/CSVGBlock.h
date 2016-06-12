#ifndef CSVGBlock_H
#define CSVGBlock_H

#include <CSVGObject.h>
#include <CSVGTypes.h>
#include <CSVGBlockData.h>

class CSVGBlock : public CSVGObject {
 public:
  CSVG_OBJECT_DEF("block", CSVGObjTypeId::BLOCK)

  CSVGBlock(CSVG &svg);
  CSVGBlock(const CSVGBlock &block);

  CSVGBlock *dup() const override;

  //---

  // pixel bbox
  double getX() const;
  void setX(double x) { x_ = x; }

  double getY() const;
  void setY(double y) { y_ = y; }

  double getWidth() const;
  void setWidth(double w) { width_ = w; }

  double getHeight() const;
  void setHeight(double h) { height_ = h; }

  // preserve aspect
  bool hasPreserveAspect() const { return preserveAspect_.isValid(); }
  CSVGPreserveAspect preserveAspect(const CSVGPreserveAspect &a=CSVGPreserveAspect()) const {
    return preserveAspect_.getValue(a); }
  void setPreserveAspect(const CSVGPreserveAspect &a) { preserveAspect_ = a; }

  CHAlignType getHAlign() const { return preserveAspect().getHAlign(); }
  void setHAlign(const CHAlignType &a) {
    CSVGPreserveAspect preserveAspect = this->preserveAspect();
    preserveAspect.setHAlign(a);
    preserveAspect_ = preserveAspect;
  }

  CVAlignType getVAlign() const { return preserveAspect().getVAlign(); }
  void setVAlign(const CVAlignType &a) {
    CSVGPreserveAspect preserveAspect = this->preserveAspect();
    preserveAspect.setVAlign(a);
    preserveAspect_ = preserveAspect;
  }

  CSVGScale getScale () const { return preserveAspect().getScale (); }
  void setScale(const CSVGScale &s) {
    CSVGPreserveAspect preserveAspect = this->preserveAspect();
    preserveAspect.setScale(s);
    preserveAspect_ = preserveAspect;
  }

  // pixel and view box
  CBBox2D calcPixelBox() const;
  CBBox2D calcViewBox() const;

  //---

  bool getBBox(CBBox2D &bbox) const override;

  void setSize(const CSize2D &size);

  bool processOption(const std::string &name, const std::string &value) override;

  void drawInit() override;

  bool draw() override;

  void drawTerm() override;

  void print(std::ostream &os, bool hier) const override;

  friend std::ostream &operator<<(std::ostream &os, const CSVGBlock &block);

 private:
  COptReal                     x_;
  COptReal                     y_;
  COptValT<CScreenUnits>       width_;
  COptValT<CScreenUnits>       height_;
  COptValT<CSVGPreserveAspect> preserveAspect_;
  mutable CSVGBuffer*          oldBuffer_ { 0 };
  mutable CSVGBlockData        blockData_;
};

#endif
