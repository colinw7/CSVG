#include <CQSVGBufferView.h>
#include <CQSVG.h>
#include <CQSVGRenderer.h>
#include <CSVGBuffer.h>
#include <CQSVGUtil.h>
#include <CQImage.h>
#include <CQUtil.h>

#include <QVBoxLayout>
#include <QComboBox>
#include <QCheckBox>
#include <QLabel>
#include <QPainter>
#include <QMouseEvent>

CQSVGBufferView::
CQSVGBufferView(CSVG *svg) :
 svg_(svg)
{
  setObjectName("bufferView");

  setWindowTitle("Buffer View");

  auto *layout = CQUtil::makeLayout<QVBoxLayout>(this, 0, 2);

  //---

  auto *controlFrame = CQUtil::makeWidget<QFrame>("control");

  controlFrame->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

  layout->addWidget(controlFrame);

  auto *controlLayout = CQUtil::makeLayout<QHBoxLayout>(controlFrame, 0, 2);

  bgCheck_ = CQUtil::makeLabelWidget<QCheckBox>("Checkerboard", "checkerboard");

  connect(bgCheck_, SIGNAL(stateChanged(int)), this, SLOT(updateBuffer()));

  controlLayout->addWidget(bgCheck_);

  combo_ = CQUtil::makeWidget<QComboBox>("combo");

  connect(combo_, SIGNAL(currentIndexChanged(int)), this, SLOT(updateBuffer()));

  controlLayout->addWidget(combo_);

  //---

  canvas_ = new CQSVGBufferCanvas(this);

  layout->addWidget(canvas_);

  //---

  auto *status = new QFrame;
  status->setObjectName("status");

  status->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

  layout->addWidget(status);

  auto *statusLayout = new QHBoxLayout(status);
  statusLayout->setMargin(0); statusLayout->setSpacing(2);

  statusLabel_ = new QLabel;
  statusLabel_->setObjectName("statusLabel");

  statusLayout->addWidget(statusLabel_);

  statusLayout->addStretch();

  posLabel_ = new QLabel;
  posLabel_->setObjectName("posLabel");

  statusLayout->addWidget(posLabel_);

  //---

  updateState();

  updateBuffer();
}

void
CQSVGBufferView::
updateState()
{
  combo_->clear();

  std::vector<std::string> names;

  svg_->getBufferNames(names);

  for (const auto &n : names)
    combo_->addItem(n.c_str());
}

void
CQSVGBufferView::
updateBuffer()
{
  QString text;

  auto *buffer = svg_->getBuffer(bufferName().toStdString());

  if (buffer) {
    text += QString("Opacity: %1").arg(buffer->opacity());

    if (buffer->isAlpha())
      text += ", Alpha";

    if (buffer->isClip())
      text += ", Clip";
  }

  statusLabel_->setText(text);

  canvas_->update();
}

QString
CQSVGBufferView::
bufferName() const
{
  return combo_->currentText();
}

QImage
CQSVGBufferView::
bufferImage() const
{
  auto *buffer = svg_->getBuffer(bufferName().toStdString());

  auto *renderer = dynamic_cast<CQSVGRenderer *>(buffer->getRenderer());
  if (! renderer) return QImage();

  return renderer->qimage();
}

CBBox2D
CQSVGBufferView::
bufferBBox() const
{
  return svg_->getBuffer(bufferName().toStdString())->bbox();
}

CPoint2D
CQSVGBufferView::
bufferOrigin() const
{
  return svg_->getBuffer(bufferName().toStdString())->origin();
}

void
CQSVGBufferView::
showPos(const QPoint &ppos)
{
  auto ptext = QString("%1,%2").arg(ppos.x()).arg(ppos.y());

  posLabel_->setText(QString("(%1)").arg(ptext));
}

bool
CQSVGBufferView::
isCheckerboard() const
{
  return bgCheck_->isChecked();
}

//---

CQSVGBufferCanvas::
CQSVGBufferCanvas(CQSVGBufferView *view) :
 view_(view)
{
  setFocusPolicy(Qt::StrongFocus);

  setMouseTracking(true);
}

void
CQSVGBufferCanvas::
paintEvent(QPaintEvent *)
{
  QPainter painter(this);

  if (view_->isCheckerboard())
    CQSVGUtil::drawCheckerboard(&painter, 32);
  else
    painter.fillRect(rect(), Qt::white);

  auto image  = view_->bufferImage ();
  auto origin = view_->bufferOrigin();

  if (! image.isNull())
    painter.drawImage(QPointF(origin.x, origin.y), image);

  auto bbox = view_->bufferBBox();

  if (bbox.isSet()) {
    // TODO: transform
    painter.setPen(Qt::red);

    painter.drawRect(CQUtil::toQRect(bbox));
  }
}

void
CQSVGBufferCanvas::
mouseMoveEvent(QMouseEvent *e)
{
  view_->showPos(e->pos());
}

QSize
CQSVGBufferCanvas::
sizeHint() const
{
  QFontMetrics fm(font());

  int w = fm.horizontalAdvance("X")*20;
  int h = fm.height()*20;

  int s = std::max(w, h);

  return QSize(s, s);
}
