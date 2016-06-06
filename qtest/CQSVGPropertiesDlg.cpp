#include <CQSVGPropertiesDlg.h>
#include <CQSVGWindow.h>
#include <CQPropertyTree.h>
#include <CQSVGStyleTree.h>
#include <QVBoxLayout>
#include <QTabWidget>

CQSVGPropertiesDlg::
CQSVGPropertiesDlg(CQSVGWindow *window) :
 window_(window)
{
  setWindowTitle("Properties");

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setMargin(0); layout->setSpacing(2);

  QTabWidget *tab = new QTabWidget;

  layout->addWidget(tab);

  //---

  QFrame *propertiesFrame = new QFrame;

  QVBoxLayout *propertiesLayout = new QVBoxLayout(propertiesFrame);
  propertiesLayout->setMargin(0); propertiesLayout->setSpacing(2);

  propertiesTree_ = new CQPropertyTree;

  connect(propertiesTree_, SIGNAL(valueChanged(QObject *, const QString &)),
          window, SLOT(redraw()));

  connect(propertiesTree_, SIGNAL(itemSelected(QObject *, const QString &)),
          window, SLOT(itemSelectedSlot(QObject *, const QString &)));

  propertiesLayout->addWidget(propertiesTree_);

  //---

  QFrame *cssFrame = new QFrame;

  QVBoxLayout *cssLayout = new QVBoxLayout(cssFrame);
  cssLayout->setMargin(0); cssLayout->setSpacing(2);

  cssTree_ = new CQSVGStyleTree(window->svg());

  cssLayout->addWidget(cssTree_);

  //---

  tab->addTab(propertiesFrame, "Properties");
  tab->addTab(cssFrame       , "CSS");
}

void
CQSVGPropertiesDlg::
loadCSS()
{
  cssTree()->load();
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
