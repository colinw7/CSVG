#include <CSVGJTransform.h>
#include <CSVGJavaScript.h>
#include <CSVG.h>
#include <CSVGObject.h>
#include <CSVGJavaScript.h>

CSVGJTransformStackType::
CSVGJTransformStackType() :
 CJObjectType(CJToken::Type::Object, "SVGTransformStack")
{
  addFunction("baseVal");
  addFunction("getItem");
}

//---

CSVGJTransformStack::
CSVGJTransformStack(CSVG *svg, CSVGObject *obj, bool baseVal) :
 CJObject(svg->js()->transformStackType()), svg_(svg), obj_(obj), baseVal_(baseVal)
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
  if (ind >= 0 && ind < length())
    return CJValueP(new CSVGJTransform(svg_, obj_, baseVal_, ind));
  else
    return CJValueP();
}

CMatrixStack2D
CSVGJTransformStack::
matrixStack() const
{
  CMatrixStack2D stack = obj_->getTransform();

  if (! stack.length() && baseVal_) {
    CMatrix2D m;

    m.setIdentity();

    stack.matrix(m);
  }

  return stack;
}

CJValueP
CSVGJTransformStack::
execNameFn(CJavaScript *, const std::string &name, const Values &values)
{
  if      (name == "baseVal") {
    return CJValueP(new CSVGJTransformStack(svg_, obj_, /*baseVal*/true));
  }
  else if (name == "getItem") {
    if (values.size() != 2)
      return CJValueP();

    long ind = values[1]->toInteger();

    return indexValue(ind);
  }
  else {
    return CJValueP();
  }
}

//------

CSVGJTransformType::
CSVGJTransformType() :
 CJObjectType(CJToken::Type::Object, "SVGTransform")
{
  addFunction("type");
  addFunction("matrix");
  addFunction("setRotate");
  addFunction("setScale");
  addFunction("setTranslate");
}

//---

CSVGJTransform::
CSVGJTransform(CSVG *svg, CSVGObject *obj, bool baseVal, int ind) :
 CJObject(svg->js()->transformType()), svg_(svg), obj_(obj), baseVal_(baseVal), ind_(ind)
{
}

CMatrixStack2D::Transform
CSVGJTransform::
transform() const
{
  CMatrixStack2D stack = obj_->getTransform();

  if (! stack.length() && baseVal_) {
    CMatrix2D m;

    m.setIdentity();

    stack.matrix(m);
  }

  return stack.transform(ind_);
}

void
CSVGJTransform::
setTransform(const CMatrixStack2D::Transform &transform)
{
  CMatrixStack2D stack = obj_->getTransform();

  stack.setTransform(ind_, transform);

  obj_->setTransform(stack);
}

CJValueP
CSVGJTransform::
execNameFn(CJavaScript *, const std::string &name, const Values &values)
{
  if      (name == "type") {
    CMatrixTransformType type = transform().type();

    if (type == CMatrixTransformType::SCALE1 || type == CMatrixTransformType::SCALE2)
      type = CMatrixTransformType::SCALE1;

    if (type == CMatrixTransformType::ROTATE || type == CMatrixTransformType::ROTATE_ORIGIN)
      type = CMatrixTransformType::ROTATE;

    return svg_->js()->createNumberValue(long(type));
  }
  else if (name == "matrix") {
    return CJValueP(new CSVGJMatrix(svg_, obj_, baseVal_, ind_));
  }
  else if (name == "setRotate") {
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

  return CJValueP();
}

//------

CSVGJMatrixType::
CSVGJMatrixType() :
 CJObjectType(CJToken::Type::Object, "SVGMatrix")
{
  addFunction("a");
  addFunction("b");
  addFunction("c");
  addFunction("d");
  addFunction("e");
  addFunction("f");
}

//---

CSVGJMatrix::
CSVGJMatrix(CSVG *svg, CSVGObject *obj, bool baseVal, int ind) :
 CJObject(svg->js()->transformType()), svg_(svg), obj_(obj), baseVal_(baseVal), ind_(ind)
{
}

CMatrix2D
CSVGJMatrix::
matrix() const
{
  CMatrixStack2D stack = obj_->getTransform();

  if (! stack.length() && baseVal_) {
    CMatrix2D m;

    m.setIdentity();

    stack.matrix(m);
  }

  return stack.transform(ind_).calcMatrix();
}

CJValueP
CSVGJMatrix::
execNameFn(CJavaScript *, const std::string &name, const Values &)
{
  double values[9];

  matrix().getValues(values, 9);

  if      (name == "a") { return svg_->js()->createNumberValue(values[0]); }
  else if (name == "b") { return svg_->js()->createNumberValue(values[3]); }
  else if (name == "c") { return svg_->js()->createNumberValue(values[1]); }
  else if (name == "d") { return svg_->js()->createNumberValue(values[4]); }
  else if (name == "e") { return svg_->js()->createNumberValue(values[2]); }
  else if (name == "f") { return svg_->js()->createNumberValue(values[5]); }

  return CJValueP();
}
