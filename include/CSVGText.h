#ifndef CSVGText_H
#define CSVGText_H

#include <CSVGObject.h>

class CSVGText : public CSVGObject, public CSVGPrintBase<CSVGText> {
 public:
  using Reals   = std::vector<double>;
  using Coords  = std::vector<CScreenUnits>;
  using Lengths = std::vector<CScreenUnits>;

  using OptReals   = std::optional<Reals>;
  using OptCoords  = std::optional<Coords>;
  using OptLengths = std::optional<Lengths>;

  enum class LengthAdjustType {
    LENGTHADJUST_UNKNOWN          = 0,
    LENGTHADJUST_SPACING          = 1,
    LENGTHADJUST_SPACINGANDGLYPHS = 2
  };

 public:
  CSVG_OBJECT_DEF("text", CSVGObjTypeId::TEXT)

  CSVGText(CSVG &svg);
  CSVGText(const CSVGText &text);

  CSVGText *dup() const override;

  CScreenUnits getX() const;
  void setX(const CScreenUnits &x);

  CScreenUnits getY() const;
  void setY(const CScreenUnits &y);

  CScreenUnits getDX() const;
  void setDX(const CScreenUnits &x);

  CScreenUnits getDY() const;
  void setDY(const CScreenUnits &y);

  Reals getRotate() const { return rotate_.value_or(Reals()); }
  void setRotate(const Reals &r) { rotate_ = r; }

  std::string getTextLength() const { return textLength_.value_or(""); }
  void setTextLength(const std::string &s) { textLength_ = s; }

  std::string getLengthAdjust() const { return lengthAdjust_.value_or("spacing"); }
  void setLengthAdjust(const std::string &s) { lengthAdjust_ = s; }

  int getLengthAdjustValue() const {
    if (getLengthAdjust() == "spacing")
      return int(LengthAdjustType::LENGTHADJUST_SPACING);
    if (getLengthAdjust() == "spacingAndGlyphs")
      return int(LengthAdjustType::LENGTHADJUST_SPACINGANDGLYPHS);

    return int(LengthAdjustType::LENGTHADJUST_UNKNOWN);
  }

  CPoint2D getPosition() const;

  bool hasFont() const override { return true; }

  bool canFlatten() const override { return false; }

  bool processOption(const std::string &name, const std::string &value) override;

  std::optional<std::string> getNameValue(const std::string &name) const override;

  bool draw() override;

  bool getBBox(CBBox2D &bbox) const override;

  const CPoint2D &lastPos() const { return lastPos_; }
  void setLastPos(const CPoint2D &p) { lastPos_ = p; }

  void print(std::ostream &os, bool hier=false) const override;

  void printValues(std::ostream &os, bool flat=false) const override;

  void accept(CSVGVisitor *visitor) override { visitor->visit(this); }

 protected:
  void init();

  void moveDelta(const CVector2D &delta) override;

 private:
  OptCoords                  x_;
  OptCoords                  y_;
  OptLengths                 dx_;
  OptLengths                 dy_;
  OptReals                   rotate_;
  std::optional<std::string> textLength_;
  std::optional<std::string> lengthAdjust_;
  CPoint2D                   lastPos_;
};

#endif
