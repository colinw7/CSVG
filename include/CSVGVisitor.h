#ifndef CSVG_VISITOR_H
#define CSVG_VISITOR_H

class CSVGAnchor;
class CSVGBlock;
class CSVGAnimate;
class CSVGAnimateColor;
class CSVGAnimateMotion;
class CSVGAnimateTransform;
class CSVGAudio;
class CSVGCircle;
class CSVGClipPath;
class CSVGColorProfile;
class CSVGDefs;
class CSVGDesc;
class CSVGEllipse;

class CSVGFeBlend;
class CSVGFeColorMatrix;
class CSVGFeComponentTransfer;
class CSVGFeComposite;
class CSVGFeConvolveMatrix;
class CSVGFeDiffuseLighting;
class CSVGFeDisplacementMap;
class CSVGFeDistantLight;
class CSVGFeFlood;
class CSVGFeFunc;
class CSVGFeGaussianBlur;
class CSVGFeImage;
class CSVGFeLighting;
class CSVGFeMerge;
class CSVGFeMergeNode;
class CSVGFeMorphology;
class CSVGFeOffset;
class CSVGFePointLight;
class CSVGFeSpecularLighting;
class CSVGFeSpotLight;
class CSVGFeTile;
class CSVGFeTurbulence;
class CSVGFilter;

class CSVGFont;
class CSVGFontFace;
class CSVGFontFaceSrc;
class CSVGFontFaceUri;
class CSVGGlyph;
class CSVGGroup;
class CSVGHKern;
class CSVGImage;
class CSVGLinearGradient;
class CSVGLine;
class CSVGMarker;
class CSVGMask;
class CSVGMetaData;
class CSVGMissingGlyph;
class CSVGMPath;
class CSVGPath;
class CSVGPattern;
class CSVGPolygon;
class CSVGPolyLine;
class CSVGRadialGradient;
class CSVGRect;
class CSVGScript;
class CSVGSet;
class CSVGStop;
class CSVGStyle;
class CSVGSwitch;
class CSVGSymbol;
class CSVGTBreak;
class CSVGText;
class CSVGTextPath;
class CSVGTitle;
class CSVGTSpan;
class CSVGUse;

class CSVGAnimateBase; // TODO: remove
class CSVGObject;      // TODO remove

class CSVGVisitor {
 public:
  CSVGVisitor();

  virtual ~CSVGVisitor();

  virtual void preVisit (CSVGObject *) { }
  virtual void postVisit(CSVGObject *) { }
  virtual void postExec (CSVGObject *) { }

  virtual void visit(CSVGAnchor *o) = 0;
  virtual void visit(CSVGBlock *o) = 0;
  virtual void visit(CSVGAnimate *o) = 0;
  virtual void visit(CSVGAnimateColor *o) = 0;
  virtual void visit(CSVGAnimateMotion *o) = 0;
  virtual void visit(CSVGAnimateTransform *o) = 0;
  virtual void visit(CSVGAudio *o) = 0;
  virtual void visit(CSVGCircle *o) = 0;
  virtual void visit(CSVGClipPath *o) = 0;
  virtual void visit(CSVGColorProfile *o) = 0;
  virtual void visit(CSVGDefs *o) = 0;
  virtual void visit(CSVGDesc *o) = 0;
  virtual void visit(CSVGEllipse *o) = 0;

  virtual void visit(CSVGFeBlend *o) = 0;
  virtual void visit(CSVGFeColorMatrix *o) = 0;
  virtual void visit(CSVGFeComponentTransfer *o) = 0;
  virtual void visit(CSVGFeComposite *o) = 0;
  virtual void visit(CSVGFeConvolveMatrix *o) = 0;
  virtual void visit(CSVGFeDiffuseLighting *o) = 0;
  virtual void visit(CSVGFeDisplacementMap *o) = 0;
  virtual void visit(CSVGFeDistantLight *o) = 0;
  virtual void visit(CSVGFeFlood *o) = 0;
  virtual void visit(CSVGFeFunc *o) = 0;
  virtual void visit(CSVGFeGaussianBlur *o) = 0;
  virtual void visit(CSVGFeImage *o) = 0;
  virtual void visit(CSVGFeLighting *o) = 0;
  virtual void visit(CSVGFeMerge *o) = 0;
  virtual void visit(CSVGFeMergeNode *o) = 0;
  virtual void visit(CSVGFeMorphology *o) = 0;
  virtual void visit(CSVGFeOffset *o) = 0;
  virtual void visit(CSVGFePointLight *o) = 0;
  virtual void visit(CSVGFeSpecularLighting *o) = 0;
  virtual void visit(CSVGFeSpotLight *o) = 0;
  virtual void visit(CSVGFeTile *o) = 0;
  virtual void visit(CSVGFeTurbulence *o) = 0;
  virtual void visit(CSVGFilter *o) = 0;

  virtual void visit(CSVGFont *o) = 0;
  virtual void visit(CSVGFontFace *o) = 0;
  virtual void visit(CSVGFontFaceSrc *o) = 0;
  virtual void visit(CSVGFontFaceUri *o) = 0;
  virtual void visit(CSVGGlyph *o) = 0;
  virtual void visit(CSVGGroup *o) = 0;
  virtual void visit(CSVGHKern *o) = 0;
  virtual void visit(CSVGImage *o) = 0;
  virtual void visit(CSVGLinearGradient *o) = 0;
  virtual void visit(CSVGLine *o) = 0;
  virtual void visit(CSVGMarker *o) = 0;
  virtual void visit(CSVGMask *o) = 0;
  virtual void visit(CSVGMetaData *o) = 0;
  virtual void visit(CSVGMPath *o) = 0;
  virtual void visit(CSVGPath *o) = 0;
  virtual void visit(CSVGPattern *o) = 0;
  virtual void visit(CSVGPolygon *o) = 0;
  virtual void visit(CSVGPolyLine *o) = 0;
  virtual void visit(CSVGRadialGradient *o) = 0;
  virtual void visit(CSVGRect *o) = 0;
  virtual void visit(CSVGScript *o) = 0;
  virtual void visit(CSVGSet *o) = 0;
  virtual void visit(CSVGStop *o) = 0;
  virtual void visit(CSVGStyle *o) = 0;
  virtual void visit(CSVGSwitch *o) = 0;
  virtual void visit(CSVGSymbol *o) = 0;
  virtual void visit(CSVGTBreak *o) = 0;
  virtual void visit(CSVGText *o) = 0;
  virtual void visit(CSVGTextPath *o) = 0;
  virtual void visit(CSVGTitle *o) = 0;
  virtual void visit(CSVGTSpan *o) = 0;
  virtual void visit(CSVGUse *o) = 0;

  virtual void visit(CSVGAnimateBase *o) = 0; // TODO: remove (assert)
  virtual void visit(CSVGObject *o) = 0; // TODO: remove (assert)

  virtual void exec(CSVGObject *object);
};

#endif
