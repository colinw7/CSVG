#include <CQSVGUtil.h>
#include <QPainter>

void
CQSVGUtil::
drawCheckerboard(QPainter *painter, int cs)
{
  int w = painter->device()->width ();
  int h = painter->device()->height();

  int nc = (w + cs - 1)/cs;
  int nr = (h + cs - 1)/cs;

  for (int r = 0; r < nr; ++r) {
    int y = r*cs;

    for (int c = 0; c < nc; ++c) {
      int x = c*cs;

      if ((r + c) & 1)
        painter->fillRect(QRect(x, y, cs, cs), QColor(200, 200, 200));
      else
        painter->fillRect(QRect(x, y, cs, cs), Qt::white);
    }
  }
}
