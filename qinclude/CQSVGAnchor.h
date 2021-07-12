#ifndef CQSVGAnchor_H
#define CQSVGAnchor_H

#include <CQSVGObject.h>
#include <CSVGAnchor.h>

class CQSVG;

class CQSVGAnchor : public CQSVGObject, public CSVGAnchor {
  Q_OBJECT

  Q_PROPERTY(QString xlink READ getXLink WRITE setXLink)

 public:
  CQSVGAnchor(CQSVG *svg);

  QString getXLink() const;
  void setXLink(const QString &s);

  void addProperties(CQPropertyTree *tree, const std::string &name) override;
};

#endif
