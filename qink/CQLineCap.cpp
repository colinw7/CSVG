#include <CQLineCap.h>
#include <CQUtil.h>
#include <CQPixmapCache.h>
#include <QAction>

#include <svg/cap_square_svg.h>
#include <svg/cap_butt_svg.h>
#include <svg/cap_round_svg.h>

CQLineCap::
CQLineCap(QWidget *parent) :
 QComboBox(parent)
{
  addItem(CQPixmapCacheInst->getIcon("CAP_BUTT"  ), "Butt"  );
  addItem(CQPixmapCacheInst->getIcon("CAP_ROUND" ), "Round" );
  addItem(CQPixmapCacheInst->getIcon("CAP_SQUARE"), "Square");

  setToolTip("Line Cap");

  connect(this, SIGNAL(currentIndexChanged(int)), this, SLOT(capChangedSlot(int)));
}

void
CQLineCap::
setLineCap(CLineCapType cap)
{
  setCurrentIndex(int(cap) - 1);
}

CLineCapType
CQLineCap::
getLineCap() const
{
  return CLineCapType(currentIndex() + 1);
}

void
CQLineCap::
capChangedSlot(int value)
{
  emit valueChanged(CLineCapType(value + 1));
}
