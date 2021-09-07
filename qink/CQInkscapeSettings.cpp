#include <CQInkscapeSettings.h>
#include <CQInkscapeWindow.h>
#include <CQUtil.h>

#include <QVBoxLayout>
#include <QCheckBox>
#include <QPushButton>

namespace CQInkscape {

//---

Settings::
Settings(Window *window) :
 QDialog(window), window_(window)
{
  setObjectName("settings");

  //---

  auto *layout = CQUtil::makeLayout<QVBoxLayout>(this, 2, 2);

  checkerboardCheck_ = CQUtil::makeLabelWidget<QCheckBox>("Checkerboard", "checkerboard");
  viewBoxCheck_      = CQUtil::makeLabelWidget<QCheckBox>("View Box"    , "viewBox");
  consoleCheck_      = CQUtil::makeLabelWidget<QCheckBox>("Console"     , "console");
  bufferViewCheck_   = CQUtil::makeLabelWidget<QCheckBox>("Buffer View" , "bufferView");

  layout->addWidget(checkerboardCheck_);
  layout->addWidget(viewBoxCheck_);
  layout->addWidget(consoleCheck_);
  layout->addWidget(bufferViewCheck_);

  //---

  auto *buttonBar    = CQUtil::makeWidget<QFrame>("buttonBar");
  auto *buttonLayout = CQUtil::makeLayout<QHBoxLayout>(buttonBar, 2, 2);

  layout->addWidget(buttonBar);

  auto *applyButton = CQUtil::makeLabelWidget<QPushButton>("Apply", "apply");
  auto *doneButton  = CQUtil::makeLabelWidget<QPushButton>("Done", "done");

  connect(applyButton, SIGNAL(clicked()), this, SLOT(accept()));
  connect(doneButton, SIGNAL(clicked()), this, SLOT(doneSlot()));

  buttonLayout->addWidget(applyButton);
  buttonLayout->addWidget(doneButton);
  buttonLayout->addStretch(1);
}

void
Settings::
updateState()
{
  checkerboardCheck_->setChecked(window_->isCheckerboard());
  viewBoxCheck_     ->setChecked(window_->isShowViewBox());
  consoleCheck_     ->setChecked(window_->isConsoleVisible());
  bufferViewCheck_  ->setChecked(window_->isBufferViewVisible());
}

void
Settings::
doneSlot()
{
  accept();

  close();
}

void
Settings::
accept()
{
  window_->setCheckerboard     (checkerboardCheck_->isChecked());
  window_->setShowViewBox      (viewBoxCheck_     ->isChecked());
  window_->setConsoleVisible   (consoleCheck_     ->isChecked());
  window_->setBufferViewVisible(bufferViewCheck_  ->isChecked());
}

}
