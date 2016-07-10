#include <CSVG.h>
#include <CSVGAnchor.h>
#include <CSVGAnimate.h>
#include <CSVGAnimateColor.h>
#include <CSVGAnimateMotion.h>
#include <CSVGAnimateTransform.h>
#include <CSVGBuffer.h>
#include <CSVGCircle.h>
#include <CSVGClipPath.h>
#include <CSVGColorProfile.h>
#include <CSVGDefs.h>
#include <CSVGDesc.h>
#include <CSVGEllipse.h>
#include <CSVGFeBlend.h>
#include <CSVGFeColorMatrix.h>
#include <CSVGFeComponentTransfer.h>
#include <CSVGFeComposite.h>
#include <CSVGFeConvolveMatrix.h>
#include <CSVGFeDiffuseLighting.h>
#include <CSVGFeDisplacementMap.h>
#include <CSVGFeDistantLight.h>
#include <CSVGFeFlood.h>
#include <CSVGFeFunc.h>
#include <CSVGFeGaussianBlur.h>
#include <CSVGFeImage.h>
#include <CSVGFeMerge.h>
#include <CSVGFeMergeNode.h>
#include <CSVGFeMorphology.h>
#include <CSVGFeOffset.h>
#include <CSVGFePointLight.h>
#include <CSVGFeSpecularLighting.h>
#include <CSVGFeSpotLight.h>
#include <CSVGFeTile.h>
#include <CSVGFeTurbulence.h>
#include <CSVGFilter.h>
#include <CSVGFont.h>
#include <CSVGFontFace.h>
#include <CSVGFontFaceSrc.h>
#include <CSVGFontFaceUri.h>
#include <CSVGGroup.h>
#include <CSVGHKern.h>
#include <CSVGImage.h>
#include <CSVGLine.h>
#include <CSVGLinearGradient.h>
#include <CSVGLog.h>
#include <CSVGMask.h>
#include <CSVGMarker.h>
#include <CSVGMissingGlyph.h>
#include <CSVGMPath.h>
#include <CSVGPath.h>
#include <CSVGPathPart.h>
#include <CSVGPattern.h>
#include <CSVGPolygon.h>
#include <CSVGPolyLine.h>
#include <CSVGRadialGradient.h>
#include <CSVGRect.h>
#include <CSVGRenderer.h>
#include <CSVGScript.h>
#include <CSVGSet.h>
#include <CSVGStop.h>
#include <CSVGStyle.h>
#include <CSVGStyleData.h>
#include <CSVGSwitch.h>
#include <CSVGSymbol.h>
#include <CSVGText.h>
#include <CSVGTextPath.h>
#include <CSVGTitle.h>
#include <CSVGTSpan.h>
#include <CSVGUse.h>
#include <CSVGUtil.h>
#include <CSVGFontObj.h>

#include <CSVGJavaScript.h>
#include <CSVGJDocument.h>
#include <CSVGJObject.h>
#include <CSVGJEvent.h>
#include <CSVGJTransform.h>

#include <CRadialGradient.h>
#include <CLinearGradient.h>
#include <CConfig.h>
#include <CXML.h>
#include <CXMLToken.h>
#include <CXMLExecute.h>
#include <CXMLText.h>
#include <CRegExp.h>
#include <CStrParse.h>
#include <CRGBName.h>

CSVG::
CSVG(CSVGRenderer *renderer) :
 renderer_ (renderer),
 styleData_(*this),
 cssData_  (*this)
{
  xml_ = new CXML();

  bufferMgr_ = new CSVGBufferMgr(*this);

  //---

  if (getenv("CSVG_DEBUG"))
    setDebug(true);

  if (getenv("CSVG_DEBUG_IMAGE"))
    setDebugImage(true);

  if (getenv("CSVG_DEBUG_OBJ_IMAGE"))
    setDebugObjImage(true);

  if (getenv("CSVG_DEBUG_FILTER"))
    setDebugFilter(true);

  if (getenv("CSVG_DEBUG_MASK"))
    setDebugMask(true);

  if (getenv("CSVG_DEBUG_USE"))
    setDebugUse(true);

  //---

  js_ = createJavaScript();

  jsDocumentType_       = CJObjectTypeP(new CSVGJDocumentType());
  jsObjectType_         = CJObjectTypeP(new CSVGJObjectType());
  jsEventType_          = CJObjectTypeP(new CSVGJEventType());
  jsTransformStackType_ = CJObjectTypeP(new CSVGJTransformStackType());
  jsTransformType_      = CJObjectTypeP(new CSVGJTransformType());
  jsMatrixType_         = CJObjectTypeP(new CSVGJMatrixType());

  jsDocument_  = CJValueP(new CSVGJDocument(this));
  jsEvent_     = CJValueP(new CSVGJEvent(this));

  CJVariableP var1(js()->createVariable("document"   , jsDocument()));
  CJVariableP var2(js()->createVariable("svgDocument", jsDocument()));

  js()->addVariable(var1);
  js()->addVariable(var2);

  CJDictionary *transformDict = new CJDictionary(js());

  js()->addVariable(CJVariableP(js()->createVariable("SVGTransform", CJValueP(transformDict))));

  transformDict->addProperty(js(), "SVG_TRANSFORM_TRANSLATE", int(CMatrixTransformType::TRANSLATE));
  transformDict->addProperty(js(), "SVG_TRANSFORM_SCALE"    , int(CMatrixTransformType::SCALE1));
  transformDict->addProperty(js(), "SVG_TRANSFORM_ROTATE"   , int(CMatrixTransformType::ROTATE));
}

CSVG::
~CSVG()
{
}

CSVG *
CSVG::
dup() const
{
  return new CSVG;
}

CSVGBlock *
CSVG::
getRoot() const
{
  // get root block
  if (! block_.isValid()) {
    CSVG *th = const_cast<CSVG *>(this);

    th->block_ = th->createBlock();
  }

  return block_;
}

CPoint2D
CSVG::
flatOffset() const
{
  CPoint2D o = blockData_.offset();

  for (const auto &b : blockDataStack_)
    o += b.offset();

  return o;
}

double
CSVG::
flatXScale() const
{
  double s = blockData_.xscale();

  for (const auto &b : blockDataStack_)
    s *= b.xscale();

  return s;
}

double
CSVG::
flatYScale() const
{
  double s = blockData_.yscale();

  for (const auto &b : blockDataStack_)
    s *= b.yscale();

  return s;
}

void
CSVG::
setRenderer(CSVGRenderer *renderer)
{
  renderer_ = renderer;
}

CSVGRenderer *
CSVG::
createRenderer()
{
  if (renderer_)
    return renderer_->dup();

  return 0;
}

void
CSVG::
setPaintBox(const CBBox2D &bbox)
{
  bufferMgr_->setPaintBBox(bbox);
}

void
CSVG::
setPaintStyle(const CSVGFill &fill, const CSVGStroke &stroke)
{
  bufferMgr_->setPaintFill  (fill);
  bufferMgr_->setPaintStroke(stroke);
}

void
CSVG::
setAntiAlias(bool flag)
{
  bufferMgr_->setAntiAlias(flag);
}

CSVGObject *
CSVG::
currentDrawObject() const
{
  if (drawObjects_.empty())
    return 0;

  return drawObjects_.back();
}

void
CSVG::
pushDrawObject(CSVGObject *o)
{
  drawObjects_.push_back(o);
}

void
CSVG::
popDrawObject()
{
  assert(! drawObjects_.empty());

  drawObjects_.pop_back();
}

void
CSVG::
setAutoName(bool autoName)
{
  autoName_ = autoName;
}

void
CSVG::
setDebug(bool b)
{
  debug_ = b;

  xml_->setDebug(debug_);
}

void
CSVG::
setDebugImage(bool b)
{
  debugImage_ = b;
}

void
CSVG::
setDebugObjImage(bool b)
{
  debugObjImage_ = b;
}

void
CSVG::
setDebugFilter(bool b)
{
  debugFilter_ = b;
}

void
CSVG::
setDebugMask(bool b)
{
  debugMask_ = b;
}

void
CSVG::
setDebugUse(bool b)
{
  debugUse_ = b;
}

void
CSVG::
init()
{
}

void
CSVG::
clear()
{
  if (bufferMgr_.isValid())
    bufferMgr_->clear();

  if (xml_.isValid())
    xml_->clear();

  buffer_ = 0;

  rootBlockData_.reset();

  blockData_.reset();

  block_      = 0;
  xmlTag_     = 0;
  background_ = CRGBA(1,1,1);

  fontList_.clear();

  idObjectMap_.clear();
  cssData_    .clear();

  styleData_.reset();

  drawObjects_.clear();
}

bool
CSVG::
read(const std::string &filename)
{
  clear();

  return read(filename, getRoot());
}

bool
CSVG::
read(const std::string &filename, CSVGObject *object)
{
  if (! CFile::exists(filename) || ! CFile::isRegular(filename))
    return false;

  if (! xml_->read(filename, &xmlTag_))
    return false;

  if (xmlTag_->getName() != "svg")
    return false;

  //------

  // Process top (execute) tokens
  for (const auto &token : xml_->getTokens()) {
    if (token->isExecute()) {
      CXMLExecute *exec = token->getExecute();

      const std::string &id = exec->getId();

      if (id == "xml-stylesheet") {
        CSVGXmlStyleSheet xmlStyleSheet;

        uint numOptions = exec->getNumOptions();

        for (uint j = 0; j < numOptions; ++j) {
          const CXMLExecute::Option &opt = exec->getOption(j);

          if      (opt.getName() == "href")
            xmlStyleSheet.ref = opt.getValue();
          else if (opt.getName() == "type")
            xmlStyleSheet.is_css = (opt.getValue() == "text/css");
        }

        if (xmlStyleSheet.is_css.getValue(false) &&
            ! xmlStyleSheet.ref.getValue("").empty())
          readCSSFile(xmlStyleSheet.ref.getValue());

        xmlStyleSheet_ = xmlStyleSheet;
      }
    }
  }

  //------

  // Process svg options
  for (const auto &opt : xmlTag_->getOptions()) {
    const std::string &opt_name  = opt->getName ();
    const std::string &opt_value = opt->getValue();

    if (! object->processOption(opt_name, opt_value))
      CSVGLog() << "Invalid option '" << opt_name << "=" << opt_value <<
                   " for " << object->getTagName();
  }

  //------

  // Process svg children
  object->setXMLTag(xmlTag_);

  for (const auto &token : xmlTag_->getChildren()) {
    CSVGObject *object1 = tokenToObject(object, token);

    if (object1)
      object->addChildObject(object1);
  }

  //------

  getRoot()->execEvent(CSVGEventType::LOAD);

  return true;
}

CSVGObject *
CSVG::
tokenToObject(CSVGObject *parent, const CXMLToken *token)
{
  if (! token->isTag())
    return 0;

  CXMLTag *tag = token->getTag();

  //-----

  const std::string &tag_name = tag->getName();

  if (tag_name == "d:SVGTestCase")
    return 0;

  //-----

  // Create object from tag name
  CSVGObject *object = createObjectByName(tag_name);

  if (! object) {
    CSVGLog() << "Unknown tag '" << tag_name << "'";
    return 0;
  }

  object->setParent(parent);

  //-----

  // inform object we are about to parse tag
  object->initParse();

  //-----

  // get tag text (including embedded tspan tags)
  std::string textStr;
  bool        hasTextSpan = false;

  for (const auto &token : tag->getChildren()) {
    if      (token->isText()) {
      CXMLText *text = token->getText();

      if (hasTextSpan) {
        CSVGTSpan *tspan = dynamic_cast<CSVGTSpan *>(object);

        if (! tspan) {
          tspan = createTSpan();

          object->addChildObject(tspan);
        }

        tspan->setText(text->getText());
      }
      else
        textStr += text->getText();
    }
    else if (token->isTag() && token->getTag()->getName() == "tspan") {
      if (! hasTextSpan && textStr != "") {
        CSVGTSpan *tspan = createTSpan();

        tspan->setText(textStr);

        object->addChildObject(tspan);
      }

      CSVGObject *object1 = tokenToObject(object, token);
      assert(object1);

      object->addChildObject(object1);

      hasTextSpan = true;
    }
  }

  if (! hasTextSpan && textStr != "")
    object->setText(textStr);

  //-----

  // process tag options
  for (const auto &option : tag->getOptions()) {
    const std::string &opt_name  = option->getName ();
    const std::string &opt_value = option->getValue();

    if (! object->processOption(opt_name, opt_value))
      CSVGLog() << "Unhandled tag option " << opt_name << "=" << opt_value <<
                   " for " << object->getTagName();
  }

  object->setXMLTag(tag);

  //-----

  // process tag children
  for (const auto &token : tag->getChildren()) {
    if (token->isTag() && token->getTag()->getName() == "tspan")
      continue;

    //---

    CSVGObject *object1 = tokenToObject(object, token);

    if (object1)
      object->addChildObject(object1);
  }

  //-----

  // inform object we are done parsing tag
  object->termParse();

  //-----

  return object;
}

CSVGObject *
CSVG::
createObjectByName(const std::string &name)
{
  // TODO: create routines for all object types

  CSVGObject *object;

  if      (name == "svg")
    object = createBlock();
  else if (name == "a")
    object = createAnchor();
  else if (name == "animate")
    object = createAnimate();
  else if (name == "animateColor")
    object = createAnimateColor();
  else if (name == "animateMotion")
    object = createAnimateMotion();
  else if (name == "animateTransform")
    object = createAnimateTransform();
  else if (name == "circle")
    object = createCircle();
  else if (name == "clipPath")
    object = createClipPath();
  else if (name == "color-profile")
    object = createColorProfile();
  else if (name == "defs")
    object = createDefs();
  else if (name == "desc")
    object = createDesc();
  else if (name == "ellipse")
    object = createEllipse();
  else if (name == "feBlend")
    object = createFeBlend();
  else if (name == "feColorMatrix")
    object = createFeColorMatrix();
  else if (name == "feComponentTransfer")
    object = createFeComponentTransfer();
  else if (name == "feComposite")
    object = createFeComposite();
  else if (name == "feConvolveMatrix")
    object = createFeConvolveMatrix();
  else if (name == "feDiffuseLighting")
    object = createFeDiffuseLighting();
  else if (name == "feDisplacementMap")
    object = createFeDisplacementMap();
  else if (name == "feDistantLight")
    object = createFeDistantLight();
  else if (name == "feFlood")
    object = createFeFlood();
  else if (name == "feFuncR")
    object = createFeFunc(CCOLOR_COMPONENT_RED);
  else if (name == "feFuncG")
    object = createFeFunc(CCOLOR_COMPONENT_GREEN);
  else if (name == "feFuncB")
    object = createFeFunc(CCOLOR_COMPONENT_BLUE);
  else if (name == "feFuncA")
    object = createFeFunc(CCOLOR_COMPONENT_ALPHA);
  else if (name == "feGaussianBlur")
    object = createFeGaussianBlur();
  else if (name == "feImage")
    object = createFeImage();
  else if (name == "feMerge")
    object = createFeMerge();
  else if (name == "feMergeNode")
    object = createFeMergeNode();
  else if (name == "feMorphology")
    object = createFeMorphology();
  else if (name == "feOffset")
    object = createFeOffset();
  else if (name == "fePointLight")
    object = createFePointLight();
  else if (name == "feSpecularLighting")
    object = createFeSpecularLighting();
  else if (name == "feSpotLight")
    object = createFeSpotLight();
  else if (name == "feTile")
    object = createFeTile();
  else if (name == "feTurbulence")
    object = createFeTurbulence();
  else if (name == "filter")
    object = createFilter();
  else if (name == "font")
    object = createFont();
  else if (name == "font-face")
    object = createFontFace();
  else if (name == "font-face-src")
    object = createFontFaceSrc();
  else if (name == "font-face-uri")
    object = createFontFaceUri();
  else if (name == "glyph")
    object = createGlyph();
  else if (name == "g")
    object = createGroup();
  else if (name == "hkern")
    object = createHKern();
  else if (name == "image")
    object = createImage();
  else if (name == "line")
    object = createLine();
  else if (name == "linearGradient")
    object = createLinearGradient();
  else if (name == "marker")
    object = createMarker();
  else if (name == "mask")
    object = createMask();
  else if (name == "missing-glyph")
    object = createMissingGlyph();
  else if (name == "mpath")
    object = createMPath();
  else if (name == "path")
    object = createPath();
  else if (name == "pattern")
    object = createPattern();
  else if (name == "polygon")
    object = createPolygon();
  else if (name == "polyline")
    object = createPolyLine();
  else if (name == "radialGradient")
    object = createRadialGradient();
  else if (name == "rect")
    object = createRect();
  else if (name == "script")
    object = createScript();
  else if (name == "set")
    object = createSet();
  else if (name == "stop")
    object = createStop();
  else if (name == "symbol")
    object = createSymbol();
  else if (name == "style")
    object = createStyle();
  else if (name == "switch")
    object = createSwitch();
  else if (name == "text")
    object = createText();
  else if (name == "textPath")
    object = createTextPath();
  else if (name == "title")
    object = createTitle();
  else if (name == "tspan")
    object = createTSpan();
  else if (name == "use")
    object = createUse();
  else
    object = 0;

  //------

  // automatically give object a non-empty name if required

  if (object && getAutoName())
    object->autoName();

  return object;
}

CSVGJavaScript *
CSVG::
createJavaScript()
{
  return new CSVGJavaScript(this);
}

CSVGBlock *
CSVG::
createBlock()
{
  return new CSVGBlock(*this);
}

CSVGAnchor *
CSVG::
createAnchor()
{
  return new CSVGAnchor(*this);
}

CSVGAnimate *
CSVG::
createAnimate()
{
  return new CSVGAnimate(*this);
}

CSVGAnimateColor *
CSVG::
createAnimateColor()
{
  return new CSVGAnimateColor(*this);
}

CSVGAnimateMotion *
CSVG::
createAnimateMotion()
{
  return new CSVGAnimateMotion(*this);
}

CSVGAnimateTransform *
CSVG::
createAnimateTransform()
{
  return new CSVGAnimateTransform(*this);
}

CSVGCircle *
CSVG::
createCircle()
{
  return new CSVGCircle(*this);
}

CSVGClipPath *
CSVG::
createClipPath()
{
  return new CSVGClipPath(*this);
}

CSVGColorProfile *
CSVG::
createColorProfile()
{
  return new CSVGColorProfile(*this);
}

CSVGDefs *
CSVG::
createDefs()
{
  return new CSVGDefs(*this);
}

CSVGDesc *
CSVG::
createDesc()
{
  return new CSVGDesc(*this);
}

CSVGEllipse *
CSVG::
createEllipse()
{
  return new CSVGEllipse(*this);
}

CSVGFeBlend *
CSVG::
createFeBlend()
{
  return new CSVGFeBlend(*this);
}

CSVGFeColorMatrix *
CSVG::
createFeColorMatrix()
{
  return new CSVGFeColorMatrix(*this);
}

CSVGFeComponentTransfer *
CSVG::
createFeComponentTransfer()
{
  return new CSVGFeComponentTransfer(*this);
}

CSVGFeConvolveMatrix *
CSVG::
createFeConvolveMatrix()
{
  return new CSVGFeConvolveMatrix(*this);
}

CSVGFeComposite *
CSVG::
createFeComposite()
{
  return new CSVGFeComposite(*this);
}

CSVGFeDiffuseLighting *
CSVG::
createFeDiffuseLighting()
{
  return new CSVGFeDiffuseLighting(*this);
}

CSVGFeDisplacementMap *
CSVG::
createFeDisplacementMap()
{
  return new CSVGFeDisplacementMap(*this);
}

CSVGFeDistantLight *
CSVG::
createFeDistantLight()
{
  return new CSVGFeDistantLight(*this);
}

CSVGFeFlood *
CSVG::
createFeFlood()
{
  return new CSVGFeFlood(*this);
}

CSVGFeFunc *
CSVG::
createFeFunc(CColorComponent component)
{
  return new CSVGFeFunc(*this, component);
}

CSVGFeGaussianBlur *
CSVG::
createFeGaussianBlur()
{
  return new CSVGFeGaussianBlur(*this);
}

CSVGFeImage *
CSVG::
createFeImage()
{
  return new CSVGFeImage(*this);
}

CSVGFeMerge *
CSVG::
createFeMerge()
{
  return new CSVGFeMerge(*this);
}

CSVGFeMergeNode *
CSVG::
createFeMergeNode()
{
  return new CSVGFeMergeNode(*this);
}

CSVGFeMorphology *
CSVG::
createFeMorphology()
{
  return new CSVGFeMorphology(*this);
}

CSVGFeOffset *
CSVG::
createFeOffset()
{
  return new CSVGFeOffset(*this);
}

CSVGFePointLight *
CSVG::
createFePointLight()
{
  return new CSVGFePointLight(*this);
}

CSVGFeSpecularLighting *
CSVG::
createFeSpecularLighting()
{
  return new CSVGFeSpecularLighting(*this);
}

CSVGFeSpotLight *
CSVG::
createFeSpotLight()
{
  return new CSVGFeSpotLight(*this);
}

CSVGFeTile *
CSVG::
createFeTile()
{
  return new CSVGFeTile(*this);
}

CSVGFeTurbulence *
CSVG::
createFeTurbulence()
{
  return new CSVGFeTurbulence(*this);
}

CSVGFilter *
CSVG::
createFilter()
{
  return new CSVGFilter(*this);
}

CSVGFont *
CSVG::
createFont()
{
  return new CSVGFont(*this);
}

CSVGFontFace *
CSVG::
createFontFace()
{
  return new CSVGFontFace(*this);
}

CSVGFontFaceSrc *
CSVG::
createFontFaceSrc()
{
  return new CSVGFontFaceSrc(*this);
}

CSVGFontFaceUri *
CSVG::
createFontFaceUri()
{
  return new CSVGFontFaceUri(*this);
}

CSVGGlyph *
CSVG::
createGlyph()
{
  return new CSVGGlyph(*this);
}

CSVGGroup *
CSVG::
createGroup()
{
  return new CSVGGroup(*this);
}

CSVGHKern *
CSVG::
createHKern()
{
  return new CSVGHKern(*this);
}

CSVGImage *
CSVG::
createImage()
{
  return new CSVGImage(*this);
}

CSVGLine *
CSVG::
createLine()
{
  return new CSVGLine(*this);
}

CSVGLinearGradient *
CSVG::
createLinearGradient()
{
  return new CSVGLinearGradient(*this);
}

CSVGMarker *
CSVG::
createMarker()
{
  return new CSVGMarker(*this);
}

CSVGMask *
CSVG::
createMask()
{
  return new CSVGMask(*this);
}

CSVGMissingGlyph *
CSVG::
createMissingGlyph()
{
  return new CSVGMissingGlyph(*this);
}

CSVGMPath *
CSVG::
createMPath()
{
  return new CSVGMPath(*this);
}

CSVGPath *
CSVG::
createPath()
{
  return new CSVGPath(*this);
}

CSVGPattern *
CSVG::
createPattern()
{
  return new CSVGPattern(*this);
}

CSVGPolygon *
CSVG::
createPolygon()
{
  return new CSVGPolygon(*this);
}

CSVGPolyLine *
CSVG::
createPolyLine()
{
  return new CSVGPolyLine(*this);
}

CSVGRadialGradient *
CSVG::
createRadialGradient()
{
  return new CSVGRadialGradient(*this);
}

CSVGRect *
CSVG::
createRect()
{
  return new CSVGRect(*this);
}

CSVGScript *
CSVG::
createScript()
{
  return new CSVGScript(*this);
}

CSVGSet *
CSVG::
createSet()
{
  return new CSVGSet(*this);
}

CSVGStop *
CSVG::
createStop()
{
  return new CSVGStop(*this);
}

CSVGSymbol *
CSVG::
createSymbol()
{
  return new CSVGSymbol(*this);
}

CSVGStyle *
CSVG::
createStyle()
{
  return new CSVGStyle(*this);
}

CSVGSwitch *
CSVG::
createSwitch()
{
  return new CSVGSwitch(*this);
}

CSVGText *
CSVG::
createText()
{
  return new CSVGText(*this);
}

CSVGTextPath *
CSVG::
createTextPath()
{
  return new CSVGTextPath(*this);
}

CSVGTitle *
CSVG::
createTitle()
{
  return new CSVGTitle(*this);
}

CSVGTSpan *
CSVG::
createTSpan()
{
  return new CSVGTSpan(*this);
}

CSVGUse *
CSVG::
createUse()
{
  return new CSVGUse(*this);
}

CSVGBuffer *
CSVG::
createBuffer(const std::string &name)
{
  return new CSVGBuffer(*this, name);
}

//------

CSVGPathMoveTo *
CSVG::
createPathMoveTo(double x, double y)
{
  return new CSVGPathMoveTo(*this, x, y);
}

CSVGPathRMoveTo *
CSVG::
createPathRMoveTo(double x, double y)
{
  return new CSVGPathRMoveTo(*this, x, y);
}

CSVGPathLineTo *
CSVG::
createPathLineTo(double x, double y)
{
  return new CSVGPathLineTo(*this, x, y);
}

CSVGPathRLineTo *
CSVG::
createPathRLineTo(double x, double y)
{
  return new CSVGPathRLineTo(*this, x, y);
}

CSVGPathHLineTo *
CSVG::
createPathHLineTo(double x)
{
  return new CSVGPathHLineTo(*this, x);
}

CSVGPathRHLineTo *
CSVG::
createPathRHLineTo(double x)
{
  return new CSVGPathRHLineTo(*this, x);
}

CSVGPathVLineTo *
CSVG::
createPathVLineTo(double y)
{
  return new CSVGPathVLineTo(*this, y);
}

CSVGPathRVLineTo *
CSVG::
createPathRVLineTo(double y)
{
  return new CSVGPathRVLineTo(*this, y);
}

CSVGPathArcTo *
CSVG::
createPathArcTo(double rx, double ry, double xa, double fa, double fs, double x2, double y2)
{
  return new CSVGPathArcTo(*this, rx, ry, xa, fa, fs, x2, y2);
}

CSVGPathRArcTo *
CSVG::
createPathRArcTo(double rx, double ry, double xa, double fa, double fs, double x2, double y2)
{
  return new CSVGPathRArcTo(*this, rx, ry, xa, fa, fs, x2, y2);
}

CSVGPathBezier2To *
CSVG::
createPathBezier2To(double x1, double y1, double x2, double y2)
{
  return new CSVGPathBezier2To(*this, x1, y1, x2, y2);
}

CSVGPathMBezier2To *
CSVG::
createPathMBezier2To(double x2, double y2)
{
  return new CSVGPathMBezier2To(*this, x2, y2);
}

CSVGPathRBezier2To *
CSVG::
createPathRBezier2To(double x1, double y1, double x2, double y2)
{
  return new CSVGPathRBezier2To(*this, x1, y1, x2, y2);
}

CSVGPathMRBezier2To *
CSVG::
createPathMRBezier2To(double x2, double y2)
{
  return new CSVGPathMRBezier2To(*this, x2, y2);
}

CSVGPathBezier3To *
CSVG::
createPathBezier3To(double x1, double y1, double x2, double y2, double x3, double y3)
{
  return new CSVGPathBezier3To(*this, x1, y1, x2, y2, x3, y3);
}

CSVGPathMBezier3To *
CSVG::
createPathMBezier3To(double x2, double y2, double x3, double y3)
{
  return new CSVGPathMBezier3To(*this, x2, y2, x3, y3);
}

CSVGPathRBezier3To *
CSVG::
createPathRBezier3To(double x1, double y1, double x2, double y2, double x3, double y3)
{
  return new CSVGPathRBezier3To(*this, x1, y1, x2, y2, x3, y3);
}

CSVGPathMRBezier3To *
CSVG::
createPathMRBezier3To(double x2, double y2, double x3, double y3)
{
  return new CSVGPathMRBezier3To(*this, x2, y2, x3, y3);
}

CSVGPathClosePath *
CSVG::
createPathClosePath(bool relative)
{
  return new CSVGPathClosePath(*this, relative);
}

CSVGImageData *
CSVG::
createImageData()
{
  return new CSVGImageData;
}

CSVGFontObj *
CSVG::
createFontObj(const CSVGFontDef &def)
{
  return new CSVGFontObj(def);
}

//------

double
CSVG::
getXMin() const
{
  return getRoot()->getXMin();
}

double
CSVG::
getYMin() const
{
  return getRoot()->getYMin();
}

double
CSVG::
getWidth() const
{
  return getRoot()->getWidth();
}

double
CSVG::
getHeight() const
{
  return getRoot()->getHeight();
}

void
CSVG::
addFont(CSVGFont *font)
{
  fontList_.push_back(font);
}

CSVGFont *
CSVG::
getFont() const
{
  if (fontList_.empty())
    return 0;

  return fontList_[0];
}

CSVGGlyph *
CSVG::
getCharGlyph(char c) const
{
  CSVGFont *font = getFont();

  if (font)
    return font->getCharGlyph(c);
  else
    return 0;
}

CSVGGlyph *
CSVG::
getUnicodeGlyph(const std::string &unicode) const
{
  if (fontList_.empty())
    return 0;

  CSVGFont *font = fontList_[0];

  return font->getUnicodeGlyph(unicode);
}

void
CSVG::
drawToBuffer(CSVGBuffer *buffer, int w, int h, const CPoint2D &offset,
             double xscale, double yscale)
{
  drawToRenderer(buffer->getRenderer(), w, h, offset, xscale, yscale);
}

void
CSVG::
drawToRenderer(CSVGRenderer *renderer, int w, int h, const CPoint2D &offset,
               double xscale, double yscale)
{
  renderer->setSize(xscale*w, yscale*h);

  setRenderer(renderer);

  draw(offset, xscale, yscale);
}

bool
CSVG::
hasAnimation() const
{
  return getRoot()->hasAnimation();
}

void
CSVG::
execJsEvent(CSVGObject *obj, const std::string &str)
{
  setEventObject(obj);

  CJVariableP var3(js()->createVariable("evt", jsEvent()));

  js()->addVariable(var3);

  js()->loadString(str);

  js()->exec();
}

void
CSVG::
draw()
{
  CPoint2D offset;
  double   scale = 1;

  draw(offset, scale, scale);
}

void
CSVG::
draw(const CPoint2D &offset, double xscale, double yscale)
{
  CSVGPreserveAspect preserveAspect;

  drawRoot(getRoot(), offset, xscale, yscale, preserveAspect);
}

void
CSVG::
drawRoot(CSVGBlock *block, const CPoint2D &offset, double xscale, double yscale,
         const CSVGPreserveAspect &preserveAspect)
{
  if (! renderer_)
    return;

  // set root block scale and offset
  CSVGBlockData blockData(block->calcPixelBox(), block->calcViewBox(), offset,
                          xscale, yscale, preserveAspect);

  rootBlockData_ = blockData;

  //------

  const CMatrixStack2D &viewMatrix = rootBlockData_.viewMatrix();

  renderer_->beginDraw();

  CPoint2D bmin, bmax;

  viewMatrix.multiplyPoint(CPoint2D(block->getXMin(), block->getYMin()), bmin);
  viewMatrix.multiplyPoint(CPoint2D(block->getXMax(), block->getYMax()), bmax);

  renderer_->setDataRange(bmin.x, bmin.y, bmax.x, bmax.y);

  //------

  resetStroke();
  resetFill();
  resetClip();
  resetFontDef();

  //------

  CSVGBuffer *bgBuffer = getBuffer("BackgroundImage");

  setCurrentBuffer(bgBuffer);

  bgBuffer->clear();

  beginDrawBuffer(bgBuffer, rootBlockOffset(), rootBlockXScale(), rootBlockYScale());

  //------

  block->drawObject();

  //------

  endDrawBuffer(bgBuffer);

  //------

  renderer_->endDraw();

  //------

  renderer_->setImage(bgBuffer->getRenderer());
}

CSVGBuffer *
CSVG::
pushBuffer(const std::string &name)
{
  if (buffer_)
    bufferStack_.push_back(buffer_);

  CSVGBuffer *buffer = getBuffer(name);

  buffer->setParentBuffer(buffer_);

  setCurrentBuffer(buffer);

  //printBufferStack("Push");

  return buffer;
}

CSVGBuffer *
CSVG::
popBuffer()
{
  assert(! bufferStack_.empty());

  CSVGBuffer *buffer = bufferStack_.back();

  buffer->setParentBuffer(0);

  bufferStack_.pop_back();

  setCurrentBuffer(buffer);

  //printBufferStack("Pop");

  return buffer;
}

void
CSVG::
printBufferStack(const std::string &desc) const
{
  std::cerr << desc;

  for (const auto &buffer : bufferStack_) {
    std::cerr << " " << buffer->getName();
  }

  if (buffer_)
    std::cerr << " " << buffer_->getName();

  std::cerr << std::endl;
}

CSVGBuffer *
CSVG::
getCurrentBuffer() const
{
  return buffer_;
}

void
CSVG::
setCurrentBuffer(CSVGBuffer *buffer)
{
  buffer_ = buffer;
}

CSVGBuffer *
CSVG::
getBuffer(const std::string &name)
{
  return bufferMgr_->lookupBuffer(name, /*create*/true);
}

void
CSVG::
getBufferNames(std::vector<std::string> &names, bool includeAlpha) const
{
  bufferMgr_->getBufferNames(names, includeAlpha);
}

void
CSVG::
beginDrawBuffer(CSVGBuffer *buffer, const CPoint2D &offset, double xs, double ys)
{
  CSVGBlock *block = getRoot();

  CBBox2D pixelBox = block->calcPixelBox();
  CBBox2D viewBox  = block->calcViewBox ();

  beginDrawBuffer(buffer, pixelBox, viewBox, offset, xs, ys, blockPreserveAspect());
}

void
CSVG::
beginDrawBuffer(CSVGBuffer *buffer, const CBBox2D &viewBox)
{
  CSVGBlock *block = getRoot();

  CBBox2D pixelBox = block->calcPixelBox();

  beginDrawBuffer(buffer, pixelBox, viewBox,
                  rootBlockOffset(), rootBlockXScale(), rootBlockYScale(),
                  blockPreserveAspect());
}

void
CSVG::
beginDrawBuffer(CSVGBuffer *buffer, const CBBox2D &pixelBox, const CBBox2D &viewBox,
                const CPoint2D &offset, double xs, double ys,
                const CSVGPreserveAspect &preserveAspect)
{
  // save current block
  blockDataStack_.push_back(blockData_);

  // start new block
  CSVGBlockData blockData(pixelBox, viewBox, offset, xs, ys, preserveAspect);

  blockData_ = blockData;

  if (getenv("CSVG_DEBUG_BLOCK_DATA"))
    std::cerr << "beginDrawBuffer:" << std::endl << blockData_ << std::endl;

  //---

  CBBox2D bbox1 = blockData_.viewBBox();

  double w = bbox1.getWidth ();
  double h = bbox1.getHeight();

  bbox1.moveBy(flatOffset());

  double xscale = flatXScale();
  double yscale = flatYScale();

  buffer->beginDraw(w*xscale, h*yscale, bbox1);

  updateDrawBuffer(buffer);
}

void
CSVG::
endDrawBuffer(CSVGBuffer *buffer)
{
  // restore current block
  blockData_ = blockDataStack_.back();

  blockDataStack_.pop_back();

  if (getenv("CSVG_DEBUG_BLOCK_DATA"))
    std::cerr << "endDrawBuffer:" << std::endl << blockData_ << std::endl;

  //---

  buffer->endDraw();

  updateDrawBuffer(buffer);
}

void
CSVG::
updateDrawBuffer(CSVGBuffer *buffer)
{
  const CMatrixStack2D &viewMatrix = blockData_.viewMatrix();

  buffer->setViewMatrix(viewMatrix);

  buffer->setAlign(blockData_.preserveAspect().getHAlign(),
                   blockData_.preserveAspect().getVAlign());

  buffer->setEqualScale(blockData_.preserveAspect().getScale() != CSVGScale::FREE);
  buffer->setScaleMin  (blockData_.preserveAspect().getScale() == CSVGScale::FIXED_MEET);
}

//------

void
CSVG::
pushStyle(CSVGObject *object)
{
  styleDataStack_.push_back(styleData_);

  //---

  setStyleObject(object);

  if (object) {
    if (! object->getSelected())
      updateStroke(object->getStroke());
    else
      setSelectedStroke();
  }
  else
    resetStroke();

  if (object)
    updateFill(object->getFill());
  else
    resetFill();

  if (object)
    updateClip(object->getClip());
  else
    resetClip();

  if (object)
    updateFontDef(object->getFontDef());
  else
    resetFontDef();

  CScreenUnitsMgrInst->setEmSize(styleData_.fontDef.getSize().pxValue(1));

  double xSize;

  styleData_.fontDef.textSize("x", &xSize, 0, 0);

  CScreenUnitsMgrInst->setExSize(xSize);
}

void
CSVG::
popStyle()
{
  assert(! styleDataStack_.empty());

  styleData_ = styleDataStack_.back();

  styleDataStack_.pop_back();

  CScreenUnitsMgrInst->setEmSize(styleData_.fontDef.getSize().pxValue(1));

  double xSize;

  styleData_.fontDef.textSize("x", &xSize, 0, 0);

  CScreenUnitsMgrInst->setExSize(xSize);
}

void
CSVG::
resetStroke()
{
  styleData_.stroke.reset();
}

void
CSVG::
updateStroke(const CSVGStroke &stroke)
{
  styleData_.stroke.update(stroke);
}

void
CSVG::
setSelectedStroke()
{
  styleData_.stroke.setColor    (CSVGColor(CRGB(1,0,0)));
  styleData_.stroke.setWidth    (2);
  styleData_.stroke.setOpacity  (1);
  styleData_.stroke.setDashArray("solid");
}

bool
CSVG::
isStroked() const
{
  return (styleData_.stroke.isStroked());
}

void
CSVG::
setStrokeBuffer(CSVGBuffer *buffer)
{
  buffer->setStroke(styleData_.stroke);
}

//------

void
CSVG::
resetFill()
{
  styleData_.fill.reset();
}

void
CSVG::
updateFill(const CSVGFill &fill)
{
  styleData_.fill.update(fill);
}

bool
CSVG::
isFilled() const
{
  return styleData_.fill.isFilled();
}

void
CSVG::
setFillBuffer(CSVGBuffer *buffer)
{
  buffer->setFill(styleData_.fill);
}

CRGBA
CSVG::
colorToRGBA(const CSVGColor &color) const
{
  if (color.isRGBA())
    return color.rgba();

  CSVGObject *drawObject = currentDrawObject();

  if (drawObject)
    return drawObject->colorToRGBA(color);

  return CRGBA(0,0,0,0);
}

void
CSVG::
resetClip()
{
  styleData_.clip.reset();
}

void
CSVG::
updateClip(const CSVGClip &clip)
{
  if (clip.getRuleValid())
    styleData_.clip.setRule(clip.getRule());
}

bool
CSVG::
isClipped() const
{
  return true;
}

void
CSVG::
setClip()
{
}

void
CSVG::
resetFontDef()
{
  styleData_.fontDef.reset();
}

void
CSVG::
updateFontDef(const CSVGFontDef &fontDef)
{
  if (fontDef.hasFamily())
    styleData_.fontDef.setFamily(fontDef.getFamily());

  if (fontDef.hasSize())
    styleData_.fontDef.setSize(fontDef.getSize());

  if (fontDef.hasStyle())
    styleData_.fontDef.setStyle(fontDef.getStyle());

  double w, a, d;

  styleData_.fontDef.textSize("x", &w, &a, &d);

  CScreenUnitsMgrInst->setEmSize(a + d);
  CScreenUnitsMgrInst->setExSize(w);
}

void
CSVG::
setTransform(const CMatrixStack2D &matrix)
{
  buffer_->setTransform(matrix);
}

void
CSVG::
drawLine(double x1, double y1, double x2, double y2)
{
  setStrokeBuffer(buffer_);

  buffer_->pathInit();

  buffer_->pathMoveTo(x1, y1);
  buffer_->pathLineTo(x2, y2);

  buffer_->pathStroke();
}

void
CSVG::
drawCircle(double x, double y, double r)
{
  buffer_->pathInit();

  buffer_->pathMoveTo(x + r, y);
  buffer_->pathArcTo (x, y, r, r, 0,  M_PI);
  buffer_->pathArcTo (x, y, r, r, M_PI, 2*M_PI);

  buffer_->pathClose();

  if (isFilled() || isStroked()) {
    if (isFilled()) {
      setFillBuffer(buffer_);

      buffer_->pathFill();
    }

    if (isStroked()) {
      setStrokeBuffer(buffer_);

      buffer_->pathStroke();
    }
  }
  else {
    setFillBuffer(buffer_);

    buffer_->pathFill();
  }
}

void
CSVG::
drawEllipse(double x, double y, double rx, double ry)
{
  buffer_->pathInit();

  buffer_->pathMoveTo(x + rx, y);
  buffer_->pathArcTo(x, y, rx, ry, 0,  M_PI);
  buffer_->pathArcTo(x, y, rx, ry, M_PI, 2*M_PI);

  buffer_->pathClose();

  if (isFilled() || isStroked()) {
    if (isFilled()) {
      setFillBuffer(buffer_);

      buffer_->pathFill();
    }

    if (isStroked()) {
      setStrokeBuffer(buffer_);

      buffer_->pathStroke();
    }
  }
  else {
    setFillBuffer(buffer_);

    buffer_->pathFill();
  }
}

void
CSVG::
drawArc(double xc, double yc, double xr, double yr, double angle1, double angle2)
{
  setStrokeBuffer(buffer_);

  buffer_->pathInit();

  buffer_->pathArcTo(xc, yc, xr, yr, angle1, angle2);

  buffer_->pathStroke();
}

void
CSVG::
fillArc(double xc, double yc, double xr, double yr, double angle1, double angle2)
{
  setFillBuffer(buffer_);

  buffer_->pathInit();

  buffer_->pathArcTo(xc, yc, xr, yr, angle1, angle2);

  buffer_->pathFill();
}

void
CSVG::
drawPolygon(const std::vector<CPoint2D> &points)
{
  int num_points = points.size();

  if (! num_points)
    return;

  setStrokeBuffer(buffer_);

  buffer_->pathInit();

  buffer_->pathMoveTo(points[0].x, points[0].y);

  for (int i = 1; i < num_points; ++i)
    buffer_->pathLineTo(points[i].x, points[i].y);

  buffer_->pathClose();

  buffer_->pathStroke();

  //---

  std::vector<double> angles;

  for (int i1 = num_points - 1, i2 = 0; i2 < num_points; i1 = i2++) {
    double a = atan2(points[i2].y - points[i1].y, points[i2].x - points[i1].x);

    angles.push_back(a);
  }

  drawMarkers(points, angles);
}

void
CSVG::
fillPolygon(const std::vector<CPoint2D> &points)
{
  uint num_points = points.size();

  if (! num_points)
    return;

  setFillBuffer(buffer_);

  buffer_->pathInit();

  buffer_->pathMoveTo(points[0].x, points[0].y);

  for (uint i = 1; i < num_points; ++i)
    buffer_->pathLineTo(points[i].x, points[i].y);

  buffer_->pathClose();

  buffer_->pathFill();
}

void
CSVG::
fillDrawText(double x, double y, const std::string &text, const CSVGFontDef &fontDef,
             CHAlignType align, bool isFilled, bool isStroked)
{
  if (isFilled || isStroked) {
    if (isFilled)
      fillText(x, y, text, fontDef, align);

    if (isStroked)
      drawText(x, y, text, fontDef, align);
  }
  else
    fillText(x, y, text, fontDef, align);
}

void
CSVG::
drawText(double x, double y, const std::string &text, const CSVGFontDef &fontDef,
         CHAlignType align)
{
  setStrokeBuffer(buffer_);

  CSVGFont *svg_font = getFont();

  if (svg_font) {
    CSVGFontFace *font_face = svg_font->getFontFace();

    double units = 1000;

    if (font_face)
      units = font_face->getUnits();

    //-----

    CMatrixStack2D transform = buffer_->transform();

    double font_size = 10.0;

    transform.scale    (font_size/units, font_size/units);
    transform.translate(x, y);

    uint len = text.size();

    for (uint i = 0; i < len; ++i) {
      CSVGGlyph *glyph = getCharGlyph(text[i]);

      if (glyph) {
        setTransform(transform);

        // TODO: handle display "none"
        glyph->drawSubObject();

        x += font_size;

        transform.translate(x, y);
      }
    }

    setTransform(transform);
  }
  else {
    CMatrixStack2D transform = buffer_->transform();

    CMatrixStack2D transform1 = transform;

    if (fontDef.getAngle()) {
      transform1.rotate(fontDef.getAngle(), CPoint2D(x, y));

      setTransform(transform1);
    }

    //---

    buffer_->pathInit();

    buffer_->pathMoveTo(x, y);

    buffer_->pathText(text, fontDef, align);

    buffer_->pathClose();

    buffer_->pathStroke();

    //---

    setTransform(transform);
  }
}

void
CSVG::
fillText(double x, double y, const std::string &text, const CSVGFontDef &fontDef,
         CHAlignType align)
{
  setFillBuffer(buffer_);

  CSVGFont *svg_font = getFont();

  if (svg_font) {
    CSVGFontFace *font_face = svg_font->getFontFace();

    double units = 1000;

    if (font_face)
      units = font_face->getUnits();

    //-----

    CMatrixStack2D transform = buffer_->transform();

    double font_size = 10.0;

    transform.scale    (font_size/units, font_size/units);
    transform.translate(x, y);

    uint len = text.size();

    for (uint i = 0; i < len; ++i) {
      CSVGGlyph *glyph = getCharGlyph(text[i]);

      if (glyph) {
        setTransform(transform);

        // TODO: handle display "none"
        glyph->drawSubObject();

        x += font_size;

        transform.translate(x, y);
      }
    }

    setTransform(transform);
  }
  else {
    CMatrixStack2D transform = buffer_->transform();

    CMatrixStack2D transform1 = transform;

    if (fontDef.getAngle()) {
      transform1.rotate(fontDef.getAngle(), CPoint2D(x, y));

      setTransform(transform1);
    }

    //---

    buffer_->pathInit();

    buffer_->pathMoveTo(x, y);

    buffer_->pathText(text, fontDef, align);

    buffer_->pathClose();

    buffer_->pathFill();

    //---

    setTransform(transform);
  }
}

//--------------

bool
CSVG::
pathOption(const std::string &opt_name, const std::string &opt_value,
           const std::string &name, CSVGPathPartList &parts)
{
  if (opt_name != name)
    return false;

  if (! pathStringToParts(opt_value, parts))
    return false;

  return true;
}

bool
CSVG::
pathStringToParts(const std::string &data, CSVGPathPartList &parts)
{
  auto parseFlag = [](CStrParse &parse, int *i) -> bool {
    if (! parse.isChar('0') && ! parse.isChar('1'))
      return false;

    *i = (parse.getCharAt() - '0');

    parse.skipChar();

    return true;
  };

  //---

  char c = '\0', lastC = '\0';

  bool rereadCmd = false;

  CStrParse parse(data);

  parse.skipSpace();

  bool flag = true;

  while (! parse.eof()) {
    if (! rereadCmd)
      parse.readChar(&c);
    else
      rereadCmd = false;

    // relative move to
    if      (c == 'm') {
      lastC = c;

      double x, y;

      parse.skipSpace();

      if (! parse.readReal(&x)) { flag = false; break; }

      skipCommaSpace(parse);

      if (! parse.readReal(&y)) { flag = false; break; }

      skipCommaSpace(parse);

      parts.push_back(createPathRMoveTo(x, y));

      // TODO: leave to rereadCmd logic
      while (parse.isDigit() || parse.isChar('-') || parse.isChar('+') || parse.isChar('.')) {
        if (! parse.readReal(&x)) { flag = false; break; }

        skipCommaSpace(parse);

        if (! parse.readReal(&y)) { flag = false; break; }

        skipCommaSpace(parse);

        parts.push_back(createPathRLineTo(x, y));
      }
    }
    // absolute move to
    else if (c == 'M') {
      lastC = c;

      double x, y;

      parse.skipSpace();

      if (! parse.readReal(&x)) { flag = false; break; }

      skipCommaSpace(parse);

      if (! parse.readReal(&y)) { flag = false; break; }

      skipCommaSpace(parse);

      parts.push_back(createPathMoveTo(x, y));

      // TODO: leave to rereadCmd logic
      while (parse.isDigit() || parse.isChar('-') || parse.isChar('+') || parse.isChar('.')) {
        if (! parse.readReal(&x)) { flag = false; break; }

        skipCommaSpace(parse);

        if (! parse.readReal(&y)) { flag = false; break; }

        skipCommaSpace(parse);

        parts.push_back(createPathLineTo(x, y));
      }
    }
    // relative line to
    else if (c == 'l') {
      lastC = c;

      double x, y;

      parse.skipSpace();

      if (! parse.readReal(&x)) { flag = false; break; }

      skipCommaSpace(parse);

      if (! parse.readReal(&y)) { flag = false; break; }

      skipCommaSpace(parse);

      parts.push_back(createPathRLineTo(x, y));

      // TODO: leave to rereadCmd logic
      while (parse.isDigit() || parse.isChar('-') || parse.isChar('+') || parse.isChar('.')) {
        if (! parse.readReal(&x)) { flag = false; break; }

        skipCommaSpace(parse);

        if (! parse.readReal(&y)) { flag = false; break; }

        skipCommaSpace(parse);

        parts.push_back(createPathRLineTo(x, y));
      }
    }
    // absolute line to
    else if (c == 'L') {
      lastC = c;

      double x, y;

      parse.skipSpace();

      if (! parse.readReal(&x)) { flag = false; break; }

      skipCommaSpace(parse);

      if (! parse.readReal(&y)) { flag = false; break; }

      skipCommaSpace(parse);

      parts.push_back(createPathLineTo(x, y));

      // TODO: leave to rereadCmd logic
      while (parse.isDigit() || parse.isChar('-') || parse.isChar('+') || parse.isChar('.')) {
        if (! parse.readReal(&x)) { flag = false; break; }

        skipCommaSpace(parse);

        if (! parse.readReal(&y)) { flag = false; break; }

        skipCommaSpace(parse);

        parts.push_back(createPathLineTo(x, y));
      }
    }
    // relative hline to
    else if (c == 'h') {
      lastC = c;

      double d;

      parse.skipSpace();

      if (! parse.readReal(&d)) { flag = false; break; }

      parts.push_back(createPathRHLineTo(d));
    }
    // absolute hline to
    else if (c == 'H') {
      lastC = c;

      double d;

      parse.skipSpace();

      if (! parse.readReal(&d)) { flag = false; break; }

      parts.push_back(createPathHLineTo(d));
    }
    // relative vline to
    else if (c == 'v') {
      lastC = c;

      double d;

      parse.skipSpace();

      if (! parse.readReal(&d)) { flag = false; break; }

      parts.push_back(createPathRVLineTo(d));
    }
    // absolute vline to
    else if (c == 'V') {
      lastC = c;

      double d;

      parse.skipSpace();

      if (! parse.readReal(&d)) { flag = false; break; }

      parts.push_back(createPathVLineTo(d));
    }
    // relative arc to
    else if (c == 'a') {
      lastC = c;

      parse.skipSpace();

      // read (rx, ry)
      double rx, ry;

      if (! parse.readReal(&rx)) { flag = false; break; }

      skipCommaSpace(parse);

      if (! parse.readReal(&ry)) { flag = false; break; }

      skipCommaSpace(parse);

      // read xa
      double xa;

      if (! parse.readReal(&xa)) { flag = false; break; }

      skipCommaSpace(parse);

      // read fa, fs (single digit integers)
      int fa, fs;
      if (! parseFlag(parse, &fa)) { flag = false; break; }

      skipCommaSpace(parse);

      if (! parseFlag(parse, &fs)) { flag = false; break; }

      skipCommaSpace(parse);

      // read (x2, y2)
      double x2, y2;

      if (! parse.readReal(&x2)) { flag = false; break; }

      skipCommaSpace(parse);

      if (! parse.readReal(&y2)) { flag = false; break; }

      // add arc
      parts.push_back(createPathRArcTo(rx, ry, xa, fa, fs, x2, y2));

      skipCommaSpace(parse);

      // TODO: leave to rereadCmd logic
      while (parse.isDigit() || parse.isChar('-') || parse.isChar('+') || parse.isChar('.')) {
        // read (rx, ry)
        if (! parse.readReal(&rx)) { flag = false; break; }

        skipCommaSpace(parse);

        if (! parse.readReal(&ry)) { flag = false; break; }

        skipCommaSpace(parse);

        // read xa
        if (! parse.readReal(&xa)) { flag = false; break; }

        skipCommaSpace(parse);

        // read fa, fs (single digit integers)
        int fa, fs;

        if (! parseFlag(parse, &fa)) { flag = false; break; }

        skipCommaSpace(parse);

        if (! parseFlag(parse, &fs)) { flag = false; break; }

        skipCommaSpace(parse);

        // read (x2, y2)
        if (! parse.readReal(&x2)) { flag = false; break; }

        skipCommaSpace(parse);

        if (! parse.readReal(&y2)) { flag = false; break; }

        // add arc
        parts.push_back(createPathRArcTo(rx, ry, xa, fa, fs, x2, y2));

        parse.skipSpace();
      }
    }
    // absolute arc to
    else if (c == 'A') {
      lastC = c;

      int    fa, fs;
      double rx, ry, xa, x2, y2;

      parse.skipSpace();

      if (! parse.readReal(&rx)) { flag = false; break; }

      skipCommaSpace(parse);

      if (! parse.readReal(&ry)) { flag = false; break; }

      skipCommaSpace(parse);

      if (! parse.readReal(&xa)) { flag = false; break; }

      skipCommaSpace(parse);

      if (! parse.readInteger(&fa)) { flag = false; break; }

      skipCommaSpace(parse);

      if (! parse.readInteger(&fs)) { flag = false; break; }

      skipCommaSpace(parse);

      if (! parse.readReal(&x2)) { flag = false; break; }

      skipCommaSpace(parse);

      if (! parse.readReal(&y2)) { flag = false; break; }

      parts.push_back(createPathArcTo(rx, ry, xa, fa, fs, x2, y2));
    }
    // relative quadratic bezier to
    else if (c == 'q') {
      lastC = c;

      double x1, y1, x2, y2;

      parse.skipSpace();

      if (! parse.readReal(&x1)) { flag = false; break; }

      skipCommaSpace(parse);

      if (! parse.readReal(&y1)) { flag = false; break; }

      skipCommaSpace(parse);

      if (! parse.readReal(&x2)) { flag = false; break; }

      skipCommaSpace(parse);

      if (! parse.readReal(&y2)) { flag = false; break; }

      skipCommaSpace(parse);

      parts.push_back(createPathRBezier2To(x1, y1, x2, y2));

      // TODO: leave to rereadCmd logic
      while (parse.isDigit() || parse.isChar('-') || parse.isChar('+') || parse.isChar('.')) {
        if (! parse.readReal(&x1)) { flag = false; break; }

        skipCommaSpace(parse);

        if (! parse.readReal(&y1)) { flag = false; break; }

        skipCommaSpace(parse);

        if (! parse.readReal(&x2)) { flag = false; break; }

        skipCommaSpace(parse);

        if (! parse.readReal(&y2)) { flag = false; break; }

        skipCommaSpace(parse);

        parts.push_back(createPathRBezier2To(x1, y1, x2, y2));
      }
    }
    // absolute quadratic bezier to
    else if (c == 'Q') {
      lastC = c;

      double x1, y1, x2, y2;

      parse.skipSpace();

      if (! parse.readReal(&x1)) { flag = false; break; }

      skipCommaSpace(parse);

      if (! parse.readReal(&y1)) { flag = false; break; }

      skipCommaSpace(parse);

      if (! parse.readReal(&x2)) { flag = false; break; }

      skipCommaSpace(parse);

      if (! parse.readReal(&y2)) { flag = false; break; }

      skipCommaSpace(parse);

      parts.push_back(createPathBezier2To(x1, y1, x2, y2));

      // TODO: leave to rereadCmd logic
      while (parse.isDigit() || parse.isChar('-') || parse.isChar('+') || parse.isChar('.')) {
        if (! parse.readReal(&x1)) { flag = false; break; }

        skipCommaSpace(parse);

        if (! parse.readReal(&y1)) { flag = false; break; }

        skipCommaSpace(parse);

        if (! parse.readReal(&x2)) { flag = false; break; }

        skipCommaSpace(parse);

        if (! parse.readReal(&y2)) { flag = false; break; }

        skipCommaSpace(parse);

        parts.push_back(createPathBezier2To(x1, y1, x2, y2));
      }
    }
    // relative quadratic bezier to (with mirrored control point)
    else if (c == 't') {
      lastC = c;

      double x2, y2;

      parse.skipSpace();

      if (! parse.readReal(&x2)) { flag = false; break; }

      skipCommaSpace(parse);

      if (! parse.readReal(&y2)) { flag = false; break; }

      parts.push_back(createPathMRBezier2To(x2, y2));
    }
    // absolute quadratic bezier to (with mirrored control point)
    else if (c == 'T') {
      lastC = c;

      double x2, y2;

      parse.skipSpace();

      if (! parse.readReal(&x2)) { flag = false; break; }

      skipCommaSpace(parse);

      if (! parse.readReal(&y2)) { flag = false; break; }

      parts.push_back(createPathMBezier2To(x2, y2));
    }
    // relative cubic bezier to
    else if (c == 'c') {
      lastC = c;

      double x1, y1, x2, y2, x3, y3;

      parse.skipSpace();

      if (! parse.readReal(&x1)) { flag = false; break; }

      skipCommaSpace(parse);

      if (! parse.readReal(&y1)) { flag = false; break; }

      skipCommaSpace(parse);

      if (! parse.readReal(&x2)) { flag = false; break; }

      skipCommaSpace(parse);

      if (! parse.readReal(&y2)) { flag = false; break; }

      skipCommaSpace(parse);

      if (! parse.readReal(&x3)) { flag = false; break; }

      skipCommaSpace(parse);

      if (! parse.readReal(&y3)) { flag = false; break; }

      skipCommaSpace(parse);

      parts.push_back(createPathRBezier3To(x1, y1, x2, y2, x3, y3));

      // TODO: leave to rereadCmd logic
      while (parse.isDigit() || parse.isChar('-') || parse.isChar('+') || parse.isChar('.')) {
        if (! parse.readReal(&x1)) { flag = false; break; }

        skipCommaSpace(parse);

        if (! parse.readReal(&y1)) { flag = false; break; }

        skipCommaSpace(parse);

        if (! parse.readReal(&x2)) { flag = false; break; }

        skipCommaSpace(parse);

        if (! parse.readReal(&y2)) { flag = false; break; }

        skipCommaSpace(parse);

        if (! parse.readReal(&x3)) { flag = false; break; }

        skipCommaSpace(parse);

        if (! parse.readReal(&y3)) { flag = false; break; }

        skipCommaSpace(parse);

        parts.push_back(createPathRBezier3To(x1, y1, x2, y2, x3, y3));
      }
    }
    // absolute cubic bezier to
    else if (c == 'C') {
      lastC = c;

      double x1, y1, x2, y2, x3, y3;

      parse.skipSpace();

      if (! parse.readReal(&x1)) { flag = false; break; }

      skipCommaSpace(parse);

      if (! parse.readReal(&y1)) { flag = false; break; }

      skipCommaSpace(parse);

      if (! parse.readReal(&x2)) { flag = false; break; }

      skipCommaSpace(parse);

      if (! parse.readReal(&y2)) { flag = false; break; }

      skipCommaSpace(parse);

      if (! parse.readReal(&x3)) { flag = false; break; }

      skipCommaSpace(parse);

      if (! parse.readReal(&y3)) { flag = false; break; }

      skipCommaSpace(parse);

      parts.push_back(createPathBezier3To(x1, y1, x2, y2, x3, y3));

      // TODO: leave to rereadCmd logic
      while (parse.isDigit() || parse.isChar('-') || parse.isChar('+') || parse.isChar('.')) {
        if (! parse.readReal(&x1)) { flag = false; break; }

        skipCommaSpace(parse);

        if (! parse.readReal(&y1)) { flag = false; break; }

        skipCommaSpace(parse);

        if (! parse.readReal(&x2)) { flag = false; break; }

        skipCommaSpace(parse);

        if (! parse.readReal(&y2)) { flag = false; break; }

        skipCommaSpace(parse);

        if (! parse.readReal(&x3)) { flag = false; break; }

        skipCommaSpace(parse);

        if (! parse.readReal(&y3)) { flag = false; break; }

        skipCommaSpace(parse);

        parts.push_back(createPathBezier3To(x1, y1, x2, y2, x3, y3));
      }
    }
    // relative cubic bezier to (with mirrored control point)
    else if (c == 's') {
      lastC = c;

      double x2, y2, x3, y3;

      parse.skipSpace();

      if (! parse.readReal(&x2)) { flag = false; break; }

      skipCommaSpace(parse);

      if (! parse.readReal(&y2)) { flag = false; break; }

      skipCommaSpace(parse);

      if (! parse.readReal(&x3)) { flag = false; break; }

      skipCommaSpace(parse);

      if (! parse.readReal(&y3)) { flag = false; break; }

      parts.push_back(createPathMRBezier3To(x2, y2, x3, y3));
    }
    // absolute cubic bezier to (with mirrored control point)
    else if (c == 'S') {
      lastC = c;

      double x2, y2, x3, y3;

      parse.skipSpace();

      if (! parse.readReal(&x2)) { flag = false; break; }

      skipCommaSpace(parse);

      if (! parse.readReal(&y2)) { flag = false; break; }

      skipCommaSpace(parse);

      if (! parse.readReal(&x3)) { flag = false; break; }

      skipCommaSpace(parse);

      if (! parse.readReal(&y3)) { flag = false; break; }

      parts.push_back(createPathMBezier3To(x2, y2, x3, y3));
    }
    // relative close path
    else if (c == 'z') {
      lastC = c;

      parts.push_back(createPathClosePath(/*relative*/true));
    }
    // absolute close path
    else if (c == 'Z') {
      lastC = c;

      parts.push_back(createPathClosePath(/*relative*/false));
    }
    else {
      // repeat last command if any
      bool isNumber = false;

      if (c == '-' || c == '+' || c == '.') {
        if (isdigit(parse.getCharAt()))
          isNumber = true;
      }
      else if (isdigit(c)) {
        isNumber = true;
      }

      if (isNumber && lastC != '\0') {
        parse.unreadChar();

        c = lastC;

        rereadCmd = true;

        continue;
      }

      CSVGLog() << "Invalid path command '" << c << "'";

      flag = false;

      break;
    }

    skipCommaSpace(parse);
  }

  if (! flag) {
    CSVGLog() << "Path parse fail :";
    CSVGLog() << parse.getBefore() << ">" << parse.getAt() << "<" << parse.getAfter();
  }

  // stop at syntax error but still pass
  return true;
}

void
CSVG::
drawParts(const CSVGPathPartList &parts)
{
  std::vector<CPoint2D> points;
  std::vector<double>   angles;

  // add path parts and same path part points for markers (if any)
  buffer_->pathInit();

  parts.draw(buffer_, points, angles);

  // fill and/or stroke path
  if (isFilled() || isStroked()) {
    if (isFilled()) {
      setFillBuffer(buffer_);

      buffer_->pathFill();
    }

    if (isStroked()) {
      setStrokeBuffer(buffer_);

      buffer_->pathStroke();
    }
  }
  else {
    setFillBuffer(buffer_);

    buffer_->pathFill();
  }

  //------

  drawMarkers(points, angles);
}

void
CSVG::
drawMarkers(const std::vector<CPoint2D> &points, const std::vector<double> &angles)
{
  CSVGObject *drawObject = currentDrawObject();

  // get markers
  CSVGObject *startMarker = 0, *midMarker = 0, *endMarker = 0;

  if (drawObject) {
    startMarker = drawObject->getFlatMarkerStart();
    midMarker   = drawObject->getFlatMarkerMid  ();
    endMarker   = drawObject->getFlatMarkerEnd  ();
  }

  //------

  // draw markers
  if (startMarker || midMarker || endMarker) {
    uint num = points.size();

    if (num <= 0)
      return;

#if 0
    double x1 = points[0].x;
    double y1 = points[0].y;

    double x2 = x1;
    double y2 = y1;

    if (num > 1) {
      x2 = points[1].x; y2 = points[1].y;
    }

    double x3 = x2;
    double y3 = y2;

    if (num > 2) {
      x3 = points[2].x; y3 = points[2].y;
    }

    double g1 = atan2(y2 - y1, x2 - x1);
    double g2 = atan2(y3 - y2, x3 - x2);

    double gg = (g1 + g2)/2;

    if (startMarker) {
      CSVGMarker *marker = dynamic_cast<CSVGMarker *>(startMarker);

      if (marker)
        marker->drawMarker(x1, y1, gg);
    }

    for (uint i = 1; i < num; ++i) {
      x2 = points[i].x; y2 = points[i].y;

      if (i != num - 1) {
        x3 = points[i + 1].x; y3 = points[i + 1].y;
      }
      else {
        x3 = x2; y3 = y2;
      }

      g1 = atan2(y2 - y1, x2 - x1);
      g2 = atan2(y3 - y2, x3 - x2);

      double gg = (g1 + g2)/2;

      if (i != num - 1) {
        if (midMarker) {
          CSVGMarker *marker = dynamic_cast<CSVGMarker *>(midMarker);

          if (marker)
            marker->drawMarker(x2, y2, gg);
        }
      }
      else {
        if (endMarker) {
          CSVGMarker *marker = dynamic_cast<CSVGMarker *>(endMarker);

          if (marker)
            marker->drawMarker(x2, y2, g1);
        }
      }

      g1 = g2;

      x1 = x2; y1 = y2;
      x2 = x3; y2 = y3;
    }
#else
    if (startMarker) {
      CSVGMarker *marker = dynamic_cast<CSVGMarker *>(startMarker);

      if (marker)
        marker->drawMarker(points[0].x, points[0].y, angles[0]);
    }

    if (midMarker) {
      CSVGMarker *marker = dynamic_cast<CSVGMarker *>(midMarker);

      if (marker) {
        for (uint i = 1; i < num - 1; ++i)
          marker->drawMarker(points[i].x, points[i].y, angles[i]);
      }
    }

    if (endMarker) {
      CSVGMarker *marker = dynamic_cast<CSVGMarker *>(endMarker);

      if (marker)
        marker->drawMarker(points[num - 1].x, points[num - 1].y, angles[num - 1]);
    }
#endif
  }
}

//--------------

bool
CSVG::
coordOption(const std::string &opt_name, const std::string &opt_value,
            const std::string &name, CScreenUnits &length)
{
  if (opt_name != name)
    return false;

  double value;

  std::vector<std::string> match_strs;

  if (CRegExpUtil::parse(opt_value, "\\(.*\\)%", match_strs)) {
    if (! CStrUtil::toReal(match_strs[0], &value)) {
      CSVGLog() << "Illegal coord value " << match_strs[0];
      return false;
    }

    length = CScreenUnits(CScreenUnits::Units::PERCENT, value);
  }
  else {
    CScreenUnits length1;

    if (! decodeLengthValue(opt_value, length1)) {
      CSVGLog() << "Illegal coord value " << opt_value;
      return false;
    }

    length = length1;
  }

  return true;
}

bool
CSVG::
coordOption(const std::string &opt_name, const std::string &opt_value,
            const std::string &name, double *value)
{
  if (opt_name != name)
    return false;

  bool flag = true;

  std::vector<std::string> match_strs;

  if (CRegExpUtil::parse(opt_value, "\\(.*\\)%", match_strs)) {
    if (! CStrUtil::toReal(match_strs[0], value)) {
      *value = 0;
      flag   = false;
    }
    else
      *value /= 100;
  }
  else {
    CScreenUnits lvalue;

    if (! decodeLengthValue(opt_value, lvalue)) {
      CSVGLog() << "Illegal length value '" << opt_value << "' for " << name;
      *value = 0;
      flag   = false;
    }
    else
      *value = lvalue.pxValue();
  }

  return flag;
}

bool
CSVG::
lengthOption(const std::string &opt_name, const std::string &opt_value,
             const std::string &name, CScreenUnits &length)
{
  if (opt_name != name)
    return false;

  CScreenUnits length1;

  if (! decodeLengthValue(opt_value, length1)) {
    CSVGLog() << "Illegal length value '" << opt_value << "' for " << name;
    return false;
  }

  length = length1;

  return true;
}

bool
CSVG::
decodeLengthValue(const std::string &str, CScreenUnits &lvalue)
{
  static CRegExp em_pattern("\\(.*\\)em");
  static CRegExp ex_pattern("\\(.*\\)ex");
  static CRegExp pt_pattern("\\(.*\\)pt");
  static CRegExp pc_pattern("\\(.*\\)pc");
  static CRegExp cm_pattern("\\(.*\\)cm");
  static CRegExp mm_pattern("\\(.*\\)mm");
  static CRegExp in_pattern("\\(.*\\)in");
  static CRegExp px_pattern("\\(.*\\)px");
  static CRegExp ph_pattern("\\(.*\\)%");

  double value;

  std::vector<std::string> match_strs;

  if      (CRegExpUtil::parse(str, em_pattern, match_strs)) {
    if (! CStrUtil::toReal(match_strs[0], &value))
      return false;

    lvalue = CScreenUnits(CScreenUnits::Units::EM, value);

    //CSVGLog() << "em conversion not handled";
  }
  else if (CRegExpUtil::parse(str, ex_pattern, match_strs)) {
    if (! CStrUtil::toReal(match_strs[0], &value))
      return false;

    lvalue = CScreenUnits(CScreenUnits::Units::EX, value);

    //CSVGLog() << "ex conversion not handled";
  }
  else if (CRegExpUtil::parse(str, pt_pattern, match_strs)) {
    if (! CStrUtil::toReal(match_strs[0], &value))
      return false;

    //double ivalue = value;
    //mmToPixel((25.4*value)/72.0, &value);

    lvalue = CScreenUnits(CScreenUnits::Units::PT, value);
  }
  else if (CRegExpUtil::parse(str, pc_pattern, match_strs)) {
    if (! CStrUtil::toReal(match_strs[0], &value))
      return false;

    //double ivalue = value;
    //mmToPixel((25.4*value)/6.0, &value);

    lvalue = CScreenUnits(CScreenUnits::Units::PC, value);
  }
  else if (CRegExpUtil::parse(str, cm_pattern, match_strs)) {
    if (! CStrUtil::toReal(match_strs[0], &value))
      return false;

    //double ivalue = value;
    //mmToPixel(10*value, &value);

    lvalue = CScreenUnits(CScreenUnits::Units::CM, value);
  }
  else if (CRegExpUtil::parse(str, mm_pattern, match_strs)) {
    if (! CStrUtil::toReal(match_strs[0], &value))
      return false;

    //double ivalue = value;
    //mmToPixel(value, &value);

    lvalue = CScreenUnits(CScreenUnits::Units::MM, value);
  }
  else if (CRegExpUtil::parse(str, in_pattern, match_strs)) {
    if (! CStrUtil::toReal(match_strs[0], &value))
      return false;

    //double ivalue = value;
    //mmToPixel(25.4*value, &value);

    lvalue = CScreenUnits(CScreenUnits::Units::IN, value);
  }
  else if (CRegExpUtil::parse(str, px_pattern, match_strs)) {
    if (! CStrUtil::toReal(match_strs[0], &value))
      return false;

    lvalue = CScreenUnits(CScreenUnits::Units::PX, value);
  }
  else if (CRegExpUtil::parse(str, ph_pattern, match_strs)) {
    if (! CStrUtil::toReal(match_strs[0], &value)) {
      CSVGLog() << "Illegal coord value " << match_strs[0];
      return false;
    }

    lvalue = CScreenUnits(CScreenUnits::Units::PERCENT, value);
  }
  else {
    if (! CStrUtil::toReal(str, &value))
      return false;

    lvalue = CScreenUnits(CScreenUnits::Units::PX, value);
  }

  return true;
}

bool
CSVG::
angleOption(const std::string &opt_name, const std::string &opt_value,
            const std::string &name, double *value)
{
  // TODO: handle deg, grad, rad suffixes (default is degrees)
  if (opt_name != name)
    return false;

  if (! CStrUtil::toReal(opt_value, value)) {
    CSVGLog() << "Illegal angle value '" << opt_value << "' for " << name;
    return false;
  }

  return true;
}

// TODO: frequency (Hz, kHz)
// TODO: time (ms, s)

bool
CSVG::
realOption(const std::string &opt_name, const std::string &opt_value,
           const std::string &name, double *value)
{
  if (opt_name != name)
    return false;

  if (! CStrUtil::toReal(opt_value, value)) {
    CSVGLog() << "Illegal real value '" << opt_value << "' for " << name;
    return false;
  }

  return true;
}

bool
CSVG::
integerOption(const std::string &opt_name, const std::string &opt_value,
              const std::string &name, long *value)
{
  if (opt_name != name)
    return false;

  if (! CStrUtil::toInteger(opt_value, value)) {
    CSVGLog() << "Illegal integer value '" << opt_value << "' for " << name;
    return false;
  }

  return true;
}

bool
CSVG::
stringOption(const std::string &opt_name, const std::string &opt_value,
             const std::string &name, std::string &value)
{
  if (opt_name != name)
    return false;

  value = opt_value;

  return true;
}

bool
CSVG::
percentOption(const std::string &opt_name, const std::string &opt_value,
              const std::string &name, CScreenUnits &length)
{
  std::string str;

  if (! stringOption(opt_name, opt_value, name, str))
    return false;

  if (! decodePercentString(str, length))
    return false;

  return true;
}

bool
CSVG::
coordUnitsOption(const std::string &opt_name, const std::string &opt_value,
                 const std::string &name, CSVGCoordUnits &units)
{
  std::string str;

  if (! stringOption(opt_name, opt_value, name, str))
    return false;

  if (! decodeUnitsString(str, units))
    return false;

  return true;
}

bool
CSVG::
bboxOption(const std::string &opt_name, const std::string &opt_value,
           const std::string &name, CBBox2D *bbox)
{
  if (opt_name != name)
    return false;

  std::vector<std::string> words;

  CStrUtil::addWords(opt_value, words, " ,");

  if (words.size() != 4) {
    CSVGLog() << "Bad bbox " << opt_value;
    return false;
  }

  double x, y, w, h;

  if (! CStrUtil::toReal(CStrUtil::stripSpaces(words[0]), &x)) return false;
  if (! CStrUtil::toReal(CStrUtil::stripSpaces(words[1]), &y)) return false;
  if (! CStrUtil::toReal(CStrUtil::stripSpaces(words[2]), &w)) return false;
  if (! CStrUtil::toReal(CStrUtil::stripSpaces(words[3]), &h)) return false;

  *bbox = CBBox2D(x, y, x + w, y + h);

  return true;
}

bool
CSVG::
preserveAspectOption(const std::string &opt_name, const std::string &opt_value,
                     const std::string &name, CSVGPreserveAspect &preserveAspect)
{
  if (opt_name != name)
    return false;

  CHAlignType halign;
  CVAlignType valign;
  CSVGScale   scale;

  if (! decodePreserveAspectRatio(opt_value, &halign, &valign, &scale))
    return false;

  preserveAspect = CSVGPreserveAspect(halign, valign, scale);

  return true;
}

bool
CSVG::
pointListOption(const std::string &opt_name, const std::string &opt_value,
                const std::string &name, std::vector<CPoint2D> &points)
{
  if (opt_name != name)
    return false;

  std::vector<double> reals;

  CStrParse parse(opt_value);

  parse.skipSpace();

  while (! parse.eof()) {
    double r;

    if (! parse.readReal(&r)) {
      CSVGLog() << "Bad point list value " << opt_value;
      return false;
    }

    reals.push_back(r);

    parse.skipSpace();

    while (parse.isChar(',')) {
      parse.skipChar();

      parse.skipSpace();
    }
  }

  //---

  uint num_xy = reals.size()/2;

  for (uint i = 0, j = 0; j < num_xy; i += 2, ++j) {
    points.push_back(CPoint2D(reals[i], reals[i + 1]));
  }

  return true;
}

bool
CSVG::
realListOption(const std::string &opt_name, const std::string &opt_value,
               const std::string &name, std::vector<double> &reals)
{
  if (opt_name != name)
    return false;

  return stringToReals(opt_value, reals);
}

bool
CSVG::
stringToReals(const std::string &str, std::vector<double> &reals)
{
  std::vector<std::string> words;

  CStrUtil::addWords(str, words, " ,\n\t");

  uint num_reals = words.size();

  reals.resize(num_reals);

  for (uint i = 0; i < num_reals; ++i) {
    std::string word = CStrUtil::stripSpaces(words[i]);

    if (! CStrUtil::toReal(word, &reals[i])) {
      CSVGLog() << "Bad real value " << word;
      return false;
    }
  }

  return true;
}

bool
CSVG::
eventValueOption(const std::string &opt_name, const std::string &opt_value,
                 const std::string &name, CSVGEventValue &event)
{
  if (opt_name != name)
    return false;

  uint   pos = 0;
  double r   = 0.0;

  if (CStrUtil::readReal(opt_value, &pos, &r)) {
    CSVGTimeValue time;

    std::string units = opt_value.substr(pos);

    if      (units == "h")
      time = CSVGTimeValue(CSVGTimeValueType::HOURS, r);
    else if (units == "min")
      time = CSVGTimeValue(CSVGTimeValueType::MINUTES, r);
    else if (units == "s")
      time = CSVGTimeValue(CSVGTimeValueType::SECONDS, r);
    else if (units == "ms")
      time = CSVGTimeValue(CSVGTimeValueType::MILLISECONDS, r);
    else
      time = CSVGTimeValue(r);

    event = CSVGEventValue(time);
  }
  else {
    std::string id, eventName;

    auto p = opt_value.find('.');

    if (p != std::string::npos) {
      id        = CStrUtil::stripSpaces(opt_value.substr(0, p));
      eventName = CStrUtil::stripSpaces(opt_value.substr(p + 1));
    }
    else
      eventName = opt_value;

    auto p1 = eventName.find('+');

    std::string timeStr;

    if (p1 != std::string::npos) {
      timeStr   = CStrUtil::stripSpaces(eventName.substr(p1 + 1));
      eventName = CStrUtil::stripSpaces(eventName.substr(0, p1));
    }

    CSVGTimeValue time;

    if (timeStr != "") {
      if (! stringToTime(timeStr, time))
        return false;
    }

    std::string args;

    auto p2 = eventName.find('(');

    if (p2 != std::string::npos) {
      args      = CStrUtil::stripSpaces(eventName.substr(p2 + 1));
      eventName = CStrUtil::stripSpaces(eventName.substr(0, p2));

      auto p3 = args.find(')');

      if (p3 != std::string::npos)
        args = CStrUtil::stripSpaces(args.substr(0, p3));
    }

    if      (eventName == "click")
      event = CSVGEventValue(CSVGEventType::CLICK, id, time);
    else if (eventName == "mousedown")
      event = CSVGEventValue(CSVGEventType::MOUSE_DOWN, id, time);
    else if (eventName == "mouseup")
      event = CSVGEventValue(CSVGEventType::MOUSE_UP, id, time);
    else if (eventName == "mouseover")
      event = CSVGEventValue(CSVGEventType::MOUSE_OVER, id, time);
    else if (eventName == "mouseout")
      event = CSVGEventValue(CSVGEventType::MOUSE_OUT, id, time);
    else if (eventName == "begin")
      event = CSVGEventValue(CSVGEventType::ANIMATE_BEGIN, id, time);
    else if (eventName == "end")
      event = CSVGEventValue(CSVGEventType::ANIMATE_END, id, time);
    else if (eventName == "repeat")
      event = CSVGEventValue(CSVGEventType::ANIMATE_REPEAT, id, time, args);
    else
      return false;
  }

  return true;
}

bool
CSVG::
timeValueOption(const std::string &opt_name, const std::string &opt_value,
                const std::string &name, CSVGTimeValue &time)
{
  if (opt_name != name)
    return false;

  if (! stringToTime(opt_value, time))
    return false;

  return true;
}

bool
CSVG::
stringToTime(const std::string &str, CSVGTimeValue &time) const
{
  // Clock-val         ::= Full-clock-val | Partial-clock-val | Timecount-val
  // Full-clock-val    ::= Hours ":" Minutes ":" Seconds ("." Fraction)?
  // Partial-clock-val ::= Minutes ":" Seconds ("." Fraction)?
  // Timecount-val     ::= Timecount ("." Fraction)? (Metric)?
  // Metric            ::= "h" | "min" | "s" | "ms"
  // Hours             ::= DIGIT+; any positive number
  // Minutes           ::= 2DIGIT; range from 00 to 59
  // Seconds           ::= 2DIGIT; range from 00 to 59
  // Fraction          ::= DIGIT+
  // Timecount         ::= DIGIT+
  // 2DIGIT            ::= DIGIT DIGIT
  // DIGIT             ::= [0-9]

  uint   pos = 0;
  double r   = 0.0;

  if (! CStrUtil::readReal(str, &pos, &r))
    return false;

  std::string units = str.substr(pos);

  if      (units == "h")
    time = CSVGTimeValue(CSVGTimeValueType::HOURS, r);
  else if (units == "min")
    time = CSVGTimeValue(CSVGTimeValueType::MINUTES, r);
  else if (units == "s")
    time = CSVGTimeValue(CSVGTimeValueType::SECONDS, r);
  else if (units == "ms")
    time = CSVGTimeValue(CSVGTimeValueType::MILLISECONDS, r);
  else
    time = CSVGTimeValue(r);

  return true;
}

bool
CSVG::
transformOption(const std::string &opt_name, const std::string &opt_value,
                const std::string &name, CMatrixStack2D &matrixStack)
{
  if (opt_name != name)
    return false;

  if (! decodeTransform(opt_value, matrixStack))
    return false;

  return true;
}

bool
CSVG::
decodeTransform(const std::string &str, CMatrixStack2D &matrix)
{
  matrix.reset();

  CStrParse parse(str);

  std::string keyword;

  while (! parse.eof()) {
    parse.skipSpace();

    if (! parse.readIdentifier(keyword)) {
      CSVGLog() << "Invalid transform " << str;
      return false;
    }

    parse.skipSpace();

    if      (keyword == "scale") {
      if (! parse.isChar('(')) {
        CSVGLog() << "Invalid scale " << str;
        return false;
      }

      parse.skipChar();

      parse.skipSpace();

      double sx, sy;

      if (! parse.readReal(&sx)) {
        CSVGLog() << "Invalid scale " << str;
        return false;
      }

      skipCommaSpace(parse);

      bool isEqualScale = true;

      if (! parse.isChar(')')) {
        if (! parse.readReal(&sy)) {
          CSVGLog() << "Invalid scale " << str;
          return false;
        }

        parse.skipSpace();

        isEqualScale = false;
      }
      else
        sy = sx;

      if (! parse.isChar(')')) {
        CSVGLog() << "Invalid scale " << str;
        return false;
      }

      parse.skipChar();

      skipCommaSpace(parse);

      //------

      if (isEqualScale)
        matrix.scale(sx);
      else
        matrix.scale(sx, sy);
    }
    else if (keyword == "translate") {
      if (! parse.isChar('(')) {
        CSVGLog() << "Invalid translate " << str;
        return false;
      }

      parse.skipChar();

      parse.skipSpace();

      double dx = 0.0, dy = 0.0;

      if (! parse.isChar(')')) {
        if (! parse.readReal(&dx)) {
          CSVGLog() << "Invalid translate " << str;
          return false;
        }

        skipCommaSpace(parse);

        if (! parse.isChar(')')) {
          if (! parse.readReal(&dy)) {
            CSVGLog() << "Invalid translate " << str;
            return false;
          }

          parse.skipSpace();
        }
      }

      if (! parse.isChar(')')) {
        CSVGLog() << "Invalid translate " << str;
        return false;
      }

      parse.skipChar();

      skipCommaSpace(parse);

      //------

      matrix.translate(dx, dy);
    }
    else if (keyword == "rotate") {
      if (! parse.isChar('(')) {
        CSVGLog() << "Invalid rotate " << str;
        return false;
      }

      parse.skipChar();

      parse.skipSpace();

      double a;

      if (! parse.readReal(&a)) {
        CSVGLog() << "Invalid rotate " << str;
        return false;
      }

      parse.skipSpace();

      bool translate = false;

      double cx = 0.0, cy = 0.0;

      if (! parse.isChar(')')) {
        skipCommaSpace(parse);

        if (! parse.readReal(&cx)) {
          CSVGLog() << "Invalid rotate " << str;
          return false;
        }

        skipCommaSpace(parse);

        if (! parse.readReal(&cy)) {
          CSVGLog() << "Invalid rotate " << str;
          return false;
        }

        parse.skipSpace();

        translate = true;
      }

      if (! parse.isChar(')')) {
        CSVGLog() << "Invalid rotate " << str;
        return false;
      }

      parse.skipChar();

      skipCommaSpace(parse);

      //------

      if (translate)
        matrix.rotate(CMathGen::DegToRad(a), CPoint2D(cx, cy));
      else
        matrix.rotate(CMathGen::DegToRad(a));
    }
    else if (keyword == "skewX") {
      if (! parse.isChar('(')) {
        CSVGLog() << "Invalid skewX " << str;
        return false;
      }

      parse.skipChar();

      parse.skipSpace();

      double angle;

      if (! parse.readReal(&angle)) {
        CSVGLog() << "Invalid skewX " << str;
        return false;
      }

      parse.skipSpace();

      if (! parse.isChar(')')) {
        CSVGLog() << "Invalid skewX " << str;
        return false;
      }

      parse.skipChar();

      skipCommaSpace(parse);

      //------

      matrix.skewX(CMathGen::DegToRad(angle));
    }
    else if (keyword == "skewY") {
      if (! parse.isChar('(')) {
        CSVGLog() << "Invalid skewY " << str;
        return false;
      }

      parse.skipChar();

      parse.skipSpace();

      double angle;

      if (! parse.readReal(&angle)) {
        CSVGLog() << "Invalid skewY " << str;
        return false;
      }

      parse.skipSpace();

      if (! parse.isChar(')')) {
        CSVGLog() << "Invalid skewY " << str;
        return false;
      }

      parse.skipChar();

      skipCommaSpace(parse);

      //------

      matrix.skewY(CMathGen::DegToRad(angle));
    }
    else if (keyword == "matrix") {
      if (! parse.isChar('(')) {
        CSVGLog() << "Invalid matrix " << str;
        return false;
      }

      parse.skipChar();

      parse.skipSpace();

      // SVG is ((a c e) (b d f) (0 0 1))
      double m00, m01, m10, m11, tx, ty;

      if (! parse.readReal(&m00)) {
        CSVGLog() << "Invalid matrix " << str;
        return false;
      }

      skipCommaSpace(parse);

      if (! parse.readReal(&m10)) {
        CSVGLog() << "Invalid matrix " << str;
        return false;
      }

      skipCommaSpace(parse);

      if (! parse.readReal(&m01)) {
        CSVGLog() << "Invalid matrix " << str;
        return false;
      }

      skipCommaSpace(parse);

      if (! parse.readReal(&m11)) {
        CSVGLog() << "Invalid matrix " << str;
        return false;
      }

      skipCommaSpace(parse);

      if (! parse.readReal(&tx)) {
        CSVGLog() << "Invalid matrix " << str;
        return false;
      }

      skipCommaSpace(parse);

      if (! parse.readReal(&ty)) {
        CSVGLog() << "Invalid matrix " << str;
        return false;
      }

      parse.skipSpace();

      if (! parse.isChar(')')) {
        CSVGLog() << "Invalid matrix " << str;
        return false;
      }

      parse.skipChar();

      skipCommaSpace(parse);

      //------

      matrix.matrix(m00, m01, m10, m11, tx, ty);
    }
    else {
      CSVGLog() << "Unrecognised transform keyword " << keyword << " in " << str;
      return false;
    }
  }

  return true;
}

bool
CSVG::
decodePreserveAspectRatio(const std::string &str, CHAlignType *halign,
                          CVAlignType *valign, CSVGScale *scale)
{
  *halign = CHALIGN_TYPE_CENTER;
  *valign = CVALIGN_TYPE_CENTER;
  *scale  = CSVGScale::FIXED_MEET;

  std::vector<std::string> words;

  CStrUtil::addWords(str, words, " ,");

  CRegExp mmp("xM[ia][ndx]YM[ia][ndx]");

  for (const auto &word : words) {
    if (mmp.find(word)) {
      std::string lword = word.substr(0, 4);
      std::string rword = word.substr(4, 8);

      if      (lword == "xMin") *halign = CHALIGN_TYPE_LEFT;
      else if (lword == "xMid") *halign = CHALIGN_TYPE_CENTER;
      else if (lword == "xMax") *halign = CHALIGN_TYPE_RIGHT;

      if      (rword == "YMin") *valign = CVALIGN_TYPE_BOTTOM;
      else if (rword == "YMid") *valign = CVALIGN_TYPE_CENTER;
      else if (rword == "YMax") *valign = CVALIGN_TYPE_TOP;
    }
    else if (word == "meet")
      *scale = CSVGScale::FIXED_MEET;
    else if (word == "slice")
      *scale = CSVGScale::FIXED_SLICE;
    else if (word == "none")
      *scale = CSVGScale::FREE;
  }

  return true;
}

/*TODO*/
double
CSVG::
decodeWidthString(const std::string &width_str)
{
  double width = 1.0;

  std::vector<std::string> match_strs;

  if      (CRegExpUtil::parse(width_str, "\\(.*\\)pt", match_strs)) {
    if (! CStrUtil::toReal(match_strs[0], &width)) {
      CSVGLog() << "Illegal width value " << width_str;
      width = 1.0;
    }

    CScreenUnits units(width, CScreenUnits::Units::PT);

    width = units.pxValue();
    //mmToPixel(25.4*width/72.0, &width);
  }
  else if (CRegExpUtil::parse(width_str, "\\(.*\\)px", match_strs)) {
    if (! CStrUtil::toReal(match_strs[0], &width)) {
      CSVGLog() << "Illegal width value " << width_str;
      width = 1.0;
    }
  }
  else {
    if (! CStrUtil::toReal(width_str, &width)) {
      CSVGLog() << "Illegal width value " << width_str;
      width = 1.0;
    }
  }

  return width;
}

/*TODO*/
double
CSVG::
decodeOpacityString(const std::string &opacity_str)
{
  double opacity;

  if (! CStrUtil::toReal(opacity_str, &opacity))
    opacity = 1.0;

  return opacity;
}

CFillType
CSVG::
decodeFillRuleString(const std::string &rule_str) const
{
  if      (rule_str == "nonzero")
    return FILL_TYPE_WINDING;
  else if (rule_str == "evenodd")
    return FILL_TYPE_EVEN_ODD;
  else if (rule_str == "inherit")
    return FILL_TYPE_NONE;
  else
    return FILL_TYPE_NONE;
}

std::string
CSVG::
encodeFillRuleString(CFillType rule) const
{
  if      (rule == FILL_TYPE_WINDING)
    return "nonzero";
  else if (rule == FILL_TYPE_EVEN_ODD)
    return "evenodd";
  else if (rule == FILL_TYPE_NONE)
    return "inherit";
  else
    return "inherit";
}

bool
CSVG::
decodeDashString(const std::string &dash_str, std::vector<CScreenUnits> &lengths, bool &solid)
{
  solid = false;

  // solid, empty length list
  if (dash_str == "solid") {
    solid = true;
    return true;
  }

  if (dash_str == "none") {
    return true;
  }

  //---

  std::vector<std::string> words;

  CStrUtil::addWords(dash_str, words, " ,");

  uint num_words = words.size();

  bool duplicate = (num_words & 1);

  for (uint i = 0; i < num_words; ++i) {
    std::string word = CStrUtil::stripSpaces(words[i]);

    CScreenUnits length;

    if (! decodeLengthValue(word, length)) {
      CSVGLog() << "Bad dash length value " << word;
      continue;
    }

    lengths.push_back(length);
  }

  if (duplicate) {
    num_words = lengths.size();

    for (uint i = 0; i < num_words; ++i)
      lengths.push_back(lengths[i]);
  }

  return true;
}

bool
CSVG::
decodeColorString(const std::string &colorStr, CSVGColor &color)
{
  if      (colorStr == "none")
    color = CSVGColor(CSVGColor::Type::NONE);
  else if (colorStr == "currentColor")
    color = CSVGColor(CSVGColor::Type::CURRENT);
  else if (colorStr == "inherit")
    color = CSVGColor(CSVGColor::Type::INHERIT);
  else {
    CRGBA rgba;

    if (! decodeRGBAString(colorStr, rgba))
      return false;

    color = CSVGColor(rgba);

    colors_[colorStr] = rgba;
  }

  return true;
}

bool
CSVG::
decodeRGBAString(const std::string &colorStr, CRGBA &rgba)
{
  std::vector<std::string> match_strs;

  if      (CRegExpUtil::parse(colorStr, "rgb(\\(.*\\))", match_strs)) {
    double rgb[3];

    std::vector<std::string> words;

    CStrUtil::addWords(match_strs[0], words, " ,");

    while (words.size() < 3)
      words.push_back("0");

    for (uint i = 0; i < 3; ++i) {
      std::vector<std::string> match_strs1;

      if (CRegExpUtil::parse(words[i], "\\(.*\\)%", match_strs1)) {
        if (! CStrUtil::toReal(match_strs1[0], &rgb[i]))
          rgb[i] = 0;

        rgb[i] /= 100;
      }
      else {
        if (! CStrUtil::toReal(words[i], &rgb[i]))
          rgb[i] = 0;

        rgb[i] /= 255;
      }
    }

    rgba = CRGBA(rgb[0], rgb[1], rgb[2]);
  }
  else if (colorStr.size() == 4 &&
           CRegExpUtil::parse(colorStr, "#" RE_HEXDEC RE_HEXDEC RE_HEXDEC, match_strs)) {
    std::string color_str1 = "#";

    color_str1 += colorStr.substr(1, 1) + colorStr.substr(1, 1) +
                  colorStr.substr(2, 1) + colorStr.substr(2, 1) +
                  colorStr.substr(3, 1) + colorStr.substr(3, 1);

    rgba = nameToRGBA(color_str1);
  }
  else if (colorStr.size() == 7 &&
           CRegExpUtil::parse(colorStr, "#" RE_HEXDEC RE_HEXDEC RE_HEXDEC
                       RE_HEXDEC RE_HEXDEC RE_HEXDEC, match_strs)) {
    std::string color_str1 = "#";

    color_str1 += colorStr.substr(1, 2) +
                  colorStr.substr(3, 2) +
                  colorStr.substr(5, 2);

    rgba = nameToRGBA(color_str1);
  }
  else {
    if (colorStr == "none")
      rgba = CRGBA(0,0,0,0);
    else
      rgba = nameToRGBA(colorStr);
  }

  return true;
}

CSVGColor
CSVG::
nameToColor(const std::string &name) const
{
  if (name == "currentColor")
    return CSVGColor(CSVGColor::Type::CURRENT);

  return CSVGColor(nameToRGBA(name));
}

CRGBA
CSVG::
nameToRGBA(const std::string &name) const
{
  double r, g, b, a;

  if (CRGBName::lookupHtml(name, &r, &g, &b, &a))
    return CRGBA(r, g, b, a);

  // style like colors
  if      (CStrUtil::casecmp(name, "background") == 0)
    return CRGBName::toRGBA("#005c5c");
  else if (CStrUtil::casecmp(name, "appworkspace") == 0)
    return CRGBName::toRGBA("#005c5c");
  else if (CStrUtil::casecmp(name, "window") == 0)
    return CRGBName::toRGBA("#dfdfdf");
  else if (CStrUtil::casecmp(name, "windowtext") == 0)
    return CRGBA(0.00, 0.00, 0.00);
  else if (CStrUtil::casecmp(name, "windowframe") == 0)
    return CRGBName::toRGBA("#c4c4c4");
  else if (CStrUtil::casecmp(name, "highlighttext") == 0)
    return CRGBName::toRGBA("#c4c4c4");
  else if (CStrUtil::casecmp(name, "captiontext") == 0) // title bar text
    return CRGBA(0.00, 0.00, 0.00);
  else if (CStrUtil::casecmp(name, "menu") == 0)
    return CRGBName::toRGBA("#c4c4c4");
  else if (CStrUtil::casecmp(name, "menutext") == 0)
    return CRGBA(0.00, 0.00, 0.00);
  else if (CStrUtil::casecmp(name, "buttonface") == 0)
    return CRGBName::toRGBA("#dfdfdf");
  else if (CStrUtil::casecmp(name, "buttonhighlight") == 0)
    return CRGBName::toRGBA("#ffffff");
  else if (CStrUtil::casecmp(name, "buttonshadow") == 0)
    return CRGBName::toRGBA("#4a4a4a");
  else if (CStrUtil::casecmp(name, "activecaption") == 0)
    return CRGBName::toRGBA("#000080");
  else if (CStrUtil::casecmp(name, "activeborder") == 0)
    return CRGBName::toRGBA("#c4c4c4");
  else if (CStrUtil::casecmp(name, "threedface") == 0)
    return CRGBName::toRGBA("#dfdfdf");
  else if (CStrUtil::casecmp(name, "threedlightshadow") == 0)
    return CRGBName::toRGBA("#ebebeb");
  else if (CStrUtil::casecmp(name, "threeddarkshadow") == 0)
    return CRGBName::toRGBA("#4a4a4a");

  CSVGLog() << "Illegal color name '" << name << "'";

  return CRGBA(0,0,0);
}

CFontStyle
CSVG::
decodeFontWeightString(const std::string &font_weight_str)
{
  if (font_weight_str == "bold")
    return CFONT_STYLE_BOLD;
  else
    return CFONT_STYLE_NORMAL;
}

CFontStyle
CSVG::
decodeFontStyleString(const std::string &font_style_str)
{
  if      (font_style_str == "oblique")
    return CFONT_STYLE_ITALIC;
  else if (font_style_str == "italic")
    return CFONT_STYLE_ITALIC;
  else
    return CFONT_STYLE_NORMAL;
}

bool
CSVG::
decodePercentString(const std::string &str, CScreenUnits &length)
{
  double value;

  std::vector<std::string> match_strs;

  if (CRegExpUtil::parse(str, "\\(.*\\)%", match_strs)) {
    if (! CStrUtil::toReal(match_strs[0], &value)) {
      CSVGLog() << "Illegal coord value " << match_strs[0];
      return false;
    }

    length = CScreenUnits(CScreenUnits::Units::PERCENT, value);
  }
  else {
    if (! CStrUtil::toReal(str, &value))
      return false;

    length = CScreenUnits(CScreenUnits::Units::RATIO, value);
  }

  return true;
}

bool
CSVG::
decodeUnitsString(const std::string &str, CSVGCoordUnits &units)
{
  if      (str == "objectBoundingBox")
    units = CSVGCoordUnits::OBJECT_BBOX;
  else if (str == "userSpaceOnUse")
    units = CSVGCoordUnits::USER_SPACE;
  else if (str == "strokeWidth")
    units = CSVGCoordUnits::STROKE_WIDTH;
  else
    return false;

  return true;
}

std::string
CSVG::
encodeUnitsString(const CSVGCoordUnits &units)
{
  if      (units == CSVGCoordUnits::OBJECT_BBOX)
    return "objectBoundingBox";
  else if (units == CSVGCoordUnits::USER_SPACE)
    return "userSpaceOnUse";
  else if (units == CSVGCoordUnits::STROKE_WIDTH)
    return "strokeWidth";
  else
    return "??";
}

bool
CSVG::
decodeGradientSpread(const std::string &str, CGradientSpreadType &spread)
{
  if      (str == "pad")
    spread = CGRADIENT_SPREAD_PAD;
  else if (str == "repeat")
    spread = CGRADIENT_SPREAD_REPEAT;
  else if (str == "reflect")
    spread = CGRADIENT_SPREAD_REFLECT;
  else
    return false;

  return true;
}

std::string
CSVG::
encodeGradientSpread(const CGradientSpreadType &spread)
{
  if      (spread == CGRADIENT_SPREAD_PAD)
    return "pad";
  else if (spread == CGRADIENT_SPREAD_REPEAT)
    return "repeat";
  else if (spread == CGRADIENT_SPREAD_REFLECT)
    return "reflect";
  else
    return "??";
}

bool
CSVG::
urlOption(const std::string &opt_name, const std::string &opt_value,
          const std::string &name, CSVGObject **obj)
{
  if (opt_name != name)
    return false;

  *obj = 0;

  if (opt_value == "none")
    return true;

  std::string  id;
  CSVGObject  *obj1;

  if (! decodeUrlObject(opt_value, id, &obj1)) {
    CSVGLog() << "Illegal url value '" << id << "' for " << name;
    return true; // don't propagate waring
  }

  *obj = obj1;

  return true;
}

bool
CSVG::
decodeUrlObject(const std::string &str, std::string &id, CSVGObject **object)
{
  std::vector<std::string> match_strs;

  if      (CRegExpUtil::parse(str, "url(#\\(.*\\))", match_strs))
    id = match_strs[0];
  else if (str.size() > 0 && str[0] == '#')
    id = str.substr(1);
  else
    id = str;

  *object = lookupObjectById(id);

  return (*object != 0);
}

void
CSVG::
skipCommaSpace(CStrParse &parse)
{
  parse.skipSpace();

  while (parse.isChar(',')) {
    parse.skipChar();

    parse.skipSpace();
  }
}

CConfig *
CSVG::
getConfig()
{
  static CConfig config("svg");

  return &config;
}

bool
CSVG::
getTitle(std::string &str)
{
  std::vector<CSVGObject *> objects;

  getRoot()->getChildrenOfType(CSVGObjTypeId::TITLE, objects);

  if (objects.empty())
    return false;

  CSVGTitle *title = dynamic_cast<CSVGTitle *>(objects[0]);

  str = title->getText();

  return true;
}

CBBox2D
CSVG::
transformBBox(const CMatrixStack2D &m, const CBBox2D &bbox) const
{
  if (bbox.isSet()) {
    CPoint2D p1, p2, p3, p4;

    m.multiplyPoint(bbox.getLL(), p1);
    m.multiplyPoint(bbox.getLR(), p2);
    m.multiplyPoint(bbox.getUL(), p3);
    m.multiplyPoint(bbox.getUR(), p4);

    CBBox2D bbox1(p1, p2);

    bbox1 += p3;
    bbox1 += p4;

    return bbox1;
  }
  else
    return bbox;
}

void
CSVG::
lengthToPixel(double xi, double yi, double *xo, double *yo)
{
  double px1, py1, px2, py2;

  windowToPixel( 0,  0, &px1, &py1);
  windowToPixel(xi, yi, &px2, &py2);

  *xo = fabs(px2 - px1);
  *yo = fabs(py2 - py1);
}

void
CSVG::
windowToPixel(double xi, double yi, double *xo, double *yo)
{
#if 0
  CPoint2D pi(xi, yi);

  CPoint2D po;

  renderer_->windowToPixel(pi, po);

  *xo = po.x;
  *yo = po.y;
#endif

  //*xo = rootBlockXScale()*xi;
  //*yo = rootBlockYScale()*yi;
  *xo = flatXScale()*xi;
  *yo = flatYScale()*yi;
}

void
CSVG::
setObjectById(const std::string &id, CSVGObject *object)
{
  idObjectMap_[id] = object;
}

CSVGObject *
CSVG::
lookupObjectById(const std::string &id) const
{
  auto p = idObjectMap_.find(id);

  if (p != idObjectMap_.end())
    return (*p).second;

  return 0;
}

bool
CSVG::
readCSSFile(const std::string &filename)
{
  if (! css_.processFile(filename))
    return false;

  return processCSSIds();
}

bool
CSVG::
readCSSString(const std::string &str)
{
  if (! css_.processLine(str))
    return false;

  return processCSSIds();
}

bool
CSVG::
processCSSIds()
{
  std::vector<CCSS::Selector> selectors;

  css_.getSelectors(selectors);

  for (const auto &selector : selectors) {
    std::string id = selector.id();

    const CCSS::StyleData &cssStyleData = css_.getStyleData(selector);

    std::string objName, objType, objClass;

    auto p = id.find(".");

    if (p != std::string::npos) {
      objType  = id.substr(0, p);
      objClass = id.substr(p + 1);
    }
    else
      objType = id;

    if (objType.size() > 0 && objType[0] == '#') {
      objName = objType.substr(1);
      objType = "";
    }

    if (objName == "" && objType == "" && objClass == "")
      continue;

    if (objType  == "*") objType  = "";
    if (objClass == "*") objClass = "";

    if      (objName != "")
      addStyleValues(cssData_.getNameStyleData(objName), cssStyleData);
    else if (objType != "" && objClass == "")
      addStyleValues(cssData_.getTypeStyleData(objType), cssStyleData);
    else if (objType == "" && objClass != "")
      addStyleValues(cssData_.getClassStyleData(objClass), cssStyleData);
    else if (objType != "" && objClass  != "")
      addStyleValues(cssData_.getTypeClassStyleData(objType, objClass), cssStyleData);
    else
      addStyleValues(cssData_.getGlobalStyleData(), cssStyleData);
  }

  return true;
}

void
CSVG::
addStyleValues(CSVGStyleData &svgStyleData, const CCSS::StyleData &cssStyleData)
{
  for (const auto &option : cssStyleData.getOptions())
    svgStyleData.setValue(option.getName(), option.getValue());
}

//------

void
CSVG::
setScript(const std::string &str)
{
  js()->loadString(str);

  js()->exec();
}

//------

namespace {

template<typename VISITOR>
bool
visitStyleData(CSVGCSSData &cssData, const CSVGObject *obj, VISITOR &visitor)
{
  if (cssData.hasNameStyleData(obj->getId())) {
    CSVGStyleData &nameStyleData = cssData.getNameStyleData(obj->getId());

   if (visitor(CSVGCSSType::NAME, nameStyleData))
     return true;
  }

  for (const auto &c : obj->getClasses()) {
    if (cssData.hasTypeClassStyleData(obj->getTagName(), c)) {
      CSVGStyleData &typeClassStyleData = cssData.getTypeClassStyleData(obj->getTagName(), c);

      if (visitor(CSVGCSSType::TYPE_CLASS, typeClassStyleData))
        return true;
    }
  }

  if (cssData.hasTypeStyleData(obj->getTagName())) {
    CSVGStyleData &typeStyleData = cssData.getTypeStyleData(obj->getTagName());

    if (visitor(CSVGCSSType::TYPE, typeStyleData))
      return true;
  }

  for (const auto &c : obj->getClasses()) {
    if (cssData.hasClassStyleData(c)) {
      CSVGStyleData &classStyleData = cssData.getClassStyleData(c);

      if (visitor(CSVGCSSType::CLASS, classStyleData))
        return true;
    }
  }

  if (cssData.hasGlobalStyleData()) {
    CSVGStyleData &globalStyleData = cssData.getGlobalStyleData();

    if (visitor(CSVGCSSType::GLOBAL, globalStyleData))
      return true;
  }

  return false;
}

}

//------

bool
CSVG::
getStyleStrokeColor(const CSVGObject *obj, CSVGColor &color, CSVGCSSType &type)
{
  auto visitor = [&](CSVGCSSType styleType, CSVGStyleData &styleData) -> bool {
    if (styleData.getStrokeColorValid()) {
      color = styleData.getStrokeColor();
      type  = styleType;
      return true;
    }

    return false;
  };

  return visitStyleData(cssData_, obj, visitor);
}

bool
CSVG::
getStyleStrokeOpacity(const CSVGObject *obj, double &opacity, CSVGCSSType &type)
{
  auto visitor = [&](CSVGCSSType styleType, CSVGStyleData &styleData) -> bool {
    if (styleData.getStrokeOpacityValid()) {
      opacity = styleData.getStrokeOpacity();
      type    = styleType;
      return true;
    }

    return false;
  };

  return visitStyleData(cssData_, obj, visitor);
}

bool
CSVG::
getStyleStrokeRule(const CSVGObject *obj, CFillType &rule, CSVGCSSType &type)
{
  auto visitor = [&](CSVGCSSType styleType, CSVGStyleData &styleData) -> bool {
    if (styleData.getStrokeRuleValid()) {
      rule = styleData.getStrokeRule();
      type = styleType;
      return true;
    }

    return false;
  };

  return visitStyleData(cssData_, obj, visitor);
}

bool
CSVG::
getStyleStrokeUrl(const CSVGObject *obj, std::string &url, CSVGCSSType &type)
{
  auto visitor = [&](CSVGCSSType styleType, CSVGStyleData &styleData) -> bool {
    if (styleData.getStrokeUrlValid()) {
      url  = styleData.getStrokeUrl();
      type = styleType;
      return true;
    }

    return false;
  };

  return visitStyleData(cssData_, obj, visitor);
}

bool
CSVG::
getStyleStrokeFillObject(const CSVGObject *obj, CSVGObject* &fillObject, CSVGCSSType &type)
{
  auto visitor = [&](CSVGCSSType styleType, CSVGStyleData &styleData) -> bool {
    if (styleData.getStrokeFillObjectValid()) {
      fillObject = styleData.getStrokeFillObject();
      type       = styleType;
      return true;
    }

    return false;
  };

  return visitStyleData(cssData_, obj, visitor);
}

bool
CSVG::
getStyleStrokeWidth(const CSVGObject *obj, double &width, CSVGCSSType &type)
{
  auto visitor = [&](CSVGCSSType styleType, CSVGStyleData &styleData) -> bool {
    if (styleData.getStrokeWidthValid()) {
      width = styleData.getStrokeWidth();
      type  = styleType;
      return true;
    }

    return false;
  };

  return visitStyleData(cssData_, obj, visitor);
}

bool
CSVG::
getStyleStrokeDash(const CSVGObject *obj, CSVGStrokeDash &dash, CSVGCSSType &type)
{
  auto visitor = [&](CSVGCSSType styleType, CSVGStyleData &styleData) -> bool {
    if (styleData.getStrokeDashValid()) {
      dash = styleData.getStrokeDash();
      type = styleType;
      return true;
    }

    return false;
  };

  return visitStyleData(cssData_, obj, visitor);
}

bool
CSVG::
getStyleStrokeCap(const CSVGObject *obj, CLineCapType &cap, CSVGCSSType &type)
{
  auto visitor = [&](CSVGCSSType styleType, CSVGStyleData &styleData) -> bool {
    if (styleData.getStrokeLineCapValid()) {
      cap  = styleData.getStrokeLineCap();
      type = styleType;
      return true;
    }

    return false;
  };

  return visitStyleData(cssData_, obj, visitor);
}

bool
CSVG::
getStyleStrokeJoin(const CSVGObject *obj, CLineJoinType &join, CSVGCSSType &type)
{
  auto visitor = [&](CSVGCSSType styleType, CSVGStyleData &styleData) -> bool {
    if (styleData.getStrokeLineJoinValid()) {
      join = styleData.getStrokeLineJoin();
      type = styleType;
      return true;
    }

    return false;
  };

  return visitStyleData(cssData_, obj, visitor);
}

bool
CSVG::
getStyleStrokeMitreLimit(const CSVGObject *obj, double &limit, CSVGCSSType &type)
{
  auto visitor = [&](CSVGCSSType styleType, CSVGStyleData &styleData) -> bool {
    if (styleData.getStrokeMitreLimitValid()) {
      limit = styleData.getStrokeMitreLimit();
      type  = styleType;
      return true;
    }

    return false;
  };

  return visitStyleData(cssData_, obj, visitor);
}

//------

bool
CSVG::
getStyleFillColor(const CSVGObject *obj, CSVGColor &color, CSVGCSSType &type)
{
  auto visitor = [&](CSVGCSSType styleType, CSVGStyleData &styleData) -> bool {
    if (styleData.getFillColorValid()) {
      color = styleData.getFillColor();
      type  = styleType;
      return true;
    }

    return false;
  };

  return visitStyleData(cssData_, obj, visitor);
}

bool
CSVG::
getStyleFillOpacity(const CSVGObject *obj, double &opacity, CSVGCSSType &type)
{
  auto visitor = [&](CSVGCSSType styleType, CSVGStyleData &styleData) -> bool {
    if (styleData.getFillOpacityValid()) {
      opacity = styleData.getFillOpacity();
      type    = styleType;
      return true;
    }

    return false;
  };

  return visitStyleData(cssData_, obj, visitor);
}

bool
CSVG::
getStyleFillRule(const CSVGObject *obj, CFillType &rule, CSVGCSSType &type)
{
  auto visitor = [&](CSVGCSSType styleType, CSVGStyleData &styleData) -> bool {
    if (styleData.getFillRuleValid()) {
      rule = styleData.getFillRule();
      type = styleType;
      return true;
    }

    return false;
  };

  return visitStyleData(cssData_, obj, visitor);
}

bool
CSVG::
getStyleFillUrl(const CSVGObject *obj, std::string &url, CSVGCSSType &type)
{
  auto visitor = [&](CSVGCSSType styleType, CSVGStyleData &styleData) -> bool {
    if (styleData.getFillUrlValid()) {
      url  = styleData.getFillUrl();
      type = styleType;
      return true;
    }

    return false;
  };

  return visitStyleData(cssData_, obj, visitor);
}

bool
CSVG::
getStyleFillFillObject(const CSVGObject *obj, CSVGObject* &fillObject, CSVGCSSType &type)
{
  auto visitor = [&](CSVGCSSType styleType, CSVGStyleData &styleData) -> bool {
    if (styleData.getFillFillObjectValid()) {
      fillObject = styleData.getFillFillObject();
      type       = styleType;
      return true;
    }

    return false;
  };

  return visitStyleData(cssData_, obj, visitor);
}

bool
CSVG::
getStyleMarkerStart(const CSVGObject *obj, CSVGObject* &marker, CSVGCSSType &type)
{
  auto visitor = [&](CSVGCSSType styleType, CSVGStyleData &styleData) -> bool {
    if (styleData.getMarkerStart()) {
      marker  = styleData.getMarkerStart();
      type    = styleType;
      return true;
    }

    return false;
  };

  return visitStyleData(cssData_, obj, visitor);
}

bool
CSVG::
getStyleMarkerMid(const CSVGObject *obj, CSVGObject* &marker, CSVGCSSType &type)
{
  auto visitor = [&](CSVGCSSType styleType, CSVGStyleData &styleData) -> bool {
    if (styleData.getMarkerMid()) {
      marker  = styleData.getMarkerMid();
      type    = styleType;
      return true;
    }

    return false;
  };

  return visitStyleData(cssData_, obj, visitor);
}

bool
CSVG::
getStyleMarkerEnd(const CSVGObject *obj, CSVGObject* &marker, CSVGCSSType &type)
{
  auto visitor = [&](CSVGCSSType styleType, CSVGStyleData &styleData) -> bool {
    if (styleData.getMarkerEnd()) {
      marker  = styleData.getMarkerEnd();
      type    = styleType;
      return true;
    }

    return false;
  };

  return visitStyleData(cssData_, obj, visitor);
}

void
CSVG::
getAllChildren(ObjectList &objects) const
{
  getRoot()->getAllChildren(objects);
}

void
CSVG::
getObjectsAtPoint(const CPoint2D &p, ObjectList &objects) const
{
  getRoot()->getObjectsAtPoint(p, objects);
}

void
CSVG::
sendEvent(CSVGEventType type, const std::string &id, const std::string &data)
{
  getRoot()->handleEvent(type, id, data);
}

void
CSVG::
print(std::ostream &os, bool hier) const
{
  getRoot()->printRoot(os, css_, hier);
}

void
CSVG::
printFlat(std::ostream &os) const
{
  if (xmlStyleSheet_.isValid()) {
    os << "<?xml-stylesheet";

    if (xmlStyleSheet_.getValue().ref.isValid())
      os << " href=\"" << xmlStyleSheet_.getValue().ref.getValue() << "\"";

    if (xmlStyleSheet_.getValue().is_css.isValid())
      os << " type=\"text/css\"";

    os << "?>" << std::endl;
  }

  getRoot()->printFlat(os, -1);

  //css_.print(os);
}
