#include <CSVGJTransform.h>
#include <CSVGJavaScript.h>
#include <CSVG.h>
#include <CSVGObject.h>
#include <CSVGJavaScript.h>

CJObjectTypeP CSVGJTransformStackType::type_;

CJObjectTypeP
CSVGJTransformStackType::
instance(CJavaScript *js)
{
  if (! type_) {
    type_ = CJObjectTypeP(new CSVGJTransformStackType(js));

    js->addObjectType("SVGTransformStack", type_);
  }

  return type_;
}

CSVGJTransformStackType::
CSVGJTransformStackType(CJavaScript *js) :
 CJObjectType(CJToken::Type::Object, "SVGTransformStack")
{
  addFunction(js, "getItem");
  addFunction(js, "consolidate");
  addFunction(js, "createSVGTransformFromMatrix");
}

//---

CSVGJTransformStack::
CSVGJTransformStack(CSVG *svg, CSVGObject *obj, bool baseVal) :
 CJObject(CSVGJTransformStackType::instance(svg->js())), svg_(svg), obj_(obj), baseVal_(baseVal)
{
}

CSVGJTransformStack::
CSVGJTransformStack(CSVG *svg, const CMatrixStack2D &stack) :
 CJObject(CSVGJTransformStackType::instance(svg->js())), svg_(svg), stack_(stack)
{
}

int
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

CJValueP
CSVGJTransformStack::
getProperty(const std::string &key) const
{
  if      (key == "baseVal") {
    if (obj_)
      return CJValueP(new CSVGJTransformStack(svg_, obj_, /*baseVal*/true));
    else
      return CJValueP(new CSVGJTransformStack(svg_, stack_));
  }
  else if (key == "matrix") {
    CMatrixStack2D stack;

    if (obj_)
      stack = obj_->getTransform();
    else
      stack = stack_;

    CMatrix2D m = stack.getMatrix();

    return CJValueP(new CSVGJMatrix(svg_, m));
  }
  else
    return CJObject::getProperty(key);
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
        CJObjectP matrixObj = std::static_pointer_cast<CJObject>(matrixVal);

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

CJObjectTypeP CSVGJTransformType::type_;

CJObjectTypeP
CSVGJTransformType::
instance(CJavaScript *js)
{
  if (! type_) {
    type_ = CJObjectTypeP(new CSVGJTransformType(js));

    js->addObjectType("SVGTransform", type_);
  }

  return type_;
}

CSVGJTransformType::
CSVGJTransformType(CJavaScript *js) :
 CJObjectType(CJToken::Type::Object, "SVGTransform")
{
  addFunction(js, "setRotate");
  addFunction(js, "setScale");
  addFunction(js, "setTranslate");
  addFunction(js, "setMatrix");
}

//---

CSVGJTransform::
CSVGJTransform(CSVG *svg, CSVGObject *obj, bool baseVal, int ind) :
 CJObject(CSVGJTransformType::instance(svg->js())), svg_(svg), obj_(obj),
 baseVal_(baseVal), ind_(ind)
{
}

CSVGJTransform::
CSVGJTransform(CSVG *svg, CSVGJMatrixP matrix) :
 CJObject(CSVGJTransformType::instance(svg->js())), svg_(svg), matrix_(matrix)
{
}

CJValueP
CSVGJTransform::
getProperty(const std::string &key) const
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

    return svg_->js()->createNumberValue(long(type));
  }
  else if (key == "matrix") {
    if (! matrix_)
      return CSVGJMatrixP(new CSVGJMatrix(svg_, obj_, baseVal_, ind_));

    return std::static_pointer_cast<CJValue>(matrix_);
  }
  else {
    return CJObject::getProperty(key);
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
      CJObjectP matrixObj = std::static_pointer_cast<CJObject>(matrixVal);

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

CJObjectTypeP CSVGJMatrixType::type_;

CJObjectTypeP
CSVGJMatrixType::
instance(CJavaScript *js)
{
  if (! type_) {
    type_ = CJObjectTypeP(new CSVGJMatrixType());

    js->addObjectType("SVGMatrix", type_);
  }

  return type_;
}

CSVGJMatrixType::
CSVGJMatrixType() :
 CJObjectType(CJToken::Type::Object, "SVGMatrix")
{
}

//---

CSVGJMatrix::
CSVGJMatrix(CSVG *svg, CSVGObject *obj, bool baseVal, int ind) :
 CJObject(CSVGJMatrixType::instance(svg->js())), svg_(svg), obj_(obj), baseVal_(baseVal), ind_(ind)
{
}

CSVGJMatrix::
CSVGJMatrix(CSVG *svg, const CMatrix2D &m) :
 CJObject(CSVGJMatrixType::instance(svg->js())), svg_(svg), m_(m)
{
}

int
CSVGJMatrix::
cmp(CJObjectP obj) const
{
  CSVGJMatrixP matrix;

  if (obj->type()->name() == "SVGMatrix")
    matrix = std::static_pointer_cast<CSVGJMatrix>(obj);
  else
    return CJObject::cmp(obj);

  return this->matrix().cmp(matrix->matrix());
}

CJValueP
CSVGJMatrix::
getProperty(const std::string &key) const
{
  CJavaScript *js = svg_->js();

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
    return CJObject::getProperty(key);
  }
}

void
CSVGJMatrix::
setProperty(const std::string &key, CJValueP value)
{
  if (key == "a" || key == "b" || key == "c" || key == "d" || key == "e" || key == "f") {
    double v = value->toReal();

    double values[9];

    CMatrix2D m = matrix();

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
    CJObject::setProperty(key, value);
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
