#ifndef CQSVGMPath_H
#define CQSVGMPath_H

#include <CQSVGObject.h>
#include <CSVGMPath.h>

class CQSVG;

class CQSVGMPath : public CQSVGObject, public CSVGMPath {
  Q_OBJECT

 public:
  CQSVGMPath(CQSVG *svg);
};

#endif
