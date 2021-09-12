#include <CSVG.h>
#include <CSVGAnchor.h>
#include <CSVGAnimate.h>
#include <CSVGAnimateColor.h>
#include <CSVGAnimateMotion.h>
#include <CSVGAnimateTransform.h>
#include <CSVGAudio.h>
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
#include <CSVGMetaData.h>
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
#include <CSVGSwitch.h>
#include <CSVGSymbol.h>
#include <CSVGTBreak.h>
#include <CSVGText.h>
#include <CSVGTextPath.h>
#include <CSVGTitle.h>
#include <CSVGTSpan.h>
#include <CSVGUse.h>
#include <CSVGFontObj.h>
#include <CSVGObjectCSSTagData.h>

#include <CSVGJavaScript.h>
#include <CSVGUtil.h>

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

CSVG::NameToRGBA CSVG::colors_;

CSVG::
CSVG(CSVGRenderer *renderer) :
 renderer_ (renderer),
 styleData_(*this)
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

#ifdef CSVG_JAVASCRIPT
  js_ = createJavaScript();

  js_->init();
#endif
}

CSVG::
~CSVG()
{
  delete xmlTag_;
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
  if (altData_.isSet())
    return altData_.block;

  return getRootBlock();
}

CSVGBlock *
CSVG::
getRootBlock() const
{
  // get root block
  if (! block_.isValid()) {
    auto *th = const_cast<CSVG *>(this);

    th->block_ = th->createBlock();
  }

  return block_;
}

CPoint2D
CSVG::
flatOffset() const
{
  auto o = blockData_.offset();

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

  return nullptr;
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
    return nullptr;

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
setAltRoot(CSVGObject *o)
{
  if (altData_.isSet()) {
    // remove from alt block and add back to original parent
    altData_.block->deleteChildObject(altData_.object);

    altData_.parent->addChildObject(altData_.object);

    delete altData_.block;

    altData_.reset();
  }

  if (o) {
    // get current object postion

    //auto m   = o->getFlatTransform();
    //auto m1  = m.getMatrix();
    //auto im1 = m1.inverse();

    //double tx, ty;

    //m1.getTranslate(&tx, &ty);

    CBBox2D flatBBox;

    (void) o->getFlatTransformedBBox(flatBBox);

    double flatX = flatBBox.getXMin();
    double flatY = flatBBox.getYMin();

    double scale = 1.0;

    if (altSize_.isSet()) {
      double flatW = flatBBox.getWidth();
      double flatH = flatBBox.getHeight();

      double xs = altSize_.getWidth ()/flatW;
      double ys = altSize_.getHeight()/flatH;

      scale = std::min(xs, ys);
    }

    //---

    altData_.block  = createBlock();
    altData_.object = o;
    altData_.parent = o->getParent();

    altData_.parent->deleteChildObject(altData_.object);

    altData_.block->addChildObject(altData_.object);

    //---

    auto *block = getRootBlock();

    altData_.block->setX     (block->getX     ());
    altData_.block->setY     (block->getY     ());
    altData_.block->setWidth (block->getWidth ());
    altData_.block->setHeight(block->getHeight());

    //---

    // place block so object appears top left
    CMatrixStack2D altM;

    altM.scale(scale, scale);
    altM.translate(-flatX, -flatY);
    //altM.matrix(im1);

    altData_.block->setTransform(altM);
  }
}

void
CSVG::
setDebug(bool b)
{
  debugData_.debug = b;

  xml_->setDebug(getDebug());
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

  buffer_ = nullptr;

  rootBlockData_.reset();

  blockData_.reset();

  block_      = nullptr;
  xmlTag_     = nullptr;
  background_ = CRGBA(1, 1, 1);

  fontList_.clear();

  idObjectMap_.clear();
  cssList_    .clear();

  styleData_.reset();

  drawObjects_.clear();
}

bool
CSVG::
read(const std::string &filename)
{
  clear();

  return read(filename, getRootBlock());
}

bool
CSVG::
read(const std::string &filename, CSVGObject *rootBlock)
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
      auto *exec = token->getExecute();

      const auto &id = exec->getId();

      if (id == "xml-stylesheet") {
        CSVGXmlStyleSheet xmlStyleSheet;

        uint numOptions = exec->getNumOptions();

        for (uint j = 0; j < numOptions; ++j) {
          const auto &opt = exec->getOption(j);

          if      (opt.getName() == "href")
            xmlStyleSheet.ref = opt.getValue();
          else if (opt.getName() == "type")
            xmlStyleSheet.is_css = (opt.getValue() == "text/css");
        }

        if (xmlStyleSheet.is_css.getValue(false) && ! xmlStyleSheet.ref.getValue("").empty())
          readCSSFile(xmlStyleSheet.ref.getValue());

        xmlStyleSheet_ = xmlStyleSheet;
      }
    }
  }

  //------

  // Process svg options
  for (const auto &opt : xmlTag_->getOptions()) {
    const auto &optName  = opt->getName ();
    const auto &optValue = opt->getValue();

    if (! rootBlock->handleOption(optName, optValue)) {
      if (! isQuiet())
        CSVGLog() << "Invalid option '" << optName << "=" << optValue <<
                     " for " << rootBlock->getTagName();
    }
  }

  //------

  // Process svg children
  rootBlock->setXMLTag(xmlTag_);

  for (const auto &token : xmlTag_->getChildren()) {
    auto *object = tokenToObject(rootBlock, token);

    if (object)
      rootBlock->addChildObject(object);

    applyStyle(object);
  }

  //------

  rootBlock->execEvent(CSVGEventType::LOAD);

  return true;
}

CSVGObject *
CSVG::
tokenToObject(CSVGObject *parent, const CXMLToken *token)
{
  if (! token->isTag())
    return nullptr;

  auto *tag = token->getTag();

  //-----

  const std::string &tag_name = tag->getName();

  if (tag_name == "d:SVGTestCase")
    return nullptr;

  //-----

  // Create object from tag name
  auto *object = createObjectByName(tag_name);

  if (! object) {
    if (CRegExpUtil::parse(tag_name, "sodipodi:.*") ||
        CRegExpUtil::parse(tag_name, "inkscape:.*") ||
        CRegExpUtil::parse(tag_name, "rdf:.*"))
      return nullptr;

    CSVGLog() << "Unknown tag '" << tag_name << "'";

    return nullptr;
  }

  object->setParent(parent);

  //-----

  // inform object we are about to parse tag
  object->initParse();

  //-----

  // get tag text (including embedded tspan tags)
  std::string textStr;
  bool        hasTextSpan = false;

  for (const auto &ttoken : tag->getChildren()) {
    if      (ttoken->isText()) {
      auto *text = ttoken->getText();

      if (hasTextSpan) {
        auto *tspan = dynamic_cast<CSVGTSpan *>(object);

        if (! tspan) {
          tspan = createTSpan();

          object->addChildObject(tspan);
        }

        tspan->setText(text->getText());
      }
      else
        textStr += text->getText();
    }
    else if (ttoken->isTag() && ttoken->getTag()->getName() == "tspan") {
      if (! hasTextSpan && textStr != "") {
        auto *tspan = createTSpan();

        tspan->setText(textStr);

        object->addChildObject(tspan);
      }

      auto *object1 = tokenToObject(object, ttoken);
      assert(object1);

      object->addChildObject(object1);

      hasTextSpan = true;
    }
  }

  if (! hasTextSpan && textStr != "")
    object->setText(textStr);

  //-----

  // Process tag options
  for (const auto &option : tag->getOptions()) {
    const auto &optName  = option->getName ();
    const auto &optValue = option->getValue();

    if (! object->handleOption(optName, optValue)) {
      if (CRegExpUtil::parse(optName, "sodipodi:.*") ||
          CRegExpUtil::parse(optName, "inkscape:.*"))
        continue;

      if (! isQuiet())
        CSVGLog() << "Unhandled tag option " << optName << "=" << optValue <<
                     " for " << object->getTagName();
    }
  }

  object->setXMLTag(tag);

  //-----

  // process tag children
  for (const auto &ttoken : tag->getChildren()) {
    if (ttoken->isTag() && ttoken->getTag()->getName() == "tspan")
      continue;

    //---

    auto *object1 = tokenToObject(object, ttoken);

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
//else if (name == "altGlyphDef")
//  object = createAltGlyphDef();
//else if (name == "animation")
//  object = createAnimation();
  else if (name == "animate")
    object = createAnimate();
  else if (name == "animateColor")
    object = createAnimateColor();
  else if (name == "animateMotion")
    object = createAnimateMotion();
  else if (name == "animateTransform")
    object = createAnimateTransform();
  else if (name == "audio")
    object = createAudio();
  else if (name == "circle")
    object = createCircle();
  else if (name == "clipPath")
    object = createClipPath();
  else if (name == "color-profile")
    object = createColorProfile();
//else if (name == "cursor")
//  object = createCursor();
  else if (name == "defs")
    object = createDefs();
  else if (name == "desc")
    object = createDesc();
//else if (name == "discard")
//  object = createDiscard();
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
    object = createFeFunc(CRGBA_COMPONENT_RED);
  else if (name == "feFuncG")
    object = createFeFunc(CRGBA_COMPONENT_GREEN);
  else if (name == "feFuncB")
    object = createFeFunc(CRGBA_COMPONENT_BLUE);
  else if (name == "feFuncA")
    object = createFeFunc(CRGBA_COMPONENT_ALPHA);
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
//else if (name == "foreignObject")
//  object = createForeignObject();
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
  else if (name == "metadata")
    object = createMetaData();
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
//else if (name == "prefetch")
//  object = createPrefetch();
  else if (name == "radialGradient")
    object = createRadialGradient();
  else if (name == "rect")
    object = createRect();
  else if (name == "script")
    object = createScript();
  else if (name == "set")
    object = createSet();
//else if (name == "solidColor")
//  object = createSolidColor();
  else if (name == "stop")
    object = createStop();
  else if (name == "style")
    object = createStyle();
  else if (name == "switch")
    object = createSwitch();
  else if (name == "symbol")
    object = createSymbol();
  else if (name == "tbreak")
    object = createTBreak();
  else if (name == "text")
    object = createText();
//else if (name == "textArea")
//  object = createTextArea();
  else if (name == "textPath")
    object = createTextPath();
  else if (name == "title")
    object = createTitle();
  else if (name == "tspan")
    object = createTSpan();
  else if (name == "use")
    object = createUse();
//else if (name == "video")
//  object = createVideo();
//else if (name == "view")
//  object = createView();
  else
    object = nullptr;

  //------

  // automatically give object a non-empty name if required

  if (object && getAutoName())
    object->autoName();

  return object;
}

#ifdef CSVG_JAVASCRIPT
CSVGJavaScript *
CSVG::
createJavaScript()
{
  return new CSVGJavaScript(this);
}
#endif

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

CSVGAudio *
CSVG::
createAudio()
{
  return new CSVGAudio(*this);
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
createFeFunc(CRGBAComponent component)
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

CSVGMetaData *
CSVG::
createMetaData()
{
  return new CSVGMetaData(*this);
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

CSVGTBreak *
CSVG::
createTBreak()
{
  return new CSVGTBreak(*this);
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
    return nullptr;

  return fontList_[0];
}

CSVGGlyph *
CSVG::
getCharGlyph(char c) const
{
  auto *font = getFont();

  if (font)
    return font->getCharGlyph(c);
  else
    return nullptr;
}

CSVGGlyph *
CSVG::
getUnicodeGlyph(const std::string &unicode) const
{
  if (fontList_.empty())
    return nullptr;

  auto *font = fontList_[0];

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
#ifdef CSVG_JAVASCRIPT
  setEventObject(obj);

  js()->setProperty("evt", js()->event());

  js()->loadString(str);

  js()->exec();
#else
  assert(obj);

  std::cerr << "CSVG::execJsEvent " << str << "\n";
#endif
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

  const auto &viewMatrix = rootBlockData_.viewMatrix();

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

  auto *bgBuffer = getBuffer("BackgroundImage");

  setCurrentBuffer(bgBuffer);

  bgBuffer->clear();

  beginDrawBuffer(bgBuffer, rootBlockOffset(), rootBlockXScale(), rootBlockYScale());

  //------

  drawBackground();

  block->drawObject();

  drawForeground();

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

  auto *buffer = getBuffer(name);

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

  auto *buffer = bufferStack_.back();

  buffer->setParentBuffer(nullptr);

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
  auto *block = getRoot();

  auto pixelBox = block->calcPixelBox();
  auto viewBox  = block->calcViewBox ();

  beginDrawBuffer(buffer, pixelBox, viewBox, offset, xs, ys, blockPreserveAspect());

  if (block->hasViewportFillColor()) {
    auto c = block->viewportFillColor().getValue().rgba();

    if (block->getViewportFillOpacityValid() && ! block->getViewportFillOpacity().isInherit())
      c.setAlpha(block->getViewportFillOpacity().getValue());

    buffer->clear(c);
  }
}

void
CSVG::
beginDrawBuffer(CSVGBuffer *buffer, const CBBox2D &viewBox)
{
  auto *block = getRoot();

  auto pixelBox = block->calcPixelBox();

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

  auto bbox1 = blockData_.viewBBox();

  //double w = bbox1.getWidth ();
  //double h = bbox1.getHeight();
  double w = pixelBox.getWidth ();
  double h = pixelBox.getHeight();

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
  const auto &viewMatrix = blockData_.viewMatrix();

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
    if (! object->getSelected() || ! hasSelectedStroke())
      updateStroke(object->getStroke());
    else
      updatedSelectedStroke();
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

  CScreenUnitsMgrInst->setEmSize(styleData_.fontDef.getSize().getValue().pxValue(1));

  double xSize;

  styleData_.fontDef.textSize("x", &xSize, nullptr, nullptr);

  CScreenUnitsMgrInst->setExSize(xSize);
}

void
CSVG::
popStyle()
{
  assert(! styleDataStack_.empty());

  styleData_ = styleDataStack_.back();

  styleDataStack_.pop_back();

  CScreenUnitsMgrInst->setEmSize(styleData_.fontDef.getSize().getValue().pxValue(1));

  double xSize;

  styleData_.fontDef.textSize("x", &xSize, nullptr, nullptr);

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
updatedSelectedStroke()
{
  auto c = CRGB(1, 0, 0);

  styleData_.stroke.setColor    (Color(CSVGColor(c)));
  styleData_.stroke.setWidth    (Width(2));
  styleData_.stroke.setOpacity  (Opacity(1));
  styleData_.stroke.setDashArray("solid");
}

bool
CSVG::
isStroked() const
{
  return getStroke().isStroked();
}

void
CSVG::
setStrokeBuffer(CSVGBuffer *buffer)
{
  buffer->setStroke(getStroke());
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
  return getFill().isFilled();
}

void
CSVG::
setFillBuffer(CSVGBuffer *buffer)
{
  buffer->setFill(getFill());
}

CRGBA
CSVG::
colorToRGBA(const Color &color) const
{
  CSVGColor c;

  if (! color.isInherit())
    c = color.getValue();

  if (c.isRGBA())
    return c.rgba();

  auto *drawObject = currentDrawObject();

  if (drawObject)
    return drawObject->colorToRGBA(color);

  return CRGBA(0, 0, 0, 0);
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

//---

void
CSVG::
setTransform(const CMatrixStack2D &matrix)
{
  if (buffer_)
    buffer_->setTransform(matrix);
}

//---

void
CSVG::
drawLine(double x1, double y1, double x2, double y2)
{
  auto *buffer = getCurrentBuffer();

  buffer->drawLine(x1, y1, x2, y2);
}

void
CSVG::
drawCircle(double x, double y, double r)
{
  auto *buffer = getCurrentBuffer();

  buffer->drawCircle(x, y, r);
}

void
CSVG::
drawEllipse(double x, double y, double rx, double ry)
{
  auto *buffer = getCurrentBuffer();

  buffer->drawEllipse(x, y, rx, ry);
}

void
CSVG::
drawArc(double xc, double yc, double xr, double yr, double angle1, double angle2)
{
  auto *buffer = getCurrentBuffer();

  buffer->drawArc(xc, yc, xr, yr, angle1, angle2);
}

void
CSVG::
fillArc(double xc, double yc, double xr, double yr, double angle1, double angle2)
{
  auto *buffer = getCurrentBuffer();

  buffer->fillArc(xc, yc, xr, yr, angle1, angle2);
}

void
CSVG::
drawPolygon(const std::vector<CPoint2D> &points)
{
  auto *buffer = getCurrentBuffer();

  buffer->drawPolygon(points);
}

void
CSVG::
fillPolygon(const std::vector<CPoint2D> &points)
{
  auto *buffer = getCurrentBuffer();

  buffer->fillPolygon(points);
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
  auto *buffer = getCurrentBuffer();

  buffer->drawText(x, y, text, fontDef, align);
}

void
CSVG::
fillText(double x, double y, const std::string &text, const CSVGFontDef &fontDef,
         CHAlignType align)
{
  auto *buffer = getCurrentBuffer();

  buffer->fillText(x, y, text, fontDef, align);
}

//--------------

bool
CSVG::
pathOption(const std::string &optName, const std::string &optValue,
           const std::string &name, CSVGPathPartList &parts)
{
  if (optName != name)
    return false;

  if (! pathStringToParts(optValue, parts))
    return false;

  return true;
}

bool
CSVG::
pathStringToParts(const std::string &data, CSVGPathPartList &parts)
{
  class PathVisitor : public CSVGUtil::PathVisitor {
   public:
    PathVisitor(CSVG *svg) :
     svg_(svg) {
    }

    const CSVGPathPartList &parts() const { return parts_; }

    void moveTo(double x, double y) override {
      parts_.push_back(svg_->createPathMoveTo(x, y));
    }

    void rmoveTo(double dx, double dy) override {
      parts_.push_back(svg_->createPathRMoveTo(dx, dy));
    }

    void lineTo(double x, double y) override {
      parts_.push_back(svg_->createPathLineTo(x, y));
    }

    void rlineTo(double dx, double dy) override {
      parts_.push_back(svg_->createPathRLineTo(dx, dy));
    }

    void hlineTo(double x) override {
      parts_.push_back(svg_->createPathHLineTo(x));
    }

    void vlineTo(double y) override {
      parts_.push_back(svg_->createPathVLineTo(y));
    }

    void rhlineTo(double dx) override {
      parts_.push_back(svg_->createPathRHLineTo(dx));
    }

    void rvlineTo(double dy) override {
      parts_.push_back(svg_->createPathRVLineTo(dy));
    }

    void arcTo(double rx, double ry, double xa, int fa, int fs, double x2, double y2) override {
      parts_.push_back(svg_->createPathArcTo(rx, ry, xa, fa, fs, x2, y2));
    }

    void rarcTo(double rx, double ry, double xa, int fa, int fs, double dx2, double dy2) override {
      parts_.push_back(svg_->createPathRArcTo(rx, ry, xa, fa, fs, dx2, dy2));
    }

    void bezier2To(double x1, double y1, double x2, double y2) override {
      parts_.push_back(svg_->createPathBezier2To(x1, y1, x2, y2));
    }

    void rbezier2To(double dx1, double dy1, double dx2, double dy2) override {
      parts_.push_back(svg_->createPathRBezier2To(dx1, dy1, dx2, dy2));
    }

    void bezier3To(double x1, double y1, double x2, double y2, double x3, double y3) override {
      parts_.push_back(svg_->createPathBezier3To(x1, y1, x2, y2, x3, y3));
    }

    void rbezier3To(double dx1, double dy1, double dx2, double dy2,
                    double dx3, double dy3) override {
      parts_.push_back(svg_->createPathRBezier3To(dx1, dy1, dx2, dy2, dx3, dy3));
    }

    void mbezier2To(double x2, double y2) override {
      parts_.push_back(svg_->createPathMBezier2To(x2, y2));
    }

    void mrbezier2To(double dx2, double dy2) override {
      parts_.push_back(svg_->createPathMRBezier2To(dx2, dy2));
    }

    void mbezier3To(double x2, double y2, double x3, double y3) override {
      parts_.push_back(svg_->createPathMBezier3To(x2, y2, x3, y3));
    }

    void mrbezier3To(double dx2, double dy2, double dx3, double dy3) override {
      parts_.push_back(svg_->createPathMRBezier3To(dx2, dy2, dx3, dy3));
    }

    void closePath(bool relative) override {
      parts_.push_back(svg_->createPathClosePath(relative));
    }

    void handleError(const std::string &preStr, const std::string &atStr,
                     const std::string &postStr) const override {
      CSVGLog() << "Path parse fail :";
      CSVGLog() << preStr << ">" << atStr << "<" << postStr;
    }

   private:
    CSVG*            svg_ { nullptr };
    CSVGPathPartList parts_;
  };

  //---

  PathVisitor pathVisitor(this);

  if (! CSVGUtil::visitPath(data, pathVisitor)) {
    // TODO: flag failure
  }

  parts = pathVisitor.parts();

  // stop at syntax error but still pass
  return true;
}

void
CSVG::
drawParts(const CSVGPathPartList &parts)
{
  auto *buffer = getCurrentBuffer();

  buffer->drawParts(parts);
}

void
CSVG::
drawMarkers(const std::vector<CPoint2D> &points, const std::vector<double> &angles)
{
  auto *buffer = getCurrentBuffer();

  buffer->drawMarkers(points, angles);
}

//--------------

bool
CSVG::
coordListOption(const std::string &optName, const std::string &optValue,
                const std::string &name, std::vector<CScreenUnits> &lengths)
{
  if (optName != name)
    return false;

  CStrParse parse(optValue);

  parse.skipSpace();

  while (! parse.eof()) {
    int pos = parse.getPos();

    while (! parse.eof() && ! parse.isSpace() && ! parse.isChar(','))
      parse.skipChar();

    std::string str = parse.getBefore(pos);

    CScreenUnits length;

    if (! decodeCoordValue(str, length)) {
      std::string msg = "Illegal coord list value " + str;
      syntaxError(msg);
      return false;
    }

    lengths.push_back(length);

    parse.skipSpace();

    while (parse.isChar(',')) {
      parse.skipChar();

      parse.skipSpace();
    }
  }

  return true;
}

bool
CSVG::
coordOption(const std::string &optName, const std::string &optValue,
            const std::string &name, CScreenUnits &length)
{
  if (optName != name)
    return false;

  CScreenUnits length1;

  if (! decodeCoordValue(optValue, length1)) {
    std::string msg = "Illegal coord value " + optValue;
    syntaxError(msg);
    return false;
  }

  length = length1;

  return true;
}

bool
CSVG::
coordOption(const std::string &optName, const std::string &optValue,
            const std::string &name, double *value)
{
  CScreenUnits length;

  if (! coordOption(optName, optValue, name, length))
    return false;

  *value = length.pxValue();

  return true;
}

bool
CSVG::
decodeCoordValue(const std::string &str, CScreenUnits &length)
{
  double value;

  std::vector<std::string> matchStrs;

  if (CRegExpUtil::parse(str, "\\(.*\\)%", matchStrs)) {
    if (! CStrUtil::toReal(matchStrs[0], &value))
      return false;

    length = CScreenUnits(CScreenUnits::Units::PERCENT, value);
  }
  else {
    auto optLength = decodeLengthValue(str);

    if (! optLength.isValid())
      return false;

    length = optLength.getValue();
  }

  return true;
}

//--------------

bool
CSVG::
lengthListOption(const std::string &optName, const std::string &optValue,
                 const std::string &name, std::vector<CScreenUnits> &lengths)
{
  if (optName != name)
    return false;

  if (! decodeLengthListValue(optValue, lengths)) {
    std::string msg = "Illegal length list value " + optValue;
    syntaxError(msg);
    return false;
  }

  return true;
}

bool
CSVG::
decodeLengthListValue(const std::string &str, std::vector<CScreenUnits> &lengths)
{
  CStrParse parse(str);

  parse.skipSpace();

  while (! parse.eof()) {
    int pos = parse.getPos();

    while (! parse.eof() && ! parse.isSpace() && ! parse.isChar(','))
      parse.skipChar();

    std::string str1 = parse.getBefore(pos);

    auto optLength = decodeLengthValue(str1);

    if (! optLength.isValid())
      return false;

    lengths.push_back(optLength.getValue());

    parse.skipSpace();

    while (parse.isChar(',')) {
      parse.skipChar();

      parse.skipSpace();
    }
  }

  return true;
}

bool
CSVG::
fontFamilyOption(const std::string &optName, const std::string &optValue,
                 const std::string &name, std::string &family, bool &inherit)
{
  inherit = false;

  if (optName != name)
    return false;

  if (optValue == "inherit")
    inherit = true;
  else
    family = optValue;

  return true;
}

bool
CSVG::
fontSizeOption(const std::string &optName, const std::string &optValue,
               const std::string &name, CScreenUnits &length, bool &inherit)
{
  inherit = false;

  if (optName != name)
    return false;

  if      (optValue == "xx-small")
    length = CScreenUnits(CScreenUnits::Units::PX, 6); // TODO
  else if (optValue == "x-small")
    length = CScreenUnits(CScreenUnits::Units::PX, 8); // TODO
  else if (optValue == "small")
    length = CScreenUnits(CScreenUnits::Units::PX, 10); // TODO
  else if (optValue == "medium")
    length = CScreenUnits(CScreenUnits::Units::PX, 12); // TODO
  else if (optValue == "large")
    length = CScreenUnits(CScreenUnits::Units::PX, 14); // TODO
  else if (optValue == "x-large")
    length = CScreenUnits(CScreenUnits::Units::PX, 16); // TODO
  else if (optValue == "xx-large")
    length = CScreenUnits(CScreenUnits::Units::PX, 18); // TODO
  else if (optValue == "larger")
    length = CScreenUnits(CScreenUnits::Units::PX, 14); // TODO
  else if (optValue == "smaller")
    length = CScreenUnits(CScreenUnits::Units::PX, 10); // TODO
  else if (optValue == "inherit")
    inherit = true;
  else {
    auto optLength = decodeLengthValue(optValue);

    if (! optLength.isValid()) {
      std::string msg = "Illegal font size value '" + optValue + "' for " + name;
      syntaxError(msg);
      return false;
    }

    length = optLength.getValue();
  }

  return true;
}

bool
CSVG::
letterSpacingOption(const std::string &optName, const std::string &optValue,
                    const std::string &name, CScreenUnits &length)
{
  if (optName != name)
    return false;

  if      (optValue == "normal") {
    // TODO
  }
  else if (optValue == "inherit") {
    // TODO
  }
  else {
    auto optLength = decodeLengthValue(optValue);

    if (! optLength.isValid()) {
      std::string msg = "Illegal font size value '" + optValue + "' for " + name;
      syntaxError(msg);
      return false;
    }

    length = optLength.getValue();
  }

  return true;
}

bool
CSVG::
wordSpacingOption(const std::string &optName, const std::string &optValue,
                  const std::string &name, CScreenUnits &length)
{
  if (optName != name)
    return false;

  if      (optValue == "normal") {
    // TODO
  }
  else if (optValue == "inherit") {
    // TODO
  }
  else {
    auto optLength = decodeLengthValue(optValue);

    if (! optLength.isValid()) {
      std::string msg = "Illegal font size value '" + optValue + "' for " + name;
      syntaxError(msg);
      return false;
    }

    length = optLength.getValue();
  }

  return true;
}

bool
CSVG::
lengthOption(const std::string &optName, const std::string &optValue,
             const std::string &name, CScreenUnits &length)
{
  if (optName != name)
    return false;

  auto optLength = decodeLengthValue(optValue);

  if (! optLength.isValid()) {
    std::string msg = "Illegal length value '" + optValue + "' for " + name;
    syntaxError(msg);
    return false;
  }

  length = optLength.getValue();

  return true;
}

COptValT<CScreenUnits>
CSVG::
decodeLengthValue(const std::string &str)
{
  CScreenUnits lvalue;

  // TODO:  ch, rem, vw, vh, vmin, vmax

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

  std::vector<std::string> matchStrs;

  // em, font size of the element
  if      (CRegExpUtil::parse(str, em_pattern, matchStrs)) {
    if (! CStrUtil::toReal(matchStrs[0], &value))
      return COptValT<CScreenUnits>();

    lvalue = CScreenUnits(CScreenUnits::Units::EM, value);

    //CSVGLog() << "em conversion not handled";
  }
  // ex, x-height of the element’s font
  else if (CRegExpUtil::parse(str, ex_pattern, matchStrs)) {
    if (! CStrUtil::toReal(matchStrs[0], &value))
      return COptValT<CScreenUnits>();

    lvalue = CScreenUnits(CScreenUnits::Units::EX, value);

    //CSVGLog() << "ex conversion not handled";
  }
  // pt, points, 1pt = 1/72th of 1in
  else if (CRegExpUtil::parse(str, pt_pattern, matchStrs)) {
    if (! CStrUtil::toReal(matchStrs[0], &value))
      return COptValT<CScreenUnits>();

    //double ivalue = value;
    //mmToPixel((25.4*value)/72.0, &value);

    lvalue = CScreenUnits(CScreenUnits::Units::PT, value);
  }
  // pc, picas, 1pc = 1/6th of 1in
  else if (CRegExpUtil::parse(str, pc_pattern, matchStrs)) {
    if (! CStrUtil::toReal(matchStrs[0], &value))
      return COptValT<CScreenUnits>();

    //double ivalue = value;
    //mmToPixel((25.4*value)/6.0, &value);

    lvalue = CScreenUnits(CScreenUnits::Units::PC, value);
  }
  // cm, centimeters, 1cm = 96px/2.54
  else if (CRegExpUtil::parse(str, cm_pattern, matchStrs)) {
    if (! CStrUtil::toReal(matchStrs[0], &value))
      return COptValT<CScreenUnits>();

    //double ivalue = value;
    //mmToPixel(10*value, &value);

    lvalue = CScreenUnits(CScreenUnits::Units::CM, value);
  }
  // mm, millimeters, 1mm = 1/10th of 1cm
  else if (CRegExpUtil::parse(str, mm_pattern, matchStrs)) {
    if (! CStrUtil::toReal(matchStrs[0], &value))
      return COptValT<CScreenUnits>();

    //double ivalue = value;
    //mmToPixel(value, &value);

    lvalue = CScreenUnits(CScreenUnits::Units::MM, value);
  }
  // in, inches, 1in = 2.54cm = 96px
  else if (CRegExpUtil::parse(str, in_pattern, matchStrs)) {
    if (! CStrUtil::toReal(matchStrs[0], &value))
      return COptValT<CScreenUnits>();

    //double ivalue = value;
    //mmToPixel(25.4*value, &value);

    lvalue = CScreenUnits(CScreenUnits::Units::IN, value);
  }
  // px, pixels, 1px = 1/96th of 1in
  else if (CRegExpUtil::parse(str, px_pattern, matchStrs)) {
    if (! CStrUtil::toReal(matchStrs[0], &value))
      return COptValT<CScreenUnits>();

    //lvalue = CScreenUnits(CScreenUnits::Units::PX, value);
    lvalue = CScreenUnits(CScreenUnits::Units::IN, value/96.0);
  }
  // %
  else if (CRegExpUtil::parse(str, ph_pattern, matchStrs)) {
    if (! CStrUtil::toReal(matchStrs[0], &value))
      return COptValT<CScreenUnits>();

    lvalue = CScreenUnits(CScreenUnits::Units::PERCENT, value);
  }
  // default (document units) pixels
  else {
    if (! CStrUtil::toReal(str, &value))
      return COptValT<CScreenUnits>();

    lvalue = CScreenUnits(CScreenUnits::Units::PX, value);
  }

  return COptValT<CScreenUnits>(lvalue);
}

//--------------

// TODO: frequency (Hz, kHz)
// TODO: time (ms, s)

bool
CSVG::
realOption(const std::string &optName, const std::string &optValue,
           const std::string &name, double *value)
{
  if (optName != name)
    return false;

  if (! CStrUtil::toReal(optValue, value)) {
    CSVGLog() << "Illegal real value '" << optValue << "' for " << name;
    return false;
  }

  return true;
}

bool
CSVG::
integerOption(const std::string &optName, const std::string &optValue,
              const std::string &name, long *value)
{
  if (optName != name)
    return false;

  if (! CStrUtil::toInteger(optValue, value)) {
    CSVGLog() << "Illegal integer value '" << optValue << "' for " << name;
    return false;
  }

  return true;
}

bool
CSVG::
stringOption(const std::string &optName, const std::string &optValue,
             const std::string &name, std::string &value)
{
  if (optName != name)
    return false;

  value = optValue;

  return true;
}

//--------------

bool
CSVG::
orientOption(const std::string &optName, const std::string &optValue,
             const std::string &name, CSVGOrient &orient)
{
  if (optName != name)
    return false;

  auto optOrient = decodeOrientString(optValue);

  if (! optOrient.isValid()) {
    std::string msg = "Illegal orient value '" + optValue + "' for " + name;
    syntaxError(msg);
    return false;
  }

  orient = optOrient.getValue();

  return true;
}

COptValT<CSVGOrient>
CSVG::
decodeOrientString(const std::string &str)
{
  CSVGOrient orient;

  if (str == "auto") {
    orient.setIsAuto(true);
    return COptValT<CSVGOrient>(orient);
  }

  auto angle = decodeAngleString(str);

  if (! angle.isValid())
    return COptValT<CSVGOrient>();

  orient.setAngle(angle.getValue());

  return COptValT<CSVGOrient>(orient);
}

std::string
CSVG::
encodeOrientString(const CSVGOrient &orient)
{
  if (orient.isAuto())
    return "auto";

  return encodeAngleString(orient.angle());
}

//--------------

bool
CSVG::
angleOption(const std::string &optName, const std::string &optValue,
            const std::string &name, CAngle &angle)
{
  if (optName != name)
    return false;

  auto optAngle = decodeAngleString(optValue);

  if (! optAngle.isValid()) {
    std::string msg = "Invalid angle value '" + optValue + "' for " + name;
    syntaxError(msg);
    return false;
  }

  angle = optAngle.getValue();

  return true;
}

// When an <angle> is used in a style sheet or with a property in a ‘style’ attribute,
// the syntax must match the following pattern:
//   angle ::= number (~"deg" | ~"grad" | ~"rad")?
// When an <angle> is used in an SVG presentation attribute, the syntax must match
// the following pattern:
//   angle ::= number ("deg" | "grad" | "rad")?
COptValT<CAngle>
CSVG::
decodeAngleString(const std::string &str)
{
  CStrParse parse(str);

  parse.skipSpace();

  double r;

  if (! parse.readReal(&r))
    return COptValT<CAngle>();

  CAngle angle;

  if      (parse.isString("deg"))
    angle.setDegrees(r);
  else if (parse.isString("grad"))
    angle.setGrads(r);
  else if (parse.isString("rad"))
    angle.setRadians(r);
  else
    angle.setDegrees(r); // TODO: handle bad chars after real

  return COptValT<CAngle>(angle);
}

std::string
CSVG::
encodeAngleString(const CAngle &angle)
{
  std::string str = CStrUtil::toString(angle.value());

  if      (angle.type() == CAngle::Type::DEGREES)
    str += "deg";
  else if (angle.type() == CAngle::Type::RADIANS)
    str += "rad";
  else if (angle.type() == CAngle::Type::GRADS)
    str += "grad";

  return str;
}

//--------------

bool
CSVG::
percentOption(const std::string &optName, const std::string &optValue,
              const std::string &name, CScreenUnits &length)
{
  std::string str;

  if (! stringOption(optName, optValue, name, str))
    return false;

  if (! decodePercentString(str, length)) {
    std::string msg = "Illegal coord value " + str;
    syntaxError(msg);
    return false;
  }

  return true;
}

bool
CSVG::
decodePercentString(const std::string &str, CScreenUnits &length)
{
  double value;

  std::vector<std::string> matchStrs;

  if (CRegExpUtil::parse(str, "\\(.*\\)%", matchStrs)) {
    if (! CStrUtil::toReal(matchStrs[0], &value))
      return false;

    length = CScreenUnits(CScreenUnits::Units::PERCENT, value);
  }
  else {
    if (! CStrUtil::toReal(str, &value))
      return false;

    length = CScreenUnits(CScreenUnits::Units::RATIO, value);
  }

  return true;
}

//--------------

bool
CSVG::
coordUnitsOption(const std::string &optName, const std::string &optValue,
                 const std::string &name, CSVGCoordUnits &units)
{
  std::string str;

  if (! stringOption(optName, optValue, name, str))
    return false;

  if (! decodeUnitsString(str, units))
    return false;

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

//--------------

bool
CSVG::
bboxOption(const std::string &optName, const std::string &optValue,
           const std::string &name, CBBox2D &bbox)
{
  if (optName != name)
    return false;

  if (! decodeBBoxString(optValue, bbox)) {
    std::string msg = "Bad bbox " + optValue;
    syntaxError(msg);
    return false;
  }

  return true;
}

bool
CSVG::
decodeBBoxString(const std::string &name, CBBox2D &bbox)
{
  std::vector<std::string> words;

  CStrUtil::addWords(name, words, " ,");

  if (words.size() != 4)
    return false;

  double x, y, w, h;

  if (! CStrUtil::toReal(CStrUtil::stripSpaces(words[0]), &x)) return false;
  if (! CStrUtil::toReal(CStrUtil::stripSpaces(words[1]), &y)) return false;
  if (! CStrUtil::toReal(CStrUtil::stripSpaces(words[2]), &w)) return false;
  if (! CStrUtil::toReal(CStrUtil::stripSpaces(words[3]), &h)) return false;

  bbox = CBBox2D(x, y, x + w, y + h);

  return true;
}

//------

bool
CSVG::
preserveAspectOption(const std::string &optName, const std::string &optValue,
                     const std::string &name, CSVGPreserveAspect &preserveAspect)
{
  if (optName != name)
    return false;

  if (! decodePreserveAspectRatio(optValue, preserveAspect))
    return false;

  return true;
}

bool
CSVG::
decodePreserveAspectRatio(const std::string &str, CSVGPreserveAspect &preserveAspect)
{
  CHAlignType halign = CHALIGN_TYPE_CENTER;
  CVAlignType valign = CVALIGN_TYPE_CENTER;
  CSVGScale   scale  = CSVGScale::FIXED_MEET;

  std::vector<std::string> words;

  CStrUtil::addWords(str, words, " ,");

  CRegExp mmp("xM[ia][ndx]YM[ia][ndx]");

  for (const auto &word : words) {
    if (mmp.find(word)) {
      std::string lword = word.substr(0, 4);
      std::string rword = word.substr(4, 8);

      if      (lword == "xMin") halign = CHALIGN_TYPE_LEFT;
      else if (lword == "xMid") halign = CHALIGN_TYPE_CENTER;
      else if (lword == "xMax") halign = CHALIGN_TYPE_RIGHT;

      if      (rword == "YMin") valign = CVALIGN_TYPE_BOTTOM;
      else if (rword == "YMid") valign = CVALIGN_TYPE_CENTER;
      else if (rword == "YMax") valign = CVALIGN_TYPE_TOP;
    }
    else if (word == "meet")
      scale = CSVGScale::FIXED_MEET;
    else if (word == "slice")
      scale = CSVGScale::FIXED_SLICE;
    else if (word == "none")
      scale = CSVGScale::FREE;
  }

  preserveAspect = CSVGPreserveAspect(halign, valign, scale);

  return true;
}

//------

bool
CSVG::
pointListOption(const std::string &optName, const std::string &optValue,
                const std::string &name, std::vector<CPoint2D> &points)
{
  if (optName != name)
    return false;

  std::vector<double> reals;

  CStrParse parse(optValue);

  parse.skipSpace();

  while (! parse.eof()) {
    double r;

    if (! parse.readReal(&r)) {
      std::string msg = "Bad point list value " + optValue;
      syntaxError(msg);
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
realListOption(const std::string &optName, const std::string &optValue,
               const std::string &name, std::vector<double> &reals)
{
  if (optName != name)
    return false;

  return stringToReals(optValue, reals);
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
eventValueOption(const std::string &optName, const std::string &optValue,
                 const std::string &name, CSVGEventValue &event)
{
  if (optName != name)
    return false;

  uint   pos = 0;
  double r   = 0.0;

  if (CStrUtil::readReal(optValue, &pos, &r)) {
    CSVGTimeValue time;

    std::string units = optValue.substr(pos);

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

    auto p = optValue.find('.');

    if (p != std::string::npos) {
      id        = CStrUtil::stripSpaces(optValue.substr(0, p));
      eventName = CStrUtil::stripSpaces(optValue.substr(p + 1));
    }
    else
      eventName = optValue;

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
timeValueOption(const std::string &optName, const std::string &optValue,
                const std::string &name, CSVGTimeValue &time)
{
  if (optName != name)
    return false;

  if (! stringToTime(optValue, time))
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

//--------------

bool
CSVG::
transformOption(const std::string &optName, const std::string &optValue,
                const std::string &name, CMatrixStack2D &matrixStack)
{
  if (optName != name)
    return false;

  if (! decodeTransform(optValue, matrixStack)) {
    CSVGLog() << "Invalid transform " << optValue;
    return false;
  }

  return true;
}

bool
CSVG::
decodeTransform(const std::string &str, CMatrixStack2D &matrix)
{
  matrix.reset();

  CStrParse parse(str);

  //---

  auto skipCommaSpace = [&]() {
    parse.skipSpace();

    while (parse.isChar(',')) {
      parse.skipChar();

      parse.skipSpace();
    }
  };

  //---

  std::string keyword;

  while (! parse.eof()) {
    parse.skipSpace();

    if (! parse.readIdentifier(keyword))
      return false;

    parse.skipSpace();

    if      (keyword == "scale") {
      if (! parse.isChar('('))
        return false;

      parse.skipChar();

      parse.skipSpace();

      double sx, sy;

      if (! parse.readReal(&sx))
        return false;

      skipCommaSpace();

      bool isEqualScale = true;

      if (! parse.isChar(')')) {
        if (! parse.readReal(&sy))
          return false;

        parse.skipSpace();

        isEqualScale = false;
      }
      else
        sy = sx;

      if (! parse.isChar(')'))
        return false;

      parse.skipChar();

      skipCommaSpace();

      //------

      if (isEqualScale)
        matrix.scale(sx);
      else
        matrix.scale(sx, sy);
    }
    else if (keyword == "translate") {
      if (! parse.isChar('('))
        return false;

      parse.skipChar();

      parse.skipSpace();

      double dx = 0.0, dy = 0.0;

      if (! parse.isChar(')')) {
        if (! parse.readReal(&dx))
          return false;

        skipCommaSpace();

        if (! parse.isChar(')')) {
          if (! parse.readReal(&dy))
            return false;

          parse.skipSpace();
        }
      }

      if (! parse.isChar(')'))
        return false;

      parse.skipChar();

      skipCommaSpace();

      //------

      matrix.translate(dx, dy);
    }
    else if (keyword == "rotate") {
      if (! parse.isChar('('))
        return false;

      parse.skipChar();

      parse.skipSpace();

      double a;

      if (! parse.readReal(&a))
        return false;

      parse.skipSpace();

      bool translate = false;

      double cx = 0.0, cy = 0.0;

      if (! parse.isChar(')')) {
        skipCommaSpace();

        if (! parse.readReal(&cx))
          return false;

        skipCommaSpace();

        if (! parse.readReal(&cy))
          return false;

        parse.skipSpace();

        translate = true;
      }

      if (! parse.isChar(')'))
        return false;

      parse.skipChar();

      skipCommaSpace();

      //------

      if (translate)
        matrix.rotate(CMathGen::DegToRad(a), CPoint2D(cx, cy));
      else
        matrix.rotate(CMathGen::DegToRad(a));
    }
    else if (keyword == "skewX") {
      if (! parse.isChar('('))
        return false;

      parse.skipChar();

      parse.skipSpace();

      double angle;

      if (! parse.readReal(&angle))
        return false;

      parse.skipSpace();

      if (! parse.isChar(')'))
        return false;

      parse.skipChar();

      skipCommaSpace();

      //------

      matrix.skewX(CMathGen::DegToRad(angle));
    }
    else if (keyword == "skewY") {
      if (! parse.isChar('('))
        return false;

      parse.skipChar();

      parse.skipSpace();

      double angle;

      if (! parse.readReal(&angle))
        return false;

      parse.skipSpace();

      if (! parse.isChar(')'))
        return false;

      parse.skipChar();

      skipCommaSpace();

      //------

      matrix.skewY(CMathGen::DegToRad(angle));
    }
    else if (keyword == "matrix") {
      if (! parse.isChar('('))
        return false;

      parse.skipChar();

      parse.skipSpace();

      // SVG is ((a c e) (b d f) (0 0 1))
      double m00, m01, m10, m11, tx, ty;

      if (! parse.readReal(&m00))
        return false;

      skipCommaSpace();

      if (! parse.readReal(&m10))
        return false;

      skipCommaSpace();

      if (! parse.readReal(&m01))
        return false;

      skipCommaSpace();

      if (! parse.readReal(&m11))
        return false;

      skipCommaSpace();

      if (! parse.readReal(&tx))
        return false;

      skipCommaSpace();

      if (! parse.readReal(&ty))
        return false;

      parse.skipSpace();

      if (! parse.isChar(')'))
        return false;

      parse.skipChar();

      skipCommaSpace();

      //------

      matrix.matrix(m00, m01, m10, m11, tx, ty);
    }
    else
      return false;
  }

  return true;
}

//--------------

/*TODO*/
bool
CSVG::
decodeWidthString(const std::string &widthStr, double &width, bool &inherit)
{
  width   = 1.0;
  inherit = false;

  if (widthStr == "inherit") {
    inherit = true;
    return true;
  }

  std::vector<std::string> matchStrs;

  if      (CRegExpUtil::parse(widthStr, "\\(.*\\)pt", matchStrs)) {
    if (! CStrUtil::toReal(matchStrs[0], &width))
      return false;

    CScreenUnits units(width, CScreenUnits::Units::PT);

    width = units.pxValue();
    //mmToPixel(25.4*width/72.0, &width);
  }
  else if (CRegExpUtil::parse(widthStr, "\\(.*\\)px", matchStrs)) {
    if (! CStrUtil::toReal(matchStrs[0], &width))
      return false;
  }
  else {
    if (! CStrUtil::toReal(widthStr, &width))
      return false;
  }

  return true;
}

// <opacity-value> | inherit
bool
CSVG::
opacityOption(const std::string &optName, const std::string &optValue,
              const std::string &name, double &value, bool &inherit)
{
  if (optName != name)
    return false;

  std::string str;

  if (! stringOption(optName, optValue, name, str))
    return false;

  if (! decodeOpacityString(str, value, inherit)) {
    CLog() << "Invalid opacity '" << str << "'";
    return false;
  }

  return true;
}

// <opacity-value> | inherit
bool
CSVG::
decodeOpacityString(const std::string &opacityStr, double &opacity, bool &inherit)
{
  opacity = 1.0;
  inherit = false;

  if (opacityStr == "inherit")
    inherit = true;
  else {
    if (! CStrUtil::toReal(opacityStr, &opacity))
      return false;
  }

  return true;
}

// nonzero | evenodd | inherit
CSVG::FillType
CSVG::
decodeFillRuleString(const std::string &ruleStr)
{
  if      (ruleStr == "nonzero")
    return FillType(FILL_TYPE_WINDING);
  else if (ruleStr == "evenodd")
    return FillType(FILL_TYPE_EVEN_ODD);
  else if (ruleStr == "inherit")
    return FillType::makeInherit();
  else
    return FillType(FILL_TYPE_NONE);
}

std::string
CSVG::
encodeFillRuleString(const FillType &rule)
{
  if (rule.isInherit())
    return "inherit";

  auto fillType = rule.getValue();

  if      (fillType == FILL_TYPE_WINDING)
    return "nonzero";
  else if (fillType == FILL_TYPE_EVEN_ODD)
    return "evenodd";
  else if (fillType == FILL_TYPE_NONE)
    return "none";

  return "none";
}

// none | <list-of-lengths> | inherit
bool
CSVG::
decodeDashString(const std::string &dashStr, std::vector<CScreenUnits> &lengths, bool &solid)
{
  solid = false;

  // solid, empty length list
  if (dashStr == "solid") {
    solid = true;
    return true;
  }

  if (dashStr == "none") {
    return true;
  }

  //---

  std::vector<std::string> words;

  CStrUtil::addWords(dashStr, words, " ,");

  uint num_words = words.size();

  bool duplicate = (num_words & 1);

  for (uint i = 0; i < num_words; ++i) {
    std::string word = CStrUtil::stripSpaces(words[i]);

    auto optLength = decodeLengthValue(word);

    if (! optLength.isValid())
      return false;

    lengths.push_back(optLength.getValue());
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
colorOption(const std::string &optName, const std::string &optValue,
            const std::string &name, CSVGColor &color, bool &inherit)
{
  if (optName != name)
    return false;

  std::string str;

  if (! stringOption(optName, optValue, name, str))
    return false;

  if (! decodeColorString(str, color, inherit)) {
    CLog() << "Invalid color '" << str << "'";
    return false;
  }

  return true;
}

// none | currentColor | <color> |
// <FuncIRI> [ none | currentColor | <color>] |
// <system paint> | inherit
bool
CSVG::
decodeColorString(const std::string &colorStr, CSVGColor &color, bool &inherit)
{
  inherit = false;

  if      (colorStr == "none")
    color = CSVGColor(CSVGColor::Type::NONE);
  else if (colorStr == "currentColor")
    color = CSVGColor(CSVGColor::Type::CURRENT);
  else if (colorStr == "inherit")
    inherit = true;
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
  std::vector<std::string> matchStrs;

  // color ::= "rgb(" wsp* integer comma integer comma integer wsp* ")"
  // color ::= "rgb(" wsp* integer "%" comma integer "%" comma integer "%" wsp* ")"
  if      (CRegExpUtil::parse(colorStr, "rgb(\\(.*\\))", matchStrs)) {
    double rgb[3];

    std::vector<std::string> words;

    CStrUtil::addWords(matchStrs[0], words, " ,");

    while (words.size() < 3)
      words.push_back("0");

    for (uint i = 0; i < 3; ++i) {
      std::vector<std::string> matchStrs1;

      if (CRegExpUtil::parse(words[i], "\\(.*\\)%", matchStrs1)) {
        if (! CStrUtil::toReal(matchStrs1[0], &rgb[i]))
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
  // color ::= "#" hexdigit hexdigit hexdigit
  else if (colorStr.size() == 4 &&
           CRegExpUtil::parse(colorStr, "#" RE_HEXDEC RE_HEXDEC RE_HEXDEC, matchStrs)) {
    std::string colorStr1 = "#";

    colorStr1 += colorStr.substr(1, 1) + colorStr.substr(1, 1) +
                 colorStr.substr(2, 1) + colorStr.substr(2, 1) +
                 colorStr.substr(3, 1) + colorStr.substr(3, 1);

    rgba = nameToRGBA(colorStr1);
  }
  // color ::= "#" hexdigit hexdigit hexdigit hexdigit hexdigit hexdigit
  else if (colorStr.size() == 7 &&
           CRegExpUtil::parse(colorStr, "#" RE_HEXDEC RE_HEXDEC RE_HEXDEC
                              RE_HEXDEC RE_HEXDEC RE_HEXDEC, matchStrs)) {
    std::string colorStr1 = "#";

    colorStr1 += colorStr.substr(1, 2) +
                 colorStr.substr(3, 2) +
                 colorStr.substr(5, 2);

    rgba = nameToRGBA(colorStr1);
  }
  // color ::= color-keyword
  else {
    if (colorStr == "none")
      rgba = CRGBA(0, 0, 0, 0);
    else
      rgba = nameToRGBA(colorStr);
  }

  return true;
}

CSVGColor
CSVG::
nameToColor(const std::string &name)
{
  if (name == "currentColor")
    return CSVGColor(CSVGColor::Type::CURRENT);

  return CSVGColor(nameToRGBA(name));
}

CRGBA
CSVG::
nameToRGBA(const std::string &name)
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

  return CRGBA(0, 0, 0);
}

// Value: normal | bold | bolder | lighter | 100 | 200 | 300
//        | 400 | 500 | 600 | 700 | 800 | 900 | inherit
CFontStyle
CSVG::
decodeFontWeightString(const std::string &fontWeightStr, bool &inherit)
{
  inherit = false;

  CFontStyle fontStyle = CFONT_STYLE_NORMAL;

  if      (fontWeightStr == "normal"|| fontWeightStr == "400")
    fontStyle = CFONT_STYLE_NORMAL;
  else if (fontWeightStr == "bold"|| fontWeightStr == "700")
    fontStyle = CFONT_STYLE_BOLD;
  else
    fontStyle = CFONT_STYLE_NORMAL;

  return fontStyle;
}

// normal | italic | oblique | inherit
CFontStyle
CSVG::
decodeFontStyleString(const std::string &fontStyleStr, bool &inherit)
{
  inherit = false;

  CFontStyle fontStyle = CFONT_STYLE_NORMAL;

  if      (fontStyleStr == "normal")
    fontStyle = CFONT_STYLE_NORMAL;
  else if (fontStyleStr == "italic")
    fontStyle = CFONT_STYLE_ITALIC;
  else if (fontStyleStr == "oblique")
    fontStyle = CFONT_STYLE_ITALIC;
  else if (fontStyleStr == "inherit")
    inherit = true;

  return fontStyle;
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

//--------------

bool
CSVG::
urlOption(const std::string &optName, const std::string &optValue,
          const std::string &name, CSVGObject **obj)
{
  if (optName != name)
    return false;

  *obj = nullptr;

  if (optValue == "none")
    return true;

  std::string  id;
  CSVGObject  *obj1;

  if (! decodeUrlObject(optValue, id, &obj1)) {
    CSVGLog() << "Illegal url value '" << id << "' for " << name;
    return true; // don't propagate waring
  }

  *obj = obj1;

  return true;
}

bool
CSVG::
decodeUrlObject(const std::string &str, std::string &id, CSVGObject **object) const
{
  std::vector<std::string> matchStrs;

  if      (CRegExpUtil::parse(str, "url(#\\(.*\\))", matchStrs))
    id = matchStrs[0];
  else if (str.size() > 0 && str[0] == '#')
    id = str.substr(1);
  else
    id = str;

  *object = lookupObjectById(id);

  return (*object != nullptr);
}

//--------------

bool
CSVG::
booleanOption(const std::string &optName, const std::string &optValue,
              const std::string &name, bool &b)
{
  if (optName != name)
    return false;

  if (! decodeBoolean(optValue, b))
    return false;

  return true;
}

bool
CSVG::
decodeBoolean(const std::string &str, bool &b)
{
  CStrParse parse(str);

  parse.skipSpace();

  std::string val;

  parse.readNonSpace(val);

  std::string lval = CStrUtil::toLower(val);

  if      (lval == "true" || lval == "1")
    b = true;
  else if (lval == "false" || lval == "0")
    b = false;
  else
    return false;

  return true;
}

//--------------

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

  auto *title = dynamic_cast<CSVGTitle *>(objects[0]);

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

CBBox2D
CSVG::
untransformBBox(const CMatrixStack2D &m, const CBBox2D &bbox) const
{
  if (bbox.isSet()) {
    auto im = m.getIMatrix();

    CPoint2D p1, p2, p3, p4;

    im.multiplyPoint(bbox.getLL(), p1);
    im.multiplyPoint(bbox.getLR(), p2);
    im.multiplyPoint(bbox.getUL(), p3);
    im.multiplyPoint(bbox.getUR(), p4);

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

  return nullptr;
}

bool
CSVG::
readCSSFile(const std::string &filename)
{
  CCSS css;

  if (! css.processFile(filename))
    return false;

  cssList_.push_back(css);

  return true;
}

bool
CSVG::
readCSSString(const std::string &str)
{
  CCSS css;

  if (! css.processLine(str))
    return false;

  cssList_.push_back(css);

  return true;
}

//------

void
CSVG::
setScript(const std::string &str)
{
#ifdef CSVG_JAVASCRIPT
  js()->loadString(str);

  js()->exec();
#else
  std::cerr << "CSVG::setScript " << str << "\n";
#endif
}

void
CSVG::
setScriptFile(const std::string &filename)
{
#ifdef CSVG_JAVASCRIPT
  js()->loadFile(filename);

  js()->exec();
#else
  std::cerr << "CSVG::setScriptFile " << filename << "\n";
#endif
}

//------

bool
CSVG::
applyStyle(CSVGObject *obj)
{
  CCSSTagDataP tagData(new CSVGObjectCSSTagData(obj));

  bool rc = true;

  for (const auto &css : cssList_) {
    if (! visitStyleData(css, tagData))
      rc = false;
  }

  return rc;
}

bool
CSVG::
visitStyleData(const CCSS &css, const CCSSTagDataP &tagData)
{
  bool match = false;

  std::vector<CCSS::SelectorList> selectorListArray;

  css.getSelectors(selectorListArray);

  for (const auto &selectorList : selectorListArray) {
    const auto &styleData = css.getStyleData(selectorList);

    if (! styleData.checkMatch(tagData))
      continue;

    auto *obj = dynamic_cast<CSVGObjectCSSTagData *>(tagData.get())->obj();

    for (const auto &opt : styleData.getOptions()) {
      obj->setStyleValue(opt.getName(), opt.getValue());
    }

    match = true;
  }

  return match;
}

//------

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
getChildrenOfId(const std::string &id, ObjectList &objects) const
{
  getRoot()->getChildrenOfId(id, objects);
}

void
CSVG::
getSelectedObjects(ObjectList &objects) const
{
  // TODO: cache ?
  ObjectList children;

  getAllChildren(children);

  for (const auto &child : children) {
    if (child->getSelected())
      objects.push_back(child);
  }
}

//---

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
  getRoot()->printRoot(os, cssList_, hier);
}

void
CSVG::
printFlat(std::ostream &os, bool force) const
{
  if (xmlStyleSheet_.isValid()) {
    os << "<?xml-stylesheet";

    if (xmlStyleSheet_.getValue().ref.isValid())
      os << " href=\"" << xmlStyleSheet_.getValue().ref.getValue() << "\"";

    if (xmlStyleSheet_.getValue().is_css.isValid())
      os << " type=\"text/css\"";

    os << "?>" << std::endl;
  }

  getRoot()->printFlat(os, force, -1);

  //css_.print(os);
}

void
CSVG::
syntaxError(const std::string &msg)
{
#ifdef CSVG_JAVASCRIPT
  if (! js_)
    CSVGLog() << "SyntaxError: " << msg;
  else
    js_->throwSyntaxError(0, msg);
#else
  CSVGLog() << "SyntaxError: " << msg;
#endif
}
