#ifndef CQSVGObject_H
#define CQSVGObject_H

#include <QObject>
#include <QColor>
#include <CQUtil.h>
#include <CQSVGEnum.h>
#include <CLineDash.h>
#include <CFillType.h>

class CQSVG;
class CSVGObject;

class CQSVGObject : public QObject {
  Q_OBJECT

  Q_PROPERTY(QString id READ id WRITE setId)

  Q_PROPERTY(bool selected READ isSelected WRITE setSelected)
  Q_PROPERTY(bool filtered READ isFiltered WRITE setFiltered)
  Q_PROPERTY(bool clipped  READ isClipped  WRITE setClipped )
  Q_PROPERTY(bool masked   READ isMasked   WRITE setMasked  )
  Q_PROPERTY(bool visible  READ isVisible  WRITE setVisible )

  Q_PROPERTY(QString transform READ transformStr)

  Q_PROPERTY(QColor                  strokeColor   READ strokeColor   WRITE setStrokeColor  )
  Q_PROPERTY(double                  strokeOpacity READ strokeOpacity WRITE setStrokeOpacity)
  Q_PROPERTY(double                  strokeWidth   READ strokeWidth   WRITE setStrokeWidth  )
  Q_PROPERTY(CLineDash               strokeDash    READ strokeDash    WRITE setStrokeDash   )
  Q_PROPERTY(CQSVGEnum::LineCapType  strokeCap     READ strokeCap     WRITE setStrokeCap    )
  Q_PROPERTY(CQSVGEnum::LineJoinType strokeJoin    READ strokeJoin    WRITE setStrokeJoin   )

  Q_PROPERTY(bool fillIsNoColor      READ fillIsNoColor      WRITE setFillIsNoColor     )
  Q_PROPERTY(bool fillIsCurrentColor READ fillIsCurrentColor WRITE setFillIsCurrentColor)

  Q_PROPERTY(QColor              fillColor   READ fillColor   WRITE setFillColor  )
  Q_PROPERTY(double              fillOpacity READ fillOpacity WRITE setFillOpacity)
  Q_PROPERTY(CQSVGEnum::FillType fillRule    READ fillRule    WRITE setFillRule   )
  Q_PROPERTY(QString             fillUrl     READ fillUrl     WRITE setFillUrl    )

  Q_PROPERTY(QString              fontFamily READ getFontFamily WRITE setFontFamily)
//Q_PROPERTY(CQSVGEnum::FontStyle fontStyle  READ getFontStyle  WRITE setFontStyle )
  Q_PROPERTY(double               fontSize   READ getFontSize   WRITE setFontSize  )
  Q_PROPERTY(QFont                font       READ getFont       WRITE setFont      )

 public:
  CQSVGObject(CQSVG *svg, CSVGObject *obj);

  CSVGObject *object() const { return obj_; }

  QString id() const;
  void setId(const QString &id);

  bool isSelected() const { return selected_; }
  void setSelected(bool b) { selected_ = b; }

  bool isFiltered() const;
  void setFiltered(bool b);

  bool isClipped() const;
  void setClipped(bool b);

  bool isMasked() const;
  void setMasked(bool b);

  bool isVisible() const;
  void setVisible(bool b);

  QString transformStr() const;

  QColor strokeColor() const;
  void setStrokeColor(const QColor &c);

  double strokeOpacity() const;
  void setStrokeOpacity(double r);

  double strokeWidth() const;
  void setStrokeWidth(double r);

  CLineDash strokeDash() const;
  void setStrokeDash(const CLineDash &dash);

  CQSVGEnum::LineCapType strokeCap() const;
  void setStrokeCap(const CQSVGEnum::LineCapType &a);

  CQSVGEnum::LineJoinType strokeJoin() const;
  void setStrokeJoin(const CQSVGEnum::LineJoinType &a);

  bool fillIsNoColor() const;
  void setFillIsNoColor(bool b);

  bool fillIsCurrentColor() const;
  void setFillIsCurrentColor(bool b);

  QColor fillColor() const;
  void setFillColor(const QColor &c);

  double fillOpacity() const;
  void setFillOpacity(double r);

  CQSVGEnum::FillType fillRule() const;
  void setFillRule(const CQSVGEnum::FillType &r);

  QString fillUrl() const;
  void setFillUrl(const QString &str);

  QString getFontFamily() const;
  void setFontFamily(const QString &str);

  double getFontSize() const;
  void setFontSize(double s);

  QFont getFont() const;
  void setFont(QFont f);

  void drawSelected();

 protected:
  CQSVG      *qsvg_     { 0 };
  CSVGObject *obj_      { 0 };
  bool        selected_ { false };
};

#endif
