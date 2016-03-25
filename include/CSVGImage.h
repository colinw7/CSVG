#ifndef CSVGImage_H
#define CSVGImage_H

#include <CSVGObject.h>
#include <CSVGXLink.h>

class CSVGImage : public CSVGObject {
 public:
  CSVG_OBJECT_DEF("image", CSVGObjTypeId::IMAGE)

  CSVGImage(CSVG &svg);
  CSVGImage(const CSVGImage &image);

 ~CSVGImage();

  CSVGImage *dup() const override;

  CImagePtr getImage() const;

  double getX() const { return x_.getValue(0); }
  void setX(double x) { x_ = x; }

  double getY() const { return y_.getValue(0); }
  void setY(double y) { y_ = y; }

  double getWidth () const { return w_.getValue(CSVGLengthValue(100)).value(); }
  void setWidth(double w) { w_ = w; }

  double getHeight() const { return h_.getValue(CSVGLengthValue(100)).value(); }
  void setHeight(double h) { h_ = h; }

  CPoint2D getPosition() const { return CPoint2D(getX(), getY()); }

  bool getSize(CSize2D &size) const override;

  void setOrigin(const CPoint2D &point);
  void setSize(const CSize2D &size);

  void setImage(const std::string &filename);

  bool processOption(const std::string &name, const std::string &value) override;

  void draw() override;

  bool getBBox(CBBox2D &bbox) const override;

  void moveBy(const CVector2D &delta) override;
  void resizeTo(const CSize2D &size) override;

  void print(std::ostream &os, bool hier) const override;

  friend std::ostream &operator<<(std::ostream &os, const CSVGImage &image);

 private:
  CSize2D getSizeInternal() const { return CSize2D(getWidth(), getHeight()); }

 private:
  CSVGXLink                 xlink_;
  COptValT<double>          x_;
  COptValT<double>          y_;
  COptValT<CSVGLengthValue> w_;
  COptValT<CSVGLengthValue> h_;
};

#endif
