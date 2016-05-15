#include <CQScreenUnits.h>
#include <CScreenUnits.h>

#include <QApplication>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QPainter>
#include <QFrame>
#include <QLineEdit>
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

  Canvas *canvas = new Canvas;

  layout->addWidget(canvas);

  QFrame *controlFrame = new QFrame;

  controlFrame->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);

  layout->addWidget(controlFrame);

  QVBoxLayout *controlLayout = new QVBoxLayout(controlFrame);

  dpiEdit_ = new QLineEdit;

  dpiEdit_->setText(QString("%1").arg(CScreenUnitsMgrInst->dpi()));

  connect(dpiEdit_, SIGNAL(editingFinished()), this, SLOT(dpiSlot()));

  controlLayout->addWidget(dpiEdit_);

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

//------

Canvas::
Canvas() :
 QWidget(0)
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

  typedef std::pair<CScreenUnits::Type,int> TypeSize;

  std::vector<TypeSize> typeSizes = {{
   {CScreenUnits::Type::EM     ,  1},
   {CScreenUnits::Type::EX     ,  4},
   {CScreenUnits::Type::PX     , 5*tw},
   {CScreenUnits::Type::PT     , 72},
   {CScreenUnits::Type::PC     ,  6},
   {CScreenUnits::Type::CM     ,  2},
   {CScreenUnits::Type::MM     , 20},
   {CScreenUnits::Type::IN     ,  1},
   {CScreenUnits::Type::PERCENT, 10}
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
}

void
Canvas::
keyPressEvent(QKeyEvent *)
{
}

void
Canvas::
mousePressEvent(QMouseEvent *)
{
}

QSize
Canvas::
sizeHint() const
{
  return QSize(800, 800);
}
