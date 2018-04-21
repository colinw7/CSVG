#ifndef CQSVGText_H
#define CQSVGText_H

#include <CQSVGObject.h>
#include <CSVGText.h>

class CQSVG;

class CQSVGText : public CQSVGObject, public CSVGText {
  Q_OBJECT

  Q_PROPERTY(double  x            READ getX            WRITE setX           )
  Q_PROPERTY(double  y            READ getY            WRITE setY           )
  Q_PROPERTY(double  dx           READ getDX           WRITE setDX          )
  Q_PROPERTY(double  dy           READ getDY           WRITE setDY          )
  Q_PROPERTY(QString text         READ getQText        WRITE setQText       )
  Q_PROPERTY(QString rotate       READ getRotate       WRITE setRotate      )
  Q_PROPERTY(QString textLength   READ getTextLength   WRITE setTextLength  )
  Q_PROPERTY(QString lengthAdjust READ getLengthAdjust WRITE setLengthAdjust)

 public:
  CQSVGText(CQSVG *svg);

  double getX() const;
  void setX(double x);

  double getY() const;
  void setY(double x);

  double getDX() const;
  void setDX(double x);

  double getDY() const;
  void setDY(double y);

  QString getQText() const { return CSVGText::getText().c_str(); }
  void setQText(const QString &s) { CSVGText::setText(s.toStdString()); }

  QString getRotate() const;
  void setRotate(const QString &s);

  QString getTextLength() const;
  void setTextLength(const QString &s);

  QString getLengthAdjust() const;
  void setLengthAdjust(const QString &s);

  void drawTerm() override;
};

#endif
