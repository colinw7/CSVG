#ifndef CQSVGFeMorphology_H
#define CQSVGFeMorphology_H

#include <CQSVGObject.h>
#include <CSVGFeMorphology.h>

class CQSVG;

class CQSVGFeMorphology : public CQSVGObject, public CSVGFeMorphology {
  Q_OBJECT

 public:
  CQSVGFeMorphology(CQSVG *svg);
};

#endif
