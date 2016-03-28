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

  CHAlignType getHAlign() const { return halign_.getValue(CHALIGN_TYPE_CENTER); }
  void setHAlign(const CHAlignType &a) { halign_ = a; }

  CVAlignType getVAlign() const { return valign_.getValue(CVALIGN_TYPE_CENTER); }
  void setVAlign(const CVAlignType &a) { valign_ = a; }

  CSVGScale getScale() const { return scale_.getValue(CSVGScale::FREE); }
  void setScale(const CSVGScale &s) { scale_ = s; }

  void setSize(const CSize2D &size);

  bool processOption(const std::string &name, const std::string &value) override;

  void draw() override;

  bool getBBox(CBBox2D &bbox) const override;

  void print(std::ostream &os, bool hier) const override;

  friend std::ostream &operator<<(std::ostream &os, const CSVGBlock &block);

 private:
  COptValT<double>          x_;
  COptValT<double>          y_;
  COptValT<CSVGLengthValue> width_;
  COptValT<CSVGLengthValue> height_;
  COptValT<CHAlignType>     halign_;
  COptValT<CVAlignType>     valign_;
  COptValT<CSVGScale>       scale_;
};

#endif
