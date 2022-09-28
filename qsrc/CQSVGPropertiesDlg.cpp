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

  auto *layout = new QVBoxLayout(this);
  layout->setMargin(0); layout->setSpacing(2);

  auto *tab = new QTabWidget;

  layout->addWidget(tab);

  //---

  auto *propertiesFrame = new QFrame;

  auto *propertiesLayout = new QVBoxLayout(propertiesFrame);
  propertiesLayout->setMargin(0); propertiesLayout->setSpacing(2);

  propertiesTree_ = new CQPropertyTree;

  connect(propertiesTree_, SIGNAL(valueChanged(QObject *, const QString &)),
          window, SLOT(redraw()));

  connect(propertiesTree_, SIGNAL(itemSelected(QObject *, const QString &)),
          window, SLOT(itemSelectedSlot(QObject *, const QString &)));

  propertiesLayout->addWidget(propertiesTree_);

  //---

  auto *cssFrame = new QFrame;

  auto *cssLayout = new QVBoxLayout(cssFrame);
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

  int w = fm.horizontalAdvance("X")*50;
  int h = fm.height()*25;

  return QSize(w, h);
}
