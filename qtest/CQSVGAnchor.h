#ifndef CQSVGAnchor_H
#define CQSVGAnchor_H

#include <CQSVGObject.h>
#include <CSVGAnchor.h>

class CQSVG;

class CQSVGAnchor : public CQSVGObject, public CSVGAnchor {
  Q_OBJECT

 public:
  CQSVGAnchor(CQSVG *svg);
};

#endif
