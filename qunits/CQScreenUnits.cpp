#include <CQScreenUnits.h>
#include <CScreenUnits.h>

#include <QApplication>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QPainter>
#include <QFrame>
#include <QLineEdit>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QScreen>
#include <iostream>

#define ROUND(a) int((a) + 0.5)

int
main(int argc, char **argv)
{
  QApplication app(argc, argv);

  Window *window = new Window;

  window->show();

  return app.exec();
}

//------

Window::
Window() :
 QWidget(0)
{
  QHBoxLayout *layout = new QHBoxLayout(this);

  Canvas *canvas = new Canvas(this);

  layout->addWidget(canvas);

  QFrame *controlFrame = new QFrame;

  controlFrame->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);

  layout->addWidget(controlFrame);

  QVBoxLayout *controlLayout = new QVBoxLayout(controlFrame);

  dpiEdit_ = new QLineEdit;

  dpiEdit_->setText(QString("%1").arg(CScreenUnitsMgrInst->dpi()));

  connect(dpiEdit_, SIGNAL(editingFinished()), this, SLOT(dpiSlot()));

  controlLayout->addWidget(dpiEdit_);

  label_ = new QLabel;

  controlLayout->addWidget(label_);

  controlLayout->addStretch(1);
}

void
Window::
dpiSlot()
{
  bool ok;

  double r = dpiEdit_->text().toDouble(&ok);
  if (! ok) return;

  CScreenUnitsMgrInst->setDpi(r);

  update();
}

void
Window::
setLabel(const QString &s)
{
  label_->setText(s);
}

//------

Canvas::
Canvas(Window *window) :
 QWidget(window), window_(window)
{
  QFontMetrics fm(qApp->font());

  CScreenUnitsMgrInst->setEmSize(fm.height());
  CScreenUnitsMgrInst->setExSize(fm.width("x"));

  QScreen *srn = QApplication::screens().at(0);
  double dotsPerInch = srn->logicalDotsPerInch();
std::cout << dotsPerInch << std::endl;

  CScreenUnitsMgrInst->setDpi(165);
}

void
Canvas::
paintEvent(QPaintEvent *)
{
  QFontMetrics fm(font());

  QPainter p(this);

  p.fillRect(rect(), Qt::white);

  p.setPen(Qt::black);

  int tw = fm.width("x");

  int y  = 16;
  int dx = 16;
  int dy = 2*fm.height();

  typedef std::pair<CScreenUnits::Units,int> TypeSize;

  std::vector<TypeSize> typeSizes = {{
   {CScreenUnits::Units::EM     ,  1},
   {CScreenUnits::Units::EX     ,  4},
   {CScreenUnits::Units::PX     , 5*tw},
   {CScreenUnits::Units::PT     , 72},
   {CScreenUnits::Units::PC     ,  6},
   {CScreenUnits::Units::CM     ,  2},
   {CScreenUnits::Units::MM     , 20},
   {CScreenUnits::Units::IN     ,  1},
   {CScreenUnits::Units::PERCENT, 10}
  }};

  double rw = width() - 2*dx;

  for (const auto &typeSize : typeSizes) {
    CScreenUnits unit(typeSize.second, typeSize.first);

    int ni = (int) (rw/unit.px(rw).value());

    p.drawLine(dx, y, width() - dx, y);

    for (int i = 0; i <= ni; ++i) {
      int px = dx + i*unit.px(rw).value();

      p.drawLine(px, y, px, y + 8);

      QString label = QString("%1").arg(i*typeSize.second);

      int tw = fm.width(label);

      p.drawText(px - tw/2, y + 8 + fm.ascent(), label);
    }

    QString typeName = unit.typeName().c_str();

    int tw = fm.width(typeName);

    int td = fm.height()/2 - fm.descent();

    QRect rect(width() - tw - 4, y - fm.height()/2, tw, fm.height());

    p.fillRect(rect, Qt::white);

    p.drawText(width() - tw - 4, y + td, typeName);

    y += dy;
  }

  if (pressed_) {
    p.setPen(Qt::red);

    p.drawLine(pressX_, 0, pressX_, height() - 1);
  }
}

void
Canvas::
keyPressEvent(QKeyEvent *)
{
}

void
Canvas::
mousePressEvent(QMouseEvent *e)
{
  pressed_ = true;
  pressX_  = e->pos().x();

  setInfo();

  update();
}

void
Canvas::
mouseReleaseEvent(QMouseEvent *)
{
  pressed_ = false;

  update();
}

void
Canvas::
mouseMoveEvent(QMouseEvent *e)
{
  pressX_ = e->pos().x();

  setInfo();

  update();
}

void
Canvas::
setInfo()
{
  int dx = 16;

  CScreenUnits px(pressX_ - dx);

  double rv = width() - 2*dx;

  double em1      = px.em     (rv).value(CScreenUnits::Units::EM);
  double ex1      = px.ex     (rv).value(CScreenUnits::Units::EX);
  double px1      = px.px     (rv).value(CScreenUnits::Units::PX);
  double pt1      = px.pt     (rv).value(CScreenUnits::Units::PT);
  double pc1      = px.pc     (rv).value(CScreenUnits::Units::PC);
  double cm1      = px.cm     (rv).value(CScreenUnits::Units::CM);
  double mm1      = px.mm     (rv).value(CScreenUnits::Units::MM);
  double in1      = px.in     (rv).value(CScreenUnits::Units::IN);
  double percent1 = px.percent(rv).value(CScreenUnits::Units::PERCENT);

  QString info = QString("em=%1\nex=%2\npx=%3\npt=%4\npc=%5\ncm=%6\nmm=%7\nin=%8\n%=%9").
   arg(em1).arg(ex1).arg(px1).arg(pt1).arg(pc1).arg(cm1).arg(mm1).arg(in1).arg(percent1);

  window_->setLabel(info);
}

QSize
Canvas::
sizeHint() const
{
  return QSize(800, 800);
}
