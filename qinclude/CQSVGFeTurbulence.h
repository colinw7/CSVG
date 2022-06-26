#ifndef CQSVGFeTurbulence_H
#define CQSVGFeTurbulence_H

#include <CQSVGObject.h>
#include <CSVGFeTurbulence.h>

class CQSVG;

class CQSVGFeTurbulence : public CQSVGObject, public CSVGFeTurbulence {
  Q_OBJECT

  Q_PROPERTY(bool   fractalNoise READ isFractalNoise WRITE setFractalNoise)
  Q_PROPERTY(double baseFreqX    READ getBaseFreqX   WRITE setBaseFreqX   )
  Q_PROPERTY(double baseFreqY    READ getBaseFreqY   WRITE setBaseFreqY   )
  Q_PROPERTY(int    numOctaves   READ getNumOctaves  WRITE setNumOctaves  )
  Q_PROPERTY(double seed         READ getSeed        WRITE setSeed        )

 public:
  CQSVGFeTurbulence(CQSVG *svg);

  void addProperties(CQPropertyTree *tree, const std::string &name) override;
};

#endif
