#ifndef CQSVGTSpan_H
#define CQSVGTSpan_H

#include <CQSVGObject.h>
#include <CSVGTSpan.h>

class CQSVG;

class CQSVGTSpan : public CQSVGObject, public CSVGTSpan {
  Q_OBJECT

  Q_PROPERTY(double  x    READ getX     WRITE setX    )
  Q_PROPERTY(double  y    READ getY     WRITE setY    )
  Q_PROPERTY(QString text READ getQText WRITE setQText)

 public:
  CQSVGTSpan(CQSVG *svg);

  QString getQText() const { return CSVGTSpan::getText().c_str(); }
  void setQText(const QString &s) { CSVGTSpan::setText(s.toStdString()); }

  void addProperties(CQPropertyTree *tree, const std::string &name) override;

  void drawTerm() override;
};

#endif
