#ifndef CSVG_WRITE_H
#define CSVG_WRITE_H

#include <CSVGVisitor.h>
#include <iostream>

class CSVGWriteVisitor : public CSVGVisitor {
 public:
  CSVGWriteVisitor(std::ostream &os);

 ~CSVGWriteVisitor() { }

  void exec(CSVGObject *object) override;

  void preVisit (CSVGObject *o) override;
  void postVisit(CSVGObject *o) override;
  void postExec (CSVGObject *o) override;

  void visit(CSVGAnchor *o) override;
  void visit(CSVGBlock *o) override;
  void visit(CSVGAnimate *o) override;
  void visit(CSVGAnimateColor *o) override;
  void visit(CSVGAnimateMotion *o) override;
  void visit(CSVGAnimateTransform *o) override;
  void visit(CSVGAudio *o) override;
  void visit(CSVGCircle *o) override;
  void visit(CSVGClipPath *o) override;
  void visit(CSVGColorProfile *o) override;
  void visit(CSVGDefs *o) override;
  void visit(CSVGDesc *o) override;
  void visit(CSVGEllipse *o) override;

  void visit(CSVGFeBlend *o) override;
  void visit(CSVGFeColorMatrix *o) override;
  void visit(CSVGFeComponentTransfer *o) override;
  void visit(CSVGFeComposite *o) override;
  void visit(CSVGFeConvolveMatrix *o) override;
  void visit(CSVGFeDiffuseLighting *o) override;
  void visit(CSVGFeDisplacementMap *o) override;
  void visit(CSVGFeDistantLight *o) override;
  void visit(CSVGFeFlood *o) override;
  void visit(CSVGFeFunc *o) override;
  void visit(CSVGFeGaussianBlur *o) override;
  void visit(CSVGFeImage *o) override;
  void visit(CSVGFeLighting *o) override;
  void visit(CSVGFeMerge *o) override;
  void visit(CSVGFeMergeNode *o) override;
  void visit(CSVGFeMorphology *o) override;
  void visit(CSVGFeOffset *o) override;
  void visit(CSVGFePointLight *o) override;
  void visit(CSVGFeSpecularLighting *o) override;
  void visit(CSVGFeSpotLight *o) override;
  void visit(CSVGFeTile *o) override;
  void visit(CSVGFeTurbulence *o) override;
  void visit(CSVGFilter *o) override;

  void visit(CSVGFont *o) override;
  void visit(CSVGFontFace *o) override;
  void visit(CSVGFontFaceSrc *o) override;
  void visit(CSVGFontFaceUri *o) override;
  void visit(CSVGGlyph *o) override;
  void visit(CSVGGroup *o) override;
  void visit(CSVGHKern *o) override;
  void visit(CSVGImage *o) override;
  void visit(CSVGLinearGradient *o) override;
  void visit(CSVGLine *o) override;
  void visit(CSVGMarker *o) override;
  void visit(CSVGMask *o) override;
  void visit(CSVGMetaData *o) override;
  void visit(CSVGMPath *o) override;
  void visit(CSVGPath *o) override;
  void visit(CSVGPattern *o) override;
  void visit(CSVGPolygon *o) override;
  void visit(CSVGPolyLine *o) override;
  void visit(CSVGRadialGradient *o) override;
  void visit(CSVGRect *o) override;
  void visit(CSVGScript *o) override;
  void visit(CSVGSet *o) override;
  void visit(CSVGStop *o) override;
  void visit(CSVGStyle *o) override;
  void visit(CSVGSwitch *o) override;
  void visit(CSVGSymbol *o) override;
  void visit(CSVGTBreak *o) override;
  void visit(CSVGText *o) override;
  void visit(CSVGTextPath *o) override;
  void visit(CSVGTitle *o) override;
  void visit(CSVGTSpan *o) override;
  void visit(CSVGUse *o) override;

  void visit(CSVGAnimateBase *o) override;
  void visit(CSVGObject *o) override;

 private:
  //void writeStyle(CSVGObject *o) const;

 private:
  std::ostream &os_;
  int           depth_ { 0 };
  bool          newline_ { false };
};

#endif
