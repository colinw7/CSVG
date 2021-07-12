#ifndef CQSVGBlock_H
#define CQSVGBlock_H

#include <CQSVGObject.h>
#include <CSVGBlock.h>

class CQSVG;

class CQSVGBlock : public CQSVGObject, public CSVGBlock {
  Q_OBJECT

  Q_PROPERTY(double x READ getX WRITE setX)
  Q_PROPERTY(double y READ getY WRITE setY)

  Q_PROPERTY(double width  READ getWidth  WRITE setWidth )
  Q_PROPERTY(double height READ getHeight WRITE setHeight)

#if 0
  Q_PROPERTY(CSVGPreserveAspect preserveAspect READ preserveAspect WRITE setPreserveAspect)
#endif

  Q_PROPERTY(CQSVGEnum::HAlignType halign READ halignType WRITE setHAlignType)
  Q_PROPERTY(CQSVGEnum::VAlignType valign READ valignType WRITE setVAlignType)

#if 0
  Q_PROPERTY(CSVGScale scale READ getScale WRITE setScale)
#endif

 public:
  CQSVGBlock(CQSVG *svg);

  int getScreenWidth () const override;
  int getScreenHeight() const override;

  //---

  CQSVGEnum::HAlignType halignType() const;
  void setHAlignType(const CQSVGEnum::HAlignType &h);

  CQSVGEnum::VAlignType valignType() const;
  void setVAlignType(const CQSVGEnum::VAlignType &v);

  //---

  void addProperties(CQPropertyTree *tree, const std::string &name) override;

  void drawTerm() override;
};

#endif
