#ifndef CQSVGFeDisplacementMap_H
#define CQSVGFeDisplacementMap_H

#include <CQSVGObject.h>
#include <CSVGFeDisplacementMap.h>

class CQSVG;

class CQSVGFeDisplacementMap : public CQSVGObject, public CSVGFeDisplacementMap {
  Q_OBJECT

  Q_PROPERTY(double scale READ getScale WRITE setScale)

 public:
  CQSVGFeDisplacementMap(CQSVG *svg);
};

#endif
