#ifndef CQSVGTitle_H
#define CQSVGTitle_H

#include <CQSVGObject.h>
#include <CSVGTitle.h>

class CQSVG;

class CQSVGTitle : public CQSVGObject, public CSVGTitle {
  Q_OBJECT

  Q_PROPERTY(QString text READ getQText WRITE setQText)

 public:
  CQSVGTitle(CQSVG *svg);

  QString getQText() const { return CSVGTitle::getText().c_str(); }
  void setQText(const QString &s) { CSVGTitle::setText(s.toStdString()); }

  void drawTerm() override;
};

#endif
