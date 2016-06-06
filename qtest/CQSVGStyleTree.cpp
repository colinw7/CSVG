#include <CQSVGStyleTree.h>
#include <CQSVG.h>

CQSVGStyleTree::
CQSVGStyleTree(CQSVG *svg) :
 svg_(svg)
{
}

void
CQSVGStyleTree::
load()
{
  clear();

  setHeaderLabels(QStringList() << "Name" << "Value");

  setColumnCount(2);

  const CSVGCSSData &cssData = svg_->getCSSData();

  //---

  QTreeWidgetItem *globalItem = new QTreeWidgetItem(QStringList() << "Global");

  addStyleData(globalItem, cssData.globalStyle());

  addTopLevelItem(globalItem);

  //---

  QTreeWidgetItem *nameItem = new QTreeWidgetItem(QStringList() << "Name");

  addStyleData(nameItem, cssData.nameStyle());

  addTopLevelItem(nameItem);

  //---

  QTreeWidgetItem *typeItem = new QTreeWidgetItem(QStringList() << "Type");

  addStyleData(typeItem, cssData.typeStyle());

  addTopLevelItem(typeItem);

  //---

  QTreeWidgetItem *classItem = new QTreeWidgetItem(QStringList() << "Class");

  addStyleData(classItem, cssData.classStyle());

  addTopLevelItem(classItem);

  //---

  QTreeWidgetItem *typeClassItem = new QTreeWidgetItem(QStringList() << "Type/Class");

  addStyleData(typeClassItem, cssData.typeClassStyle());

  addTopLevelItem(typeClassItem);
}

void
CQSVGStyleTree::
addStyleData(QTreeWidgetItem *item, const CSVGCSSData::TypeStyleDataMap &styleData)
{
  for (const auto &s : styleData) {
    QTreeWidgetItem *childItem = new QTreeWidgetItem(QStringList() << s.first.c_str());

    addStyleData(childItem, s.second);

    item->addChild(childItem);
  }
}

void
CQSVGStyleTree::
addStyleData(QTreeWidgetItem *item, const CSVGCSSData::StyleDataMap &styleData)
{
  for (const auto &s : styleData) {
    QTreeWidgetItem *childItem = new QTreeWidgetItem(QStringList() << s.first.c_str());

    addStyleData(childItem, s.second);

    item->addChild(childItem);
  }
}

void
CQSVGStyleTree::
addStyleData(QTreeWidgetItem *item, const CSVGStyleData &styleData)
{
  for (const auto &s : styleData.nameValues()) {
    QStringList strs = QStringList() << s.first.c_str() << s.second.c_str();

    QTreeWidgetItem *childItem = new QTreeWidgetItem(strs);

    item->addChild(childItem);
  }
}
