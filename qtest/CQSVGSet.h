#ifndef CQSVGSet_H
#define CQSVGSet_H

#include <CQSVGObject.h>
#include <CSVGSet.h>

class CQSVG;

class CQSVGSet : public CQSVGObject, public CSVGSet {
  Q_OBJECT

 public:
  CQSVGSet(CQSVG *svg);
};

#endif
