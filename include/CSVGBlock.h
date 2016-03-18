#ifndef CSVGBlock_H
#define CSVGBlock_H

#include <CSVGObject.h>
#include <CSVGTypes.h>

class CSVGBlock : public CSVGObject {
 public:
  CSVG_OBJECT_DEF("block", CSVG_OBJ_TYPE_BLOCK)

  CSVGBlock(CSVG &svg);
  CSVGBlock(const CSVGBlock &block);

  CSVGBlock *dup() const;

  double getX() const;
  double getY() const;

  double getWidth() const;
  double getHeight() const;

  CHAlignType getHAlign() const { return halign_; }
  CVAlignType getVAlign() const { return valign_; }
  CSVGScale   getScale () const { return scale_ ; }

  void setSize(const CSize2D &size);

  bool processOption(const std::string &name, const std::string &value);

  void draw();

  bool getBBox(CBBox2D &bbox) const;

  void print(std::ostream &os) const;

  friend std::ostream &operator<<(std::ostream &os, const CSVGBlock &block);

 private:
  COptValT<double> x_;
  COptValT<double> y_;
  COptValT<double> width_;
  COptValT<double> height_;
  CHAlignType      halign_ { CHALIGN_TYPE_CENTER };
  CVAlignType      valign_ { CVALIGN_TYPE_CENTER };
  CSVGScale        scale_  { CSVG_SCALE_FREE };
};

#endif
