#ifndef CQSVGFeTurbulence_H
#define CQSVGFeTurbulence_H

#include <CQSVGObject.h>
#include <CSVGFeTurbulence.h>

class CQSVG;

class CQSVGFeTurbulence : public CQSVGObject, public CSVGFeTurbulence {
  Q_OBJECT

  Q_PROPERTY(bool   fractalNoise READ isFractalNoise WRITE setFractalNoise)
  Q_PROPERTY(double baseFreq     READ getBaseFreq    WRITE setBaseFreq    )
  Q_PROPERTY(int    numOctaves   READ getNumOctaves  WRITE setNumOctaves  )
  Q_PROPERTY(int    seed         READ getSeed        WRITE setSeed        )

 public:
  CQSVGFeTurbulence(CQSVG *svg);
};

#endif
