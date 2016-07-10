#include <CSVGJTransform.h>
#include <CSVGJavaScript.h>
#include <CSVG.h>
#include <CSVGObject.h>

CSVGJTransformStack::
CSVGJTransformStack(CSVG *svg, const CMatrixStack2D &transformStack) :
 CJObject(svg->jsTransformStackType()), svg_(svg), transformStack_(transformStack)
{
  type_->addFunction("baseVal");
  type_->addFunction("getItem");
}

int
CSVGJTransformStack::
length() const
{
  return transformStack_.length();
}

CJValueP
CSVGJTransformStack::
indexValue(int i) const
{
  if (i >= 0 && i < length())
    return CJValueP(new CSVGJTransform(svg_, transformStack_.transform(i)));
  else
    return CJValueP();
}

CJValueP
CSVGJTransformStack::
execNameFn(CJavaScript *, const std::string &name, const Values &values)
{
  if      (name == "baseVal") {
    CMatrixStack2D stack = transformStack_;

    if (! stack.length()) {
      CMatrix2D m;

      m.setIdentity();

      stack.matrix(m);
    }

    return CJValueP(new CSVGJTransformStack(svg_, stack));
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

CSVGJTransform::
CSVGJTransform(CSVG *svg, const CMatrixStack2D::Transform &transform) :
 CJObject(svg->jsTransformType()), svg_(svg), transform_(transform)
{
  type_->addFunction("type");
  type_->addFunction("matrix");
}

CJValueP
CSVGJTransform::
execNameFn(CJavaScript *, const std::string &name, const Values &)
{
  double values[9];

  transform_.calcMatrix().getValues(values, 9);

  if      (name == "type") {
    return svg_->js()->createNumberValue(long(0));
  }
  else if (name == "matrix") {
    return CJValueP(new CSVGJMatrix(svg_, transform_.calcMatrix()));
  }

  return CJValueP();
}

//------

CSVGJMatrix::
CSVGJMatrix(CSVG *svg, const CMatrix2D &matrix) :
 CJObject(svg->jsTransformType()), svg_(svg), matrix_(matrix)
{
  type_->addFunction("a");
  type_->addFunction("b");
  type_->addFunction("c");
  type_->addFunction("d");
  type_->addFunction("e");
  type_->addFunction("f");
}

CJValueP
CSVGJMatrix::
execNameFn(CJavaScript *, const std::string &name, const Values &)
{
  double values[9];

  matrix_.getValues(values, 9);

  if      (name == "a") { return svg_->js()->createNumberValue(values[0]); }
  else if (name == "b") { return svg_->js()->createNumberValue(values[1]); }
  else if (name == "c") { return svg_->js()->createNumberValue(values[2]); }
  else if (name == "d") { return svg_->js()->createNumberValue(values[3]); }
  else if (name == "e") { return svg_->js()->createNumberValue(values[4]); }
  else if (name == "f") { return svg_->js()->createNumberValue(values[5]); }

  return CJValueP();
}
