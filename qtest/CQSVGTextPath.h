#ifndef CQSVGTextPath_H
#define CQSVGTextPath_H

#include <CQSVGObject.h>
#include <CSVGTextPath.h>

class CQSVG;

class CQSVGTextPath : public CQSVGObject, public CSVGTextPath {
  Q_OBJECT

  Q_PROPERTY(QString text READ getQText WRITE setQText)

 public:
  CQSVGTextPath(CQSVG *svg);

  QString getQText() const { return CSVGTextPath::getText().c_str(); }
  void setQText(const QString &s) { CSVGTextPath::setText(s.toStdString()); }

  void drawTerm() override;
};

#endif
