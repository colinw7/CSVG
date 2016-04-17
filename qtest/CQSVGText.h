#ifndef CQSVGText_H
#define CQSVGText_H

#include <CQSVGObject.h>
#include <CSVGText.h>

class CQSVG;

class CQSVGText : public CQSVGObject, public CSVGText {
  Q_OBJECT

  Q_PROPERTY(double  x    READ getX     WRITE setX    )
  Q_PROPERTY(double  y    READ getY     WRITE setY    )
  Q_PROPERTY(QString text READ getQText WRITE setQText)

 public:
  CQSVGText(CQSVG *svg);

  QString getQText() const { return CSVGText::getText().c_str(); }
  void setQText(const QString &s) { CSVGText::setText(s.toStdString()); }

  void drawTerm() override;
};

#endif
