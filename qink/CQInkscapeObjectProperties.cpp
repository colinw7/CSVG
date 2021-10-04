#include <CQInkscapeObjectProperties.h>

#include <CSVGObject.h>
#include <CQUtil.h>

#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>

namespace CQInkscape {

ObjectProperties::
ObjectProperties(Window *window) :
 Palette(window, "Object Properties")
{
  setObjectName("objectProperties");

  auto *layout = CQUtil::makeLayout<QVBoxLayout>(this);

  auto *editLayout = CQUtil::makeLayout<QGridLayout>();

  layout->addLayout(editLayout);

  int row = 0;

  auto makeLineEdit = [&](const QString &name, const QString &desc,
                          const char *slotName=nullptr) {
    auto *label = (name != "" ? CQUtil::makeLabelWidget<QLabel>(name, "label") : nullptr);
    auto *edit  = CQUtil::makeWidget<QLineEdit>("edit");

    if (label)
      editLayout->addWidget(label, row, 0);

    editLayout->addWidget(edit, row, 1);

    edit->setToolTip(desc);

    if (slotName)
      connect(edit, SIGNAL(textChanged(const QString &)), this, slotName);

    ++row;

    return edit;
  };

  idEdit_    = makeLineEdit("ID"   , "Id");
  labelEdit_ = makeLineEdit("Label", "Label");
  titleEdit_ = makeLineEdit("Title", "Title");

  layout->addStretch(1);

  auto *buttonBar    = CQUtil::makeWidget<QFrame>("buttonBar");
  auto *buttonLayout = CQUtil::makeLayout<QHBoxLayout>(buttonBar, 2, 2);

  layout->addWidget(buttonBar);

  auto *setButton = CQUtil::makeLabelWidget<QPushButton>("Set", "set");

  buttonLayout->addStretch();
  buttonLayout->addWidget(setButton);

  connect(setButton, SIGNAL(clicked()), this, SLOT(setSlot()));
}

void
ObjectProperties::
setObject(CSVGObject *obj)
{
  object_ = obj;

  idEdit_   ->setText(QString::fromStdString(object_ ? object_->getId   () : ""));
//labelEdit_->setText(QString::fromStdString(object_ ? object_->getLabel() : ""));

  std::string title;

  if (object_ && object_->getTitle(title))
    titleEdit_->setText(QString::fromStdString(title));
  else
    titleEdit_->setText("");
}

void
ObjectProperties::
setSlot()
{
  if (object_) {
    auto id    = idEdit_   ->text().toStdString();
    auto title = titleEdit_->text().toStdString();

    if (id != "")
      object_->setId(id);

    if (title != "")
      object_->setTitle(title);
  }
}

}
