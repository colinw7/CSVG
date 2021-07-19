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

class CQPropertyTree;

class CQSVGObject : public QObject {
  Q_OBJECT

  Q_PROPERTY(QString id READ id WRITE setId)

  Q_PROPERTY(bool selected READ isSelected WRITE setSelected)
  Q_PROPERTY(bool filtered READ isFiltered WRITE setFiltered)
  Q_PROPERTY(bool clipped  READ isClipped  WRITE setClipped )
  Q_PROPERTY(bool masked   READ isMasked   WRITE setMasked  )
  Q_PROPERTY(bool visible  READ isVisible  WRITE setVisible )

  Q_PROPERTY(QString transform READ transformStr)

  Q_PROPERTY(double opacity READ getDrawOpacity WRITE setDrawOpacity)

  Q_PROPERTY(bool strokeNoColor      READ strokeIsNoColor      WRITE setStrokeIsNoColor     )
  Q_PROPERTY(bool strokeCurrentColor READ strokeIsCurrentColor WRITE setStrokeIsCurrentColor)

  Q_PROPERTY(QColor                  strokeColor   READ strokeColor   WRITE setStrokeColor  )
  Q_PROPERTY(double                  strokeOpacity READ strokeOpacity WRITE setStrokeOpacity)
  Q_PROPERTY(double                  strokeWidth   READ strokeWidth   WRITE setStrokeWidth  )
  Q_PROPERTY(CLineDash               strokeDash    READ strokeDash    WRITE setStrokeDash   )
  Q_PROPERTY(double                  strokeOffset  READ strokeOffset  WRITE setStrokeOffset )
  Q_PROPERTY(CQSVGEnum::LineCapType  strokeCap     READ strokeCap     WRITE setStrokeCap    )
  Q_PROPERTY(CQSVGEnum::LineJoinType strokeJoin    READ strokeJoin    WRITE setStrokeJoin   )

  Q_PROPERTY(bool fillNoColor      READ fillIsNoColor      WRITE setFillIsNoColor     )
  Q_PROPERTY(bool fillCurrentColor READ fillIsCurrentColor WRITE setFillIsCurrentColor)

  Q_PROPERTY(QColor              fillColor   READ fillColor   WRITE setFillColor  )
  Q_PROPERTY(double              fillOpacity READ fillOpacity WRITE setFillOpacity)
  Q_PROPERTY(CQSVGEnum::FillType fillRule    READ fillRule    WRITE setFillRule   )
  Q_PROPERTY(QString             fillUrl     READ fillUrl     WRITE setFillUrl    )

  Q_PROPERTY(QString              fontFamily READ getFontFamily WRITE setFontFamily)
//Q_PROPERTY(CQSVGEnum::FontStyle fontStyle  READ getFontStyle  WRITE setFontStyle )
  Q_PROPERTY(double               fontSize   READ getFontSize   WRITE setFontSize  )
  Q_PROPERTY(QFont                font       READ getFont       WRITE setFont      )

 public:
  using Color = CSVGInheritValT<CSVGColor>;

 public:
  CQSVGObject(CQSVG *svg, CSVGObject *obj);

  CQSVG *qsvg() const { return qsvg_; }

  CSVGObject *object() const { return obj_; }

  //---

  //! get/set id
  QString id() const;
  void setId(const QString &id);

  //! get/set selected
  bool isSelected() const { return selected_; }
  void setSelected(bool b) { selected_ = b; }

  //! get/set filtered
  bool isFiltered() const;
  void setFiltered(bool b);

  //! get/set clipped
  bool isClipped() const;
  void setClipped(bool b);

  //! get/set masked
  bool isMasked() const;
  void setMasked(bool b);

  //! get/set visible
  bool isVisible() const;
  void setVisible(bool b);

  //---

  QString transformStr() const;

  //---

  double getDrawOpacity() const;
  void setDrawOpacity(double r);

  //---

  // stroke

  bool strokeIsNoColor() const;
  void setStrokeIsNoColor(bool b);

  bool strokeIsCurrentColor() const;
  void setStrokeIsCurrentColor(bool b);

  QColor strokeColor() const;
  void setStrokeColor(const QColor &c);

  double strokeOpacity() const;
  void setStrokeOpacity(double r);

  double strokeWidth() const;
  void setStrokeWidth(double r);

  CLineDash strokeDash() const;
  void setStrokeDash(const CLineDash &dash);

  double strokeOffset() const;
  void setStrokeOffset(double o);

  CQSVGEnum::LineCapType strokeCap() const;
  void setStrokeCap(const CQSVGEnum::LineCapType &a);

  CQSVGEnum::LineJoinType strokeJoin() const;
  void setStrokeJoin(const CQSVGEnum::LineJoinType &a);

  //---

  // fill

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

  //---

  // font
  QString getFontFamily() const;
  void setFontFamily(const QString &str);

  double getFontSize() const;
  void setFontSize(double s);

  QFont getFont() const;
  void setFont(QFont f);

  //---

  virtual void addProperties(CQPropertyTree *tree, const std::string &name);

  void drawSelected();

 protected:
  CQSVG      *qsvg_     { nullptr };
  CSVGObject *obj_      { nullptr };
  bool        selected_ { false };
};

#endif
