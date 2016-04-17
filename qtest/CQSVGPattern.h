#ifndef CQSVGPattern_H
#define CQSVGPattern_H

#include <CQSVGObject.h>
#include <CSVGPattern.h>

class CQSVG;

class CQSVGPattern : public CQSVGObject, public CSVGPattern {
  Q_OBJECT

 public:
  CQSVGPattern(CQSVG *svg);
};

#endif
