#ifndef CQSVGStop_H
#define CQSVGStop_H

#include <CQSVGObject.h>
#include <CSVGStop.h>

class CQSVG;

class CQSVGStop : public CQSVGObject, public CSVGStop {
  Q_OBJECT

  Q_PROPERTY(double offset  READ getOffset  WRITE setOffset )
  Q_PROPERTY(double opacity READ getOpacity WRITE setOpacity)

 public:
  CQSVGStop(CQSVG *svg);

  double getOffset() const;
};

#endif
