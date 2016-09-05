#include <CSVGJTypes.h>
#include <CSVGJavaScript.h>
#include <CSVG.h>
#include <CSVGObject.h>
#include <CSVGJavaScript.h>
#include <CJUtil.h>

CJObjTypeP CSVGJTransformListType::type_;

CJObjTypeP
CSVGJTransformListType::
instance(CJavaScript *js)
{
  if (! type_) {
    type_ = CJObjTypeP(new CSVGJTransformListType(js));

    js->addObjectType("SVGTransformList", type_);
  }

  return type_;
}

CSVGJTransformListType::
CSVGJTransformListType(CJavaScript *js) :
 CJObjType(js, CJToken::Type::Object, "SVGTransformList")
{
  addObjectFunction(js, "getItem");
  addObjectFunction(js, "consolidate");
  addObjectFunction(js, "createSVGTransformFromMatrix");
}

//---

/*
  interface SVGTransformList {
     readonly attribute unsigned long numberOfItems;

    void clear() raises(DOMException);
    SVGTransform initialize(in SVGTransform newItem) raises(DOMException);
    SVGTransform getItem(in unsigned long index) raises(DOMException);
    SVGTransform insertItemBefore(in SVGTransform newItem, in unsigned long index)
      raises(DOMException);
    SVGTransform replaceItem(in SVGTransform newItem, in unsigned long index) raises(DOMException);
    SVGTransform removeItem(in unsigned long index) raises(DOMException);
    SVGTransform appendItem(in SVGTransform newItem) raises(DOMException);
    SVGTransform createSVGTransformFromMatrix(in SVGMatrix matrix);
    SVGTransform consolidate() raises(DOMException);
  };
*/
CSVGJTransformList::
CSVGJTransformList(CSVG *svg, CSVGObject *obj) :
 CJObj(svg->js(), CSVGJTransformListType::instance(svg->js())),
 svg_(svg), obj_(obj)
{
}

CSVGJTransformList::
CSVGJTransformList(CSVG *svg, const CMatrixStack2D &stack) :
 CJObj(svg->js(), CSVGJTransformListType::instance(svg->js())), svg_(svg), stack_(stack)
{
}

long
CSVGJTransformList::
length() const
{
  return matrixStack().length();
}

CJValueP
CSVGJTransformList::
indexValue(int ind) const
{
  if (ind >= 0 && ind < length()) {
    if (obj_)
      return CJValueP(new CSVGJTransform(svg_, obj_, ind));
    else {
      CSVGJMatrixP matrix(new CSVGJMatrix(svg_, stack_.transform(ind).calcMatrix()));

      return CJValueP(new CSVGJTransform(svg_, matrix));
    }
  }
  else
    return CJValueP();
}

CMatrixStack2D
CSVGJTransformList::
matrixStack() const
{
  if (obj_) {
    CMatrixStack2D stack = obj_->getTransform();

    if (! stack.length()) {
      CMatrix2D m;

      m.setIdentity();

      stack.matrix(m);
    }

    return stack;
  }
  else
    return stack_;
}

CMatrix2D
CSVGJTransformList::
matrix() const
{
  return matrixStack().getMatrix();
}

void
CSVGJTransformList::
setMatrix(const CMatrix2D &m)
{
  CMatrixStack2D stack(m);

  if (obj_)
    obj_->setTransform(stack);
  else
    stack_ = stack;
}

CJValueP
CSVGJTransformList::
getProperty(CJavaScript *js, const std::string &key) const
{
  if      (key == "matrix") {
    CSVGJTransformList *th = const_cast<CSVGJTransformList *>(this);

    CSVGJTransformListP p = std::static_pointer_cast<CSVGJTransformList>(th->shared_from_this());

    return CJValueP(new CSVGJMatrix(svg_, p));
  }
  else if (key == "numberOfItems") {
    return js->createNumberValue(long(matrixStack().length()));
  }
  else
    return CJObj::getProperty(js, key);
}

CJValueP
CSVGJTransformList::
execNameFn(CJavaScript *, const std::string &name, const Values &values)
{
  if      (name == "getItem") {
    if (values.size() >= 2) {
      long ind = values[1]->toInteger();

      return indexValue(ind);
    }
    else
      return CJValueP();
  }
  else if (name == "consolidate") {
    if (obj_) {
      CMatrixStack2D stack = obj_->getTransform();

      CMatrix2D m = stack.getMatrix();

      CMatrixStack2D stack1(m);

      return CJValueP(new CSVGJTransformList(svg_, stack1));
    }
    else {
      return CJValueP(new CSVGJTransformList(svg_, stack_));
    }
  }
  else if (name == "createSVGTransformFromMatrix") {
    CSVGJMatrixP matrix(new CSVGJMatrix(svg_, CMatrix2D()));

    if (values.size() >= 2) {
      CJValueP matrixVal = values[1];

      CSVGJMatrixP matrix1;

      if (matrixVal && matrixVal->type() == CJValue::Type::Object) {
        CJObjP matrixObj = std::static_pointer_cast<CJObj>(matrixVal);

        if (matrixObj->type()->name() == "SVGMatrix") {
          matrix1 = std::static_pointer_cast<CSVGJMatrix>(matrixObj);
        }
      }

      if (matrix1) {
        matrix->setMatrix(matrix1->matrix());
      }
    }

    return CJValueP(new CSVGJTransform(svg_, matrix));
  }
  else {
    return CJValueP();
  }
}

//------

CJObjTypeP CSVGJTransformType::type_;

CJObjTypeP
CSVGJTransformType::
instance(CJavaScript *js)
{
  if (! type_) {
    type_ = CJObjTypeP(new CSVGJTransformType(js));

    js->addObjectType("SVGTransform", type_);
  }

  return type_;
}

CSVGJTransformType::
CSVGJTransformType(CJavaScript *js) :
 CJObjType(js, CJToken::Type::Object, "SVGTransform")
{
  addObjectFunction(js, "setRotate");
  addObjectFunction(js, "setScale");
  addObjectFunction(js, "setTranslate");
  addObjectFunction(js, "setMatrix");
}

//---

/*
  interface SVGTransform {
    // Transform Types
    const unsigned short SVG_TRANSFORM_UNKNOWN = 0;
    const unsigned short SVG_TRANSFORM_MATRIX = 1;
    const unsigned short SVG_TRANSFORM_TRANSLATE = 2;
    const unsigned short SVG_TRANSFORM_SCALE = 3;
    const unsigned short SVG_TRANSFORM_ROTATE = 4;
    const unsigned short SVG_TRANSFORM_SKEWX = 5;
    const unsigned short SVG_TRANSFORM_SKEWY = 6;

    readonly attribute unsigned short type;
    readonly attribute SVGMatrix matrix;
    readonly attribute float angle;

    void setMatrix(in SVGMatrix matrix) raises(DOMException);
    void setTranslate(in float tx, in float ty) raises(DOMException);
    void setScale(in float sx, in float sy) raises(DOMException);
    void setRotate(in float angle, in float cx, in float cy) raises(DOMException);
    void setSkewX(in float angle) raises(DOMException);
    void setSkewY(in float angle) raises(DOMException);
  };
*/
CSVGJTransform::
CSVGJTransform(CSVG *svg, CSVGObject *obj, int ind) :
 CJObj(svg->js(), CSVGJTransformType::instance(svg->js())), svg_(svg), obj_(obj), ind_(ind)
{
}

CSVGJTransform::
CSVGJTransform(CSVG *svg, CSVGJMatrixP matrix) :
 CJObj(svg->js(), CSVGJTransformType::instance(svg->js())), svg_(svg), matrix_(std::move(matrix))
{
}

CJValueP
CSVGJTransform::
getProperty(CJavaScript *js, const std::string &key) const
{
  if      (key == "type") {
    CMatrixTransformType type = CMatrixTransformType::MATRIX;

    if (matrix_)
      type = CMatrixTransformType::MATRIX;
    else {
      type = transform().type();

      if (type == CMatrixTransformType::SCALE1 || type == CMatrixTransformType::SCALE2)
        type = CMatrixTransformType::SCALE1;

      if (type == CMatrixTransformType::ROTATE || type == CMatrixTransformType::ROTATE_ORIGIN)
        type = CMatrixTransformType::ROTATE;
    }

    return js->createNumberValue(long(type));
  }
  else if (key == "matrix") {
    if (! matrix_)
      return CSVGJMatrixP(new CSVGJMatrix(svg_, obj_, ind_));

    return std::static_pointer_cast<CJValue>(matrix_);
  }
  else {
    return CJObj::getProperty(js, key);
  }
}

CMatrixStack2D::Transform
CSVGJTransform::
transform() const
{
  if (matrix_)
    return CMatrixStack2D::Transform(matrix_->matrix());
  else {
    CMatrixStack2D stack = obj_->getTransform();

    if (! stack.length()) {
      CMatrix2D m;

      m.setIdentity();

      stack.matrix(m);
    }

    return stack.transform(ind_);
  }
}

void
CSVGJTransform::
setTransform(const CMatrixStack2D::Transform &transform)
{
  if (matrix_) {
    matrix_->setMatrix(transform.calcMatrix());
  }
  else {
   CMatrixStack2D stack = obj_->getTransform();

   stack.setTransform(ind_, transform);

   obj_->setTransform(stack);
  }
}

CJValueP
CSVGJTransform::
execNameFn(CJavaScript *, const std::string &name, const Values &values)
{
  if      (name == "setRotate") {
    if (values.size() != 2 && values.size() != 4)
      return CJValueP();

    double a = CMathGen::DegToRad(values[1]->toReal());

    if (values.size() == 4) {
      double cx = values[2]->toReal();
      double cy = values[3]->toReal();

      setTransform(CMatrixStack2D::rotateTransform(a, CPoint2D(cx, cy)));
    }
    else
      setTransform(CMatrixStack2D::rotateTransform(a));
  }
  else if (name == "setScale") {
    if (values.size() != 2 && values.size() != 3)
      return CJValueP();

    double sx = values[1]->toReal();
    double sy = sx;

    if (values.size() == 3)
      sy = values[2]->toReal();

    setTransform(CMatrixStack2D::scaleTransform(sx, sy));
  }
  else if (name == "setTranslate") {
    if (values.size() != 3)
      return CJValueP();

    double dx = values[1]->toReal();
    double dy = values[2]->toReal();

    setTransform(CMatrixStack2D::translateTransform(dx, dy));
  }
  else if (name == "setMatrix") {
    if (values.size() != 2)
      return CJValueP();

    CJValueP matrixVal = values[1];

    CSVGJMatrixP matrix;

    if (matrixVal && matrixVal->type() == CJValue::Type::Object) {
      CJObjP matrixObj = std::static_pointer_cast<CJObj>(matrixVal);

      if (matrixObj->type()->name() == "SVGMatrix") {
        matrix = std::static_pointer_cast<CSVGJMatrix>(matrixObj);
      }
    }

    if (matrix)
      setTransform(matrix->matrix());
  }

  return CJValueP();
}

//------

CJObjTypeP CSVGJMatrixType::type_;

CJObjTypeP
CSVGJMatrixType::
instance(CJavaScript *js)
{
  if (! type_) {
    type_ = CJObjTypeP(new CSVGJMatrixType(js));

    js->addObjectType("SVGMatrix", type_);
  }

  return type_;
}

CSVGJMatrixType::
CSVGJMatrixType(CJavaScript *js) :
 CJObjType(js, CJToken::Type::Object, "SVGMatrix")
{
}

//---

/*
  interface SVGMatrix {
    attribute float a setraises(DOMException);
    attribute float b setraises(DOMException);
    attribute float c setraises(DOMException);
    attribute float d setraises(DOMException);
    attribute float e setraises(DOMException);
    attribute float f setraises(DOMException);

    SVGMatrix multiply(in SVGMatrix secondMatrix);
    SVGMatrix inverse() raises(SVGException);
    SVGMatrix translate(in float x, in float y);
    SVGMatrix scale(in float scaleFactor);
    SVGMatrix scaleNonUniform(in float scaleFactorX, in float scaleFactorY);
    SVGMatrix rotate(in float angle);
    SVGMatrix rotateFromVector(in float x, in float y) raises(SVGException);
    SVGMatrix flipX();
    SVGMatrix flipY();
    SVGMatrix skewX(in float angle);
    SVGMatrix skewY(in float angle);
  };
*/
CSVGJMatrix::
CSVGJMatrix(CSVG *svg, CSVGObject *obj, int ind) :
 CJObj(svg->js(), CSVGJMatrixType::instance(svg->js())),
 svg_(svg), obj_(obj), ind_(ind)
{
}

CSVGJMatrix::
CSVGJMatrix(CSVG *svg, const CSVGJTransformListP &stack) :
 CJObj(svg->js(), CSVGJMatrixType::instance(svg->js())), svg_(svg), stack_(stack)
{
}

CSVGJMatrix::
CSVGJMatrix(CSVG *svg, const CMatrix2D &m) :
 CJObj(svg->js(), CSVGJMatrixType::instance(svg->js())), svg_(svg), m_(m)
{
}

int
CSVGJMatrix::
cmp(CJObjP obj) const
{
  CSVGJMatrixP matrix;

  if (obj->type()->name() == "SVGMatrix")
    matrix = std::static_pointer_cast<CSVGJMatrix>(obj);
  else
    return CJObj::cmp(obj);

  return this->matrix().cmp(matrix->matrix());
}

CJValueP
CSVGJMatrix::
getProperty(CJavaScript *js, const std::string &key) const
{
  if (key == "a" || key == "b" || key == "c" || key == "d" || key == "e" || key == "f") {
    double values[9];

    matrix().getValues(values, 9);

    if      (key == "a") return js->createNumberValue(values[0]);
    else if (key == "b") return js->createNumberValue(values[3]);
    else if (key == "c") return js->createNumberValue(values[1]);
    else if (key == "d") return js->createNumberValue(values[4]);
    else if (key == "e") return js->createNumberValue(values[2]);
    else if (key == "f") return js->createNumberValue(values[5]);

    return CJValueP();
  }
  else {
    return CJObj::getProperty(js, key);
  }
}

void
CSVGJMatrix::
setProperty(CJavaScript *js, const std::string &key, CJValueP value)
{
  if (key == "a" || key == "b" || key == "c" || key == "d" || key == "e" || key == "f") {
    double v = value->toReal();

    CMatrix2D m = matrix();

    double values[9];

    m.getValues(values, 9);

    if      (key == "a") values[0] = v;
    else if (key == "b") values[3] = v;
    else if (key == "c") values[1] = v;
    else if (key == "d") values[4] = v;
    else if (key == "e") values[2] = v;
    else if (key == "f") values[5] = v;

    m.setValues(values, 9);

    setMatrix(m);
  }
  else {
    CJObj::setProperty(js, key, value);
  }
}

CMatrix2D
CSVGJMatrix::
matrix() const
{
  if (obj_) {
    CMatrixStack2D stack = obj_->getTransform();

    if (! stack.length()) {
      CMatrix2D m;

      m.setIdentity();

      stack.matrix(m);
    }

    return stack.transform(ind_).calcMatrix();
  }
  else if (stack_) {
    return stack_->matrix();
  }
  else {
    return m_;
  }
}

void
CSVGJMatrix::
setMatrix(const CMatrix2D &m)
{
  if (obj_) {
    CMatrixStack2D stack = obj_->getTransform();

    stack.setTransform(ind_, CMatrixStack2D::Transform(m));

    obj_->setTransform(stack);
  }
  else if (stack_) {
    stack_->setMatrix(m);
  }
  else {
    m_ = m;
  }
}

CJValueP
CSVGJMatrix::
execNameFn(CJavaScript *, const std::string &, const Values &)
{
  return CJValueP();
}

void
CSVGJMatrix::
print(std::ostream &os) const
{
  CMatrix2D m = matrix();

  double values[9];

  m.getValues(values, 9);

  os << "SVGMatrix [ " <<
        CJUtil::realToString(values[0]) << ", " <<
        CJUtil::realToString(values[3]) << ", " <<
        CJUtil::realToString(values[1]) << ", " <<
        CJUtil::realToString(values[4]) << ", " <<
        CJUtil::realToString(values[2]) << ", " <<
        CJUtil::realToString(values[5]) << " ]";
}

//------

CJObjTypeP CSVGJRectType::type_;

CJObjTypeP
CSVGJRectType::
instance(CJavaScript *js)
{
  if (! type_) {
    type_ = CJObjTypeP(new CSVGJRectType(js));

    js->addObjectType("SVGRect", type_);
  }

  return type_;
}

CSVGJRectType::
CSVGJRectType(CJavaScript *js) :
 CJObjType(js, CJToken::Type::Object, "SVGRect")
{
}

//---

/*
  interface SVGRect {
    attribute float x setraises(DOMException);
    attribute float y setraises(DOMException);
    attribute float width setraises(DOMException);
    attribute float height setraises(DOMException);
  };
*/
CSVGJRect::
CSVGJRect(CSVG *svg, const CBBox2D &rect) :
 CJObj(svg->js(), CSVGJRectType::instance(svg->js())), svg_(svg), rect_(rect)
{
}

int
CSVGJRect::
cmp(CJObjP obj) const
{
  CSVGJRectP rect;

  if (obj->type()->name() == "SVGRect")
    rect = std::static_pointer_cast<CSVGJRect>(obj);
  else
    return CJObj::cmp(obj);

  return this->rect() == rect->rect();
}

CJValueP
CSVGJRect::
getProperty(CJavaScript *js, const std::string &key) const
{
  if      (key == "x") {
    return js->createNumberValue(rect_.getXMin());
  }
  else if (key == "y") {
    return js->createNumberValue(rect_.getYMin());
  }
  else if (key == "width") {
    return js->createNumberValue(rect_.getWidth());
  }
  else if (key == "height") {
    return js->createNumberValue(rect_.getHeight());
  }
  else {
    return CJObj::getProperty(js, key);
  }
}

void
CSVGJRect::
setProperty(CJavaScript *js, const std::string &key, CJValueP value)
{
  CJObj::setProperty(js, key, value);
}

CBBox2D
CSVGJRect::
rect() const
{
  return rect_;
}

void
CSVGJRect::
setRect(const CBBox2D &rect)
{
  rect_ = rect;
}

CJValueP
CSVGJRect::
execNameFn(CJavaScript *, const std::string &, const Values &)
{
  return CJValueP();
}

void
CSVGJRect::
print(std::ostream &os) const
{
  CBBox2D rect = this->rect();

  os << "SVGRect: [" <<
        CJUtil::realToString(rect.getXMin()) << ", " <<
        CJUtil::realToString(rect.getYMin()) << ", " <<
        CJUtil::realToString(rect.getXMax()) << ", " <<
        CJUtil::realToString(rect.getYMax()) << "]";
}

//------

/*
  interface SVGPointList {
    readonly attribute unsigned long numberOfItems;

    void clear() raises(DOMException);
    SVGPoint initialize(in SVGPoint newItem) raises(DOMException);
    SVGPoint getItem(in unsigned long index) raises(DOMException);
    SVGPoint insertItemBefore(in SVGPoint newItem, in unsigned long index) raises(DOMException);
    SVGPoint replaceItem(in SVGPoint newItem, in unsigned long index) raises(DOMException);
    SVGPoint removeItem(in unsigned long index) raises(DOMException);
    SVGPoint appendItem(in SVGPoint newItem) raises(DOMException);
  };
*/

/*
  interface SVGPoint {
    attribute float x setraises(DOMException);
    attribute float y setraises(DOMException);

    SVGPoint matrixTransform(in SVGMatrix matrix);
  };
*/

//------

CJObjTypeP CSVGJNumberListType::type_;

CJObjTypeP
CSVGJNumberListType::
instance(CJavaScript *js)
{
  if (! type_) {
    type_ = CJObjTypeP(new CSVGJNumberListType(js));

    js->addObjectType("SVGNumberList", type_);
  }

  return type_;
}

CSVGJNumberListType::
CSVGJNumberListType(CJavaScript *js) :
 CJObjType(js, CJToken::Type::Object, "SVGNumberList")
{
}

//---

/*
  interface SVGNumberList {
    readonly attribute unsigned long numberOfItems;

    void clear() raises(DOMException);
    SVGNumber initialize(in SVGNumber newItem) raises(DOMException);
    SVGNumber getItem(in unsigned long index) raises(DOMException);
    SVGNumber insertItemBefore(in SVGNumber newItem, in unsigned long index) raises(DOMException);
    SVGNumber replaceItem(in SVGNumber newItem, in unsigned long index) raises(DOMException);
    SVGNumber removeItem(in unsigned long index) raises(DOMException);
    SVGNumber appendItem(in SVGNumber newItem) raises(DOMException);
  };
*/
CSVGJNumberList::
CSVGJNumberList(CSVG *svg, const Reals &reals) :
 CJObj(svg->js(), CSVGJNumberListType::instance(svg->js())), svg_(svg), reals_(reals)
{
}

int
CSVGJNumberList::
cmp(CJObjP obj) const
{
  CSVGJNumberListP reals;

  if (obj->type()->name() == "SVGNumberList")
    reals = std::static_pointer_cast<CSVGJNumberList>(obj);
  else
    return CJObj::cmp(obj);

  return this->numberList() == reals->numberList();
}

CJValueP
CSVGJNumberList::
getProperty(CJavaScript *js, const std::string &key) const
{
  if      (key == "numberOfItems") {
    return js->createNumberValue(long(reals_.size()));
  }
  else {
    return CJObj::getProperty(js, key);
  }
}

void
CSVGJNumberList::
setProperty(CJavaScript *js, const std::string &key, CJValueP value)
{
  CJObj::setProperty(js, key, value);
}

const CSVGJNumberList::Reals &
CSVGJNumberList::
numberList() const
{
  return reals_;
}

void
CSVGJNumberList::
setNumberList(const Reals &reals)
{
  reals_ = reals;
}

CJValueP
CSVGJNumberList::
execNameFn(CJavaScript *, const std::string &, const Values &)
{
  return CJValueP();
}

void
CSVGJNumberList::
print(std::ostream &os) const
{
  Reals reals = this->numberList();

  os << "SVGNumberList: [ ";

  for (uint i = 0; i < reals.size(); ++i) {
    if (i > 0)
      os << ", ";

    os << CJUtil::realToString(reals[i]);
  }

  os << " ]";
}

//------

CJObjTypeP CSVGJNumberType::type_;

CJObjTypeP
CSVGJNumberType::
instance(CJavaScript *js)
{
  if (! type_) {
    type_ = CJObjTypeP(new CSVGJNumberType(js));

    js->addObjectType("SVGNumber", type_);
  }

  return type_;
}

CSVGJNumberType::
CSVGJNumberType(CJavaScript *js) :
 CJObjType(js, CJToken::Type::Object, "SVGNumber")
{
}

//---

/*
  interface SVGNumber {
    attribute float value setraises(DOMException);
  };
*/
CSVGJNumber::
CSVGJNumber(CSVG *svg, double r) :
 CJObj(svg->js(), CSVGJNumberType::instance(svg->js())), svg_(svg), r_(r)
{
}

int
CSVGJNumber::
cmp(CJObjP obj) const
{
  CSVGJNumberP r;

  if (obj->type()->name() == "SVGNumber")
    r = std::static_pointer_cast<CSVGJNumber>(obj);
  else
    return CJObj::cmp(obj);

  return this->number() == r->number();
}

CJValueP
CSVGJNumber::
getProperty(CJavaScript *js, const std::string &key) const
{
  if      (key == "value") {
    return js->createNumberValue(r_);
  }
  else {
    return CJObj::getProperty(js, key);
  }
}

void
CSVGJNumber::
setProperty(CJavaScript *js, const std::string &key, CJValueP value)
{
  CJObj::setProperty(js, key, value);
}

double
CSVGJNumber::
number() const
{
  return r_;
}

void
CSVGJNumber::
setNumber(double r)
{
  r_ = r;
}

CJValueP
CSVGJNumber::
execNameFn(CJavaScript *, const std::string &, const Values &)
{
  return CJValueP();
}

void
CSVGJNumber::
print(std::ostream &os) const
{
  os << "SVGNumber: " << r_;
}

//------

CJObjTypeP CSVGJLengthListType::type_;

CJObjTypeP
CSVGJLengthListType::
instance(CJavaScript *js)
{
  if (! type_) {
    type_ = CJObjTypeP(new CSVGJLengthListType(js));

    js->addObjectType("SVGLengthList", type_);
  }

  return type_;
}

CSVGJLengthListType::
CSVGJLengthListType(CJavaScript *js) :
 CJObjType(js, CJToken::Type::Object, "SVGLengthList")
{
}

//---

/*
  interface SVGLengthList {
    readonly attribute unsigned long numberOfItems;

    void clear() raises(DOMException);
    SVGLength initialize(in SVGLength newItem) raises(DOMException);
    SVGLength getItem(in unsigned long index) raises(DOMException);
    SVGLength insertItemBefore(in SVGLength newItem, in unsigned long index) raises(DOMException);
    SVGLength replaceItem(in SVGLength newItem, in unsigned long index) raises(DOMException);
    SVGLength removeItem(in unsigned long index) raises(DOMException);
    SVGLength appendItem(in SVGLength newItem) raises(DOMException);
  };
*/
CSVGJLengthList::
CSVGJLengthList(CSVG *svg, const Lengths &lengths) :
 CJObj(svg->js(), CSVGJLengthListType::instance(svg->js())), svg_(svg), lengths_(lengths)
{
}

int
CSVGJLengthList::
cmp(CJObjP obj) const
{
  CSVGJLengthListP lengths;

  if (obj->type()->name() == "SVGLengthList")
    lengths = std::static_pointer_cast<CSVGJLengthList>(obj);
  else
    return CJObj::cmp(obj);

  return this->getLengths() == lengths->getLengths();
}

CJValueP
CSVGJLengthList::
getProperty(CJavaScript *js, const std::string &key) const
{
  if (key == "numberOfItems") {
    return js->createNumberValue(long(lengths_.size()));
  }
  else {
    return CJObj::getProperty(js, key);
  }
}

void
CSVGJLengthList::
setProperty(CJavaScript *js, const std::string &key, CJValueP value)
{
  CJObj::setProperty(js, key, value);
}

const CSVGJLengthList::Lengths &
CSVGJLengthList::
getLengths() const
{
  return lengths_;
}

void
CSVGJLengthList::
setLengths(const Lengths &lengths)
{
  lengths_ = lengths;
}

CJValueP
CSVGJLengthList::
execNameFn(CJavaScript *, const std::string &, const Values &)
{
  return CJValueP();
}

void
CSVGJLengthList::
print(std::ostream &os) const
{
  const Lengths &lengths = getLengths();

  os << "SVGLengthList: [ ";

  for (uint i = 0; i < lengths.size(); ++i) {
    if (i > 0)
      os << ", ";

    os << lengths[i];
  }

  os << " ]";
}

//------

CJObjTypeP CSVGJLengthType::type_;

CJObjTypeP
CSVGJLengthType::
instance(CJavaScript *js)
{
  if (! type_) {
    type_ = CJObjTypeP(new CSVGJLengthType(js));

    js->addObjectType("SVGLength", type_);
  }

  return type_;
}

CSVGJLengthType::
CSVGJLengthType(CJavaScript *js) :
 CJObjType(js, CJToken::Type::Object, "SVGLength")
{
}

//---

/*
  interface SVGLength {
  // Length Unit Types
  const unsigned short SVG_LENGTHTYPE_UNKNOWN = 0;
  const unsigned short SVG_LENGTHTYPE_NUMBER = 1;
  const unsigned short SVG_LENGTHTYPE_PERCENTAGE = 2;
  const unsigned short SVG_LENGTHTYPE_EMS = 3;
  const unsigned short SVG_LENGTHTYPE_EXS = 4;
  const unsigned short SVG_LENGTHTYPE_PX = 5;
  const unsigned short SVG_LENGTHTYPE_CM = 6;
  const unsigned short SVG_LENGTHTYPE_MM = 7;
  const unsigned short SVG_LENGTHTYPE_IN = 8;
  const unsigned short SVG_LENGTHTYPE_PT = 9;
  const unsigned short SVG_LENGTHTYPE_PC = 10;

  readonly attribute unsigned short unitType;
           attribute float value setraises(DOMException);
           attribute float valueInSpecifiedUnits setraises(DOMException);
           attribute DOMString valueAsString setraises(DOMException);

  void newValueSpecifiedUnits(in unsigned short unitType, in float valueInSpecifiedUnits)
        raises(DOMException);
  void convertToSpecifiedUnits(in unsigned short unitType) raises(DOMException);
*/
CSVGJLength::
CSVGJLength(CSVG *svg, const CScreenUnits &length) :
 CJObj(svg->js(), CSVGJLengthType::instance(svg->js())), svg_(svg), length_(length)
{
}

CSVGJLength::
CSVGJLength(CSVG *svg, CSVGObject *obj, const std::string &name) :
 CJObj(svg->js(), CSVGJLengthType::instance(svg->js())), svg_(svg), obj_(obj), name_(name)
{
}

int
CSVGJLength::
cmp(CJObjP obj) const
{
  CSVGJLengthP length;

  if (obj->type()->name() == "SVGLength")
    length = std::static_pointer_cast<CSVGJLength>(obj);
  else
    return CJObj::cmp(obj);

  return this->getLength() == length->getLength();
}

CJValueP
CSVGJLength::
getProperty(CJavaScript *js, const std::string &key) const
{
  CScreenUnits length = getLength();

  if      (key == "unitType") {
    CSVGLengthType type = CSVGLengthType::UNKNOWN;

    switch (length.units()) {
      case CScreenUnits::Units::EM     : type = CSVGLengthType::EMS; break;
      case CScreenUnits::Units::EX     : type = CSVGLengthType::EXS; break;
      case CScreenUnits::Units::PX     : type = CSVGLengthType::PX; break;
      case CScreenUnits::Units::PT     : type = CSVGLengthType::PT; break;
      case CScreenUnits::Units::PC     : type = CSVGLengthType::PC; break;
      case CScreenUnits::Units::CM     : type = CSVGLengthType::CM; break;
      case CScreenUnits::Units::MM     : type = CSVGLengthType::MM; break;
      case CScreenUnits::Units::IN     : type = CSVGLengthType::IN; break;
      case CScreenUnits::Units::PERCENT: type = CSVGLengthType::PERCENTAGE; break;
      default                          : type = CSVGLengthType::UNKNOWN; break;
    }

    return js->createNumberValue(long(type));
  }
  else if (key == "value") {
    return js->createNumberValue(long(length.value()));
  }
  else if (key == "valueInSpecifiedUnits") {
    return js->createNumberValue(long(length.value()));
  }
  else if (key == "valueAsString") {
    return js->createStringValue(length.toString());
  }
  else {
    return CJObj::getProperty(js, key);
  }
}

void
CSVGJLength::
setProperty(CJavaScript *js, const std::string &key, CJValueP value)
{
  if (key == "valueAsString") {
    if (obj_) {
      if (! obj_->processOption(name_, value->toString()))
        js->throwException(CJExceptionType::SYNTAX_ERR);
    }
    else {
      COptValT<CScreenUnits> length = CSVG::decodeLengthValue(value->toString());

      if (length.isValid())
        length_ = length.getValue();
      else
        js->throwException(CJExceptionType::SYNTAX_ERR);
    }
  }
  else
    CJObj::setProperty(js, key, value);
}

CScreenUnits
CSVGJLength::
getLength() const
{
  if (obj_) {
    COptString value = obj_->getNameValue(name_);

    COptValT<CScreenUnits> length = CSVG::decodeLengthValue(value.getValue(""));

    return length.getValue(CScreenUnits());
  }
  else
    return length_;
}

CJValueP
CSVGJLength::
execNameFn(CJavaScript *, const std::string &, const Values &)
{
  return CJValueP();
}

void
CSVGJLength::
print(std::ostream &os) const
{
  os << "SVGLength: " << getLength();
}

//------

CJObjTypeP CSVGJStringListType::type_;

CJObjTypeP
CSVGJStringListType::
instance(CJavaScript *js)
{
  if (! type_) {
    type_ = CJObjTypeP(new CSVGJStringListType(js));

    js->addObjectType("SVGStringList", type_);
  }

  return type_;
}

CSVGJStringListType::
CSVGJStringListType(CJavaScript *js) :
 CJObjType(js, CJToken::Type::Object, "SVGStringList")
{
}

//---

/*
  interface SVGStringList {
    readonly attribute unsigned long numberOfItems;

    void clear() raises(DOMException);
    DOMString initialize(in DOMString newItem) raises(DOMException);
    DOMString getItem(in unsigned long index) raises(DOMException);
    DOMString insertItemBefore(in DOMString newItem, in unsigned long index) raises(DOMException);
    DOMString replaceItem(in DOMString newItem, in unsigned long index) raises(DOMException);
    DOMString removeItem(in unsigned long index) raises(DOMException);
    DOMString appendItem(in DOMString newItem) raises(DOMException);
  };
*/
CSVGJStringList::
CSVGJStringList(CSVG *svg, const Strings &strings) :
 CJObj(svg->js(), CSVGJStringListType::instance(svg->js())), svg_(svg), strings_(strings)
{
}

int
CSVGJStringList::
cmp(CJObjP obj) const
{
  CSVGJStringListP strings;

  if (obj->type()->name() == "SVGStringList")
    strings = std::static_pointer_cast<CSVGJStringList>(obj);
  else
    return CJObj::cmp(obj);

  return this->getStrings() == strings->getStrings();
}

CJValueP
CSVGJStringList::
getProperty(CJavaScript *js, const std::string &key) const
{
  if (key == "numberOfItems") {
    return js->createNumberValue(long(strings_.size()));
  }
  else {
    return CJObj::getProperty(js, key);
  }
}

void
CSVGJStringList::
setProperty(CJavaScript *js, const std::string &key, CJValueP value)
{
  CJObj::setProperty(js, key, value);
}

const CSVGJStringList::Strings &
CSVGJStringList::
getStrings() const
{
  return strings_;
}

void
CSVGJStringList::
setStrings(const Strings &strings)
{
  strings_ = strings;
}

CJValueP
CSVGJStringList::
execNameFn(CJavaScript *, const std::string &, const Values &)
{
  return CJValueP();
}

void
CSVGJStringList::
print(std::ostream &os) const
{
  const Strings &strings = getStrings();

  os << "SVGStringList: [ ";

  for (uint i = 0; i < strings.size(); ++i) {
    if (i > 0)
      os << ", ";

    os << strings[i];
  }

  os << " ]";
}

//------

CJObjTypeP CSVGJAngleType::type_;

CJObjTypeP
CSVGJAngleType::
instance(CJavaScript *js)
{
  if (! type_) {
    type_ = CJObjTypeP(new CSVGJAngleType(js));

    js->addObjectType("SVGAngle", type_);
  }

  return type_;
}

CSVGJAngleType::
CSVGJAngleType(CJavaScript *js) :
 CJObjType(js, CJToken::Type::Object, "SVGAngle")
{
}

//---

/*
  interface SVGAngle {
    // Angle Unit Types
    const unsigned short SVG_ANGLETYPE_UNKNOWN = 0;
    const unsigned short SVG_ANGLETYPE_UNSPECIFIED = 1;
    const unsigned short SVG_ANGLETYPE_DEG = 2;
    const unsigned short SVG_ANGLETYPE_RAD = 3;
    const unsigned short SVG_ANGLETYPE_GRAD = 4;

    readonly attribute unsigned short unitType;
             attribute float value setraises(DOMException);
             attribute float valueInSpecifiedUnits setraises(DOMException);
             attribute DOMString valueAsString setraises(DOMException);

    void newValueSpecifiedUnits(in unsigned short unitType, in float valueInSpecifiedUnits)
          raises(DOMException);
    void convertToSpecifiedUnits(in unsigned short unitType) raises(DOMException);
  };
*/
CSVGJAngle::
CSVGJAngle(CSVG *svg, const CAngle &angle) :
 CJObj(svg->js(), CSVGJAngleType::instance(svg->js())), svg_(svg), angle_(angle)
{
}

CSVGJAngle::
CSVGJAngle(CSVG *svg, CSVGObject *obj, const std::string &name) :
 CJObj(svg->js(), CSVGJAngleType::instance(svg->js())), svg_(svg), obj_(obj), name_(name)
{
}

int
CSVGJAngle::
cmp(CJObjP obj) const
{
  CSVGJAngleP angle;

  if (obj->type()->name() == "SVGAngle")
    angle = std::static_pointer_cast<CSVGJAngle>(obj);
  else
    return CJObj::cmp(obj);

  return this->getAngle() == angle->getAngle();
}

CJValueP
CSVGJAngle::
getProperty(CJavaScript *js, const std::string &key) const
{
  CAngle angle = getAngle();

  if      (key == "unitType") {
    return js->createNumberValue(long(typeValue(angle)));
  }
  else if (key == "value") {
    return js->createNumberValue(angle.value());
  }
  else if (key == "valueInSpecifiedUnits") {
    return js->createNumberValue(long(angle.value()));
  }
  else if (key == "valueAsString") {
    return js->createStringValue(angle.toString());
  }
  else {
    return CJObj::getProperty(js, key);
  }
}

void
CSVGJAngle::
setProperty(CJavaScript *js, const std::string &key, CJValueP value)
{
  if (key == "valueAsString") {
    if (obj_) {
      if (! obj_->processOption(name_, value->toString()))
        js->throwException(CJExceptionType::SYNTAX_ERR);
    }
    else {
      COptValT<CAngle> angle = CSVG::decodeAngleString(value->toString());

      if (angle.isValid())
        angle_ = angle.getValue();
      else
        js->throwException(CJExceptionType::SYNTAX_ERR);
    }
  }
  else
    CJObj::setProperty(js, key, value);
}

CAngle
CSVGJAngle::
getAngle() const
{
  if (obj_) {
    COptString value = obj_->getNameValue(name_);

    COptValT<CAngle> angle = CSVG::decodeAngleString(value.getValue(""));

    return angle.getValue(CAngle());
  }
  else
    return angle_;
}

CJValueP
CSVGJAngle::
execNameFn(CJavaScript *, const std::string &, const Values &)
{
  return CJValueP();
}

void
CSVGJAngle::
print(std::ostream &os) const
{
  os << "SVGAngle: " << getAngle();
}

//------

CJObjTypeP CSVGJColorType::type_;

CJObjTypeP
CSVGJColorType::
instance(CJavaScript *js)
{
  if (! type_) {
    type_ = CJObjTypeP(new CSVGJColorType(js));

    js->addObjectType("SVGColor", type_);
  }

  return type_;
}

CSVGJColorType::
CSVGJColorType(CJavaScript *js) :
 CJObjType(js, CJToken::Type::Object, "SVGColor")
{
}

//---

/*
  interface SVGColor : CSSValue {
    // Color Types
    const unsigned short SVG_COLORTYPE_UNKNOWN = 0;
    const unsigned short SVG_COLORTYPE_RGBCOLOR = 1;
    const unsigned short SVG_COLORTYPE_RGBCOLOR_ICCCOLOR = 2;
    const unsigned short SVG_COLORTYPE_CURRENTCOLOR = 3;

    readonly attribute unsigned short colorType;
    readonly attribute RGBColor rgbColor;
    readonly attribute SVGICCColor iccColor;

    void setRGBColor(in DOMString rgbColor) raises(SVGException);
    void setRGBColorICCColor(in DOMString rgbColor, in DOMString iccColor) raises(SVGException);
    void setColor(in unsigned short colorType, in DOMString rgbColor, in DOMString iccColor)
     raises(SVGException);
  }
*/
CSVGJColor::
CSVGJColor(CSVG *svg, const CSVGColor &color) :
 CJObj(svg->js(), CSVGJColorType::instance(svg->js())), svg_(svg), color_(color)
{
}

CSVGJColor::
CSVGJColor(CSVG *svg, CSVGObject *obj, const std::string &name) :
 CJObj(svg->js(), CSVGJColorType::instance(svg->js())), svg_(svg), obj_(obj), name_(name)
{
}

int
CSVGJColor::
cmp(CJObjP obj) const
{
  CSVGJColorP color;

  if (obj->type()->name() == "SVGColor")
    color = std::static_pointer_cast<CSVGJColor>(obj);
  else
    return CJObj::cmp(obj);

  return this->getColor() == color->getColor();
}

CJValueP
CSVGJColor::
getProperty(CJavaScript *js, const std::string &key) const
{
  return CJObj::getProperty(js, key);
}

void
CSVGJColor::
setProperty(CJavaScript *js, const std::string &key, CJValueP value)
{
  CJObj::setProperty(js, key, value);
}

CSVGColor
CSVGJColor::
getColor() const
{
#if 0
  if (obj_) {
    COptString value = obj_->getNameValue(name_);

    COptValT<CColor> color = CSVG::decodeColorString(value.getValue(""));

    return color.getValue(CColor());
  }
  else
#endif
    return color_;
}

CJValueP
CSVGJColor::
execNameFn(CJavaScript *, const std::string &, const Values &)
{
  return CJValueP();
}

void
CSVGJColor::
print(std::ostream &os) const
{
  os << "SVGColor: " << getColor();
}

//------

/*
  interface SVGICCColor {
    attribute DOMString colorProfile setraises(DOMException);
    readonly attribute SVGNumberList colors;
  };
*/

//------

/*
  interface SVGUnitTypes
    // Unit Types
    const unsigned short SVG_UNIT_TYPE_UNKNOWN = 0;
    const unsigned short SVG_UNIT_TYPE_USERSPACEONUSE = 1;
    const unsigned short SVG_UNIT_TYPE_OBJECTBOUNDINGBOX = 2;
  };
*/

//------

/*
  interface SVGStylable {
    readonly attribute SVGAnimatedString className;
    readonly attribute CSSStyleDeclaration style;
    CSSValue getPresentationAttribute(in DOMString name);
  };
*/

//------

/*
  interface SVGLocatable {
    readonly attribute SVGElement nearestViewportElement;
    readonly attribute SVGElement farthestViewportElement;

    SVGRect getBBox();
    SVGMatrix getCTM();
    SVGMatrix getScreenCTM();
    SVGMatrix getTransformToElement(in SVGElement element) raises(SVGException);
  }
*/

//------

/*
  interface SVGTransformable : SVGLocatable {
    readonly attribute SVGAnimatedTransformList transform;
  };
*/

//------

/*
  interface SVGTests {
    readonly attribute SVGStringList requiredFeatures;
    readonly attribute SVGStringList requiredExtensions;
    readonly attribute SVGStringList systemLanguage;
    boolean hasExtension(in DOMString extension);
  };
*/

//------

/*
  interface SVGLangSpace {
    attribute DOMString xmllang setraises(DOMException);
    attribute DOMString xmlspace setraises(DOMException);
  };
*/

//------

/*
  interface SVGExternalResourcesRequired {
    readonly attribute SVGAnimatedBoolean externalResourcesRequired;
  };
*/

//------

/*
  interface SVGFitToViewBox {
    readonly attribute SVGAnimatedRect viewBox;
    readonly attribute SVGAnimatedPreserveAspectRatio preserveAspectRatio;
  };
*/

//------

/*
  interface SVGFitToViewBox {
    readonly attribute SVGAnimatedRect viewBox;
    readonly attribute SVGAnimatedPreserveAspectRatio preserveAspectRatio;
  };
*/

//------

/*
  interface SVGZoomAndPan {
    // Zoom and Pan Types
    const unsigned short SVG_ZOOMANDPAN_UNKNOWN = 0;
    const unsigned short SVG_ZOOMANDPAN_DISABLE = 1;
    const unsigned short SVG_ZOOMANDPAN_MAGNIFY = 2;

    attribute unsigned short zoomAndPan setraises(DOMException);
  };
*/

//------

/*
  interface SVGViewSpec : SVGZoomAndPan, SVGFitToViewBox {
    readonly attribute SVGTransformList transform;
    readonly attribute SVGElement viewTarget;
    readonly attribute DOMString viewBoxString;
    readonly attribute DOMString preserveAspectRatioString;
    readonly attribute DOMString transformString;
    readonly attribute DOMString viewTargetString;
  };
*/

//------

/*
  interface SVGURIReference {
    readonly attribute SVGAnimatedString href;
  };
*/

//------

/*
  interface SVGCSSRule : CSSRule {
    const unsigned short COLOR_PROFILE_RULE = 7;
  };
*/

//------

/*
  interface SVGRenderingIntent {
    // Rendering Intent Types
    const unsigned short RENDERING_INTENT_UNKNOWN = 0;
    const unsigned short RENDERING_INTENT_AUTO = 1;
    const unsigned short RENDERING_INTENT_PERCEPTUAL = 2;
    const unsigned short RENDERING_INTENT_RELATIVE_COLORIMETRIC = 3;
    const unsigned short RENDERING_INTENT_SATURATION = 4;
    const unsigned short RENDERING_INTENT_ABSOLUTE_COLORIMETRIC = 5;
  };
*/
