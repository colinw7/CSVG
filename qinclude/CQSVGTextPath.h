#ifndef CQSVGTextPath_H
#define CQSVGTextPath_H

#include <CQSVGObject.h>
#include <CSVGTextPath.h>

class CQSVG;

class CQSVGTextPath : public CQSVGObject, public CSVGTextPath {
  Q_OBJECT

  Q_PROPERTY(QString text        READ getQText       WRITE setQText      )
  Q_PROPERTY(QString xlink       READ getXLink       WRITE setXLink      )
  Q_PROPERTY(double  startOffset READ getStartOffset WRITE setStartOffset)

 public:
  CQSVGTextPath(CQSVG *svg);

  QString getQText() const { return CSVGTextPath::getText().c_str(); }
  void setQText(const QString &s) { CSVGTextPath::setText(s.toStdString()); }

  QString getXLink() const;
  void setXLink(const QString &s);

  double getStartOffset() const;
  void setStartOffset(double r);

  void addProperties(CQPropertyTree *tree, const std::string &name) override;

  void drawTerm() override;
};

#endif
