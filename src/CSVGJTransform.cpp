#include <CSVGJTransform.h>
#include <CSVGJavaScript.h>
#include <CSVG.h>
#include <CSVGObject.h>
#include <CSVGJavaScript.h>
#include <CJUtil.h>

CJObjTypeP CSVGJTransformStackType::type_;

CJObjTypeP
CSVGJTransformStackType::
instance(CJavaScript *js)
{
  if (! type_) {
    type_ = CJObjTypeP(new CSVGJTransformStackType(js));

    js->addObjectType("SVGTransformStack", type_);
  }

  return type_;
}

CSVGJTransformStackType::
CSVGJTransformStackType(CJavaScript *js) :
 CJObjType(js, CJToken::Type::Object, "SVGTransformStack")
{
  addObjectFunction(js, "getItem");
  addObjectFunction(js, "consolidate");
  addObjectFunction(js, "createSVGTransformFromMatrix");
}

//---

CSVGJTransformStack::
CSVGJTransformStack(CSVG *svg, CSVGObject *obj, bool baseVal) :
 CJObj(svg->js(), CSVGJTransformStackType::instance(svg->js())),
 svg_(svg), obj_(obj), baseVal_(baseVal)
{
}

CSVGJTransformStack::
CSVGJTransformStack(CSVG *svg, const CMatrixStack2D &stack) :
 CJObj(svg->js(), CSVGJTransformStackType::instance(svg->js())), svg_(svg), stack_(stack)
{
}

long
CSVGJTransformStack::
length() const
{
  return matrixStack().length();
}

CJValueP
CSVGJTransformStack::
indexValue(int ind) const
{
  if (ind >= 0 && ind < length()) {
    if (obj_)
      return CJValueP(new CSVGJTransform(svg_, obj_, baseVal_, ind));
    else {
      CSVGJMatrixP matrix(new CSVGJMatrix(svg_, stack_.transform(ind).calcMatrix()));

      return CJValueP(new CSVGJTransform(svg_, matrix));
    }
  }
  else
    return CJValueP();
}

CMatrixStack2D
CSVGJTransformStack::
matrixStack() const
{
  if (obj_) {
    CMatrixStack2D stack = obj_->getTransform();

    if (! stack.length() && baseVal_) {
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
CSVGJTransformStack::
matrix() const
{
  return matrixStack().getMatrix();
}

void
CSVGJTransformStack::
setMatrix(const CMatrix2D &m)
{
  CMatrixStack2D stack(m);

  if (obj_)
    obj_->setTransform(stack);
  else
    stack_ = stack;
}

CJValueP
CSVGJTransformStack::
getProperty(CJavaScript *js, const std::string &key) const
{
  if      (key == "baseVal") {
    if (obj_)
      return CJValueP(new CSVGJTransformStack(svg_, obj_, /*baseVal*/true));
    else
      return CJValueP(new CSVGJTransformStack(svg_, stack_));
  }
  else if (key == "matrix") {
    CSVGJTransformStack *th = const_cast<CSVGJTransformStack *>(this);

    CSVGJTransformStackP p = std::static_pointer_cast<CSVGJTransformStack>(th->shared_from_this());

    return CJValueP(new CSVGJMatrix(svg_, p));
  }
  else
    return CJObj::getProperty(js, key);
}

CJValueP
CSVGJTransformStack::
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

      return CJValueP(new CSVGJTransformStack(svg_, stack1));
    }
    else {
      return CJValueP(new CSVGJTransformStack(svg_, stack_));
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

CSVGJTransform::
CSVGJTransform(CSVG *svg, CSVGObject *obj, bool baseVal, int ind) :
 CJObj(svg->js(), CSVGJTransformType::instance(svg->js())), svg_(svg), obj_(obj),
 baseVal_(baseVal), ind_(ind)
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
      CMatrixTransformType type = transform().type();

      if (type == CMatrixTransformType::SCALE1 || type == CMatrixTransformType::SCALE2)
        type = CMatrixTransformType::SCALE1;

      if (type == CMatrixTransformType::ROTATE || type == CMatrixTransformType::ROTATE_ORIGIN)
        type = CMatrixTransformType::ROTATE;
    }

    return js->createNumberValue(long(type));
  }
  else if (key == "matrix") {
    if (! matrix_)
      return CSVGJMatrixP(new CSVGJMatrix(svg_, obj_, baseVal_, ind_));

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

    if (! stack.length() && baseVal_) {
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

CSVGJMatrix::
CSVGJMatrix(CSVG *svg, CSVGObject *obj, bool baseVal, int ind) :
 CJObj(svg->js(), CSVGJMatrixType::instance(svg->js())),
 svg_(svg), obj_(obj), baseVal_(baseVal), ind_(ind)
{
}

CSVGJMatrix::
CSVGJMatrix(CSVG *svg, const CSVGJTransformStackP &stack) :
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

    if (! stack.length() && baseVal_) {
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

  os << "[ " << CJUtil::realToString(values[0]) << ", " <<
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

CSVGJRect::
CSVGJRect(CSVG *svg, const CBBox2D &rect) :
 CJObj(svg->js(), CSVGJMatrixType::instance(svg->js())), svg_(svg), rect_(rect)
{
}

int
CSVGJRect::
cmp(CJObjP obj) const
{
  CSVGJRectP rect;

  if (obj->type()->name() == "SVGMatrix")
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

  os << "[ " << CJUtil::realToString(rect.getXMin()) << ", " <<
                CJUtil::realToString(rect.getYMin()) << ", " <<
                CJUtil::realToString(rect.getXMax()) << ", " <<
                CJUtil::realToString(rect.getYMax()) << " ]";
}
