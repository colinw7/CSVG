#include <CQSVGPropertiesDlg.h>
#include <CQSVGWindow.h>
#include <CQPropertyTree.h>
#include <QVBoxLayout>

CQSVGPropertiesDlg::
CQSVGPropertiesDlg(CQSVGWindow *window) :
 window_(window)
{
  setWindowTitle("Properties");

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setMargin(0); layout->setSpacing(2);

  tree_ = new CQPropertyTree;

  connect(tree_, SIGNAL(valueChanged(QObject *, const QString &)),
          window, SLOT(redraw()));

  connect(tree_, SIGNAL(itemSelected(QObject *, const QString &)),
          window, SLOT(itemSelectedSlot(QObject *, const QString &)));

  layout->addWidget(tree_);
}

QSize
CQSVGPropertiesDlg::
sizeHint() const
{
  QFontMetrics fm(font());

  int w = fm.width("X")*50;
  int h = fm.height()  *25;

  return QSize(w, h);
}
