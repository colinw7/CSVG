#include <CSVG.h>
#include <CSVGAnchor.h>
#include <CSVGAnimate.h>
#include <CSVGAnimateColor.h>
#include <CSVGAnimateMotion.h>
#include <CSVGAnimateTransform.h>
#include <CSVGBuffer.h>
#include <CSVGCircle.h>
#include <CSVGClipPath.h>
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
 renderer_(renderer),
 stroke_  (*this),
 fill_    (*this),
 clip_    (*this),
 fontDef_ (*this)
{
  viewMatrix_.reset();

  xml_ = new CXML();

  buffer_mgr_ = new CSVGBufferMgr(*this);

  //---

  if (getenv("CSVG_DEBUG"))
    setDebug(true);

  if (getenv("CSVG_DEBUG_IMAGE"))
    setDebugImage(true);

  if (getenv("CSVG_DEBUG_FILTER"))
    setDebugFilter(true);

  if (getenv("CSVG_DEBUG_USE"))
    setDebugUse(true);
}

CSVG::
~CSVG()
{
}

CSVGBlock *
CSVG::
getBlock() const
{
  if (! block_.isValid()) {
    CSVG *th = const_cast<CSVG *>(this);

    th->block_ = th->createBlock();
  }

  return block_;
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
  else
    return 0;
}

void
CSVG::
setAntiAlias(bool flag)
{
  buffer_mgr_->setAntiAlias(flag);
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
setDebugFilter(bool b)
{
  debugFilter_ = b;
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

bool
CSVG::
read(const std::string &filename)
{
  return read(filename, getBlock());
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
        std::string ref;
        bool        is_css = false;

        uint numOptions = exec->getNumOptions();

        for (uint j = 0; j < numOptions; ++j) {
          const CXMLExecute::Option &opt = exec->getOption(j);

          if      (opt.getName() == "href")
            ref = opt.getValue();
          else if (opt.getName() == "type")
            is_css = (opt.getValue() == "text/css");
        }

        if (is_css && ! ref.empty())
          readCSSFile(ref);
      }
    }
  }

  //------

  // Process svg options
  for (const auto &opt : xmlTag_->getOptions()) {
    const std::string &opt_name  = opt->getName ();
    const std::string &opt_value = opt->getValue();

    if (! object->processOption(opt_name, opt_value))
      CSVGLog() << "Invalid option " << opt_name << "=" << opt_value <<
                   " for " << object->getObjName();
  }

  //------

  // Process svg children
  object->setXMLTag(xmlTag_);

  for (const auto &token : xmlTag_->getChildren()) {
    CSVGObject *object1 = tokenToObject(object, token);

    if (object1)
      object->addChildObject(object1);
  }

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

  // Create object from tag name
  const std::string &tag_name = tag->getName();

  CSVGObject *object = createObjectByName(tag_name);

  if (! object) {
    CSVGLog() << "Unknown tag " << tag_name;
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

        hasTextSpan = true;
      }

      CSVGObject *object1 = tokenToObject(object, token);
      assert(object1);

      object->addChildObject(object1);
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
      CSVGLog() << "Unhandled option " << opt_name << "=" << opt_value <<
                   " for " << object->getObjName();
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
    object = new CSVGAnimate(*this);
  else if (name == "animateColor")
    object = new CSVGAnimateColor(*this);
  else if (name == "animateMotion")
    object = new CSVGAnimateMotion(*this);
  else if (name == "animateTransform")
    object = new CSVGAnimateTransform(*this);
  else if (name == "circle")
    object = createCircle();
  else if (name == "clipPath")
    object = new CSVGClipPath(*this);
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
    object = new CSVGFont(*this);
  else if (name == "font-face")
    object = new CSVGFontFace(*this);
  else if (name == "font-face-src")
    object = new CSVGFontFaceSrc(*this);
  else if (name == "font-face-uri")
    object = new CSVGFontFaceUri(*this);
  else if (name == "glyph")
    object = new CSVGGlyph(*this);
  else if (name == "g")
    object = createGroup();
  else if (name == "hkern")
    object = new CSVGHKern(*this);
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
    object = new CSVGMissingGlyph(*this);
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
    object = new CSVGScript(*this);
  else if (name == "set")
    object = createSet();
  else if (name == "stop")
    object = createStop();
  else if (name == "symbol")
    object = createSymbol();
  else if (name == "style")
    object = new CSVGStyle(*this);
  else if (name == "switch")
    object = new CSVGSwitch(*this);
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

CSVGCircle *
CSVG::
createCircle()
{
  return new CSVGCircle(*this);
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

CSVGGroup *
CSVG::
createGroup()
{
  return new CSVGGroup(*this);
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

double
CSVG::
getXMin() const
{
  return getBlock()->getXMin();
}

double
CSVG::
getYMin() const
{
  return getBlock()->getYMin();
}

double
CSVG::
getWidth() const
{
  return getBlock()->getWidth();
}

double
CSVG::
getHeight() const
{
  return getBlock()->getHeight();
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

CImagePtr
CSVG::
drawToImage(int w, int h)
{
  CAutoPtr<CSVGRenderer> renderer;

  renderer = createRenderer();

  if (renderer) {
    renderer->setSize(w, h);

    setRenderer(renderer);

    draw();

    return renderer->getImage();
  }
  else
    return CImagePtr();
}

void
CSVG::
draw()
{
  CMatrixStack2D matrix;

  draw(matrix);
}

void
CSVG::
draw(const CMatrixStack2D &matrix, double scale)
{
  drawBlock(getBlock(), matrix, scale);
}

void
CSVG::
drawBlock(CSVGBlock *block)
{
  CMatrixStack2D matrix;

  drawBlock(block, matrix);
}

void
CSVG::
drawBlock(CSVGBlock *block, const CMatrixStack2D &matrix, double scale)
{
  if (! renderer_)
    return;

  viewMatrix_ = matrix;
  scale_      = scale;

  //------

  renderer_->beginDraw();

  CPoint2D bmin, bmax;

  viewMatrix_.multiplyPoint(CPoint2D(block->getXMin(), block->getYMin()), bmin);
  viewMatrix_.multiplyPoint(CPoint2D(block->getXMax(), block->getYMax()), bmax);

  renderer_->setDataRange(bmin.x, bmin.y, bmax.x, bmax.y);

  //------

  resetStroke();
  resetFill();
  resetClip();
  resetFontDef();

  //------

  CSVGBuffer *bgBuffer = getBuffer("BackgroundImage");

  setBuffer(bgBuffer);

  bgBuffer->clear();

  beginDrawBuffer(bgBuffer);

  //------

  block->drawObject();

  //------

  endDrawBuffer(bgBuffer);

  //------

  CImagePtr image = bgBuffer->getImage();

  renderer_->setImage(image);

  //------

  viewMatrix_.reset();

  renderer_->endDraw();
}

CSVGBuffer *
CSVG::
getBuffer() const
{
  return buffer_;
}

std::string
CSVG::
getBufferName() const
{
  if (buffer_)
    return buffer_->getName();
  else
    return "";
}

void
CSVG::
setBuffer(CSVGBuffer *buffer)
{
  buffer_ = buffer;
}

CSVGBuffer *
CSVG::
getBuffer(const std::string &name)
{
  if      (name == "SourceAlpha") {
    CSVGBuffer *buffer = buffer_mgr_->lookupBuffer("SourceGraphic", /*create*/true);

    return buffer_mgr_->lookupAlphaBuffer(buffer, /*create*/true);
  }
  else if (name == "BackgroundAlpha") {
    CSVGBuffer *buffer = buffer_mgr_->lookupBuffer("BackgroundImage", /*create*/true);

    return buffer_mgr_->lookupAlphaBuffer(buffer, /*create*/true);
  }

  CSVGBuffer *buffer = buffer_mgr_->lookupBuffer(name, /*create*/true);

  return buffer;
}

void
CSVG::
getBufferNames(std::vector<std::string> &names) const
{
  buffer_mgr_->getBufferNames(names);
}

void
CSVG::
beginDrawBuffer(CSVGBuffer *buffer)
{
  beginDrawBuffer(buffer, scale(), scale());
}

void
CSVG::
beginDrawBuffer(CSVGBuffer *buffer, double xs, double ys)
{
  CSVGBlock *block = getBlock();

  CBBox2D bbox;

  block->getBBox(bbox);

  beginDrawBuffer(buffer, bbox, xs, ys);
}

void
CSVG::
beginDrawBuffer(CSVGBuffer *buffer, const CBBox2D &bbox)
{
  beginDrawBuffer(buffer, bbox, scale(), scale());
}

void
CSVG::
beginDrawBuffer(CSVGBuffer *buffer, const CBBox2D &bbox, double xs, double ys)
{
  double w = bbox.getWidth ();
  double h = bbox.getHeight();

  buffer->beginDraw(w*xs, h*ys, bbox);

  buffer->setViewMatrix(viewMatrix_);

  CSVGBlock *block = getBlock();

  buffer->setAlign(block->getHAlign(), block->getVAlign());

  buffer->setEqualScale(block->getScale() != CSVGScale::FREE);
  buffer->setScaleMin  (block->getScale() == CSVGScale::FIXED_MEET);
}

void
CSVG::
endDrawBuffer(CSVGBuffer *buffer)
{
  buffer->endDraw();
}

void
CSVG::
resetStroke()
{
  stroke_.reset();
}

void
CSVG::
updateStroke(const CSVGStroke &stroke)
{
  stroke_.update(stroke);
}

void
CSVG::
setSelectedStroke()
{
  stroke_.setColor  (CRGB(1,0,0));
  stroke_.setWidth  (2);
  stroke_.setOpacity(1);
  stroke_.setDash   ("solid");
}

bool
CSVG::
isStroked() const
{
  if (stroke_.getColorValid()) {
    CRGBA strokeColor = stroke_.getAlphaColor();

    return (strokeColor.getAlpha() > 0);
  }

  return false;
}

void
CSVG::
setStrokeBuffer()
{
  if (stroke_.getColorValid()) {
    CRGBA strokeColor = stroke_.getAlphaColor();

    if (stroke_.getOpacityValid()) {
      CRGBA strokeColor1(strokeColor);

      strokeColor1.setAlpha(stroke_.getOpacity());

      buffer_->setStrokeColor(strokeColor1);
    }
    else
      buffer_->setStrokeColor(strokeColor);
  }
  else
    buffer_->setStrokeColor(CRGBA(0,0,0));

  if (stroke_.getWidthValid())
    buffer_->setLineWidth(stroke_.getWidth());
  else
    buffer_->setLineWidth(1);

  if (stroke_.getDashValid())
    buffer_->setLineDash(stroke_.getDash());
  else
    buffer_->setLineDash(CLineDash());

  if (stroke_.getLineCapValid())
    buffer_->setLineCap(stroke_.getLineCap());
  else
    buffer_->setLineCap(LINE_CAP_TYPE_BUTT);

  if (stroke_.getLineJoinValid())
    buffer_->setLineJoin(stroke_.getLineJoin());
  else
    buffer_->setLineJoin(LINE_JOIN_TYPE_MITRE);

  if (stroke_.getMitreLimitValid())
    buffer_->setLineMitreLimit(stroke_.getMitreLimitValid());
  else
    buffer_->setLineMitreLimit(4.0);
}

void
CSVG::
resetFill()
{
  fill_.reset();
}

void
CSVG::
updateFill(const CSVGFill &fill)
{
  fill_.update(fill);
}

bool
CSVG::
isFilled() const
{
  if (! fill_.getNoColor())
    return true;

  return false;
}

void
CSVG::
setFillBuffer(CSVGBuffer *buffer)
{
  CSVGObject *fillObject = fill_.getFillObject();

  if (fillObject) {
    CSVGLinearGradient *lg = dynamic_cast<CSVGLinearGradient *>(fillObject);
    CSVGRadialGradient *rg = dynamic_cast<CSVGRadialGradient *>(fillObject);
    CSVGPattern        *pt = dynamic_cast<CSVGPattern        *>(fillObject);

    if      (lg) {
      CAutoPtr<CLinearGradient> lg1;

      lg1 = lg->createGradient(drawObject_);

      buffer->setFillGradient(lg1);
    }
    else if (rg) {
      CAutoPtr<CRadialGradient> rg1;

      rg1 = rg->createGradient(drawObject_);

      buffer->setFillGradient(rg1);
    }
    else if (pt) {
      double w1, h1;

      CImagePtr image = pt->getImage(drawObject_, &w1, &h1);

      buffer->setFillImage(image);
    }
    else
      assert(false);
  }
  else {
    if (fill_.getColorValid()) {
      CRGBA fill_color = fill_.getAlphaColor();

      buffer->setFillColor(fill_color);
    }
    else
      buffer->setFillColor(CRGBA(0,0,0));
  }

  if (fill_.getRuleValid())
    buffer->setFillType(fill_.getRule());
}

void
CSVG::
resetClip()
{
  clip_.reset();
}

void
CSVG::
updateClip(const CSVGClip &clip)
{
  if (clip.getRuleValid())
    clip_.setRule(clip.getRule());
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
  fontDef_.reset();
}

void
CSVG::
updateFontDef(const CSVGFontDef &)
{
}

void
CSVG::
setFontDef()
{
}

void
CSVG::
setTransform(const CMatrixStack2D &matrix)
{
  setBufferTransform(buffer_, matrix);
}

void
CSVG::
setBufferTransform(CSVGBuffer *buffer, const CMatrixStack2D &matrix)
{
  buffer->setTransform(matrix);
}

void
CSVG::
getTransform(CMatrixStack2D &matrix)
{
  matrix = buffer_->transform();
}

void
CSVG::
unsetTransform()
{
  unsetBufferTransform(buffer_);
}

void
CSVG::
unsetBufferTransform(CSVGBuffer *buffer)
{
  buffer->unsetTransform();
}

void
CSVG::
drawImage(double x, double y, CImagePtr image)
{
  drawBufferImage(buffer_, x, y, image);
}

void
CSVG::
drawBufferImage(CSVGBuffer *buffer, double x, double y, CImagePtr image)
{
  buffer->drawImage(x, y, image);
}

void
CSVG::
drawImage(const CBBox2D &bbox, CImagePtr image)
{
  drawBufferImage(buffer_, bbox, image);
}

void
CSVG::
drawBufferImage(CSVGBuffer *buffer, const CBBox2D &bbox, CImagePtr image)
{
  buffer->drawImage(bbox, image);
}

void
CSVG::
drawLine(double x1, double y1, double x2, double y2)
{
  setStrokeBuffer();

  buffer_->pathInit();

  buffer_->pathMoveTo(x1, y1);
  buffer_->pathLineTo(x2, y2);

  buffer_->pathStroke();
}

void
CSVG::
drawRoundedRectangle(const CBBox2D &bbox, double rx, double ry)
{
  setStrokeBuffer();

  const CPoint2D &ll = bbox.getLL();
  const CPoint2D &ur = bbox.getUR();

  buffer_->pathInit();

  buffer_->pathMoveTo(ll.x + rx, ll.y);
  buffer_->pathLineTo(ur.x - rx, ll.y);
  buffer_->pathArcTo (ur.x - rx, ll.y + ry, rx, ry, 3*M_PI/2, 2*M_PI);
  buffer_->pathLineTo(ur.x, ll.y + ry);
  buffer_->pathArcTo (ur.x - rx, ur.y - ry, rx, ry, 0       , M_PI/2.0);
  buffer_->pathLineTo(ur.x - rx, ur.y);
  buffer_->pathArcTo (ll.x + rx, ur.y - ry, rx, ry, M_PI/2.0, M_PI);
  buffer_->pathLineTo(ll.x, ur.y - ry);
  buffer_->pathArcTo (ll.x + rx, ll.y + ry, rx, ry, M_PI    , 3*M_PI/2);

  buffer_->pathClose();

  buffer_->pathStroke();
}

void
CSVG::
fillRoundedRectangle(const CBBox2D &bbox, double rx, double ry)
{
  setFillBuffer(buffer_);

  const CPoint2D &ll = bbox.getLL();
  const CPoint2D &ur = bbox.getUR();

  buffer_->pathInit();

  buffer_->pathMoveTo(ll.x + rx, ll.y);
  buffer_->pathLineTo(ur.x - rx, ll.y);
  buffer_->pathArcTo (ur.x - rx, ll.y + ry, rx, ry, 3*M_PI/2, 2*M_PI);
  buffer_->pathLineTo(ur.x, ll.y + ry);
  buffer_->pathArcTo (ur.x - rx, ur.y - ry, rx, ry, 0       , M_PI/2.0);
  buffer_->pathLineTo(ur.x - rx, ur.y);
  buffer_->pathArcTo (ll.x + rx, ur.y - ry, rx, ry, M_PI/2.0, M_PI);
  buffer_->pathLineTo(ll.x, ur.y - ry);
  buffer_->pathArcTo (ll.x + rx, ll.y + ry, rx, ry, M_PI    , 3*M_PI/2);

  buffer_->pathClose();

  buffer_->pathFill();
}

void
CSVG::
drawRectangle(const CBBox2D &bbox)
{
  setStrokeBuffer();

  buffer_->pathInit();

  buffer_->pathMoveTo(bbox.getXMin(), bbox.getYMin());
  buffer_->pathLineTo(bbox.getXMax(), bbox.getYMin());
  buffer_->pathLineTo(bbox.getXMax(), bbox.getYMax());
  buffer_->pathLineTo(bbox.getXMin(), bbox.getYMax());

  buffer_->pathClose();

  buffer_->pathStroke();
}

void
CSVG::
fillRectangle(const CBBox2D &bbox)
{
  setFillBuffer(buffer_);

  buffer_->pathInit();

  buffer_->pathMoveTo(bbox.getXMin(), bbox.getYMin());
  buffer_->pathLineTo(bbox.getXMax(), bbox.getYMin());
  buffer_->pathLineTo(bbox.getXMax(), bbox.getYMax());
  buffer_->pathLineTo(bbox.getXMin(), bbox.getYMax());

  buffer_->pathClose();

  buffer_->pathFill();
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
      setStrokeBuffer();

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
      setStrokeBuffer();

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
  setStrokeBuffer();

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
  uint num_points = points.size();

  if (! num_points)
    return;

  setStrokeBuffer();

  buffer_->pathInit();

  buffer_->pathMoveTo(points[0].x, points[0].y);

  for (uint i = 1; i < num_points; ++i)
    buffer_->pathLineTo(points[i].x, points[i].y);

  buffer_->pathClose();

  buffer_->pathStroke();
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
fillDrawText(double x, double y, const std::string &text, CFontPtr font, CHAlignType align,
             bool isFilled, bool isStroked)
{
  if (isFilled || isStroked) {
    if (isFilled)
      fillText(x, y, text, font, align);

    if (isStroked)
      drawText(x, y, text, font, align);
  }
  else
    fillText(x, y, text, font, align);
}

void
CSVG::
drawText(double x, double y, const std::string &text, CFontPtr font, CHAlignType align)
{
  setStrokeBuffer();

  CSVGFont *svg_font = getFont();

  if (svg_font) {
    CSVGFontFace *font_face = svg_font->getFontFace();

    double units = 1000;

    if (font_face)
      units = font_face->getUnits();

    //-----

    CMatrixStack2D transform;

    getTransform(transform);

    double font_size = 10.0;

    transform.scale    (font_size/units, font_size/units);
    transform.translate(x, y);

    uint len = text.size();

    for (uint i = 0; i < len; ++i) {
      CSVGGlyph *glyph = getCharGlyph(text[i]);

      if (glyph) {
        setTransform(transform);

        glyph->drawSubObject();

        x += font_size;

        transform.translate(x, y);
      }
    }

    setTransform(transform);
  }
  else {
    CMatrixStack2D transform;

    getTransform(transform);

    CMatrixStack2D transform1 = transform;

    if (font->getAngle()) {
      transform1.rotate(font->getAngle(), CPoint2D(x, y));

      setTransform(transform1);
    }

    //---

    buffer_->pathInit();

    buffer_->pathMoveTo(x, y);

    buffer_->pathText(text, font, align);

    buffer_->pathStroke();

    //---

    setTransform(transform);
  }
}

void
CSVG::
fillText(double x, double y, const std::string &text, CFontPtr font, CHAlignType align)
{
  setFillBuffer(buffer_);

  CSVGFont *svg_font = getFont();

  if (svg_font) {
    CSVGFontFace *font_face = svg_font->getFontFace();

    double units = 1000;

    if (font_face)
      units = font_face->getUnits();

    //-----

    CMatrixStack2D transform;

    getTransform(transform);

    double font_size = 10.0;

    transform.scale    (font_size/units, font_size/units);
    transform.translate(x, y);

    uint len = text.size();

    for (uint i = 0; i < len; ++i) {
      CSVGGlyph *glyph = getCharGlyph(text[i]);

      if (glyph) {
        setTransform(transform);

        glyph->drawSubObject();

        x += font_size;

        transform.translate(x, y);
      }
    }

    setTransform(transform);
  }
  else {
    CMatrixStack2D transform;

    getTransform(transform);

    CMatrixStack2D transform1 = transform;

    if (font->getAngle()) {
      transform1.rotate(font->getAngle(), CPoint2D(x, y));

      setTransform(transform1);
    }

    //---

    buffer_->pathInit();

    buffer_->pathMoveTo(x, y);

    buffer_->pathText(text, font, align);

    buffer_->pathFill();

    //---

    setTransform(transform);
  }
}

void
CSVG::
textSize(const std::string &text, CFontPtr font, double *w, double *a, double *d) const
{
  *w = font->getStringWidth(text);
  *a = font->getCharAscent();
  *d = font->getCharDescent();
}

void
CSVG::
pathInit()
{
  buffer_->pathInit();
}

void
CSVG::
pathMoveTo(double dx, double dy)
{
  buffer_->pathMoveTo(dx, dy);
}

void
CSVG::
pathLineTo(double dx, double dy)
{
  buffer_->pathLineTo(dx, dy);
}

void
CSVG::
pathRLineTo(double dx, double dy)
{
  buffer_->pathRLineTo(dx, dy);
}

void
CSVG::
pathArcTo(double cx, double cy, double rx, double ry, double theta1, double theta2)
{
  buffer_->pathArcTo(cx, cy, rx, ry, theta1, theta2);
}

void
CSVG::
pathArcSegment(double xc, double yc, double angle1, double angle2,
               double rx, double ry, double phi)
{
  angle1 = CMathGen::DegToRad(angle1);
  angle2 = CMathGen::DegToRad(angle2);
  phi    = CMathGen::DegToRad(phi);

  double sin_a1  = sin(angle1);
  double cos_a1  = cos(angle1);
  double sin_a2  = sin(angle2);
  double cos_a2  = cos(angle2);
  double sin_phi = sin(phi);
  double cos_phi = cos(phi);

  double a00 =  cos_phi*rx;
  double a01 = -sin_phi*ry;
  double a10 =  sin_phi*rx;
  double a11 =  cos_phi*ry;

  double da2 = 0.5*(angle2 - angle1);

  double t = (8.0/3.0)*sin(da2*0.5)*sin(da2*0.5)/sin(da2);

  double x1 = xc + cos_a1 - t*sin_a1;
  double y1 = yc + sin_a1 + t*cos_a1;
  double x3 = xc + cos_a2;
  double y3 = yc + sin_a2;
  double x2 = x3 + t*sin_a2;
  double y2 = y3 - t*cos_a2;

  pathBezier3To(a00*x1 + a01*y1, a10*x1 + a11*y1,
                a00*x2 + a01*y2, a10*x2 + a11*y2,
                a00*x3 + a01*y3, a10*x3 + a11*y3);
}

void
CSVG::
pathBezier2To(double x1, double y1, double x2, double y2)
{
  buffer_->pathBezier2To(x1, y1, x2, y2);
}

void
CSVG::
pathRBezier2To(double x1, double y1, double x2, double y2)
{
  buffer_->pathRBezier2To(x1, y1, x2, y2);
}

void
CSVG::
pathBezier3To(double x1, double y1, double x2, double y2, double x3, double y3)
{
  buffer_->pathBezier3To(x1, y1, x2, y2, x3, y3);
}

void
CSVG::
pathRBezier3To(double x1, double y1, double x2, double y2, double x3, double y3)
{
  buffer_->pathRBezier3To(x1, y1, x2, y2, x3, y3);
}

void
CSVG::
pathClose()
{
  buffer_->pathClose();
}

bool
CSVG::
pathGetCurrentPoint(double *x, double *y)
{
  return buffer_->pathGetCurrentPoint(x, y);
}

void
CSVG::
pathStroke()
{
  setStrokeBuffer();

  buffer_->pathStroke();
}

void
CSVG::
pathFill()
{
  setFillBuffer(buffer_);

  buffer_->pathFill();
}

void
CSVG::
pathClip()
{
  buffer_->pathClip();
}

void
CSVG::
pathEoClip()
{
  buffer_->pathEoClip();
}

void
CSVG::
pathBBox(CBBox2D &bbox)
{
  buffer_->pathBBox(bbox);
}

void
CSVG::
initClip()
{
  buffer_->initClip();
}

//--------------

bool
CSVG::
pathOption(const std::string &opt_name, const std::string &opt_value,
           const std::string &name, CSVG::PartList &parts)
{
  if (opt_name != name)
    return false;

  if (! pathStringToParts(opt_value, parts))
    return false;

  return true;
}

bool
CSVG::
pathStringToParts(const std::string &data, CSVG::PartList &parts)
{
  COptValT<double> bezier2_x2, bezier2_y2, bezier2_x3, bezier2_y3;
  COptValT<double> bezier3_x2, bezier3_y2, bezier3_x3, bezier3_y3;

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

    if      (c == 'm') {
      lastC = c;

      double x, y;

      parse.skipSpace();

      if (! parse.readReal(&x)) { flag = false; break; }

      skipCommaSpace(parse);

      if (! parse.readReal(&y)) { flag = false; break; }

      skipCommaSpace(parse);

      parts.push_back(new CSVGPathMoveTo(*this, x, y));

      while (parse.isDigit() || parse.isChar('-')) {
        if (! parse.readReal(&x)) { flag = false; break; }

        skipCommaSpace(parse);

        if (! parse.readReal(&y)) { flag = false; break; }

        skipCommaSpace(parse);

        parts.push_back(new CSVGPathRLineTo(*this, x, y));
      }
    }
    else if (c == 'M') {
      lastC = c;

      double x, y;

      parse.skipSpace();

      if (! parse.readReal(&x)) { flag = false; break; }

      skipCommaSpace(parse);

      if (! parse.readReal(&y)) { flag = false; break; }

      skipCommaSpace(parse);

      parts.push_back(new CSVGPathMoveTo(*this, x, y));

      while (parse.isDigit() || parse.isChar('-')) {
        if (! parse.readReal(&x)) { flag = false; break; }

        skipCommaSpace(parse);

        if (! parse.readReal(&y)) { flag = false; break; }

        skipCommaSpace(parse);

        parts.push_back(new CSVGPathLineTo(*this, x, y));
      }
    }
    else if (c == 'l') {
      lastC = c;

      double x, y;

      parse.skipSpace();

      if (! parse.readReal(&x)) { flag = false; break; }

      skipCommaSpace(parse);

      if (! parse.readReal(&y)) { flag = false; break; }

      skipCommaSpace(parse);

      parts.push_back(new CSVGPathRLineTo(*this, x, y));

      while (parse.isDigit() || parse.isChar('-')) {
        if (! parse.readReal(&x)) { flag = false; break; }

        skipCommaSpace(parse);

        if (! parse.readReal(&y)) { flag = false; break; }

        skipCommaSpace(parse);

        parts.push_back(new CSVGPathRLineTo(*this, x, y));
      }
    }
    else if (c == 'L') {
      lastC = c;

      double x, y;

      parse.skipSpace();

      if (! parse.readReal(&x)) { flag = false; break; }

      skipCommaSpace(parse);

      if (! parse.readReal(&y)) { flag = false; break; }

      skipCommaSpace(parse);

      parts.push_back(new CSVGPathLineTo(*this, x, y));

      while (parse.isDigit() || parse.isChar('-')) {
        if (! parse.readReal(&x)) { flag = false; break; }

        skipCommaSpace(parse);

        if (! parse.readReal(&y)) { flag = false; break; }

        skipCommaSpace(parse);

        parts.push_back(new CSVGPathLineTo(*this, x, y));
      }
    }
    else if (c == 'h') {
      lastC = c;

      double d;

      parse.skipSpace();

      if (! parse.readReal(&d)) { flag = false; break; }

      parts.push_back(new CSVGPathRLineTo(*this, d, 0));
    }
    else if (c == 'H') {
      lastC = c;

      double d;

      parse.skipSpace();

      if (! parse.readReal(&d)) { flag = false; break; }

      parts.push_back(new CSVGPathHLineTo(*this, d));
    }
    else if (c == 'v') {
      lastC = c;

      double d;

      parse.skipSpace();

      if (! parse.readReal(&d)) { flag = false; break; }

      parts.push_back(new CSVGPathRLineTo(*this, 0, d));
    }
    else if (c == 'V') {
      lastC = c;

      double d;

      parse.skipSpace();

      if (! parse.readReal(&d)) { flag = false; break; }

      parts.push_back(new CSVGPathVLineTo(*this, d));
    }
    else if (c == 'a') {
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

      parts.push_back(new CSVGPathRArcTo(*this, rx, ry, xa, fa, fs, x2, y2));

      parse.skipSpace();

      while (parse.isDigit() || parse.isChar('-')) {
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

        parts.push_back(new CSVGPathRArcTo(*this, rx, ry, xa, fa, fs, x2, y2));

        parse.skipSpace();
      }
    }
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

      parts.push_back(new CSVGPathArcTo(*this, rx, ry, xa, fa, fs, x2, y2));
    }
    else if (c == 'q') { // Quadratic Bezier (Relative)
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

      parts.push_back(new CSVGPathRBezier2To(*this, x1, y1, x2, y2));

      while (parse.isDigit() || parse.isChar('-')) {
        if (! parse.readReal(&x1)) { flag = false; break; }

        skipCommaSpace(parse);

        if (! parse.readReal(&y1)) { flag = false; break; }

        skipCommaSpace(parse);

        if (! parse.readReal(&x2)) { flag = false; break; }

        skipCommaSpace(parse);

        if (! parse.readReal(&y2)) { flag = false; break; }

        skipCommaSpace(parse);

        parts.push_back(new CSVGPathRBezier2To(*this, x1, y1, x2, y2));
      }
    }
    else if (c == 'Q') { // Quadratic Bezier (Absolute)
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

      parts.push_back(new CSVGPathBezier2To(*this, x1, y1, x2, y2));

      bezier2_x2.setValue(x1); bezier2_y2.setValue(y1);
      bezier2_x3.setValue(x2); bezier2_y3.setValue(y2);

      while (parse.isDigit() || parse.isChar('-')) {
        if (! parse.readReal(&x1)) { flag = false; break; }

        skipCommaSpace(parse);

        if (! parse.readReal(&y1)) { flag = false; break; }

        skipCommaSpace(parse);

        if (! parse.readReal(&x2)) { flag = false; break; }

        skipCommaSpace(parse);

        if (! parse.readReal(&y2)) { flag = false; break; }

        skipCommaSpace(parse);

        parts.push_back(new CSVGPathBezier2To(*this, x1, y1, x2, y2));

        bezier2_x2.setValue(x1); bezier2_y2.setValue(y1);
        bezier2_x3.setValue(x2); bezier2_y3.setValue(y2);
      }
    }
    else if (c == 't') {
      lastC = c;

      double x2, y2;

      parse.skipSpace();

      if (! parse.readReal(&x2)) { flag = false; break; }

      skipCommaSpace(parse);

      if (! parse.readReal(&y2)) { flag = false; break; }

      if (bezier2_x2.isValid()) {
        double x1 = 2*bezier2_x3.getValue() - bezier2_x2.getValue();
        double y1 = 2*bezier2_y3.getValue() - bezier2_y2.getValue();

        parts.push_back(new CSVGPathRBezier2To(*this, x1, y1, x2, y2));
      }
    }
    else if (c == 'T') {
      lastC = c;

      double x2, y2;

      parse.skipSpace();

      if (! parse.readReal(&x2)) { flag = false; break; }

      skipCommaSpace(parse);

      if (! parse.readReal(&y2)) { flag = false; break; }

      if (bezier2_x2.isValid()) {
        double x1 = 2*bezier2_x3.getValue() - bezier2_x2.getValue();
        double y1 = 2*bezier2_y3.getValue() - bezier2_y2.getValue();

        parts.push_back(new CSVGPathBezier2To(*this, x1, y1, x2, y2));

        bezier2_x2.setValue(x1); bezier2_y2.setValue(y1);
        bezier2_x3.setValue(x2); bezier2_y3.setValue(y2);
      }
    }
    else if (c == 'c') { // Cubic Bezier (Relative)
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

      parts.push_back(new CSVGPathRBezier3To(*this, x1, y1, x2, y2, x3, y3));

      while (parse.isDigit() || parse.isChar('-')) {
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

        parts.push_back(new CSVGPathRBezier3To(*this, x1, y1, x2, y2, x3, y3));
      }
    }
    else if (c == 'C') { // Cubic Bezier (Absolute)
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

      parts.push_back(new CSVGPathBezier3To(*this, x1, y1, x2, y2, x3, y3));

      bezier3_x2.setValue(x2); bezier3_y2.setValue(y2);
      bezier3_x3.setValue(x3); bezier3_y3.setValue(y3);

      while (parse.isDigit() || parse.isChar('-')) {
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

        parts.push_back(new CSVGPathBezier3To(*this, x1, y1, x2, y2, x3, y3));

        bezier3_x2.setValue(x2); bezier3_y2.setValue(y2);
        bezier3_x3.setValue(x3); bezier3_y3.setValue(y3);
      }
    }
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

      if (bezier3_x2.isValid()) {
        double x1 = 2*bezier3_x3.getValue() - bezier3_x2.getValue();
        double y1 = 2*bezier3_y3.getValue() - bezier3_y2.getValue();

        parts.push_back(new CSVGPathRBezier3To(*this, x1, y1, x2, y2, x3, y3));

        bezier3_x2.setValue(x2); bezier3_y2.setValue(y2);
        bezier3_x3.setValue(x3); bezier3_y3.setValue(y3);
      }
    }
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

      if (bezier3_x2.isValid()) {
        double x1 = 2*bezier3_x3.getValue() - bezier3_x2.getValue();
        double y1 = 2*bezier3_y3.getValue() - bezier3_y2.getValue();

        parts.push_back(new CSVGPathBezier3To(*this, x1, y1, x2, y2, x3, y3));

        bezier3_x2.setValue(x2); bezier3_y2.setValue(y2);
        bezier3_x3.setValue(x3); bezier3_y3.setValue(y3);
      }
    }
    else if (c == 'z') {
      lastC = c;

      parts.push_back(new CSVGPathClosePath(*this));
    }
    else if (c == 'Z') {
      lastC = c;

      parts.push_back(new CSVGPathClosePath(*this));
    }
    else {
      // repeat last command if any
      if (isdigit(c) && lastC != '\0') {
        parse.unreadChar();

        c = lastC;

        rereadCmd = true;

        continue;
      }

      CSVGLog() << "Invalid path command " << c;

      return false;
    }

    skipCommaSpace(parse);
  }

  if (! flag) {
    CSVGLog() << "Path parse fail :";

    std::cout << parse.getBefore() << ">" <<
                 parse.getAt    () << "<" <<
                 parse.getAfter ();
  }

  return flag;
}

void
CSVG::
drawParts(const CSVG::PartList &parts, CSVGObjectMarker *omarker)
{
  std::vector<CPoint2D> points;

  // add path parts and same path part points for markers (if any)
  buffer_->pathInit();

  auto p1 = parts.begin();
  auto p2 = parts.end  ();

  double x1, y1;

  while (p1 != p2 && ! buffer_->pathGetCurrentPoint(&x1, &y1)) {
    (*p1)->draw();

    ++p1;
  }

  if (p1 == p2)
    return;

  points.push_back(CPoint2D(x1, y1));

  while (p1 != p2) {
    (*p1)->draw();

    ++p1;

    double x2, y2;

    buffer_->pathGetCurrentPoint(&x2, &y2);

    if (! REAL_EQ(x1, x2) || ! REAL_EQ(y1, y2)) {
      points.push_back(CPoint2D(x2, y2));

      x1 = x2;
      y1 = y2;
    }
  }

  // fill and/or stroke path
  if (isFilled() || isStroked()) {
    if (isFilled())
      pathFill();

    if (isStroked())
      pathStroke();
  }
  else
    pathFill();

  //------

  // draw markers
  if (omarker->start || omarker->mid || omarker->end) {
    uint num = points.size();

    if (num <= 0)
      return;

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

    double g1, g2;

    g1 = atan2(y2 - y1, x2 - x1);
    g2 = atan2(y3 - y2, x3 - x2);

    if (omarker->start) {
      CSVGMarker *marker = dynamic_cast<CSVGMarker *>(omarker->start);

      if (marker)
        marker->drawMarker(x1, y1, (g1 + g2)/2);
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

      if (i != num - 1) {
        if (omarker->mid) {
          CSVGMarker *marker = dynamic_cast<CSVGMarker *>(omarker->mid);

          if (marker)
            marker->drawMarker(x2, y2, (g1 + g2)/2);
        }
      }
      else {
        if (omarker->end) {
          CSVGMarker *marker = dynamic_cast<CSVGMarker *>(omarker->end);

          if (marker)
            marker->drawMarker(x2, y2, g1);
        }
      }

      g1 = g2;

      x1 = x2; y1 = y2;
      x2 = x3; y2 = y3;
    }
  }
}

bool
CSVG::
interpParts(double s, const CSVG::PartList &parts, double *xi, double *yi, double *a)
{
  *xi = 0;
  *yi = 0;

  std::vector<CPoint2D>       points;
  std::vector<CSVGPathPart *> pparts;

  // add path points
  buffer_->pathInit();

  auto p1 = parts.begin();
  auto p2 = parts.end  ();

  double x1, y1;

  while (p1 != p2 && ! buffer_->pathGetCurrentPoint(&x1, &y1)) {
    (*p1)->draw();

    ++p1;
  }

  *xi = x1;
  *yi = y1;

  if (p1 == p2)
    return false;

  points.push_back(CPoint2D(x1, y1));
  pparts.push_back(*p1);

  while (p1 != p2) {
    auto pt = p1;

    (*p1)->draw();

    ++p1;

    double x2, y2;

    buffer_->pathGetCurrentPoint(&x2, &y2);

    if (! REAL_EQ(x1, x2) || ! REAL_EQ(y1, y2)) {
      points.push_back(CPoint2D(x2, y2));
      pparts.push_back(*pt);

      x1 = x2;
      y1 = y2;
    }
  }

  // interp points
  int num = points.size();

  double xmin = points[0      ].x;
  double xmax = points[num - 1].x;

  *xi = CSVGUtil::map(s, 0, 1, xmin, xmax);

  for (int i = 0; i < num - 1; ++i) {
    if (*xi >= points[i].x && *xi <= points[i + 1].x) {
      *yi = pparts[i + 1]->interp(*xi, points[i], points[i + 1], *a);

      break;
    }
  }

  return true;
}

bool
CSVG::
getPartsBBox(const CSVG::PartList &parts, CBBox2D &bbox) const
{
  CSVG *th = const_cast<CSVG *>(this);

  th->buffer_->pathInit();

  for (const auto &p : parts)
    p->draw();

  th->pathBBox(bbox);

  return bbox.isSet();
}

void
CSVG::
printParts(std::ostream &os, const CSVG::PartList &parts) const
{
  int i = 0;

  for (const auto &p : parts) {
    if (i > 0) os << " ";

    p->print(os);

    ++i;
  }
}

//--------------

bool
CSVG::
coordOption(const std::string &opt_name, const std::string &opt_value,
            const std::string &name, CSVGLengthValue &length)
{
  if (opt_name != name)
    return false;

  bool flag = true;

  std::vector<std::string> match_strs;

  if (CRegExpUtil::parse(opt_value, "\\(.*\\)%", match_strs)) {
    double value;

    if (! CStrUtil::toReal(match_strs[0], &value)) {
      value = 0.0;
      flag = false;
    }

    length = CSVGLengthValue(CSVGLengthType::PERCENT, value/100.0);
  }
  else {
    CSVGLengthValue length1;

    if (! decodeLengthValue(opt_value, length1)) {
      CSVGLog() << "Illegal value for " << name;
      length = CSVGLengthValue();
      flag = false;
    }

    length = length1;
  }

  return flag;
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
      flag = false;
    }

    *value /= 100;
  }
  else {
    CSVGLengthValue lvalue;

    if (! decodeLengthValue(opt_value, lvalue)) {
      CSVGLog() << "Illegal value for " << name;
      *value = 0;
      flag = false;
    }

    *value = lvalue.value();
  }

  return flag;
}

bool
CSVG::
lengthOption(const std::string &opt_name, const std::string &opt_value,
             const std::string &name, CSVGLengthValue &length)
{
  if (opt_name != name)
    return false;

  CSVGLengthValue length1;

  if (! decodeLengthValue(opt_value, length1)) {
    CSVGLog() << "Illegal value for " << name;
    return false;
  }

  length = length1;

  return true;
}

bool
CSVG::
decodeLengthValue(const std::string &str, CSVGLengthValue &lvalue)
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

    lvalue = CSVGLengthValue(CSVGLengthType::EM, value);

    CSVGLog() << "em conversion not handled";
  }
  else if (CRegExpUtil::parse(str, ex_pattern, match_strs)) {
    if (! CStrUtil::toReal(match_strs[0], &value))
      return false;

    lvalue = CSVGLengthValue(CSVGLengthType::EX, value);

    CSVGLog() << "ex conversion not handled";
  }
  else if (CRegExpUtil::parse(str, pt_pattern, match_strs)) {
    if (! CStrUtil::toReal(match_strs[0], &value))
      return false;

    double ivalue = value;

    mmToPixel((25.4*value)/72.0, &value);

    lvalue = CSVGLengthValue(CSVGLengthType::PT, ivalue, value);
  }
  else if (CRegExpUtil::parse(str, pc_pattern, match_strs)) {
    if (! CStrUtil::toReal(match_strs[0], &value))
      return false;

    double ivalue = value;

    mmToPixel((25.4*value)/6.0, &value);

    lvalue = CSVGLengthValue(CSVGLengthType::PC, ivalue, value);
  }
  else if (CRegExpUtil::parse(str, cm_pattern, match_strs)) {
    if (! CStrUtil::toReal(match_strs[0], &value))
      return false;

    double ivalue = value;

    mmToPixel(10*value, &value);

    lvalue = CSVGLengthValue(CSVGLengthType::CM, ivalue, value);
  }
  else if (CRegExpUtil::parse(str, mm_pattern, match_strs)) {
    if (! CStrUtil::toReal(match_strs[0], &value))
      return false;

    double ivalue = value;

    mmToPixel(value, &value);

    lvalue = CSVGLengthValue(CSVGLengthType::MM, ivalue, value);
  }
  else if (CRegExpUtil::parse(str, in_pattern, match_strs)) {
    if (! CStrUtil::toReal(match_strs[0], &value))
      return false;

    double ivalue = value;

    mmToPixel(25.4*value, &value);

    lvalue = CSVGLengthValue(CSVGLengthType::IN, ivalue, value);
  }
  else if (CRegExpUtil::parse(str, px_pattern, match_strs)) {
    if (! CStrUtil::toReal(match_strs[0], &value))
      return false;

    lvalue = CSVGLengthValue(CSVGLengthType::PX, value);
  }
  else if (CRegExpUtil::parse(str, ph_pattern, match_strs)) {
    if (! CStrUtil::toReal(match_strs[0], &value))
      return false;

    lvalue = CSVGLengthValue(CSVGLengthType::PERCENT, value);
  }
  else {
    if (! CStrUtil::toReal(str, &value))
      return false;

    lvalue = CSVGLengthValue(CSVGLengthType::NONE, value);
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
    CSVGLog() << "Illegal value for " << name;
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
    CSVGLog() << "Illegal value for " << name;
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
    CSVGLog() << "Illegal value for " << name;
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
              const std::string &name, CSVGLengthValue &length)
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

  if (words.size() != 4)
    return false;

  double x1, y1, x2, y2;

  if (! CStrUtil::toReal(CStrUtil::stripSpaces(words[0]), &x1)) return false;
  if (! CStrUtil::toReal(CStrUtil::stripSpaces(words[1]), &y1)) return false;
  if (! CStrUtil::toReal(CStrUtil::stripSpaces(words[2]), &x2)) return false;
  if (! CStrUtil::toReal(CStrUtil::stripSpaces(words[3]), &y2)) return false;

  *bbox = CBBox2D(x1, y1, x2, y2);

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

  std::vector<std::string> words;

  CStrUtil::addWords(opt_value, words, " ,\n\t");

  uint num_xy = words.size()/2;

  points.resize(num_xy);

  for (uint i = 0, j = 0; j < num_xy; i += 2, ++j) {
    std::string x_word = CStrUtil::stripSpaces(words[i    ]);
    std::string y_word = CStrUtil::stripSpaces(words[i + 1]);

    if (! CStrUtil::toReal(x_word, &points[j].x) ||
        ! CStrUtil::toReal(y_word, &points[j].y))
      return false;
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

    if (! CStrUtil::toReal(word, &reals[i]))
      return false;
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

      double m00, m01, m10, m11, tx, ty;

      if (! parse.readReal(&m00)) {
        CSVGLog() << "Invalid matrix " << str;
        return false;
      }

      skipCommaSpace(parse);

      if (! parse.readReal(&m01)) {
        CSVGLog() << "Invalid matrix " << str;
        return false;
      }

      skipCommaSpace(parse);

      if (! parse.readReal(&m10)) {
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
  *scale  = CSVGScale::FREE;

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

    mmToPixel(25.4*width/72.0, &width);
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
decodeFillRuleString(const std::string &rule_str)
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

bool
CSVG::
decodeDashString(const std::string &dash_str, CLineDash &dash)
{
  std::vector<std::string> words;
  double                   length;
  std::vector<double>      lengths;

  if (dash_str == "solid") {
    dash.setDashes(lengths);

    return true;
  }

  CStrUtil::addWords(dash_str, words, " ,");

  uint num_words = words.size();

  bool duplicate = (num_words & 1);

  for (uint i = 0; i < num_words; ++i) {
    if (! CStrUtil::toReal(CStrUtil::stripSpaces(words[i]), &length))
      return false;

    lengths.push_back(length);
  }

  if (duplicate) {
    num_words = lengths.size();

    for (uint i = 0; i < num_words; ++i)
      lengths.push_back(lengths[i]);
  }

  dash.setDashes(lengths);

  return true;
}

bool
CSVG::
decodeColorString(const std::string &color_str, CRGBA &rgba)
{
  std::vector<std::string> match_strs;

  if      (CRegExpUtil::parse(color_str, "rgb(\\(.*\\))", match_strs)) {
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
  else if (color_str.size() == 4 &&
           CRegExpUtil::parse(color_str, "#" RE_HEXDEC RE_HEXDEC RE_HEXDEC, match_strs)) {
    std::string color_str1 = "#";

    color_str1 += color_str.substr(1, 1) + color_str.substr(1, 1) +
                  color_str.substr(2, 1) + color_str.substr(2, 1) +
                  color_str.substr(3, 1) + color_str.substr(3, 1);

    rgba = CRGBName::toRGBA(color_str1);
  }
  else if (color_str.size() == 7 &&
           CRegExpUtil::parse(color_str, "#" RE_HEXDEC RE_HEXDEC RE_HEXDEC
                       RE_HEXDEC RE_HEXDEC RE_HEXDEC, match_strs)) {
    std::string color_str1 = "#";

    color_str1 += color_str.substr(1, 2) +
                  color_str.substr(3, 2) +
                  color_str.substr(5, 2);

    rgba = CRGBName::toRGBA(color_str1);
  }
  else {
    if (color_str == "none")
      rgba = CRGBA(0,0,0,0);
    else
      rgba = CRGBName::toRGBA(color_str);
  }

  return true;
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
decodePercentString(const std::string &str, CSVGLengthValue &length)
{
  bool flag = true;

  std::vector<std::string> match_strs;

  if (CRegExpUtil::parse(str, "\\(.*\\)%", match_strs)) {
    double value;

    if (! CStrUtil::toReal(match_strs[0], &value)) {
      value = 0.0;
      flag = false;
    }

    length = CSVGLengthValue(CSVGLengthType::PERCENT, value/100.0);
  }
  else {
    double value;

    if (! CStrUtil::toReal(str, &value)) {
      value = 0.0;
      flag = false;
    }

    length = CSVGLengthValue(value);
  }

  return flag;
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

  if (opt_value == "none") {
    *obj = 0;
    return true;
  }

  CSVGObject *obj1;

  if (! decodeUrlObject(opt_value, &obj1)) {
    CSVGLog() << "Illegal value for " << name;
    return false;
  }

  *obj = obj1;

  return true;
}

bool
CSVG::
decodeUrlObject(const std::string &str, CSVGObject **object)
{
  std::string              id;
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

bool
CSVG::
mmToPixel(double mm, double *pixel)
{
  if (buffer_)
    return buffer_->mmToPixel(mm, pixel);
  else {
    // 75 dpi
    *pixel = 75.0*mm/25.4;

    return true;
  }
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

  getBlock()->getChildrenOfType(CSVGObjTypeId::TITLE, objects);

  if (objects.empty())
    return false;

  CSVGTitle *title = dynamic_cast<CSVGTitle *>(objects[0]);

  str = title->getText();

  return true;
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
  buffer_->windowToPixel(xi, yi, xo, yo);
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
  std::vector<std::string> ids;

  css_.getIds(ids);

  for (const auto &id : ids) {
    const CCSS::StyleData &cssStyleData = css_.getStyleData(id);

    std::string objType, objClass;

    auto p = id.find(".");

    if (p != std::string::npos) {
      objType  = id.substr(0, p);
      objClass = id.substr(p + 1);
    }
    else
      objType = id;

    if (objType == "" && objClass == "")
      continue;

    if (objType  == "*") objType  = "";
    if (objClass == "*") objClass = "";

    if      (objType != "" && objClass == "")
      addStyleValues(getTypeStyleData(objType), cssStyleData);
    else if (objType == "" && objClass != "")
      addStyleValues(getClassStyleData(objClass), cssStyleData);
    else if (objType != "" && objClass  != "")
      addStyleValues(getTypeClassStyleData(objType, objClass), cssStyleData);
    else
      addStyleValues(getGlobalStyleData(), cssStyleData);
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

CSVGStyleData &
CSVG::
getGlobalStyleData()
{
  auto p = globalStyleData_.find("*");

  if (p == globalStyleData_.end())
    p = globalStyleData_.insert(p,
      StyleDataMap::value_type("*", CSVGStyleData(*this, "*")));

  CSVGStyleData &styleData = (*p).second;

  return styleData;
}

CSVGStyleData &
CSVG::
getTypeStyleData(const std::string &objType)
{
  auto p = typeStyleData_.find(objType);

  if (p == typeStyleData_.end())
    p = typeStyleData_.insert(p,
      StyleDataMap::value_type(objType, CSVGStyleData(*this, objType)));

  CSVGStyleData &styleData = (*p).second;

  return styleData;
}

CSVGStyleData &
CSVG::
getClassStyleData(const std::string &objClass)
{
  auto p = classStyleData_.find(objClass);

  if (p == classStyleData_.end())
    p = classStyleData_.insert(p,
      StyleDataMap::value_type(objClass, CSVGStyleData(*this, objClass)));

  CSVGStyleData &styleData = (*p).second;

  return styleData;
}

CSVGStyleData &
CSVG::
getTypeClassStyleData(const std::string &objType, const std::string &objClass)
{
  auto p1 = typeClassStyleData_.find(objType);

  if (p1 == typeClassStyleData_.end())
    p1 = typeClassStyleData_.insert(p1,
      TypeStyleDataMap::value_type(objType, StyleDataMap()));

  StyleDataMap &typeStyleDataMap = (*p1).second;

  auto p2 = typeStyleDataMap.find(objClass);

  if (p2 == typeStyleDataMap.end())
    p2 = typeStyleDataMap.insert(p2,
      StyleDataMap::value_type(objClass, CSVGStyleData(*this, objClass)));

  CSVGStyleData &styleData = (*p2).second;

  return styleData;
}

bool
CSVG::
getStyleStrokeColor(const CSVGObject *obj, CRGBA &rgba)
{
  CSVGStyleData &typeStyleData = getTypeStyleData(obj->getObjName());

  if (typeStyleData.getStrokeColorValid()) {
    rgba = typeStyleData.getStrokeColor();
    return true;
  }

  for (const auto &c : obj->getClasses()) {
    CSVGStyleData &classStyleData = getClassStyleData(c);

    if (classStyleData.getStrokeColorValid()) {
      rgba = classStyleData.getStrokeColor();
      return true;
    }
  }

  CSVGStyleData &globalStyleData = getGlobalStyleData();

  if (globalStyleData.getStrokeColorValid()) {
    rgba = globalStyleData.getStrokeColor();
    return true;
  }

  return false;
}

bool
CSVG::
getStyleStrokeOpacity(const CSVGObject *obj, double &opacity)
{
  CSVGStyleData &typeStyleData = getTypeStyleData(obj->getObjName());

  if (typeStyleData.getStrokeOpacityValid()) {
    opacity = typeStyleData.getStrokeOpacity();
    return true;
  }

  for (const auto &c : obj->getClasses()) {
    CSVGStyleData &classStyleData = getClassStyleData(c);

    if (classStyleData.getStrokeOpacityValid()) {
      opacity = classStyleData.getStrokeOpacity();
      return true;
    }
  }

  CSVGStyleData &globalStyleData = getGlobalStyleData();

  if (globalStyleData.getStrokeOpacityValid()) {
    opacity = globalStyleData.getStrokeOpacity();
    return true;
  }

  return false;
}

bool
CSVG::
getStyleStrokeWidth(const CSVGObject *obj, double &width)
{
  CSVGStyleData &typeStyleData = getTypeStyleData(obj->getObjName());

  if (typeStyleData.getStrokeWidthValid()) {
    width = typeStyleData.getStrokeWidth();
    return true;
  }

  for (const auto &c : obj->getClasses()) {
    CSVGStyleData &classStyleData = getClassStyleData(c);

    if (classStyleData.getStrokeWidthValid()) {
      width = classStyleData.getStrokeWidth();
      return true;
    }
  }

  CSVGStyleData &globalStyleData = getGlobalStyleData();

  if (globalStyleData.getStrokeWidthValid()) {
    width = globalStyleData.getStrokeWidth();
    return true;
  }

  return false;
}

bool
CSVG::
getStyleStrokeDash(const CSVGObject *obj, CLineDash &dash)
{
  CSVGStyleData &typeStyleData = getTypeStyleData(obj->getObjName());

  if (typeStyleData.getStrokeDashValid()) {
    dash = typeStyleData.getStrokeDash();
    return true;
  }

  for (const auto &c : obj->getClasses()) {
    CSVGStyleData &classStyleData = getClassStyleData(c);

    if (classStyleData.getStrokeDashValid()) {
      dash = classStyleData.getStrokeDash();
      return true;
    }
  }

  CSVGStyleData &globalStyleData = getGlobalStyleData();

  if (globalStyleData.getStrokeDashValid()) {
    dash = globalStyleData.getStrokeDash();
    return true;
  }

  return false;
}

bool
CSVG::
getStyleFillColor(const CSVGObject *obj, CRGBA &rgba)
{
  for (const auto &c : obj->getClasses()) {
    CSVGStyleData &typeClassStyleData = getTypeClassStyleData(obj->getObjName(), c);

    if (typeClassStyleData.getFillColorValid()) {
      rgba = typeClassStyleData.getFillColor();
      return true;
    }
  }

  CSVGStyleData &typeStyleData = getTypeStyleData(obj->getObjName());

  if (typeStyleData.getFillColorValid()) {
    rgba = typeStyleData.getFillColor();
    return true;
  }

  for (const auto &c : obj->getClasses()) {
    CSVGStyleData &classStyleData = getClassStyleData(c);

    if (classStyleData.getFillColorValid()) {
      rgba = classStyleData.getFillColor();
      return true;
    }
  }

  CSVGStyleData &globalStyleData = getGlobalStyleData();

  if (globalStyleData.getFillColorValid()) {
    rgba = globalStyleData.getFillColor();
    return true;
  }

  return false;
}

bool
CSVG::
getStyleFillNoColor(const CSVGObject *obj, bool &noColor)
{
  for (const auto &c : obj->getClasses()) {
    CSVGStyleData &typeClassStyleData = getTypeClassStyleData(obj->getObjName(), c);

    if (typeClassStyleData.getFillNoColorValid()) {
      noColor = typeClassStyleData.getFillNoColor();
      return true;
    }
  }

  CSVGStyleData &typeStyleData = getTypeStyleData(obj->getObjName());

  if (typeStyleData.getFillNoColorValid()) {
    noColor = typeStyleData.getFillNoColor();
    return true;
  }

  for (const auto &c : obj->getClasses()) {
    CSVGStyleData &classStyleData = getClassStyleData(c);

    if (classStyleData.getFillNoColorValid()) {
      noColor = classStyleData.getFillNoColor();
      return true;
    }
  }

  CSVGStyleData &globalStyleData = getGlobalStyleData();

  if (globalStyleData.getFillNoColorValid()) {
    noColor = globalStyleData.getFillNoColor();
    return true;
  }

  return false;
}

void
CSVG::
getObjectsAtPoint(const CPoint2D &p, ObjectList &objects) const
{
  getBlock()->getObjectsAtPoint(p, objects);
}

void
CSVG::
sendEvent(CSVGEventType type, const std::string &id, const std::string &data)
{
  getBlock()->handleEvent(type, id, data);
}

void
CSVG::
print(std::ostream &os, bool hier) const
{
  getBlock()->print(os, hier);

  css_.print(os);
}
