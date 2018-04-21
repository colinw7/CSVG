#ifndef CQSVGAnimateBase_H
#define CQSVGAnimateBase_H

#include <CQSVGObject.h>

class CQSVG;
class CSVGAnimateBase;

class CQSVGAnimateBase : public CQSVGObject {
  Q_OBJECT

  Q_PROPERTY(QString attributeName READ attributeName WRITE setAttributeName)
  Q_PROPERTY(QString attributeType READ attributeType WRITE setAttributeType)
  Q_PROPERTY(QString begin         READ begin)
  Q_PROPERTY(QString end           READ end)
  Q_PROPERTY(QString dur           READ dur)
  Q_PROPERTY(QString from          READ from          WRITE setFrom         )
  Q_PROPERTY(QString to            READ to            WRITE setTo           )
  Q_PROPERTY(QString repeatCount   READ repeatCount   WRITE setRepeatCount  )
  Q_PROPERTY(QString repearDur     READ repearDur)
  Q_PROPERTY(QString fill          READ fill          WRITE setFill         )

 public:
  CQSVGAnimateBase(CQSVG *svg, CSVGAnimateBase *base);

  QString attributeName() const;
  void setAttributeName(const QString &s);

  QString attributeType() const;
  void setAttributeType(const QString &s);

  QString begin() const;
  //void setBegin(const QString &s);

  QString end() const;
  //void setEnd(const QString &s);

  QString dur() const;
  //void setDur(const QString &s);

  QString from() const;
  void setFrom(const QString &s);

  QString to() const;
  void setTo(const QString &s);

  QString repeatCount() const;
  void setRepeatCount(const QString &s);

  QString repearDur() const;
  //void setRepearDur(const QString &s);

  QString fill() const;
  void setFill(const QString &s);

 private:
  CSVGAnimateBase *base_ { 0 };
};

#endif
