#include <CQSVGBufferView.h>
#include <CQSVG.h>
#include <CSVGBuffer.h>
#include <CQSVGUtil.h>
#include <CQImage.h>
#include <CQUtil.h>

#include <QVBoxLayout>
#include <QComboBox>
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

  combo_ = new QComboBox;

  layout->addWidget(combo_);

  canvas_ = new CQSVGBufferCanvas(this);

  layout->addWidget(canvas_);

  connect(combo_, SIGNAL(currentIndexChanged(int)), canvas_, SLOT(update()));

  QFrame *status = new QFrame;
  status->setObjectName("status");

  layout->addWidget(status);

  QHBoxLayout *statusLayout = new QHBoxLayout(status);
  statusLayout->setMargin(0); statusLayout->setSpacing(2);

  statusLayout->addStretch();

  posLabel_ = new QLabel;
  posLabel_->setObjectName("posLabel");

  statusLayout->addWidget(posLabel_);

  //---

  updateState();
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

QString
CQSVGBufferView::
bufferName() const
{
  return combo_->currentText();
}

CImagePtr
CQSVGBufferView::
bufferImage() const
{
  CSVGBuffer *buffer = qsvg_->getBuffer(bufferName().toStdString());

  return buffer->getImage();
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

  if (checked_) {
    CQSVGUtil::drawCheckerboard(&painter, 32);
#if 0
    int cs = 32;
    int nc = (width () + cs - 1)/cs;
    int nr = (height() + cs - 1)/cs;

    for (int r = 0; r < nr; ++r) {
      int y = r*cs;

      for (int c = 0; c < nc; ++c) {
        int x = c*cs;

        if ((r + c) & 1)
          painter.fillRect(QRect(x, y, cs, cs), QColor(200,200,200));
        else
          painter.fillRect(QRect(x, y, cs, cs), Qt::white);
      }
    }
#endif
  }
  else
    painter.fillRect(rect(), Qt::white);

  CImagePtr image  = view_->bufferImage ();
  CPoint2D  origin = view_->bufferOrigin();

  if (image.isValid())
    painter.drawImage(QPointF(origin.x, origin.y), image.cast<CQImage>()->getQImage());

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
