#ifndef CQSVGStyleTree_H
#define CQSVGStyleTree_H

#include <CSVGCSSData.h>
#include <QTreeWidget>

class CQSVG;
class QTreeWidgetItem;

class CQSVGStyleTree : public QTreeWidget {
  Q_OBJECT

 public:
  CQSVGStyleTree(CQSVG *svg);

  void load();

 private:
  void addStyleData(QTreeWidgetItem *item, const CSVGCSSData::TypeStyleDataMap &styleData);
  void addStyleData(QTreeWidgetItem *item, const CSVGCSSData::StyleDataMap &styleData);
  void addStyleData(QTreeWidgetItem *item, const CSVGStyleData &styleData);

 private:
  CQSVG *svg_;
};

#endif
