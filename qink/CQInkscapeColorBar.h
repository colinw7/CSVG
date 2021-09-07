#ifndef CQInkscapeColorBar_H
#define CQInkscapeColorBar_H

#include <QFrame>

namespace CQInkscape {

class Window;

class ColorPalette {
 public:
  ColorPalette(const QString &name) :
   name_(name) {
  }

  void addColor(const QColor &c) {
    colors_.push_back(c);
  }

  int numColors() const { return colors_.size(); }

  const QColor &color(int i) const {
    return colors_[i];
  }

 private:
  using Colors = std::vector<QColor>;

  QString name_;
  Colors  colors_;
};

using Palettes = std::vector<ColorPalette>;

class ColorBar : public QFrame {
  Q_OBJECT

 public:
  ColorBar(Window *window);

  void mousePressEvent(QMouseEvent *e) override;

  void contextMenuEvent(QContextMenuEvent *e) override;

  void paintEvent(QPaintEvent *) override;

 private slots:
  void fillSlot();
  void strokeSlot();

 private:
  void addPalettes();

  void setFill(int ind);
  void setStroke(int ind);

  QColor paletteColor(int i) const;

 private:
  Window*  window_      { nullptr };
  Palettes palettes_;
  int      paletteNum_  { 0 };
  double   swatchWidth_ { 0.0 };
  int      menuInd_     { 0 };
};

}

#endif
