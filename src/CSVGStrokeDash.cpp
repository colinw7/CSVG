#include <CSVGStrokeDash.h>

CLineDash
CSVGStrokeDash::
getLineDash(double l)
{
  if (! dashes_.empty()) {
    std::vector<double> lengths;

    for (const auto &d : dashes_)
      lengths.push_back(d.pxValue(l));

    double offset = 0.0;

    if (offset_.isValid())
      offset = offset_.getValue().pxValue(l);

    return CLineDash(lengths, offset);
  }
  else
    return CLineDash();
}

void
CSVGStrokeDash::
setLineDash(const CLineDash &d)
{
  dashes_.clear();

  for (uint i = 0; i < d.getNumLengths(); ++i)
    dashes_.push_back(d.getLength(i));

  offset_ = d.getOffset();
}
