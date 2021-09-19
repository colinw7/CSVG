#include <CQRubberBand.h>
#include <QCommonStyle>
#include <QPainter>
#include <QStyleOption>

#define COMMON_STYLE 1

class CQRubberBandStyle : public QCommonStyle {
 public:
  CQRubberBandStyle(CQRubberBand *band) :
   band_(band), color_(255,0,0) {
  }

  CQRubberBand *band() const { return band_; }

  const QColor &color() const { return color_; }
  void setColor(const QColor &v) { color_ = v; }

 private:
  void drawControl(ControlElement element, const QStyleOption *opt, QPainter *p,
                   const QWidget *widget) const {
    if (element == CE_RubberBand) {
      const QStyleOptionRubberBand *rbOpt = qstyleoption_cast<const QStyleOptionRubberBand *>(opt);

#ifdef COMMON_STYLE
      QPixmap tiledPixmap(16, 16);
      QPainter pixmapPainter(&tiledPixmap);
      pixmapPainter.setPen(Qt::NoPen);
      pixmapPainter.setBrush(Qt::Dense4Pattern);
      pixmapPainter.setBackground(QBrush(opt->palette.base()));
      pixmapPainter.setBackgroundMode(Qt::OpaqueMode);
      pixmapPainter.drawRect(0, 0, tiledPixmap.width(), tiledPixmap.height());
      pixmapPainter.end();
      // ### workaround for borked XRENDER
      tiledPixmap = QPixmap::fromImage(tiledPixmap.toImage());

      p->save();
      QRect r = opt->rect;
      QStyleHintReturnMask mask;
      if (proxy()->styleHint(QStyle::SH_RubberBand_Mask, opt, widget, &mask))
        p->setClipRegion(mask.region);
      p->drawTiledPixmap(r.x(), r.y(), r.width(), r.height(), tiledPixmap);
      //p->setPen(opt->palette.color(QPalette::Active, QPalette::WindowText));
      p->setPen(color_);
      p->setBrush(Qt::NoBrush);
      p->drawRect(r.adjusted(0, 0, -1, -1));
      if (! rbOpt || rbOpt->shape == QRubberBand::Rectangle)
        p->drawRect(r.adjusted(3, 3, -4, -4));
      p->restore();
#endif
#ifdef MOTIF_STYLE
      QPixmap tiledPixmap(16, 16);
      QPainter pixmapPainter(&tiledPixmap);
      pixmapPainter.setPen(Qt::NoPen);
      pixmapPainter.setBrush(Qt::Dense4Pattern);
      pixmapPainter.setBackground(QBrush(opt->palette.base()));
      pixmapPainter.setBackgroundMode(Qt::OpaqueMode);
      pixmapPainter.drawRect(0, 0, tiledPixmap.width(), tiledPixmap.height());
      pixmapPainter.end();
      // ### workaround for borked XRENDER
      tiledPixmap = QPixmap::fromImage(tiledPixmap.toImage());

      p->save();
      QRect r = opt->rect;
      QStyleHintReturnMask mask;
      if (styleHint(QStyle::SH_RubberBand_Mask, opt, widget, &mask))
        p->setClipRegion(mask.region);
      p->drawTiledPixmap(r.x(), r.y(), r.width(), r.height(), tiledPixmap);
      p->restore();
#endif
#ifdef CDE_STYLE
      p->save();
      p->setClipping(false);
      QPainterPath path;
      path.addRect(opt->rect);
      path.addRect(opt->rect.adjusted(2, 2, -2, -2));
      p->fillPath(path, opt->palette.color(QPalette::Active, QPalette::Text));
      p->restore();
#endif
#ifdef WINDOWS_STYLE
      QPixmap tiledPixmap(16, 16);
      QPainter pixmapPainter(&tiledPixmap);
      pixmapPainter.setPen(Qt::NoPen);
      pixmapPainter.setBrush(Qt::Dense4Pattern);
      pixmapPainter.setBackground(Qt::white);
      pixmapPainter.setBackgroundMode(Qt::OpaqueMode);
      pixmapPainter.drawRect(0, 0, tiledPixmap.width(), tiledPixmap.height());
      pixmapPainter.end();
      tiledPixmap = QPixmap::fromImage(tiledPixmap.toImage());
      p->save();
      QRect r = opt->rect;
      QStyleHintReturnMask mask;
      if (proxy()->styleHint(QStyle::SH_RubberBand_Mask, opt, widget, &mask))
        p->setClipRegion(mask.region);
      p->drawTiledPixmap(r.x(), r.y(), r.width(), r.height(), tiledPixmap);
      p->restore();
#endif
#ifdef WINDOWS_XP_STYLE
      //qwindowsxpstyle
      QColor highlight = opt->palette.color(QPalette::Active, QPalette::Highlight);
      p->save();
      //QRect r = opt->rect;
      p->setPen(highlight.darker(120));
      QColor dimHighlight(qMin(highlight.red  ()/2 + 110, 255),
                          qMin(highlight.green()/2 + 110, 255),
                          qMin(highlight.blue ()/2 + 110, 255),
                          (widget && widget->isTopLevel())? 255 : 127);
      p->setBrush(dimHighlight);
      p->drawRect(opt->rect.adjusted(0, 0, -1, -1));
      p->restore();
#endif
    }
    else
      QCommonStyle::drawControl(element, opt, p, widget);
  }

 private:
  CQRubberBand *band_ { nullptr };
  QColor        color_;
};

//------

CQRubberBand::
CQRubberBand(QWidget *p) :
 QRubberBand(QRubberBand::Rectangle, p)
{
  style_ = new CQRubberBandStyle(this);

  setStyle(style_);
}

CQRubberBand::
CQRubberBand(QRubberBand::Shape shape, QWidget *p) :
 QRubberBand(shape, p)
{
  style_ = new CQRubberBandStyle(this);

  setStyle(style_);
}

CQRubberBand::
~CQRubberBand()
{
  delete style_;
}

const QColor &
CQRubberBand::
color() const
{
  return style_->color();
}

void
CQRubberBand::
setColor(const QColor &c)
{
  style_->setColor(c);
}

void
CQRubberBand::
setBounds(const QPoint &p1, const QPoint &p2)
{
  QRect rect(std::min(p1.x(), p2.x()), std::min(p1.y(), p2.y()),
             std::abs((p2.x() - p1.x()) + 1), std::abs((p2.y() - p1.y()) + 1));

  setGeometry(rect);
}
