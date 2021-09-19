#include <CQLineJoin.h>
#include <CQUtil.h>
#include <CQPixmapCache.h>
#include <QAction>

#include <svg/join_mitre_svg.h>
#include <svg/join_round_svg.h>
#include <svg/join_bevel_svg.h>

CQLineJoin::
CQLineJoin(QWidget *parent) :
 QComboBox(parent)
{
  addItem(CQPixmapCacheInst->getIcon("JOIN_MITRE"), "Mitre");
  addItem(CQPixmapCacheInst->getIcon("JOIN_ROUND"), "Round");
  addItem(CQPixmapCacheInst->getIcon("JOIN_BEVEL"), "Bevel");

  setToolTip("Line Join");

  connect(this, SIGNAL(currentIndexChanged(int)), this, SLOT(joinChangedSlot(int)));
}

void
CQLineJoin::
setLineJoin(CLineJoinType join)
{
  setCurrentIndex(int(join) - 1);
}

CLineJoinType
CQLineJoin::
getLineJoin() const
{
  return CLineJoinType(currentIndex() + 1);
}

void
CQLineJoin::
joinChangedSlot(int ind)
{
  emit valueChanged(CLineJoinType(ind + 1));
}
