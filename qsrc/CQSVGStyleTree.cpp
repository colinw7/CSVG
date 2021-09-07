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

#if 0
  const CSVGCSSData &cssData = svg_->getCSSData();

  //---

  auto *globalItem = new QTreeWidgetItem(QStringList() << "Global");

  addStyleData(globalItem, cssData.globalStyle());

  addTopLevelItem(globalItem);

  //---

  auto *nameItem = new QTreeWidgetItem(QStringList() << "Name");

  addStyleData(nameItem, cssData.nameStyle());

  addTopLevelItem(nameItem);

  //---

  auto *typeItem = new QTreeWidgetItem(QStringList() << "Type");

  addStyleData(typeItem, cssData.typeStyle());

  addTopLevelItem(typeItem);

  //---

  auto *classItem = new QTreeWidgetItem(QStringList() << "Class");

  addStyleData(classItem, cssData.classStyle());

  addTopLevelItem(classItem);

  //---

  auto *typeClassItem = new QTreeWidgetItem(QStringList() << "Type/Class");

  addStyleData(typeClassItem, cssData.typeClassStyle());

  addTopLevelItem(typeClassItem);
#endif
}

#if 0
void
CQSVGStyleTree::
addStyleData(QTreeWidgetItem *item, const CSVGCSSData::TypeStyleDataMap &styleData)
{
  for (const auto &s : styleData) {
    auto *childItem = new QTreeWidgetItem(QStringList() << s.first.c_str());

    addStyleData(childItem, s.second);

    item->addChild(childItem);
  }
}

void
CQSVGStyleTree::
addStyleData(QTreeWidgetItem *item, const CSVGCSSData::StyleDataMap &styleData)
{
  for (const auto &s : styleData) {
    auto *childItem = new QTreeWidgetItem(QStringList() << s.first.c_str());

    addStyleData(childItem, s.second);

    item->addChild(childItem);
  }
}

void
CQSVGStyleTree::
addStyleData(QTreeWidgetItem *item, const CSVGStyleData &styleData)
{
  for (const auto &s : styleData.nameValues()) {
    auto strs = QStringList() << s.first.c_str() << s.second.c_str();

    auto *childItem = new QTreeWidgetItem(strs);

    item->addChild(childItem);
  }
}
#endif
