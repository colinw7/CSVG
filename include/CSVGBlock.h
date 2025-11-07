#ifndef CSVGBlock_H
#define CSVGBlock_H

#include <CSVGObject.h>
#include <CSVGTypes.h>
#include <CSVGBlockData.h>

class CCSS;

class CSVGBlock : public CSVGObject, public CSVGPrintBase<CSVGBlock> {
 public:
  CSVG_OBJECT_DEF("block", CSVGObjTypeId::BLOCK)

  CSVGBlock(CSVG &svg);
  CSVGBlock(const CSVGBlock &block);

  CSVGBlock *dup() const override;

  std::string getTagName() const override { return "svg"; }

  //---

  virtual int getScreenWidth () const { return 400; }
  virtual int getScreenHeight() const { return 400; }

  //---

  // pixel bbox (x, y, width, height)

  //! get/set x
  double getX() const;
  void setX(double x) { x_ = x; }

  //! get/set y
  double getY() const;
  void setY(double y) { y_ = y; }

  //! get/set width
  double getWidth() const;
  void setWidth(double w) { width_ = CScreenUnits(w); }

  //! get/set height
  double getHeight() const;
  void setHeight(double h) { height_ = CScreenUnits(h); }

  //---

  //! get/set preserve aspect
  bool hasPreserveAspect() const { return !!preserveAspect_; }
  CSVGPreserveAspect preserveAspect(const CSVGPreserveAspect &a=CSVGPreserveAspect()) const {
    return preserveAspect_.value_or(a); }
  void setPreserveAspect(const CSVGPreserveAspect &a) { preserveAspect_ = a; }

  //---

  // align

  //! get/set horizontal align
  CHAlignType getHAlign() const { return preserveAspect().getHAlign(); }
  void setHAlign(const CHAlignType &a) {
    CSVGPreserveAspect preserveAspect = this->preserveAspect();
    preserveAspect.setHAlign(a);
    preserveAspect_ = preserveAspect;
  }

  //! get/set vertical align
  CVAlignType getVAlign() const { return preserveAspect().getVAlign(); }
  void setVAlign(const CVAlignType &a) {
    CSVGPreserveAspect preserveAspect = this->preserveAspect();
    preserveAspect.setVAlign(a);
    preserveAspect_ = preserveAspect;
  }

  //! get/set scale
  CSVGScale getScale() const { return preserveAspect().getScale (); }
  void setScale(const CSVGScale &s) {
    CSVGPreserveAspect preserveAspect = this->preserveAspect();
    preserveAspect.setScale(s);
    preserveAspect_ = preserveAspect;
  }

  //---

  // pixel and view box
  CBBox2D calcPixelBox() const;
  CBBox2D calcViewBox() const;

  //---

  bool getBBox(CBBox2D &bbox) const override;

  void setSize(const CSize2D &size);

  bool processOption(const std::string &name, const std::string &value) override;

  std::optional<std::string> getNameValue(const std::string &name) const override;

  bool propagateFlat() const override { return false; }

  //---

  void drawInit() override;

  bool draw() override;

  void drawTerm() override;

  //---

  void printRoot(std::ostream &os, const std::vector<CCSS> &cssList, bool hier) const;

  void print(std::ostream &os, bool hier=false) const override;

  void printValues(std::ostream &os, bool flat=false) const override;

  void accept(CSVGVisitor *visitor) override { visitor->visit(this); }

 private:
  std::optional<double>             x_;
  std::optional<double>             y_;
  std::optional<CScreenUnits>       width_;
  std::optional<CScreenUnits>       height_;
  std::optional<CSVGPreserveAspect> preserveAspect_;
  mutable CSVGBuffer*               oldBuffer_ { nullptr };
  mutable CSVGBlockData             blockData_;
};

#endif
