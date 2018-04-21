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
CQSVGBufferView(CQSVG *qsvg) :
 qsvg_(qsvg)
{
  setObjectName("bufferView");

  setWindowTitle("Buffer View");

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setMargin(0); layout->setSpacing(2);

  //---

  QFrame *controlFrame = new QFrame;
  controlFrame->setObjectName("control");

  controlFrame->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

  layout->addWidget(controlFrame);

  QHBoxLayout *controlLayout = new QHBoxLayout(controlFrame);
  controlLayout->setMargin(0); controlLayout->setSpacing(2);

  bgCheck_ = new QCheckBox("Checkerboard");

  connect(bgCheck_, SIGNAL(stateChanged(int)), this, SLOT(updateBuffer()));

  controlLayout->addWidget(bgCheck_);

  combo_ = new QComboBox;

  connect(combo_, SIGNAL(currentIndexChanged(int)), this, SLOT(updateBuffer()));

  controlLayout->addWidget(combo_);

  //---

  canvas_ = new CQSVGBufferCanvas(this);

  layout->addWidget(canvas_);

  //---

  QFrame *status = new QFrame;
  status->setObjectName("status");

  status->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

  layout->addWidget(status);

  QHBoxLayout *statusLayout = new QHBoxLayout(status);
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

  qsvg_->getBufferNames(names);

  for (const auto &n : names)
    combo_->addItem(n.c_str());
}

void
CQSVGBufferView::
updateBuffer()
{
  QString text;

  CSVGBuffer *buffer = qsvg_->getBuffer(bufferName().toStdString());

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

#if 0
CImagePtr
CQSVGBufferView::
bufferImage() const
{
  CSVGBuffer *buffer = qsvg_->getBuffer(bufferName().toStdString());

  return buffer->getRenderer()->getImage();
}
#endif

QImage
CQSVGBufferView::
bufferImage() const
{
  CSVGBuffer *buffer = qsvg_->getBuffer(bufferName().toStdString());

  CQSVGRenderer *renderer = dynamic_cast<CQSVGRenderer *>(buffer->getRenderer());
  if (! renderer) return QImage();

  return renderer->qimage();
}

CBBox2D
CQSVGBufferView::
bufferBBox() const
{
  return qsvg_->getBuffer(bufferName().toStdString())->bbox();
}

CPoint2D
CQSVGBufferView::
bufferOrigin() const
{
  return qsvg_->getBuffer(bufferName().toStdString())->origin();
}

void
CQSVGBufferView::
showPos(const QPoint &ppos)
{
  QString ptext = QString("%1,%2").arg(ppos.x()).arg(ppos.y());

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

  QImage    image  = view_->bufferImage ();
  CPoint2D  origin = view_->bufferOrigin();

  if (! image.isNull())
    painter.drawImage(QPointF(origin.x, origin.y), image);

  CBBox2D bbox = view_->bufferBBox();

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

  int w = fm.width("X")*20;
  int h = fm.height()  *20;

  int s = std::max(w, h);

  return QSize(s, s);
}
