#include <CSVGI.h>
#include <CConfig.h>
#include <CCSS.h>
#include <CRegExp.h>
#include <CStrParse.h>

CSVG::
CSVG(CSVGRenderer *renderer) :
 renderer_     (renderer),
 buffer_mgr_   (NULL),
 buffer_       (NULL),
 view_matrix_  (),
 transform_    (),
 block_        (NULL),
 xml_          (NULL),
 xml_tag_      (NULL),
 stroke_       (*this),
 fill_         (*this),
 clip_         (*this),
 font_def_     (*this),
 font_list_    (),
 id_object_map_(),
 styleData_    (),
 uniquify_     (false),
 autoName_     (false),
 debug_        (false)
{
  view_matrix_.setIdentity();
  transform_  .setIdentity();

  block_ = new CSVGBlock(*this);

  xml_ = new CXML();

  buffer_mgr_ = new CSVGBufferMgr(*this);
}

CSVG::
~CSVG()
{
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
  return renderer_->dup();
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
setDebug(bool debug)
{
  debug_ = debug;

  xml_->setDebug(debug);
}

void
CSVG::
init()
{
}

bool
CSVG::
read(const string &filename)
{
  return read(filename, block_);
}

bool
CSVG::
read(const string &filename, CSVGObject *object)
{
  if (! CFile::exists(filename) || ! CFile::isRegular(filename))
    return false;

  if (! xml_->read(filename, &xml_tag_))
    return false;

  if (xml_tag_->getName() != "svg")
    return false;

  //------

  // Process top tokens

  uint num = xml_->getNumTokens();

  for (uint i = 0; i < num; ++i) {
    CXMLToken *token = xml_->getToken(i);

    if (token->isExecute()) {
      CXMLExecute *exec = token->getExecute();

      const string &id = exec->getId();

      if (id == "xml-stylesheet") {
        string ref;
        bool   is_css = false;

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

  CXMLTag::OptionArray::iterator poption1 = xml_tag_->getOptionsBegin();
  CXMLTag::OptionArray::iterator poption2 = xml_tag_->getOptionsEnd  ();

  for ( ; poption1 < poption2; ++poption1) {
    const string &opt_name  = (*poption1)->getName ();
    const string &opt_value = (*poption1)->getValue();

    if (! object->processOption(opt_name, opt_value))
      CSVGLog() << "Invalid option " << opt_name << "=" << opt_value <<
                   " for " << object->getObjName();
  }

  //------

  // Process svg children

  object->setXMLTag(xml_tag_);

  CXMLTag::TokenArray children = xml_tag_->getChildren();

  CXMLTag::TokenArray::const_iterator pchild1 = children.begin();
  CXMLTag::TokenArray::const_iterator pchild2 = children.end  ();

  for ( ; pchild1 != pchild2; ++pchild1) {
    const CXMLToken *token = *pchild1;

    CSVGObject *object1 = tokenToObject(token);

    if (object1 != NULL)
      object->addChildObject(object1);
  }

  return true;
}

CSVGObject *
CSVG::
tokenToObject(const CXMLToken *token)
{
  if (! token->isTag())
    return NULL;

  CXMLTag *tag = token->getTag();

  //-----

  // Create object from tag name

  const string &tag_name = tag->getName();

  CSVGObject *object = createObjectByName(tag_name);

  if (object == NULL) {
    CSVGLog() << "Unknown tag " << tag_name;
    return NULL;
  }

  //-----

  // inform object we are about to parse tag

  object->initParse();

  //-----

  // get tag text (including embedded tspan tags)

  string tag_text;

  uint num_children1 = tag->getNumChildren();

  for (uint i = 0; i < num_children1; ++i) {
    const CXMLToken *token = tag->getChild(i);

    if      (token->isText()) {
      CXMLText *text = token->getText();

      tag_text += text->getText();
    }
    else if (token->isTag()) {
      CXMLTag *tag = token->getTag();

      const string &tag_name = tag->getName();

      if (tag_name == "tspan") {
        CAutoPtr<CSVGObject> to;

        to = tokenToObject(token);

        CSVGTSpan *ts = dynamic_cast<CSVGTSpan *>(to.get());

        CFontStyles styles = ts->getFontStyle();

        // TODO: handle more inline styles
        if (styles & CFONT_STYLE_BOLD  ) tag_text += "[1m";
        if (styles & CFONT_STYLE_ITALIC) tag_text += "[3m";

        tag_text += tag->getText();

        tag_text += "[0m";
      }
    }
  }

  if (tag_text != "")
    object->setText(tag_text);

  //-----

  // process tag options

  uint num_options = tag->getNumOptions();

  const CXMLTag::OptionArray &options = tag->getOptions();

  for (uint i = 0; i < num_options; ++i) {
    CXMLTagOption *option = options[i];

    const string &opt_name  = option->getName ();
    const string &opt_value = option->getValue();

    if (! object->processOption(opt_name, opt_value))
      CSVGLog() << "Unhandled option " << opt_name << "=" << opt_value <<
                   " for " << object->getObjName();
  }

  object->setXMLTag(tag);

  //-----

  // process tag children

  CXMLTag::TokenArray children = tag->getChildren();

  uint num_children = children.size();

  for (uint i = 0; i < num_children; ++i) {
    CSVGObject *object1 = tokenToObject(children[i]);

    if (object1 != NULL)
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
createObjectByName(const string &name)
{
  // TODO: create routines for all object types

  CSVGObject *object;

  if      (name == "svg")
    object = new CSVGBlock(*this);
  else if (name == "circle")
    object = createCircle();
  else if (name == "clipPath")
    object = new CSVGClipPath(*this);
  else if (name == "defs")
    object = new CSVGDefs(*this);
  else if (name == "desc")
    object = new CSVGDesc(*this);
  else if (name == "ellipse")
    object = createEllipse();
  else if (name == "feBlend")
    object = new CSVGFeBlend(*this);
  else if (name == "feColorMatrix")
    object = new CSVGFeColorMatrix(*this);
  else if (name == "feComponentTransfer")
    object = new CSVGFeComponentTransfer(*this);
  else if (name == "feComposite")
    object = new CSVGFeComposite(*this);
  else if (name == "feDiffuseLighting")
    object = new CSVGFeDiffuseLighting(*this);
  else if (name == "feDistantLight")
    object = new CSVGFeDistantLight(*this);
  else if (name == "feFlood")
    object = new CSVGFeFlood(*this);
  else if (name == "feFuncR")
    object = new CSVGFeFunc(*this, CCOLOR_COMPONENT_RED);
  else if (name == "feFuncG")
    object = new CSVGFeFunc(*this, CCOLOR_COMPONENT_GREEN);
  else if (name == "feFuncB")
    object = new CSVGFeFunc(*this, CCOLOR_COMPONENT_BLUE);
  else if (name == "feFuncA")
    object = new CSVGFeFunc(*this, CCOLOR_COMPONENT_ALPHA);
  else if (name == "feGaussianBlur")
    object = new CSVGFeGaussianBlur(*this);
  else if (name == "feImage")
    object = new CSVGFeImage(*this);
  else if (name == "feMerge")
    object = new CSVGFeMerge(*this);
  else if (name == "feMergeNode")
    object = new CSVGFeMergeNode(*this);
  else if (name == "feOffset")
    object = new CSVGFeOffset(*this);
  else if (name == "fePointLight")
    object = new CSVGFePointLight(*this);
  else if (name == "feSpecularLighting")
    object = new CSVGFeSpecularLighting(*this);
  else if (name == "feTile")
    object = new CSVGFeTile(*this);
  else if (name == "feTurbulence")
    object = new CSVGFeTurbulence(*this);
  else if (name == "filter")
    object = new CSVGFilter(*this);
  else if (name == "font")
    object = new CSVGFont(*this);
  else if (name == "font-face")
    object = new CSVGFontFace(*this);
  else if (name == "glyph")
    object = new CSVGGlyph(*this);
  else if (name == "g")
    object = new CSVGGroup(*this);
  else if (name == "hkern")
    object = new CSVGHKern(*this);
  else if (name == "image")
    object = createImage();
  else if (name == "line")
    object = createLine();
  else if (name == "linearGradient")
    object = createLinearGradient();
  else if (name == "marker")
    object = new CSVGMarker(*this);
  else if (name == "mask")
    object = new CSVGMask(*this);
  else if (name == "missing-glyph")
    object = new CSVGMissingGlyph(*this);
  else if (name == "path")
    object = new CSVGPath(*this);
  else if (name == "pattern")
    object = new CSVGPattern(*this);
  else if (name == "polygon")
    object = createPolygon();
  else if (name == "polyline")
    object = createPolyLine();
  else if (name == "radialGradient")
    object = createRadialGradient();
  else if (name == "rect")
    object = createRect();
  else if (name == "stop")
    object = createStop();
  else if (name == "symbol")
    object = new CSVGSymbol(*this);
  else if (name == "style")
    object = new CSVGStyle(*this);
  else if (name == "text")
    object = new CSVGText(*this);
  else if (name == "title")
    object = new CSVGTitle(*this);
  else if (name == "tspan")
    object = new CSVGTSpan(*this);
  else if (name == "use")
    object = new CSVGUse(*this);
  else
    object = NULL;

  //------

  // automatically give object a non-empty name if required

  if (object && getAutoName())
    object->autoName();

  return object;
}

CSVGCircle *
CSVG::
createCircle()
{
  return new CSVGCircle(*this);
}

CSVGEllipse *
CSVG::
createEllipse()
{
  return new CSVGEllipse(*this);
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

CSVGStop *
CSVG::
createStop()
{
  return new CSVGStop(*this);
}

void
CSVG::
addFont(CSVGFont *font)
{
  font_list_.push_back(font);
}

CSVGFont *
CSVG::
getFont() const
{
  if (font_list_.empty())
    return NULL;

  return font_list_[0];
}

CSVGGlyph *
CSVG::
getCharGlyph(char c) const
{
  CSVGFont *font = getFont();

  if (font != NULL)
    return font->getCharGlyph(c);
  else
    return NULL;
}

CSVGGlyph *
CSVG::
getUnicodeGlyph(const string &unicode) const
{
  if (font_list_.empty())
    return NULL;

  CSVGFont *font = font_list_[0];

  return font->getUnicodeGlyph(unicode);
}

CImagePtr
CSVG::
drawToImage(int w, int h)
{
  CAutoPtr<CSVGRenderer> renderer;

  renderer = createRenderer();

  renderer->setSize(w, h);

  setRenderer(renderer);

  draw();

  return renderer->getImage();
}

void
CSVG::
draw()
{
  CMatrix2D matrix(CMATRIX_TYPE_IDENTITY);

  draw(matrix);
}

void
CSVG::
draw(const CMatrix2D &matrix)
{
  drawObject(block_, matrix);
}

void
CSVG::
drawObject(CSVGObject *object)
{
  CMatrix2D matrix(CMATRIX_TYPE_IDENTITY);

  drawObject(object, matrix);
}

void
CSVG::
drawObject(CSVGObject *object, const CMatrix2D &matrix)
{
  if (renderer_ == NULL)
    return;

  view_matrix_ = matrix;

  //------

  renderer_->beginDraw();

  renderer_->setDataRange(object->getXMin(), object->getYMin(),
                          object->getXMax(), object->getYMax());

  //------

  resetStroke();
  resetFill();
  resetClip();
  resetFontDef();

  //------

  setBuffer("SourceGraphic");

  beginDrawBuffer();

  object->drawObject();

  endDrawBuffer();

  CImagePtr image = getBufferImage("SourceGraphic");
//image->writePNG("SourceGraphic.png");

  //------

  setBuffer("BackgroundImage");

  beginDrawBuffer();

  buffer_->setImage(image);

  endDrawBuffer();

  //------

  image = buffer_->getImage();

  renderer_->setImage(image);

  //------

  view_matrix_.setIdentity();

  renderer_->endDraw();
}

CSVGBuffer *
CSVG::
getBuffer() const
{
  return buffer_;
}

string
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
setBuffer(const string &name)
{
  string name1 = name;

  if (name == "SourceGraphic" || name == "SourceAlpha")
    name1 = "Source";

  buffer_ = getBuffer(name1);
}

CImagePtr
CSVG::
getBufferImage()
{
  if (buffer_ != NULL)
    return buffer_->getImage();
  else
    return CImagePtr();
}

CImagePtr
CSVG::
getBufferImage(const string &name)
{
  CSVGBuffer *buffer = getBuffer(name);

  CImagePtr image;

  if (name == "SourceAlpha")
    image = buffer->getImage();
  else
    image = buffer->getImage();

  return image;
}

CSVGBuffer *
CSVG::
getBuffer(const string &name)
{
  string name1 = name;

  if (name == "SourceGraphic" || name == "SourceAlpha")
    name1 = "Source";

  CSVGBuffer *buffer = buffer_mgr_->lookupBuffer(name1);

  if (buffer == NULL)
    buffer = buffer_mgr_->createBuffer(name1);

  return buffer;
}

void
CSVG::
setBufferImage(CImagePtr image)
{
  if (buffer_ != NULL)
    buffer_->setImage(image);
}

void
CSVG::
setBufferImage(const string &name, CImagePtr image)
{
  CSVGBuffer *buffer = getBuffer(name);

  if (name == "SourceAlpha")
    buffer->setImage(image);
  else
    buffer->setImage(image);
}

void
CSVG::
beginDrawBuffer()
{
  CBBox2D bbox;

  block_->getBBox(bbox);

  int width, height;

  renderer_->getSize(&width, &height);

  buffer_->beginDraw(width, height, bbox);

  buffer_->setAlign(block_->getHAlign(), block_->getVAlign());

  buffer_->setEqualScale(block_->getScale() != CSVG_SCALE_FREE);
  buffer_->setScaleMin  (block_->getScale() == CSVG_SCALE_FIXED_MEET);
}

void
CSVG::
endDrawBuffer()
{
  buffer_->endDraw();
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
    CRGBA stroke_color = stroke_.getAlphaColor();

    return (stroke_color.getAlpha() > 0);
  }

  return false;
}

void
CSVG::
setStroke()
{
  if (stroke_.getColorValid()) {
    CRGBA stroke_color = stroke_.getAlphaColor();

    if (stroke_.getOpacityValid()) {
      CRGBA stroke_color1(stroke_color);

      stroke_color1.setAlpha(stroke_.getOpacity());

      buffer_->setStrokeColor(stroke_color1);
    }
    else
      buffer_->setStrokeColor(stroke_color);
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
  if (fill_.getFillObjectValid())
    return true;

  if (fill_.getColorValid()) {
    CRGBA fill_color = fill_.getAlphaColor();

    return (fill_color.getAlpha() > 0);
  }

  return false;
}

void
CSVG::
setFill()
{
  CSVGObject *fill_object = fill_.getFillObject();

  if (fill_object) {
    CSVGLinearGradient *lg = dynamic_cast<CSVGLinearGradient *>(fill_object);
    CSVGRadialGradient *rg = dynamic_cast<CSVGRadialGradient *>(fill_object);
    CSVGPattern        *pt = dynamic_cast<CSVGPattern        *>(fill_object);

    if      (lg) {
      CAutoPtr<CLinearGradient> lg1;

      lg1 = lg->createGradient();

      buffer_->setFillGradient(lg1);
    }
    else if (rg) {
      CAutoPtr<CRadialGradient> rg1;

      rg1 = rg->createGradient();

      buffer_->setFillGradient(rg1);
    }
    else if (pt) {
      double w1, h1;

      CImagePtr image = pt->getImage(100, 100, &w1, &h1);

      buffer_->setFillImage(image);
    }
    else
      assert(false);
  }
  else {
    if (fill_.getColorValid()) {
      CRGBA fill_color = fill_.getAlphaColor();

      buffer_->setFillColor(fill_color);
    }
    else
      buffer_->setFillColor(CRGBA(0,0,0));
  }

  if (fill_.getRuleValid())
    buffer_->setFillType(fill_.getRule());
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
  font_def_.reset();
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
setTransform(const CMatrix2D &matrix)
{
  transform_ = matrix;

  buffer_->setViewMatrix(view_matrix_*transform_);
}

void
CSVG::
getTransform(CMatrix2D &matrix)
{
  matrix = transform_;
}

void
CSVG::
unsetTransform()
{
  transform_.setIdentity();

  buffer_->setViewMatrix(view_matrix_);
}

void
CSVG::
drawImage(double x, double y, CImagePtr image)
{
  buffer_->drawImage(x, y, image);
}

void
CSVG::
drawImage(const CBBox2D &bbox, CImagePtr image)
{
  buffer_->drawImage(bbox, image);
}

void
CSVG::
drawLine(double x1, double y1, double x2, double y2)
{
  setStroke();

  buffer_->pathInit();

  buffer_->pathMoveTo(x1, y1);
  buffer_->pathLineTo(x2, y2);

  buffer_->pathStroke();
}

void
CSVG::
drawRoundedRectangle(const CBBox2D &bbox, double rx, double ry)
{
  setStroke();

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
  setFill();

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
  setStroke();

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
  setFill();

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

  if (isFilled()) {
    setFill();

    buffer_->pathFill();
  }

  if (isStroked()) {
    setStroke();

    buffer_->pathStroke();
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

  if (isFilled()) {
    setFill();

    buffer_->pathFill();
  }

  if (isStroked()) {
    setStroke();

    buffer_->pathStroke();
  }
}

void
CSVG::
drawArc(double xc, double yc, double xr, double yr, double angle1, double angle2)
{
  setStroke();

  buffer_->pathInit();

  buffer_->pathArcTo(xc, yc, xr, yr, angle1, angle2);

  buffer_->pathStroke();
}

void
CSVG::
fillArc(double xc, double yc, double xr, double yr, double angle1, double angle2)
{
  setFill();

  buffer_->pathInit();

  buffer_->pathArcTo(xc, yc, xr, yr, angle1, angle2);

  buffer_->pathFill();
}

void
CSVG::
drawPolygon(const vector<CPoint2D> &points)
{
  uint num_points = points.size();

  if (num_points == 0) return;

  setStroke();

  buffer_->pathInit();

  buffer_->pathMoveTo(points[0].x, points[0].y);

  for (uint i = 1; i < num_points; ++i)
    buffer_->pathLineTo(points[i].x, points[i].y);

  buffer_->pathClose();

  buffer_->pathStroke();
}

void
CSVG::
fillPolygon(const vector<CPoint2D> &points)
{
  uint num_points = points.size();

  if (num_points == 0) return;

  setFill();

  buffer_->pathInit();

  buffer_->pathMoveTo(points[0].x, points[0].y);

  for (uint i = 1; i < num_points; ++i)
    buffer_->pathLineTo(points[i].x, points[i].y);

  buffer_->pathClose();

  buffer_->pathFill();
}

void
CSVG::
drawText(double x, double y, const string &text, CFontPtr font, CHAlignType align)
{
  setStroke();

  CSVGFont *svg_font = getFont();

  if (svg_font != NULL) {
    CSVGFontFace *font_face = svg_font->getFontFace();

    double units = 1000;

    if (font_face != NULL)
      units = font_face->getUnits();

    //-----

    CMatrix2D transform;

    getTransform(transform);

    CMatrix2D transform1;
    CMatrix2D transform2;

    double font_size = 10.0;

    transform1.setScale(font_size/units, font_size/units);
    transform2.setTranslation(x, y);

    uint len = text.size();

    for (uint i = 0; i < len; ++i) {
      CSVGGlyph *glyph = getCharGlyph(text[i]);

      if (glyph != NULL) {
        setTransform(transform*transform2*transform1);

        glyph->drawObject();

        x += font_size;

        transform2.setTranslation(x, y);
      }
    }

    setTransform(transform);
  }
  else {
    buffer_->pathInit();

    buffer_->pathMoveTo(x, y);

    buffer_->pathText(text, font, align);

    buffer_->pathStroke();
  }
}

void
CSVG::
fillText(double x, double y, const string &text, CFontPtr font, CHAlignType align)
{
  setFill();

  CSVGFont *svg_font = getFont();

  if (svg_font != NULL) {
    CSVGFontFace *font_face = svg_font->getFontFace();

    double units = 1000;

    if (font_face != NULL)
      units = font_face->getUnits();

    //-----

    CMatrix2D transform;

    getTransform(transform);

    CMatrix2D transform1;
    CMatrix2D transform2;

    double font_size = 10.0;

    transform1.setScale(font_size/units, font_size/units);
    transform2.setTranslation(x, y);

    uint len = text.size();

    for (uint i = 0; i < len; ++i) {
      CSVGGlyph *glyph = getCharGlyph(text[i]);

      if (glyph != NULL) {
        setTransform(transform*transform2*transform1);

        glyph->drawObject();

        x += font_size;

        transform2.setTranslation(x, y);
      }
    }

    setTransform(transform);
  }
  else {
    buffer_->pathInit();

    buffer_->pathMoveTo(x, y);

    buffer_->pathText(text, font, align);

    buffer_->pathFill();
  }
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
  *x = 0;
  *y = 0;

  return buffer_->pathGetCurrentPoint(x, y);
}

void
CSVG::
pathStroke()
{
  setStroke();

  buffer_->pathStroke();
}

void
CSVG::
pathFill()
{
  setFill();

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
pathStringToParts(const string &data, CSVG::PartList &parts)
{
  COptValT<double> bezier2_x2, bezier2_y2, bezier2_x3, bezier2_y3;
  COptValT<double> bezier3_x2, bezier3_y2, bezier3_x3, bezier3_y3;

  char c;

  CStrParse parse(data);

  parse.skipSpace();

  bool flag = true;

  while (! parse.eof()) {
    parse.readChar(&c);

    if      (c == 'm') {
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
      double d;

      parse.skipSpace();

      if (! parse.readReal(&d)) { flag = false; break; }

      parts.push_back(new CSVGPathRLineTo(*this, d, 0));
    }
    else if (c == 'H') {
      double d;

      parse.skipSpace();

      if (! parse.readReal(&d)) { flag = false; break; }

      parts.push_back(new CSVGPathHLineTo(*this, d));
    }
    else if (c == 'v') {
      double d;

      parse.skipSpace();

      if (! parse.readReal(&d)) { flag = false; break; }

      parts.push_back(new CSVGPathRLineTo(*this, 0, d));
    }
    else if (c == 'V') {
      double d;

      parse.skipSpace();

      if (! parse.readReal(&d)) { flag = false; break; }

      parts.push_back(new CSVGPathVLineTo(*this, d));
    }
    else if (c == 'a') {
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
      parts.push_back(new CSVGPathClosePath(*this));
    }
    else if (c == 'Z') {
      parts.push_back(new CSVGPathClosePath(*this));
    }
    else {
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
  vector<CPoint2D> points;

  double x1, y1, x2, y2, x3, y3;

  buffer_->pathInit();

  PartList::const_iterator p1 = parts.begin();
  PartList::const_iterator p2 = parts.end  ();

  while (p1 != p2 && ! pathGetCurrentPoint(&x1, &y1)) {
    (*p1)->draw();

    ++p1;
  }

  if (p1 == p2)
    return;

  points.push_back(CPoint2D(x1, y1));

  while (p1 != p2) {
    (*p1)->draw();

    ++p1;

    pathGetCurrentPoint(&x2, &y2);

    if (! REAL_EQ(x1, x2) || ! REAL_EQ(y1, y2)) {
      points.push_back(CPoint2D(x2, y2));

      x1 = x2;
      y1 = y2;
    }
  }

  if (isFilled())
    pathFill();

  if (isStroked())
    pathStroke();

  //------

  uint num = points.size();

  if (num <= 0)
    return;

  x1 = points[0].x; y1 = points[0].y;

  x2 = x1; y2 = y1;

  if (num > 1) {
    x2 = points[1].x; y2 = points[1].y;
  }

  x3 = x2; y3 = y2;

  if (num > 2) {
    x3 = points[2].x; y3 = points[2].y;
  }

  double g1, g2;

  g1 = atan2(y2 - y1, x2 - x1);
  g2 = atan2(y3 - y2, x3 - x2);

  if (omarker->start != NULL) {
    CSVGMarker *marker = dynamic_cast<CSVGMarker *>(omarker->start);

    if (marker != NULL)
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
      if (omarker->mid != NULL) {
        CSVGMarker *marker = dynamic_cast<CSVGMarker *>(omarker->mid);

        if (marker != NULL)
          marker->drawMarker(x2, y2, (g1 + g2)/2);
      }
    }
    else {
      if (omarker->end != NULL) {
        CSVGMarker *marker = dynamic_cast<CSVGMarker *>(omarker->end);

        if (marker != NULL)
          marker->drawMarker(x2, y2, g1);
      }
    }

    g1 = g2;

    x1 = x2; y1 = y2;
    x2 = x3; y2 = y3;
  }
}

bool
CSVG::
getPartsBBox(const CSVG::PartList &parts, CBBox2D &bbox) const
{
  CSVG *th = const_cast<CSVG *>(this);

  th->buffer_->pathInit();

  CAux::for_each(parts.begin(), parts.end(), &CSVGPathPart::draw);

  th->pathBBox(bbox);

  return bbox.isSet();
}

void
CSVG::
printParts(ostream &os, const CSVG::PartList &parts) const
{
  for_each(parts.begin(), parts.end(), bind2nd(mem_fun(&CSVGPathPart::printp), &os));
}

//--------------

bool
CSVG::
coordOption(const string &opt_name, const string &opt_value, const string &name, double *value)
{
  if (opt_name != name)
    return false;

  bool flag = true;

  vector<string> match_strs;

  if (CRegExpUtil::parse(opt_value, "\\(.*\\)%", match_strs)) {
    if (! CStrUtil::toReal(match_strs[0], value)) {
      *value = 0;
      flag = false;
    }

    *value /= 100;
  }
  else {
    if (! lengthToReal(opt_value, value)) {
      CSVGLog() << "Illegal value for " << name;
      *value = 0;
      flag = false;
    }
  }

  return flag;
}

bool
CSVG::
lengthOption(const string &opt_name, const string &opt_value, const string &name, double *value)
{
  if (opt_name != name)
    return false;

  if (! lengthToReal(opt_value, value)) {
    CSVGLog() << "Illegal value for " << name;
    return false;
  }

  return true;
}

bool
CSVG::
lengthToReal(const string &str, double *value)
{
  static CRegExp em_pattern("\\(.*\\)em");
  static CRegExp ex_pattern("\\(.*\\)ex");
  static CRegExp pt_pattern("\\(.*\\)pt");
  static CRegExp pc_pattern("\\(.*\\)pc");
  static CRegExp cm_pattern("\\(.*\\)cm");
  static CRegExp mm_pattern("\\(.*\\)mm");
  static CRegExp in_pattern("\\(.*\\)in");
  static CRegExp px_pattern("\\(.*\\)px");

  // TODO: handle %

  vector<string> match_strs;

  if      (CRegExpUtil::parse(str, em_pattern, match_strs)) {
    if (! CStrUtil::toReal(match_strs[0], value))
      return false;

    CSVGLog() << "em conversion not handled";
  }
  else if (CRegExpUtil::parse(str, ex_pattern, match_strs)) {
    if (! CStrUtil::toReal(match_strs[0], value))
      return false;

    CSVGLog() << "ex conversion not handled";
  }
  else if (CRegExpUtil::parse(str, pt_pattern, match_strs)) {
    if (! CStrUtil::toReal(match_strs[0], value))
      return false;

    mmToPixel(25.4*(*value)/72.0, value);
  }
  else if (CRegExpUtil::parse(str, pc_pattern, match_strs)) {
    if (! CStrUtil::toReal(match_strs[0], value))
      return false;

    mmToPixel(25.4*(*value)/6.0, value);
  }
  else if (CRegExpUtil::parse(str, cm_pattern, match_strs)) {
    if (! CStrUtil::toReal(match_strs[0], value))
      return false;

    mmToPixel(10*(*value), value);
  }
  else if (CRegExpUtil::parse(str, mm_pattern, match_strs)) {
    if (! CStrUtil::toReal(match_strs[0], value))
      return false;

    mmToPixel(*value, value);
  }
  else if (CRegExpUtil::parse(str, in_pattern, match_strs)) {
    if (! CStrUtil::toReal(match_strs[0], value))
      return false;

    mmToPixel(25.4*(*value), value);
  }
  else if (CRegExpUtil::parse(str, px_pattern, match_strs)) {
    if (! CStrUtil::toReal(match_strs[0], value))
      return false;
  }
  else {
    if (! CStrUtil::toReal(str, value))
      return false;
  }

  return true;
}

bool
CSVG::
angleOption(const string &opt_name, const string &opt_value, const string &name, double *value)
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
realOption(const string &opt_name, const string &opt_value, const string &name, double *value)
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
integerOption(const string &opt_name, const string &opt_value, const string &name, long *value)
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
stringOption(const string &opt_name, const string &opt_value, const string &name, string &value)
{
  if (opt_name != name)
    return false;

  value = opt_value;

  return true;
}

bool
CSVG::
bboxOption(const string &opt_name, const string &opt_value, const string &name, CBBox2D *bbox)
{
  double x1, y1, x2, y2;

  if (opt_name != name)
    return false;

  vector<string> words;

  CStrUtil::addWords(opt_value, words, " ,");

  if (words.size() != 4)
    return false;

  if (! CStrUtil::toReal(CStrUtil::stripSpaces(words[0]), &x1))
    return false;

  if (! CStrUtil::toReal(CStrUtil::stripSpaces(words[1]), &y1))
    return false;

  if (! CStrUtil::toReal(CStrUtil::stripSpaces(words[2]), &x2))
    return false;

  if (! CStrUtil::toReal(CStrUtil::stripSpaces(words[3]), &y2))
    return false;

  *bbox = CBBox2D(x1, y1, x2, y2);

  return true;
}

bool
CSVG::
pointListOption(const string &opt_name, const string &opt_value,
                const string &name, vector<CPoint2D> &points)
{
  if (opt_name != name)
    return false;

  vector<string> words;

  CStrUtil::addWords(opt_value, words, " ,\n\t");

  uint num_xy = words.size()/2;

  points.resize(num_xy);

  for (uint i = 0, j = 0; j < num_xy; i += 2, ++j) {
    string x_word = CStrUtil::stripSpaces(words[i    ]);
    string y_word = CStrUtil::stripSpaces(words[i + 1]);

    if (! CStrUtil::toReal(x_word, &points[j].x) ||
        ! CStrUtil::toReal(y_word, &points[j].y))
      return false;
  }

  return true;
}

bool
CSVG::
realListOption(const string &opt_name, const string &opt_value,
               const string &name, vector<double> &reals)
{
  if (opt_name != name)
    return false;

  vector<string> words;

  CStrUtil::addWords(opt_value, words, " ,\n\t");

  uint num_reals = words.size();

  reals.resize(num_reals);

  for (uint i = 0; i < num_reals; ++i) {
    string word = CStrUtil::stripSpaces(words[i]);

    if (! CStrUtil::toReal(word, &reals[i]))
      return false;
  }

  return true;
}

bool
CSVG::
decodeTransform(const string &str, CMatrix2D &matrix)
{
  matrix.setIdentity();

  CStrParse parse(str);

  string keyword;

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

      if (! parse.isChar(')')) {
        if (! parse.readReal(&sy)) {
          CSVGLog() << "Invalid scale " << str;
          return false;
        }

        parse.skipSpace();
      }
      else
        sy = sx;

      if (! parse.isChar(')')) {
        CSVGLog() << "Invalid scale " << str;
        return false;
      }

      parse.skipChar();

      //------

      CMatrix2D matrix1;

      matrix1.setScale(sx, sy);

      matrix *= matrix1;
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

      CMatrix2D matrix1;

      matrix1.setTranslation(dx, dy);

      matrix *= matrix1;
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

      if (translate) {
        CMatrix2D matrix1, matrix2, matrix3;

        matrix1.setTranslation(cx, cy);

        matrix2.setRotation(-CMathGen::DegToRad(a));

        matrix3.setTranslation(-cx, -cy);

        matrix *= matrix1*matrix2*matrix3;
      }
      else {
        CMatrix2D matrix1;

        matrix1.setRotation(-CMathGen::DegToRad(a));

        matrix *= matrix1;
      }
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

      CMatrix2D matrix1;

      matrix1.setSkewX(CMathGen::DegToRad(angle));

      matrix *= matrix1;
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

      CMatrix2D matrix1;

      matrix1.setSkewY(CMathGen::DegToRad(angle));

      matrix *= matrix1;
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

      CMatrix2D matrix1;

      matrix1.setValues(m00, m01, m10, m11, tx, ty);

      matrix *= matrix1;
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
decodePreserveAspectRatio(const string &str, CHAlignType *halign,
                          CVAlignType *valign, CSVGScale *scale)
{
  *halign = CHALIGN_TYPE_CENTER;
  *valign = CVALIGN_TYPE_CENTER;
  *scale  = CSVG_SCALE_FREE;

  vector<string> words;

  CStrUtil::addWords(str, words, " ,");

  uint num_words = words.size();

  CRegExp mmp("xM[ia][ndx]YM[ia][ndx]");

  for (uint i = 0; i < num_words; ++i) {
    if (mmp.find(words[i])) {
      string lword = words[i].substr(0, 4);
      string rword = words[i].substr(4, 8);

      if      (lword == "xMin") *halign = CHALIGN_TYPE_LEFT;
      else if (lword == "xMid") *halign = CHALIGN_TYPE_CENTER;
      else if (lword == "xMax") *halign = CHALIGN_TYPE_RIGHT;

      if      (rword == "YMin") *valign = CVALIGN_TYPE_BOTTOM;
      else if (rword == "YMid") *valign = CVALIGN_TYPE_CENTER;
      else if (rword == "YMax") *valign = CVALIGN_TYPE_TOP;
    }
    else if (words[i] == "meet")
      *scale = CSVG_SCALE_FIXED_MEET;
    else if (words[i] == "slice")
      *scale = CSVG_SCALE_FIXED_SLICE;
    else if (words[i] == "none")
      *scale = CSVG_SCALE_FREE;
  }

  return true;
}

/*TODO*/
double
CSVG::
decodeWidthString(const string &width_str)
{
  double width = 1.0;

  vector<string> match_strs;

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
decodeOpacityString(const string &opacity_str)
{
  double opacity;

  if (! CStrUtil::toReal(opacity_str, &opacity))
    opacity = 1.0;

  return opacity;
}

CFillType
CSVG::
decodeFillRuleString(const string &rule_str)
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
decodeDashString(const string &dash_str, CLineDash &dash)
{
  vector<string> words;
  double         length;
  vector<double> lengths;

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
decodeColorString(const string &color_str, CRGBA &rgba)
{
  vector<string> match_strs;

  if      (CRegExpUtil::parse(color_str, "rgb(\\(.*\\))", match_strs)) {
    double rgb[3];

    vector<string> words;

    CStrUtil::addWords(match_strs[0], words, " ,");

    while (words.size() < 3)
      words.push_back("0");

    for (uint i = 0; i < 3; ++i) {
      vector<string> match_strs1;

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
    string color_str1 = "#";

    color_str1 += color_str.substr(1, 1) + color_str.substr(1, 1) +
                  color_str.substr(2, 1) + color_str.substr(2, 1) +
                  color_str.substr(3, 1) + color_str.substr(3, 1);

    rgba = CRGBName::toRGBA(color_str1);
  }
  else if (color_str.size() == 7 &&
           CRegExpUtil::parse(color_str, "#" RE_HEXDEC RE_HEXDEC RE_HEXDEC
                       RE_HEXDEC RE_HEXDEC RE_HEXDEC, match_strs)) {
    string color_str1 = "#";

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
decodeFontWeightString(const string &font_weight_str)
{
  if (font_weight_str == "bold")
    return CFONT_STYLE_BOLD;
  else
    return CFONT_STYLE_NORMAL;
}

CFontStyle
CSVG::
decodeFontStyleString(const string &font_style_str)
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
decodePercentString(const string &str, double *real)
{
  bool flag = true;

  vector<string> match_strs;

  if (CRegExpUtil::parse(str, "\\(.*\\)%", match_strs)) {
    if (! CStrUtil::toReal(match_strs[0], real)) {
      *real = 0;
      flag = false;
    }

    *real /= 100;
  }
  else {
    if (! CStrUtil::toReal(str, real)) {
      *real = 0;
      flag = false;
    }
  }

  return flag;
}

bool
CSVG::
decodeUnitsString(const string &str, CSVGCoordUnits *units)
{
  if      (str == "objectBoundingBox")
    *units = CSVG_COORD_UNITS_OBJECT_BBOX;
  else if (str == "userSpaceOnUse")
    *units = CSVG_COORD_UNITS_USER_SPACE;
  else
    return false;

  return true;
}

bool
CSVG::
decodeGradientSpread(const string &str, CGradientSpreadType *spread)
{
  if      (str == "pad")
    *spread = CGRADIENT_SPREAD_PAD;
  else if (str == "repeat")
    *spread = CGRADIENT_SPREAD_REPEAT;
  else if (str == "reflect")
    *spread = CGRADIENT_SPREAD_REFLECT;
  else
    return false;

  return true;
}

bool
CSVG::
decodeUrlObject(const string &str, CSVGObject **object)
{
  string         id;
  vector<string> match_strs;

  if      (CRegExpUtil::parse(str, "url(#\\(.*\\))", match_strs))
    id = match_strs[0];
  else if (str.size() > 0 && str[0] == '#')
    id = str.substr(1);
  else
    id = str;

  *object = lookupObjectById(id);

  return (*object != NULL);
}

bool
CSVG::
mmToPixel(double mm, double *pixel)
{
  return buffer_->mmToPixel(mm, pixel);
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
getTitle(string &str)
{
  vector<CSVGObject *> objects;

  block_->getChildrenOfType(CSVG_OBJ_TYPE_TITLE, objects);

  if (objects.empty())
    return false;

  CSVGTitle *title = dynamic_cast<CSVGTitle *>(objects[0]);

  str = title->getText();

  return true;
}

void
CSVG::
lengthToPixel(double xi, double yi, int *xo, int *yo)
{
  int px1, py1, px2, py2;

  windowToPixel( 0,  0, &px1, &py1);
  windowToPixel(xi, yi, &px2, &py2);

  *xo = abs(px2 - px1);
  *yo = abs(py2 - py1);
}

void
CSVG::
windowToPixel(double xi, double yi, int *xo, int *yo)
{
  buffer_->windowToPixel(xi, yi, xo, yo);
}

void
CSVG::
setObjectById(const string &id, CSVGObject *object)
{
  id_object_map_[id] = object;
}

CSVGObject *
CSVG::
lookupObjectById(const string &id) const
{
  NameObjectMap::const_iterator p = id_object_map_.find(id);

  if (p != id_object_map_.end())
    return (*p).second;

  return NULL;
}

bool
CSVG::
readCSSFile(const string &filename)
{
  CCSS css;

  if (! css.processFile(filename))
    return false;

  vector<string> ids;

  css.getIds(ids);

  uint num_ids = ids.size();

  for (uint i = 0; i < num_ids; ++i) {
    const CCSS::StyleData &styleData = css.getStyleData(ids[i]);

    CSVGStyleData &styleData1 = getStyleData(ids[i]);

    uint num_options = styleData.getNumOptions();

    for (uint j = 0; j < num_options; ++j) {
      const CCSS::Option &option = styleData.getOption(j);

      styleData1.setValue(option.getName(), option.getValue());
    }
  }

  return true;
}

CSVGStyleData &
CSVG::
getStyleData(const string &id)
{
  StyleDataMap::iterator p = styleData_.find(id);

  if (p == styleData_.end())
    p = styleData_.insert(p, StyleDataMap::value_type(id, CSVGStyleData(*this, id)));

  CSVGStyleData &styleData = (*p).second;

  return styleData;
}

bool
CSVG::
getStyleStrokeColor(const string &id, CRGBA &rgba)
{
  CSVGStyleData &styleData = getStyleData(id);

  if (! styleData.getStrokeColorValid())
    return false;

  rgba = styleData.getStrokeColor();

  return true;
}

bool
CSVG::
getStyleStrokeOpacity(const string &id, double &opacity)
{
  CSVGStyleData &styleData = getStyleData(id);

  if (! styleData.getStrokeOpacityValid())
    return false;

  opacity = styleData.getStrokeOpacity();

  return true;
}

bool
CSVG::
getStyleStrokeWidth(const string &id, double &width)
{
  CSVGStyleData &styleData = getStyleData(id);

  if (! styleData.getStrokeWidthValid())
    return false;

  width = styleData.getStrokeWidth();

  return true;
}

bool
CSVG::
getStyleStrokeDash(const string &id, CLineDash &dash)
{
  CSVGStyleData &styleData = getStyleData(id);

  if (! styleData.getStrokeDashValid())
    return false;

  dash = styleData.getStrokeDash();

  return true;
}

bool
CSVG::
getStyleFillColor(const string &id, CRGBA &rgba)
{
  CSVGStyleData &styleData = getStyleData(id);

  if (! styleData.getFillColorValid())
    return false;

  rgba = styleData.getFillColor();

  return true;
}

//----------

void
CSVGUtil::
convertArcCoords(double x1, double y1, double x2, double y2, double phi,
                 double rx, double ry, int fa, int fs, bool unit_circle,
                 double *cx, double *cy, double *rx1, double *ry1,
                 double *theta, double *delta)
{
  // start and end are the same so it's a complete ellipse
  if (fabs(x2 - x1) < 1E-6 && fabs(y2 - y1) < 1E-6) {
    if (fs == 1) {
      *cx = x1 + rx;
      *cy = y1;
    }
    else {
      *cx = x1 - rx;
      *cy = y1;
    }

    *rx1 = rx;
    *ry1 = ry;

    *theta = 0;
    *delta = 360;

    return;
  }

  rx = fabs(rx);
  ry = fabs(ry);

  phi = CMathGen::DegToRad(phi);

  double sin_phi = sin(phi);
  double cos_phi = cos(phi);

  double dx = (x1 - x2)/2.0;
  double dy = (y1 - y2)/2.0;

  double dx1 =  cos_phi*dx + sin_phi*dy;
  double dy1 = -sin_phi*dx + cos_phi*dy;

  double rxx = rx*rx;
  double ryy = ry*ry;

  double dxx1 = dx1*dx1;
  double dyy1 = dy1*dy1;

  // Fix radii
  double rcheck = dxx1/rxx + dyy1/ryy;

  if (rcheck > 1) {
    double s = sqrt(rcheck);

    rx *= s;
    ry *= s;

    rxx = rx*rx;
    ryy = ry*ry;
  }

  if (unit_circle) {
    double a00 =  cos_phi/rx;
    double a01 =  sin_phi/rx;
    double a10 = -sin_phi/ry;
    double a11 =  cos_phi/ry;

    /* (xt1, yt1) is current point in transformed coordinate space.
       (xt2, yt2) is new point in transformed coordinate space.

       The arc fits a unit-radius circle in this space.
    */
    double xt1 = a00*x1 + a01*y1;
    double yt1 = a10*x1 + a11*y1;
    double xt2 = a00*x2 + a01*y2;
    double yt2 = a10*x2 + a11*y2;

    double d = (xt2 - xt1)*(xt2 - xt1) + (yt2 - yt1)*(yt2 - yt1);

    double sfactor_sq = 1.0 / d - 0.25;

    if (sfactor_sq < 0) sfactor_sq = 0;

    double sfactor = sqrt(sfactor_sq);

    if (fa == fs) sfactor = -sfactor;

    // (cx, cy) is center of the circle.
    *cx = 0.5*(xt1 + xt2) - sfactor*(yt2 - yt1);
    *cy = 0.5*(yt1 + yt2) + sfactor*(xt2 - xt1);

    // Calculate angles
    *theta = atan2(yt1 - *cy, xt1 - *cx);

    double theta1 = atan2(yt2 - *cy, xt2 - *cx);

    *delta = theta1 - *theta;

    if      (fs == 0 && *delta > 0)
      *delta -= 2*M_PI;
    else if (fs == 1 && *delta < 0)
      *delta += 2*M_PI;

    *theta = CMathGen::RadToDeg(*theta);
    *delta = CMathGen::RadToDeg(*delta);
  }
  else {
    // Calculate center of arc
    double sfactor_sq = (rxx*ryy - rxx*dyy1 - ryy*dxx1)/(rxx*dyy1 + ryy*dxx1);

    if (sfactor_sq < 0) sfactor_sq = 0;

    double sfactor = sqrt(sfactor_sq);

    if (fa == fs) sfactor = -sfactor;

    double cx1 =  sfactor*((rx*dy1)/ry);
    double cy1 = -sfactor*((ry*dx1)/rx);

    double sx2 = (x1 + x2)/2.0;
    double sy2 = (y1 + y2)/2.0;

    *cx = sx2 + cos_phi*cx1 - sin_phi*cy1;
    *cy = sy2 + sin_phi*cx1 + cos_phi*cy1;

    // Calculate arc angles
    double ux = ( dx1 - cx1)/rx;
    double uy = ( dy1 - cy1)/ry;

    double vx = (-dx1 - cx1)/rx;
    double vy = (-dy1 - cy1)/ry;

    double mod_u = sqrt(ux*ux + uy*uy);
    double mod_v = ux;

    int sign = (uy < 0) ? -1 : 1;

    *theta = sign*acos(mod_v/mod_u);
    *theta = CMathGen::RadToDeg(*theta);

    while (*theta >=  360) *theta -= 360;
    while (*theta <= -360) *theta += 360;

    mod_u = sqrt((ux*ux + uy*uy) * (vx*vx + vy*vy));
    mod_v = ux*vx + uy*vy;

    sign = ((ux*vy - uy*vx) < 0) ? -1 : 1;

    *delta = sign*acos(mod_v/mod_u);
    *delta = CMathGen::RadToDeg(*delta);

    if      (fs == 0 && *delta > 0)
      *delta -= 360;
    else if (fs == 1 && *delta < 0)
      *delta += 360;

    while (*delta >=  360) *delta -= 360;
    while (*delta <= -360) *delta += 360;
  }

  *rx1 = rx;
  *ry1 = ry;
}

//-------

CSVGXLink::
CSVGXLink(const CSVGXLink &xlink) :
 parent_  (xlink.parent_),
 resolved_(xlink.resolved_),
 str_     (xlink.str_),
 object_  (xlink.object_),
 image_   (xlink.image_)
{
}

CSVGXLink &
CSVGXLink::
operator=(const CSVGXLink &xlink)
{
  parent_   = xlink.parent_;
  resolved_ = xlink.resolved_;
  str_      = xlink.str_;
  object_   = xlink.object_;
  image_    = xlink.image_;

  return *this;
}

void
CSVGXLink::
resolve() const
{
  if (resolved_) return;

  CSVGXLink *th = const_cast<CSVGXLink *>(this);

  parent_->decodeXLink(str_, &th->object_, th->image_);

  th->resolved_ = true;
}

//------

void
CSVGStyleData::
setValue(const string &name, const string &value)
{
  if      (name == "fill")
    fill_.setColor(value);
  else if (name == "fill-opacity")
    fill_.setOpacity(value);
  else if (name == "stroke")
    stroke_.setColor(value);
  else if (name == "stroke-width")
    stroke_.setWidth(value);
  else if (name == "stroke-dasharray")
    stroke_.setDash(value);
  else
    std::cerr << "Unhandled: " << name << ":" << value << std::endl;
}
