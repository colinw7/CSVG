#ifndef CQSVGAnimate_H
#define CQSVGAnimate_H

#include <CQSVGObject.h>
#include <CSVGAnimate.h>

class CQSVG;

class CQSVGAnimate : public CQSVGObject, public CSVGAnimate {
  Q_OBJECT

  Q_PROPERTY(QString attributeName READ attributeName WRITE setAttributeName)
  Q_PROPERTY(QString attributeType READ attributeType WRITE setAttributeType)
  Q_PROPERTY(QString from          READ from          WRITE setFrom         )
  Q_PROPERTY(QString to            READ to            WRITE setTo           )
  Q_PROPERTY(QString fill          READ fill          WRITE setFill         )
  Q_PROPERTY(QString begin         READ begin)
  Q_PROPERTY(QString end           READ end)
  Q_PROPERTY(QString dur           READ dur)

 public:
  CQSVGAnimate(CQSVG *svg);

  QString attributeName() const;
  void setAttributeName(const QString &s);

  QString attributeType() const;
  void setAttributeType(const QString &s);

  QString from() const;
  void setFrom(const QString &s);

  QString to() const;
  void setTo(const QString &s);

  QString begin() const;
  //void setBegin(const QString &s);

  QString end() const;

  QString dur() const;
  //void setDur(const QString &s);

  QString fill() const;
  void setFill(const QString &s);
};

#endif
