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

  CHAlignType getHAlign() const {
    return preserveAspect_.getValue(CSVGPreserveAspect()).getHAlign();
  }
  void setHAlign(const CHAlignType &a) {
    CSVGPreserveAspect preserveAspect = preserveAspect_.getValue(CSVGPreserveAspect());
    preserveAspect.setHAlign(a);
    preserveAspect_ = preserveAspect;
  }

  CVAlignType getVAlign() const {
    return preserveAspect_.getValue(CSVGPreserveAspect()).getVAlign();
  }
  void setVAlign(const CVAlignType &a) {
    CSVGPreserveAspect preserveAspect = preserveAspect_.getValue(CSVGPreserveAspect());
    preserveAspect.setVAlign(a);
    preserveAspect_ = preserveAspect;
  }

  CSVGScale getScale() const {
    return preserveAspect_.getValue(CSVGPreserveAspect()).getScale();
  }
  void setScale(const CSVGScale &s) {
    CSVGPreserveAspect preserveAspect = preserveAspect_.getValue(CSVGPreserveAspect());
    preserveAspect.setScale(s);
    preserveAspect_ = preserveAspect;
  }

  void setSize(const CSize2D &size);

  bool processOption(const std::string &name, const std::string &value) override;

  void draw() override;

  bool getBBox(CBBox2D &bbox) const override;

  void print(std::ostream &os, bool hier) const override;

  friend std::ostream &operator<<(std::ostream &os, const CSVGBlock &block);

 private:
  COptValT<double>             x_;
  COptValT<double>             y_;
  COptValT<CSVGLengthValue>    width_;
  COptValT<CSVGLengthValue>    height_;
  COptValT<CSVGPreserveAspect> preserveAspect_;
};

#endif
