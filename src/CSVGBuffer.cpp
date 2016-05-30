#include <CSVGBuffer.h>
#include <CSVGFeFunc.h>
#include <CSVGFeDistantLight.h>
#include <CSVGFePointLight.h>
#include <CSVGFeSpotLight.h>
#include <CSVGFeMergeNode.h>
#include <CSVGFilter.h>
#include <CSVG.h>
#include <CSVGRenderer.h>
#include <CSVGUtil.h>
#include <CVector3D.h>

namespace {
  CImagePtr createImage(int w, int h) {
    CImageNoSrc src;

    CImagePtr dst_image = CImageMgrInst->createImage(src);

    dst_image->setDataSize(w, h);

    dst_image->setRGBAData(CRGBA(0,0,0,0));

    return dst_image;
  }
}

//-----

CSVGBufferMgr::
CSVGBufferMgr(CSVG &svg) :
 svg_(svg)
{
}

CSVGBufferMgr::
~CSVGBufferMgr()
{
  clear();
}

void
CSVGBufferMgr::
clear()
{
  for (const auto &b : bufferMap_)
    delete b.second;

  bufferMap_.clear();
}

CSVGBuffer *
CSVGBufferMgr::
lookupBuffer(const std::string &name, bool create)
{
  auto p = bufferMap_.find(name);

  if (p != bufferMap_.end())
    return (*p).second;

  if (! create)
    return 0;

  return createBuffer(name);
}

CSVGBuffer *
CSVGBufferMgr::
lookupAlphaBuffer(CSVGBuffer *buffer, bool create)
{
  auto p = alphaBufferMap_.find(buffer->getName());

  if (p != alphaBufferMap_.end())
    return (*p).second;

  if (! create)
    return 0;

  return createAlphaBuffer(buffer);
}

CSVGBuffer *
CSVGBufferMgr::
createBuffer(const std::string &name)
{
  CSVGBuffer *buffer = svg_.createBuffer(name);

  buffer->setAntiAlias(antiAlias_);

  addBuffer(buffer);

  return buffer;
}

CSVGBuffer *
CSVGBufferMgr::
createAlphaBuffer(CSVGBuffer *buffer)
{
  CSVGBuffer *alphaBuffer = buffer->dup();

  addAlphaBuffer(alphaBuffer);

  return alphaBuffer;
}

void
CSVGBufferMgr::
addBuffer(CSVGBuffer *buffer)
{
  bufferMap_[buffer->getName()] = buffer;
}

void
CSVGBufferMgr::
addAlphaBuffer(CSVGBuffer *buffer)
{
  alphaBufferMap_[buffer->getName()] = buffer;
}

void
CSVGBufferMgr::
getBufferNames(std::vector<std::string> &names) const
{
  for (const auto &b : bufferMap_)
    names.push_back(b.first);
}

//------------

CSVGBuffer::
CSVGBuffer(CSVG &svg, const std::string &name) :
 svg_(svg), name_(name)
{
  transform_.reset();
}

CSVGBuffer::
CSVGBuffer(CSVGBuffer *refBuffer) :
 svg_(refBuffer->svg_), refBuffer_(refBuffer)
{
  if      (refBuffer_->name_ == "SourceGraphic")
    name_ = "SourceAlpha";
  else if (refBuffer_->name_ == "BackgroundImage")
    name_ = "BackgroundAlpha";
  else
    name_ = refBuffer_->name_ + "Alpha";
}

CSVGBuffer::
~CSVGBuffer()
{
  delete renderer_;
}

CSVGBuffer *
CSVGBuffer::
dup() const
{
  return new CSVGBuffer(const_cast<CSVGBuffer *>(this));
}

CSVGRenderer *
CSVGBuffer::
getRenderer() const
{
  if (refBuffer_)
    return refBuffer_->getRenderer();

  //---

  if (! renderer_) {
    CSVGBuffer *th = const_cast<CSVGBuffer *>(this);

    th->renderer_ = svg_.createRenderer();
  }

  return renderer_;
}

bool
CSVGBuffer::
isAntiAlias() const
{
  if (refBuffer_)
    return refBuffer_->isAntiAlias();

  //---

  return getRenderer()->isAntiAlias();
}

void
CSVGBuffer::
setAntiAlias(bool flag)
{
  if (refBuffer_)
    return refBuffer_->setAntiAlias(flag);

  //---

  getRenderer()->setAntiAlias(flag);
}

void
CSVGBuffer::
blendBuffers(CSVGBuffer *inBuffer1, CSVGBuffer *inBuffer2,
             CSVGBlendMode mode, CSVGBuffer *outBuffer)
{
  CImagePtr src_image1 = inBuffer1->getImage();
  CImagePtr src_image2 = inBuffer2->getImage();

  CImagePtr dst_image = src_image1->dup();

  CRGBABlendMode mode1 = CRGBA_BLEND_NORMAL;

  if      (mode == CSVGBlendMode::NORMAL)
    mode1 = CRGBA_BLEND_NORMAL;
  else if (mode == CSVGBlendMode::MULTIPLY)
    mode1 = CRGBA_BLEND_MULTIPLY;
  else if (mode == CSVGBlendMode::SCREEN)
    mode1 = CRGBA_BLEND_SCREEN;
  else if (mode == CSVGBlendMode::DARKEN)
    mode1 = CRGBA_BLEND_DARKEN;
  else if (mode == CSVGBlendMode::LIGHTEN)
    mode1 = CRGBA_BLEND_LIGHTEN;

  dst_image->combine(src_image2, mode1);

  outBuffer->getRenderer()->setImage(dst_image);
}

void
CSVGBuffer::
colorMatrixBuffers(CSVGBuffer *inBuffer, CSVGColorMatrixType type,
                   const std::vector<double> &values, CSVGBuffer *outBuffer)
{
  CImagePtr src_image = inBuffer->getImage();
  CImagePtr dst_image = src_image->dup();

  if      (type == CSVGColorMatrixType::MATRIX) {
    dst_image->applyColorMatrix(values);
  }
  // For type="saturate", ‘values’ is a single real number value (0 to 1).
  // A saturate operation is equivalent to the following matrix operation:
  // | R' |   |0.213+0.787s 0.715-0.715s 0.072-0.072s 0 0 |   | R |
  // | G' |   |0.213-0.213s 0.715+0.285s 0.072-0.072s 0 0 |   | G |
  // | B' | = |0.213-0.213s 0.715-0.715s 0.072+0.928s 0 0 | * | B |
  // | A' |   |           0            0            0 1 0 |   | A |
  // | 1  |   |           0            0            0 0 1 |   | 1 |
  else if (type == CSVGColorMatrixType::SATURATE) {
    dst_image->saturate(values[0]);
  }
  // For type="hueRotate", ‘values’ is a single one real number value (degrees).
  // A hueRotate operation is equivalent to the following matrix operation:
  // | R' |   |a00 a01 a02 0 0 |   | R |
  // | G' |   |a10 a11 a12 0 0 |   | G |
  // | B' | = |a20 a21 a22 0 0 | * | B |
  // | A' |   |  0   0   0 1 0 |   | A |
  // | 1  |   |  0   0   0 0 1 |   | 1 |
  //
  // where the terms a00, a01, etc. are calculated as follows:
  //  | a00 a01 a02 |   [+0.213 +0.715 +0.072]
  //  | a10 a11 a12 | = [+0.213 +0.715 +0.072] +
  //  | a20 a21 a22 |   [+0.213 +0.715 +0.072]
  //
  //                        [+0.787 -0.715 -0.072]
  // cos(hueRotate value) * [-0.213 +0.285 -0.072] +
  //                        [-0.213 -0.715 +0.928]
  //                        [-0.213 -0.715 +0.928]
  // sin(hueRotate value) * [+0.143 +0.140 -0.283]
  //                        [-0.787 +0.715 +0.072]
  // Thus, the upper left term of the hue matrix turns out to be:
  //   .213 + cos(hueRotate value)*.787 - sin(hueRotate value)*.213
  else if (type == CSVGColorMatrixType::HUE_ROTATE) {
    dst_image->rotateHue(values[0]);
  }
  // For type="luminanceToAlpha", ‘values’ is not applicable.
  // A luminanceToAlpha operation is equivalent to the following matrix operation:
  // | R' |   |      0      0      0 0 0 |   | R |
  // | G' |   |      0      0      0 0 0 |   | G |
  // | B' | = |      0      0      0 0 0 | * | B |
  // | A' |   | 0.2125 0.7154 0.0721 0 0 |   | A |
  // | 1  |   |      0      0      0 0 1 |   | 1 |
  else if (type == CSVGColorMatrixType::LUMINANCE_TO_ALPHA) {
    dst_image->luminanceToAlpha();
  }

  outBuffer->getRenderer()->setImage(dst_image);
}

void
CSVGBuffer::
componentTransferBuffers(CSVGBuffer *inBuffer, const FeFuncs &funcs, CSVGBuffer *outBuffer)
{
  CImagePtr src_image = inBuffer->getImage();
  CImagePtr dst_image = src_image->dup();

  for (const auto &func : funcs) {
    CSVGFilterFuncType type      = func->getType();
    CColorComponent    component = func->getComponent();

    //CImagePtr dst_image = src_image->dup();

    if      (type == CSVGFilterFuncType::IDENTITY) {
    }
    else if (type == CSVGFilterFuncType::LINEAR) {
      dst_image->linearFunc(component, func->getSlope(), func->getIntercept());
    }
    else if (type == CSVGFilterFuncType::GAMMA) {
      dst_image->gammaFunc(component, func->getAmplitude(), func->getExponent(), func->getOffset());
    }
    else if (type == CSVGFilterFuncType::TABLE) {
      dst_image->tableFunc(component, func->getTable());
    }
    else if (type == CSVGFilterFuncType::DISCRETE) {
      dst_image->discreteFunc(component, func->getTable());
    }

    //dst_image = func->filterImage(dst_image);
  }

  outBuffer->getRenderer()->setImage(dst_image);
}

void
CSVGBuffer::
compositeBuffers(CSVGBuffer *inBuffer1, CSVGBuffer *inBuffer2, CRGBACombineFunc func,
                 double k1, double k2, double k3, double k4, CSVGBuffer *outBuffer)
{
  CImagePtr src_image1 = inBuffer1->getImage();
  CImagePtr src_image2 = inBuffer2->getImage();

  if (! src_image1.isValid() || ! src_image2.isValid())
    return;

  CImagePtr dst_image = src_image1->dup();

  CRGBACombineDef def;

  def.src_mode = CRGBA_COMBINE_ONE;
  def.dst_mode = CRGBA_COMBINE_ONE;
  def.func     = func;
  def.k1       = k1;
  def.k2       = k2;
  def.k3       = k3;
  def.k4       = k4;

  dst_image->combine(src_image2, def);

  outBuffer->getRenderer()->setImage(dst_image);
}

void
CSVGBuffer::
convolveMatrixBuffers(CSVGBuffer *inBuffer, const std::vector<double> &kernelMatrix)
{
  CImagePtr src_image = inBuffer->getImage();
  CImagePtr dst_image = src_image->dup();

  if (! kernelMatrix.empty())
    src_image->convolve(dst_image, kernelMatrix);

  inBuffer->getRenderer()->setImage(dst_image);
}

void
CSVGBuffer::
displacementMapBuffers(CSVGBuffer *inBuffer1, CSVGBuffer *inBuffer2,
                       const std::string &xchannel, const std::string &ychannel,
                       double scale, CSVGBuffer *outBuffer)
{
  CImagePtr src_image1 = inBuffer1->getImage();
  CImagePtr src_image2 = inBuffer2->getImage();

  CColorComponent xcolor = CCOLOR_COMPONENT_RED;
  CColorComponent ycolor = CCOLOR_COMPONENT_RED;

  if      (xchannel == "R") xcolor = CCOLOR_COMPONENT_RED;
  else if (xchannel == "G") xcolor = CCOLOR_COMPONENT_GREEN;
  else if (xchannel == "B") xcolor = CCOLOR_COMPONENT_BLUE;
  else if (xchannel == "A") xcolor = CCOLOR_COMPONENT_ALPHA;

  if      (ychannel == "R") ycolor = CCOLOR_COMPONENT_RED;
  else if (ychannel == "G") ycolor = CCOLOR_COMPONENT_GREEN;
  else if (ychannel == "B") ycolor = CCOLOR_COMPONENT_BLUE;
  else if (ychannel == "A") ycolor = CCOLOR_COMPONENT_ALPHA;

  CImagePtr dst_image = src_image1->displacementMap(src_image2, xcolor, ycolor, scale);

  outBuffer->getRenderer()->setImage(dst_image);
}

void
CSVGBuffer::
floodBuffers(const CRGBA &c, int w, int h, CSVGBuffer *outBuffer)
{
  // create image to flood
  CImagePtr dst_image = createImage(w, h);

  // flood
  dst_image->setRGBAData(c);

  outBuffer->getRenderer()->setImage(dst_image);
}

void
CSVGBuffer::
gaussianBlurBuffers(CSVGBuffer *inBuffer, CSVGFilterBase *filter,
                    double stdDev, CSVGBuffer *outBuffer)
{
  CBBox2D bbox;

  if (filter->getParentBBox(bbox)) {
    double x1, y1, x2, y2;

    outBuffer->svg_.windowToPixel(bbox.getXMin(), bbox.getYMin(), &x1, &y1);
    outBuffer->svg_.windowToPixel(bbox.getXMax(), bbox.getYMax(), &x2, &y2);

    bbox = CBBox2D(x1, y1, x2, y2);
  }

  if (! inBuffer->isAlpha()) {
    inBuffer->getRenderer()->gaussianBlur(outBuffer->getRenderer(), bbox, stdDev);
  }
  else {
    CImagePtr src_image = inBuffer->getImage();
    CImagePtr dst_image = src_image->dup();

    src_image->setWindow(bbox.getXMin(), bbox.getYMin(), bbox.getXMax(), bbox.getYMax());

    src_image->gaussianBlur(dst_image, stdDev, stdDev);

    outBuffer->getRenderer()->setImage(dst_image);
  }
}

void
CSVGBuffer::
imageBuffers(CSVGBuffer *inBuffer, CSVGFilterBase *filter, const CMatrixStack2D &transform,
             CSVGPreserveAspect preserveAspect, CSVGBuffer *outBuffer)
{
  CImagePtr src_image = inBuffer->getImage();
  assert(src_image.isValid());

  CImagePtr dst_image;

  CBBox2D bbox;

  // get parent transformed bounding box
  if (filter->getTransformedParentBBox(bbox)) {
    CBBox2D bbox1 = outBuffer->svg_.transformBBox(transform, bbox);

    // TODO: is this always going to be 1->1
    double x1, y1, x2, y2;

    outBuffer->svg_.windowToPixel(bbox1.getXMin(), bbox1.getYMin(), &x1, &y1);
    outBuffer->svg_.windowToPixel(bbox1.getXMax(), bbox1.getYMax(), &x2, &y2);

    //---

    if (filter->hasLink() && filter->xlink().isImage()) {
      // resize source image (to bbox pixel size)
      CImagePtr src_image1 = src_image->dup();

      CSVGScale scale = preserveAspect.getScale();
      double    pw    = fabs(x2 - x1);
      double    ph    = fabs(y2 - y1);

      if (scale == CSVGScale::FREE)
        src_image1->reshape(pw, ph);
      else
        src_image1->reshapeKeepAspect(pw, ph);

      //---

      // create dest image
      int w1 = x1 + pw + 1;
      int h1 = y1 + ph + 1;

      dst_image = createImage(w1, h1);

      //---

      // add to dest image (centered)
      double dx = (pw - src_image1->getWidth ())/2;
      double dy = (ph - src_image1->getHeight())/2;

      src_image1->copyTo(dst_image, x1 + dx, y1 + dy);
    }
    else {
      // create dest image
      dst_image = outBuffer->svg_.getBuffer()->getImage()->dup();

      // add to dest image (centered ?)
      src_image->copyTo(dst_image, x1, y1);
    }

    outBuffer->getRenderer()->setImage(dst_image);
    outBuffer->setBBox(bbox1);
  }
  else {
    dst_image = src_image->dup();

    outBuffer->getRenderer()->setImage(dst_image);
    outBuffer->setBBox(bbox);
  }
}

void
CSVGBuffer::
maskBuffers(CSVGBuffer *oldBuffer, CSVGBuffer *buffer,
            const CSVGObject *object, double x, double y)
{
  // create mask image
  CImagePtr mask_image  = buffer->getImage();
  CImagePtr mask_image1 = mask_image->createRGBAMask();

  if (oldBuffer->svg_.getDebugMask()) {
    std::string maskBufferName = "rgb_mask_" + object->getUniqueName();

    CSVGBuffer *maskBuffer = oldBuffer->svg_.getBuffer(maskBufferName);

    maskBuffer->getRenderer()->setImage(mask_image1);
  }

  //---

  // combine mask with image
  CImagePtr dest_image = oldBuffer->getImage();

  dest_image->copyAlpha(mask_image1, x, y);

  if (oldBuffer->svg_.getDebugMask()) {
    std::string maskBufferName = "alpha_mask_" + object->getUniqueName();

    CSVGBuffer *maskBuffer = oldBuffer->svg_.getBuffer(maskBufferName);

    maskBuffer->getRenderer()->setImage(dest_image);
  }

  //---

  // update image
  bool oldDrawing = oldBuffer->isDrawing();

  if (oldDrawing)
    oldBuffer->stopDraw();

  oldBuffer->getRenderer()->setImage(dest_image);

  if (oldDrawing)
    oldBuffer->startDraw();
}

void
CSVGBuffer::
mergeBuffers(CSVGFilterBase *filter, const std::vector<CSVGFeMergeNode *> &nodes,
             int w, int h, CSVGBuffer *outBuffer)
{
  // create image to merge into
  CImagePtr dst_image = createImage(w, h);

  //---

  // add merge node images
  int i = 1;

  for (const auto &node : nodes) {
    std::string filterIn = node->getFilterIn();
    CSVGBuffer* bufferIn = outBuffer->svg_.getBuffer(filterIn);

    if (outBuffer->svg_.getDebugFilter()) {
      std::string objectBufferName = "_" + filter->getUniqueName();

      CSVGBuffer *buffer =
        outBuffer->svg_.getBuffer(objectBufferName + "_node_" + std::to_string(i) + "_in");

      buffer->setImage(bufferIn);
    }

    //---

    CImagePtr imageIn = bufferIn->getImage();

    dst_image->combine(imageIn);

    //---

    if (outBuffer->svg_.getDebugFilter()) {
      std::string objectBufferName = "_" + filter->getUniqueName();

      CSVGBuffer *buffer =
        outBuffer->svg_.getBuffer(objectBufferName + "_node_" + std::to_string(i) + "_out");

      buffer->getRenderer()->setImage(dst_image);
    }

    ++i;
  }

  outBuffer->getRenderer()->setImage(dst_image);
}

void
CSVGBuffer::
morphologyBuffers(CSVGBuffer *inBuffer, CSVGMorphologyOperator op, int r)
{
  CImagePtr src_image = inBuffer->getImage();

  CImagePtr dst_image;

  if      (op == CSVGMorphologyOperator::ERODE)
    dst_image = src_image->erode (r, /*isAlpha*/true);
  else if (op == CSVGMorphologyOperator::DILATE)
    dst_image = src_image->dilate(r, /*isAlpha*/true);

  inBuffer->getRenderer()->setImage(dst_image);
}

void
CSVGBuffer::
offsetBuffers(CSVGBuffer *inBuffer, double dx, double dy, CSVGBuffer *outBuffer)
{
  double dx1, dy1;

  inBuffer->svg_.lengthToPixel(dx, dy, &dx1, &dy1);

  if (! inBuffer->isAlpha()) {
    outBuffer->getRenderer()->setOffsetImage(inBuffer->getRenderer(), dx1, dy1);
  }
  else {
    CImagePtr src_image = inBuffer->getImage();

    int w = src_image->getWidth () + dx1;
    int h = src_image->getHeight() + dy1;

    CImagePtr dst_image = createImage(w, h);

    dst_image->subCopyFrom(src_image, 0, 0, -1, -1, dx1, dy1);

    outBuffer->getRenderer()->setImage(dst_image);
  }
}

void
CSVGBuffer::
tileBuffers(CSVGBuffer *inBuffer, CSVGFilterBase *filter,
            const CMatrixStack2D &transform, CSVGBuffer *outBuffer)
{
  CImagePtr inImage = inBuffer->getImage();

  // get image contents from buffer
  CBBox2D inBBox = inBuffer->bbox();

  if (inBBox.isSet()) {
    double x1, y1, x2, y2;

    inBuffer->svg_.windowToPixel(inBBox.getXMin(), inBBox.getYMin(), &x1, &y1);
    inBuffer->svg_.windowToPixel(inBBox.getXMax(), inBBox.getYMax(), &x2, &y2);

    inImage = inImage->subImage(x1, y1, x2 - x1, y2 - y1);
  }

  // get size of area to tile
  int w = inImage->getWidth ();
  int h = inImage->getHeight();

  CBBox2D bbox, bbox1;
  double x1 = 0, y1 = 0, x2 = 0, y2 = 0;

  // get parent transformed bounding box
  if (filter->getTransformedParentBBox(bbox)) {
    CBBox2D bbox1 = outBuffer->svg_.transformBBox(transform, bbox);

    // TODO: is this always going to be 1->1
    outBuffer->svg_.windowToPixel(bbox1.getXMin(), bbox1.getYMin(), &x1, &y1);
    outBuffer->svg_.windowToPixel(bbox1.getXMax(), bbox1.getYMax(), &x2, &y2);

    //---

    double pw, ph;

    inBuffer->svg_.lengthToPixel(bbox.getWidth(), bbox.getHeight(), &pw, &ph);

    w = CSVGUtil::round(pw);
    h = CSVGUtil::round(ph);
  }

  // tile
  CImagePtr tiledImage = inImage->tile(w, h);

  // create dest image
  CImagePtr outImage = outBuffer->svg_.getBuffer()->getImage()->dup();

  outImage->setRGBAData(CRGBA());

  // place in output buffer
  tiledImage->copyTo(outImage, x1, y1);

  outBuffer->getRenderer()->setImage(outImage);

  if (bbox1.isSet())
    outBuffer->setBBox(bbox1);
}

void
CSVGBuffer::
turbulenceBuffers(CSVGBuffer *inBuffer, bool fractalNoise, double baseFreq,
                  int numOctaves, int seed, CSVGBuffer *outBuffer)
{
  CImagePtr src_image = inBuffer->getImage();
  CImagePtr dst_image = src_image->dup();

  dst_image->turbulence(fractalNoise, baseFreq, numOctaves, seed);

  outBuffer->getRenderer()->setImage(dst_image);
}

void
CSVGBuffer::
lightBuffers(CSVGBuffer *inBuffer, const std::vector<CSVGFilterBase *> &lights,
             const CSVGLightData &lightData, CSVGBuffer *outBuffer)
{
  CImagePtr src_image = inBuffer->getImage();
  CImagePtr dst_image = src_image->dup();

  for (const auto &l : lights) {
    CSVGFeDistantLight *dl = dynamic_cast<CSVGFeDistantLight *>(l);
    CSVGFePointLight   *pl = dynamic_cast<CSVGFePointLight   *>(l);
    CSVGFeSpotLight    *sl = dynamic_cast<CSVGFeSpotLight    *>(l);

    CSVGLightData lightData1 = lightData;

    lightData1.ltype = l->getObjTypeId();

    if      (dl) distantLight(dst_image, dl, lightData1);
    else if (pl) pointLight  (dst_image, pl, lightData1);
    else if (sl) spotLight   (dst_image, sl, lightData1);
  }

  outBuffer->getRenderer()->setImage(dst_image);
}

void
CSVGBuffer::
distantLight(CImagePtr image, CSVGFeDistantLight *light, CSVGLightData &lightData)
{
  lightData.lelevation = light->getElevation();
  lightData.lazimuth   = light->getAzimuth();

  int w = image->getWidth ();
  int h = image->getHeight();

  for (int y = 0; y < h; ++y) {
    for (int x = 0; x < w; ++x) {
      CRGBA rgba1 = lightPoint(image, x, y, lightData);

      image->setRGBAPixel(x, y, rgba1);
    }
  }
}

void
CSVGBuffer::
pointLight(CImagePtr image, CSVGFePointLight *light, CSVGLightData &lightData)
{
  lightData.lpoint = light->getPoint();

  int w = image->getWidth ();
  int h = image->getHeight();

  for (int y = 0; y < h; ++y) {
    for (int x = 0; x < w; ++x) {
      CRGBA rgba1 = lightPoint(image, x, y, lightData);

      image->setRGBAPixel(x, y, rgba1);
    }
  }
}

void
CSVGBuffer::
spotLight(CImagePtr image, CSVGFeSpotLight *light, CSVGLightData &lightData)
{
  lightData.lpoint    = light->getPoint();
  lightData.lpointsAt = light->getPointsAt();
  lightData.lexponent = light->getSpecularExponent();
  lightData.lcone     = (light->hasLimitingConeAngle() ? cos(light->getLimitingConeAngle()) : 0);

  int w = image->getWidth ();
  int h = image->getHeight();

  for (int y = 0; y < h; ++y) {
    for (int x = 0; x < w; ++x) {
      CRGBA rgba1 = lightPoint(image, x, y, lightData);

      image->setRGBAPixel(x, y, rgba1);
    }
  }
}

CRGBA
CSVGBuffer::
lightPoint(CImagePtr image, int x, int y, const CSVGLightData &lightData)
{
  // calc light color
  double lx, ly, lz;

  if (lightData.ltype == CSVGObjTypeId::FE_DISTANT_LIGHT) {
    lx = cos(lightData.lazimuth)*cos(lightData.lelevation);
    ly = sin(lightData.lazimuth)*cos(lightData.lelevation);
    lz = sin(lightData.lelevation);
  }
  else {
    double gray;

    image->getGrayPixel(x, y, &gray);

    double z = lightData.surfaceScale*gray;

    lx = lightData.lpoint.x - x;
    ly = lightData.lpoint.y - y;
    lz = lightData.lpoint.z - z;
  }

  CVector3D lightDir = CVector3D(lx, ly, lz).unit();

  CRGBA lcolor;

  if (lightData.ltype == CSVGObjTypeId::FE_SPOT_LIGHT) {
    CVector3D dspot(lightData.lpoint, lightData.lpointsAt);

    double ldot = lightDir.dotProduct(dspot.unit());

    if (ldot >= 0 && -ldot >= lightData.lcone)
      lcolor = lightData.lcolor*pow(-ldot, lightData.lexponent);
    else
      lcolor = CRGBA(0,0,0);
  }
  else
    lcolor = lightData.lcolor;

  //---

  // get gradient at point
  double xgray, ygray, xf, yf;

  image->sobelPixelGradient(x, y, 1, 1, xgray, ygray, xf, yf);

  // calc normal from gradient
  double nx = -lightData.surfaceScale*xf*xgray;
  double ny = -lightData.surfaceScale*yf*ygray;
  double nz = 1;

  CVector3D normal = CVector3D(nx, ny, nz).unit();

  //---

  if      (lightData.isSpecular) {
    CVector3D eye(0, 0, 1);

    CVector3D halfway = (lightDir + eye).unit();

    //---

    //double dot = normal.dotProduct(lightDir);
    double dot = normal.dotProduct(halfway);

    CRGBA  scolor = lightData.specConstant*pow(dot, lightData.specExponent)*lcolor;
    double sa     = scolor.getIntensity();

    scolor.setAlpha(sa);

    return scolor.clamp();
  }
  else if (lightData.isDiffuse) {
    double dot = normal.dotProduct(lightDir);

    CRGBA diffuse = lightData.diffuseConstant*dot*lcolor;

    diffuse.setAlpha(1);

    return diffuse.clamp();
  }
  else
    return CRGBA();
}

bool
CSVGBuffer::
isDrawing() const
{
  if (refBuffer_)
    return refBuffer_->isDrawing();

  return drawing_;
}

void
CSVGBuffer::
setDrawing(bool b)
{
  if (refBuffer_)
    return refBuffer_->setDrawing(b);

  drawing_ = b;
}

void
CSVGBuffer::
addClippedBuffer(CSVGBuffer *buffer, double x, double y,
                 double px1, double py1, double px2, double py2)
{
  if (! buffer->isAlpha())
    getRenderer()->addClippedImage(buffer->getRenderer(), x, y, px1, py1, px2, py2);
  else {
    CImagePtr image = buffer->getImage();

    image->clipOutside(px1, py1, px2, py2);

    addImage(x - px1, y - py1, image);
  }
}

void
CSVGBuffer::
setClippedBuffer(CSVGBuffer *buffer, double px1, double py1, double px2, double py2)
{
  if (! buffer->isAlpha())
    getRenderer()->setClippedImage(buffer->getRenderer(), px1, py1, px2, py2);
  else {
    CImagePtr image = buffer->getRenderer()->getImage();

    image->clipOutside(px1, py1, px2, py2);

    getRenderer()->setImage(image);
  }
}

void
CSVGBuffer::
addBuffer(CSVGBuffer *buffer, double x, double y)
{
  addImage(x, y, buffer);
}

CISize2D
CSVGBuffer::
getImageSize() const
{
  if (refBuffer_)
    return refBuffer_->getImageSize();

  //---

  return getRenderer()->getImageSize();
}

void
CSVGBuffer::
setImageFile(const std::string &filename)
{
  CImageFileSrc src(filename);

  CImagePtr image = CImageMgrInst->createImage(src);

  if (image.isValid())
    getRenderer()->setImage(image);
}

void
CSVGBuffer::
setImageFile(CFile &file)
{
  CImageFileSrc src(file);

  CImagePtr image = CImageMgrInst->createImage(src);

  if (image.isValid())
    getRenderer()->setImage(image);
}

void
CSVGBuffer::
setImage(double x, double y, CSVGBuffer *buffer)
{
  if (x != 0 || y != 0) {
    clear();

    addImage(x, y, buffer);
  }
  else
    setImage(buffer);
}

void
CSVGBuffer::
setImage(CSVGBuffer *buffer)
{
  assert(! drawing_ && ! buffer->drawing_);

  if (refBuffer_)
    return refBuffer_->setImage(buffer);

  //---

  if (! buffer->isAlpha())
    getRenderer()->setImage(buffer->getRenderer());
  else
    getRenderer()->setImage(buffer->getImage());
}

void
CSVGBuffer::
addReshapeImage(CSVGBuffer *buffer, double x, double y, int pw, int ph)
{
  bool oldDrawing = isDrawing();

  if (oldDrawing)
    stopDraw();

  int px = CSVGUtil::round(x);
  int py = CSVGUtil::round(y);

  if (! buffer->isAlpha())
    getRenderer()->addResizedImage(buffer->getRenderer(), px, py, pw, ph);
  else {
    CImagePtr image = buffer->getImage();

    image->reshape(pw, ph);

    addImage(x, y, image);
  }

  if (oldDrawing)
    startDraw();
}

void
CSVGBuffer::
addImage(CSVGBuffer *buffer)
{
  addImage(0, 0, buffer);
}

void
CSVGBuffer::
addImage(double x, double y, CSVGBuffer *buffer)
{
  assert(! drawing_ && ! buffer->drawing_);

  if (refBuffer_)
    return refBuffer_->addImage(x, y, buffer);

  //---

  int ix = CSVGUtil::round(x);
  int iy = CSVGUtil::round(y);

  if (! buffer->isAlpha()) {
    if (drawing_)
      getRenderer()->stopDraw();

    getRenderer()->combine(ix, iy, buffer->getRenderer());

    if (drawing_)
      getRenderer()->startDraw();
  }
  else {
    CImagePtr image1 = getRenderer()->getImage();

    addImage(x, y, image1);
  }
}

void
CSVGBuffer::
addImage(double x, double y, const CImagePtr &image2)
{
  if (refBuffer_)
    return refBuffer_->addImage(x, y, image2);

  //---

  int ix = CSVGUtil::round(x);
  int iy = CSVGUtil::round(y);

  CImagePtr image1 = getImage();

  int iwidth1  = image1->getWidth ();
  int iheight1 = image1->getHeight();
  int iwidth2  = image1->getWidth ();
  int iheight2 = image2->getHeight();

  int w = std::max(iwidth1 , ix + iwidth2 );
  int h = std::max(iheight1, iy + iheight2);

  if (! drawing_ && (w > iwidth1 || h > iheight1)) {
    renderer_->setSize(w, h);

    CImagePtr image3 = getImage();

    image3->combine(image1);
    image3->combine(ix, iy, image2);

    getRenderer()->setImage(image3);
  }
  else {
    image1->combine(ix, iy, image2);

    if (drawing_)
      getRenderer()->stopDraw();

    getRenderer()->setImage(image1);

    if (drawing_)
      getRenderer()->startDraw();
  }
}

void
CSVGBuffer::
reset()
{
  if (refBuffer_)
    return refBuffer_->reset();

  //---

  CSVGRenderer *renderer = getRenderer();

  renderer->setDataRange(0, 0, 1, 1);

  CMatrixStack2D matrix;

  renderer->setViewMatrix(matrix.getMatrix());

  //------

  clear();

  //------

  renderer->setStrokeColor(CRGBA(0,0,0,1));
  renderer->setFillColor  (CRGBA(0,0,0,1));
}

void
CSVGBuffer::
clear()
{
  if (refBuffer_)
    return refBuffer_->clear();

  //---

  CSVGRenderer *renderer = getRenderer();

  renderer->clear(CRGBA());
}

void
CSVGBuffer::
setup(const CBBox2D &bbox)
{
  if (refBuffer_)
    return refBuffer_->setup(bbox);

  //---

  reset();

  CSVGRenderer *renderer = getRenderer();

  double x1 = bbox.getXMin();
  double y1 = bbox.getYMin();
  double x2 = bbox.getXMax();
  double y2 = bbox.getYMax();

  int w = CSVGUtil::round(x2 - x1);
  int h = CSVGUtil::round(y2 - y1);

  renderer->setSize(w, h);

  renderer->setPixelRange(w, h);

  renderer->setDataRange(x1, y2, x2, y1);
}

void
CSVGBuffer::
beginDraw(double w, double h, const CBBox2D &bbox)
{
  if (refBuffer_)
    return refBuffer_->beginDraw(w, h, bbox);

  //---

  renderer_->setBackground(svg_.background());

  renderer_->setSize(w, h);

  renderer_->setPixelRange(w, h);

  startDraw();

  CPoint2D bmin, bmax;

  svg_.viewMatrix().multiplyPoint(bbox.getLL(), bmin);
  svg_.viewMatrix().multiplyPoint(bbox.getUR(), bmax);

  //renderer_->setDataRange(bbox.getXMin(), bbox.getYMin(), bbox.getXMax(), bbox.getYMax());
  renderer_->setDataRange(bmin.x, bmin.y, bmax.x, bmax.y);
}

void
CSVGBuffer::
endDraw()
{
  if (refBuffer_)
    return refBuffer_->endDraw();

  //---

  stopDraw();
}

void
CSVGBuffer::
startDraw()
{
  if (refBuffer_)
    return refBuffer_->startDraw();

  //---

  assert(! drawing_);

  drawing_ = true;

  renderer_->beginDraw();
}

void
CSVGBuffer::
stopDraw()
{
  if (refBuffer_)
    return refBuffer_->stopDraw();

  //---

  assert(drawing_);

  renderer_->endDraw();

  drawing_ = false;
}

void
CSVGBuffer::
setAlign(CHAlignType halign, CVAlignType valign)
{
  if (refBuffer_)
    return refBuffer_->setAlign(halign, valign);

  //---

  renderer_->setAlign(halign, valign);
}

void
CSVGBuffer::
setEqualScale(bool equalScale)
{
  if (refBuffer_)
    return refBuffer_->setEqualScale(equalScale);

  //---

  renderer_->setEqualScale(equalScale);
}

void
CSVGBuffer::
setScaleMin(bool scale)
{
  if (refBuffer_)
    return refBuffer_->setScaleMin(scale);

  //---

  renderer_->setScaleMin(scale);
}

void
CSVGBuffer::
fill(const CRGBA &bg)
{
  if (refBuffer_)
    return refBuffer_->fill(bg);

  //---

  renderer_->clear(bg);
}

//-------

void
CSVGBuffer::
setTransform(const CMatrixStack2D &matrix)
{
  if (refBuffer_)
    return refBuffer_->setTransform(matrix);

  //---

  transform_ = matrix;

  CMatrixStack2D matrix1(svg_.viewMatrix());

  matrix1.append(transform_);

  setViewMatrix(matrix1);
}

void
CSVGBuffer::
unsetTransform()
{
  if (refBuffer_)
    return refBuffer_->unsetTransform();

  //---

  transform_.reset();

  setViewMatrix(svg_.viewMatrix());
}

void
CSVGBuffer::
setViewMatrix(const CMatrixStack2D &matrix)
{
  if (refBuffer_)
    return refBuffer_->setViewMatrix(matrix);

  //---

  renderer_->setViewMatrix(matrix.getMatrix());
}

void
CSVGBuffer::
setStrokeColor(const CRGBA &color)
{
  if (refBuffer_)
    return refBuffer_->setStrokeColor(color);

  //---

  renderer_->setStrokeColor(color);
}

void
CSVGBuffer::
setStrokeFilled(bool b)
{
  if (refBuffer_)
    return refBuffer_->setStrokeFilled(b);

  //---

  renderer_->setStrokeFilled(b);
}

void
CSVGBuffer::
setStrokeFillType(CFillType type)
{
  if (refBuffer_)
    return refBuffer_->setStrokeFillType(type);

  //---

  renderer_->setStrokeFillType(type);
}

void
CSVGBuffer::
setStrokeFillGradient(CGenGradient *g)
{
  if (refBuffer_)
    return refBuffer_->setStrokeFillGradient(g);

  //---

  renderer_->setStrokeFillGradient(g);
}

void
CSVGBuffer::
setStrokeFillBuffer(CSVGBuffer *buffer)
{
  if (refBuffer_)
    return refBuffer_->setStrokeFillBuffer(buffer);

  //---

  CImagePtr image = buffer->getImage();

  renderer_->setStrokeFillImage(image);
}

void
CSVGBuffer::
setLineWidth(double width)
{
  if (refBuffer_)
    return refBuffer_->setLineWidth(width);

  //---

  renderer_->setLineWidth(width);
}

void
CSVGBuffer::
setLineDash(const CLineDash &dash)
{
  if (refBuffer_)
    return refBuffer_->setLineDash(dash);

  //---

  lineDash_ = dash;

  renderer_->setLineDash(lineDash_);
}

void
CSVGBuffer::
setLineDashOffset(double offset)
{
  if (refBuffer_)
    return refBuffer_->setLineDashOffset(offset);

  //---

  lineDash_.setOffset(offset);

  renderer_->setLineDash(lineDash_);
}

void
CSVGBuffer::
setLineCap(CLineCapType line_cap)
{
  if (refBuffer_)
    return refBuffer_->setLineCap(line_cap);

  //---

  renderer_->setLineCap(line_cap);
}

void
CSVGBuffer::
setLineJoin(CLineJoinType line_join)
{
  if (refBuffer_)
    return refBuffer_->setLineJoin(line_join);

  //---

  renderer_->setLineJoin(line_join);
}

void
CSVGBuffer::
setLineMitreLimit(double limit)
{
  if (refBuffer_)
    return refBuffer_->setLineMitreLimit(limit);

  //---

  renderer_->setMitreLimit(limit);
}

void
CSVGBuffer::
resetFill()
{
  if (refBuffer_)
    return refBuffer_->resetFill();

  //---

  renderer_->resetFill();
}

void
CSVGBuffer::
setFillType(CFillType type)
{
  if (refBuffer_)
    return refBuffer_->setFillType(type);

  //---

  renderer_->setFillType(type);
}

void
CSVGBuffer::
setFillColor(const CRGBA &color)
{
  if (refBuffer_)
    return refBuffer_->setFillColor(color);

  //---

  renderer_->setFillColor(color);
}

void
CSVGBuffer::
setFillGradient(CGenGradient *g)
{
  if (refBuffer_)
    return refBuffer_->setFillGradient(g);

  //---

  renderer_->setFillGradient(g);
}

void
CSVGBuffer::
setFillBuffer(CSVGBuffer *buffer)
{
  if (refBuffer_)
    return refBuffer_->setFillBuffer(buffer);

  //---

  CImagePtr image = buffer->getImage();

  renderer_->setFillImage(image);
}

void
CSVGBuffer::
setFillMatrix(const CMatrix2D &m)
{
  if (refBuffer_)
    return refBuffer_->setFillMatrix(m);

  //---

  renderer_->setFillMatrix(m);
}

void
CSVGBuffer::
drawImage(double x, double y, CSVGBuffer *buffer)
{
  if (refBuffer_)
    return refBuffer_->drawImage(x, y, buffer);

  renderer_->drawImage(CPoint2D(x, y), buffer->getImage());
}

CImagePtr
CSVGBuffer::
getImage() const
{
  if (refBuffer_) { // alpha
    CImagePtr image = refBuffer_->getImage();

    image->setAlphaGray(0);

    return image;
  }

  return getRenderer()->getImage();
}

#if 0
void
CSVGBuffer::
drawImage(const CBBox2D &bbox, CImagePtr image)
{
  if (refBuffer_)
    return refBuffer_->drawImage(bbox, image);

  //---

  CPoint2D p1, p2;

  renderer_->windowToPixel(bbox.getLL(), p1);
  renderer_->windowToPixel(bbox.getUR(), p2);

  int pw = CSVGUtil::round(fabs(p2.x - p1.x));
  int ph = CSVGUtil::round(fabs(p2.y - p1.y));

  image->reshape(pw, ph);

  //double x = std::min(p1.x, p2.x);
  //double y = std::min(p1.y, p2.y);

  // draw image resize to bbox
  renderer_->drawImage(bbox.getLL(), image);
}
#endif

void
CSVGBuffer::
pathText(const std::string &text, CFontPtr font, CHAlignType align)
{
  if (refBuffer_)
    return refBuffer_->pathText(text, font, align);

  //---

  renderer_->setFont(font);

  CBBox2D box;

  renderer_->textBounds(text, box);

  double dx = 0;

  if      (align == CHALIGN_TYPE_LEFT)
    dx = 0;
  else if (align == CHALIGN_TYPE_CENTER) {
    if (box.isSet())
      dx = -box.getWidth()/2;
  }
  else if (align == CHALIGN_TYPE_RIGHT) {
    if (box.isSet())
      dx = -box.getWidth();
  }

  renderer_->pathRMoveTo(CPoint2D(dx, 0));

  renderer_->pathText(text);
}

void
CSVGBuffer::
drawRoundedRectangle(const CBBox2D &bbox, double rx, double ry)
{
  svg_.setStrokeBuffer(this);

  const CPoint2D &ll = bbox.getLL();
  const CPoint2D &ur = bbox.getUR();

  pathInit();

  pathMoveTo(ll.x + rx, ll.y);
  pathLineTo(ur.x - rx, ll.y);
  pathArcTo (ur.x - rx, ll.y + ry, rx, ry, 3*M_PI/2, 2*M_PI);
  pathLineTo(ur.x, ll.y + ry);
  pathArcTo (ur.x - rx, ur.y - ry, rx, ry, 0       , M_PI/2.0);
  pathLineTo(ur.x - rx, ur.y);
  pathArcTo (ll.x + rx, ur.y - ry, rx, ry, M_PI/2.0, M_PI);
  pathLineTo(ll.x, ur.y - ry);
  pathArcTo (ll.x + rx, ll.y + ry, rx, ry, M_PI    , 3*M_PI/2);

  pathClose();

  pathStroke();
}

void
CSVGBuffer::
fillRoundedRectangle(const CBBox2D &bbox, double rx, double ry)
{
  svg_.setFillBuffer(this);

  const CPoint2D &ll = bbox.getLL();
  const CPoint2D &ur = bbox.getUR();

  pathInit();

  pathMoveTo(ll.x + rx, ll.y);
  pathLineTo(ur.x - rx, ll.y);
  pathArcTo (ur.x - rx, ll.y + ry, rx, ry, 3*M_PI/2, 2*M_PI);
  pathLineTo(ur.x, ll.y + ry);
  pathArcTo (ur.x - rx, ur.y - ry, rx, ry, 0       , M_PI/2.0);
  pathLineTo(ur.x - rx, ur.y);
  pathArcTo (ll.x + rx, ur.y - ry, rx, ry, M_PI/2.0, M_PI);
  pathLineTo(ll.x, ur.y - ry);
  pathArcTo (ll.x + rx, ll.y + ry, rx, ry, M_PI    , 3*M_PI/2);

  pathClose();

  pathFill();
}

void
CSVGBuffer::
drawRectangle(const CBBox2D &bbox)
{
  svg_.setStrokeBuffer(this);

  pathInit();

  pathMoveTo(bbox.getXMin(), bbox.getYMin());
  pathLineTo(bbox.getXMax(), bbox.getYMin());
  pathLineTo(bbox.getXMax(), bbox.getYMax());
  pathLineTo(bbox.getXMin(), bbox.getYMax());

  pathClose();

  pathStroke();
}

void
CSVGBuffer::
fillRectangle(const CBBox2D &bbox)
{
  svg_.setFillBuffer(this);

  pathInit();

  pathMoveTo(bbox.getXMin(), bbox.getYMin());
  pathLineTo(bbox.getXMax(), bbox.getYMin());
  pathLineTo(bbox.getXMax(), bbox.getYMax());
  pathLineTo(bbox.getXMin(), bbox.getYMax());

  pathClose();

  pathFill();
}

void
CSVGBuffer::
pathInit()
{
  if (refBuffer_)
    return refBuffer_->pathInit();

  //---

  renderer_->pathInit();

  pathLastControlPoint1_.setInvalid();
  pathLastControlPoint2_.setInvalid();
}

void
CSVGBuffer::
pathMoveTo(double x, double y)
{
  if (refBuffer_)
    return refBuffer_->pathMoveTo(x, y);

  //---

  renderer_->pathMoveTo(CPoint2D(x, y));
}

void
CSVGBuffer::
pathRMoveTo(double dx, double dy)
{
  if (refBuffer_)
    return refBuffer_->pathRMoveTo(dx, dy);

  //---

  renderer_->pathRMoveTo(CPoint2D(dx, dy));
}

void
CSVGBuffer::
pathLineTo(double x, double y)
{
  if (refBuffer_)
    return refBuffer_->pathLineTo(x, y);

  //---

  renderer_->pathLineTo(CPoint2D(x, y));
}

void
CSVGBuffer::
pathRLineTo(double dx, double dy)
{
  if (refBuffer_)
    return refBuffer_->pathRLineTo(dx, dy);

  //---

  renderer_->pathRLineTo(CPoint2D(dx, dy));
}

void
CSVGBuffer::
pathArcTo(double cx, double cy, double rx, double ry, double theta1, double theta2)
{
  if (refBuffer_)
    return refBuffer_->pathArcTo(cx, cy, rx, ry, theta1, theta2);

  //---

  renderer_->pathArcTo(CPoint2D(cx, cy), rx, ry, theta1, theta2);
}

void
CSVGBuffer::
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
CSVGBuffer::
pathBezier2To(double x1, double y1, double x2, double y2)
{
  if (refBuffer_)
    return refBuffer_->pathBezier2To(x1, y1, x2, y2);

  //---

  renderer_->pathCurveTo(CPoint2D(x1, y1), CPoint2D(x2, y2));
}

void
CSVGBuffer::
pathRBezier2To(double x1, double y1, double x2, double y2)
{
  if (refBuffer_)
    return refBuffer_->pathRBezier2To(x1, y1, x2, y2);

  //---

  renderer_->pathRCurveTo(CPoint2D(x1, y1), CPoint2D(x2, y2));
}

void
CSVGBuffer::
pathBezier3To(double x1, double y1, double x2, double y2, double x3, double y3)
{
  if (refBuffer_)
    return refBuffer_->pathBezier3To(x1, y1, x2, y2, x3, y3);

  //---

  renderer_->pathCurveTo(CPoint2D(x1, y1), CPoint2D(x2, y2), CPoint2D(x3, y3));
}

void
CSVGBuffer::
pathRBezier3To(double x1, double y1, double x2, double y2, double x3, double y3)
{
  if (refBuffer_)
    return refBuffer_->pathRBezier3To(x1, y1, x2, y2, x3, y3);

  //---

  renderer_->pathRCurveTo(CPoint2D(x1, y1), CPoint2D(x2, y2), CPoint2D(x3, y3));
}

void
CSVGBuffer::
pathClose()
{
  if (refBuffer_)
    return refBuffer_->pathClose();

  //---

  renderer_->pathClose();

  if (clip_)
    renderer_->savePath(transform_.getMatrix());
}

bool
CSVGBuffer::
pathGetCurrentPoint(CPoint2D &point) const
{
  double x, y;

  if (! pathGetCurrentPoint(&x, &y))
    return false;

  point = CPoint2D(x, y);

  return true;
}

bool
CSVGBuffer::
pathGetCurrentPoint(double *x, double *y) const
{
  if (refBuffer_)
    return refBuffer_->pathGetCurrentPoint(x, y);

  //---

  *x = 0;
  *y = 0;

  CPoint2D p;

  if (! renderer_->pathGetCurrentPoint(p))
    return false;

  *x = p.x;
  *y = p.y;

  return true;
}

bool
CSVGBuffer::
pathGetLastControlPoint(CPoint2D &p) const
{
  if (refBuffer_)
    return refBuffer_->pathGetLastControlPoint(p);

  //---

  if (! pathLastControlPoint1_.isValid())
    return false;

  p = pathLastControlPoint1_.getValue();

  return true;
}

bool
CSVGBuffer::
pathGetLastMControlPoint(CPoint2D &p) const
{
  if (refBuffer_)
    return refBuffer_->pathGetLastMControlPoint(p);

  //---

  if (! pathLastControlPoint1_.isValid())
    return false;

  p = pathMirrorPoint(pathLastControlPoint1_.getValue());

  return true;
}

bool
CSVGBuffer::
pathGetLastMRControlPoint(CPoint2D &p) const
{
  if (refBuffer_)
    return refBuffer_->pathGetLastMRControlPoint(p);

  //---

  if (! pathLastControlPoint1_.isValid())
    return false;

  p = pathMirrorPoint(pathLastControlPoint1_.getValue());

  CPoint2D cp;

  pathGetCurrentPoint(cp);

  p -= cp;

  return true;
}

void
CSVGBuffer::
pathSetLastControlPoint1(const CPoint2D &p)
{
  if (refBuffer_)
    return refBuffer_->pathSetLastControlPoint1(p);

  //---

  pathLastControlPoint1_ = p;
}

void
CSVGBuffer::
pathSetLastControlPoint2(const CPoint2D &p)
{
  if (refBuffer_)
    return refBuffer_->pathSetLastControlPoint2(p);

  //---

  pathLastControlPoint2_ = p;
}

void
CSVGBuffer::
pathStroke()
{
  if (refBuffer_)
    return refBuffer_->pathStroke();

  //---

  renderer_->pathStroke();
}

void
CSVGBuffer::
pathFill()
{
  if (refBuffer_)
    return refBuffer_->pathFill();

  //---

  renderer_->pathFill();
}

void
CSVGBuffer::
pathClip(CSVGBuffer *buffer)
{
  if (refBuffer_)
    return refBuffer_->pathClip(buffer);

  //---

  renderer_->pathClip(buffer ? buffer->renderer_ : 0);

  hasClipPath_ = true;
}

void
CSVGBuffer::
pathEoClip(CSVGBuffer *buffer)
{
  if (refBuffer_)
    return refBuffer_->pathEoClip(buffer);

  //---

  renderer_->pathEoclip(buffer ? buffer->renderer_ : 0);

  hasClipPath_ = true;
}

void
CSVGBuffer::
pathBBox(CBBox2D &bbox)
{
  if (refBuffer_)
    return refBuffer_->pathBBox(bbox);

  //---

  renderer_->pathBBox(bbox);
}

void
CSVGBuffer::
initClip()
{
  if (refBuffer_)
    return refBuffer_->initClip();

  //---

  renderer_->initClip();

  hasClipPath_ = false;
}

void
CSVGBuffer::
addClipPath(CSVGBuffer *origBuffer)
{
  if (refBuffer_)
    return refBuffer_->addClipPath(origBuffer);

  renderer_->addClipPath(origBuffer->getRenderer());
}

#if 0
bool
CSVGBuffer::
mmToPixel(double mm, double *pixel)
{
  if (refBuffer_)
    return refBuffer_->mmToPixel(mm, pixel);

  //---

  //return renderer_->mmToPixel(mm, pixel);

  // 75 dpi
  *pixel = 75.0*mm/25.4;

  return true;
}
#endif

CPoint2D
CSVGBuffer::
pathMirrorPoint(const CPoint2D &p) const
{
  CPoint2D cp;

  pathGetCurrentPoint(cp);

  double x1 = 2*cp.x - p.x;
  double y1 = 2*cp.y - p.y;

  return CPoint2D(x1, y1);
}

#if 0
void
CSVGBuffer::
lengthToPixel(double xi, double yi, double *xo, double *yo)
{
  if (refBuffer_)
    return refBuffer_->lengthToPixel(xi, yi, xo, yo);

  //---

  double px1, py1, px2, py2;

  windowToPixel( 0,  0, &px1, &py1);
  windowToPixel(xi, yi, &px2, &py2);

  *xo = fabs(px2 - px1);
  *yo = fabs(py2 - py1);
}

void
CSVGBuffer::
windowToPixel(double xi, double yi, double *xo, double *yo)
{
  if (refBuffer_)
    return refBuffer_->windowToPixel(xi, yi, xo, yo);

  //---

  CPoint2D p;

  renderer_->windowToPixel(CPoint2D(xi, yi), p);

  *xo = p.x;
  *yo = p.y;
}
#endif
