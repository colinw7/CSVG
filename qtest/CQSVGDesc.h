#ifndef CQSVGDesc_H
#define CQSVGDesc_H

#include <CQSVGObject.h>
#include <CSVGDesc.h>

class CQSVG;

class CQSVGDesc : public CQSVGObject, public CSVGDesc {
  Q_OBJECT

  Q_PROPERTY(QString text READ getQText WRITE setQText)

 public:
  CQSVGDesc(CQSVG *svg);

  QString getQText() const { return CSVGDesc::getText().c_str(); }
  void setQText(const QString &s) { CSVGDesc::setText(s.toStdString()); }

  void drawTerm() override;
};

#endif
