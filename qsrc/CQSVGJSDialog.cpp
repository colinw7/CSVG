#include <CQSVGJSDialog.h>
#include <CSVGJavaScript.h>
#include <CQSVGWindow.h>
#include <CQSVG.h>
#include <CQHistoryLineEdit.h>
#include <CJavaScript.h>
#include <QVBoxLayout>
#include <QTabWidget>
#include <QTextEdit>
#include <QListWidget>
#include <QPushButton>
#include <QFileDialog>
#include <QDir>

CQSVGJSDialog::
CQSVGJSDialog(CQSVGWindow *window) :
 window_(window)
{
  setWindowTitle("JavaScript");

  auto *layout = new QVBoxLayout(this);
  layout->setMargin(0); layout->setSpacing(2);

  //---

  auto *tab = new QTabWidget;

  log_ = new QTextEdit;

  log_->setReadOnly(true);

  tab->addTab(log_, "Log");

  history_ = new QListWidget;

  connect(history_, SIGNAL(itemClicked(QListWidgetItem *)),
          this, SLOT(historySlot(QListWidgetItem *)));

  tab->addTab(history_, "History");

  //----

  auto *variables = new QFrame;

  auto *variablesLayout = new QVBoxLayout(variables);
  variablesLayout->setMargin(0); variablesLayout->setSpacing(2);

  variablesList_ = new QListWidget;

  variablesLayout->addWidget(variablesList_);

  auto *variablesLoad = new QPushButton("Load");

  connect(variablesLoad, SIGNAL(clicked()), this, SLOT(loadVariables()));

  variablesLayout->addWidget(variablesLoad);

  tab->addTab(variables, "Variables");

  //----

  auto *functions = new QFrame;

  auto *functionsLayout = new QVBoxLayout(functions);
  functionsLayout->setMargin(0); functionsLayout->setSpacing(2);

  functionsList_ = new QListWidget;

  functionsLayout->addWidget(functionsList_);

  auto *functionsLoad = new QPushButton("Load");

  connect(functionsLoad, SIGNAL(clicked()), this, SLOT(loadFunctions()));

  functionsLayout->addWidget(functionsLoad);

  tab->addTab(functions, "Functions");

  //---

  layout->addWidget(tab);

  //---

  auto *entryFrame = new QFrame;

  layout->addWidget(entryFrame);

  auto *entryLayout = new QHBoxLayout(entryFrame);
  entryLayout->setMargin(0); entryLayout->setSpacing(2);

  input_ = new CQHistoryLineEdit;

  entryLayout->addWidget(input_);

  connect(input_, SIGNAL(exec(const QString &)), this, SLOT(execCmd(const QString &)));

  auto *load = new QPushButton("Load");

  entryLayout->addWidget(load);

  connect(load, SIGNAL(clicked()), this, SLOT(loadFile()));
}

void
CQSVGJSDialog::
historySlot(QListWidgetItem *item)
{
  input_->setText(item->text());
}

void
CQSVGJSDialog::
execCmd(const QString &cmd)
{
  if (! cmd.length())
    return;

  history_->addItem(cmd);

  //---

#ifdef CSVG_JAVASCRIPT
  auto *js = window_->svg()->js();

  js->loadString(cmd.toStdString());

  CJValueP value = js->exec();

  if (value) {
    std::string str = value->toString();

    auto cursor = log_->textCursor();

    cursor.insertText(str.c_str());
    cursor.insertText("\n");
  }
#endif
}

void
CQSVGJSDialog::
loadFile()
{
  auto title  = QString("Load File");
  auto cwd    = QDir::currentPath();
  auto filter = QString("JavaScript Files (*.js)");

  auto filename = QFileDialog::getOpenFileName(this, title, cwd, filter);

  if (filename == "")
    return;

#ifdef CSVG_JAVASCRIPT
  auto *js = window_->svg()->js();

  js->loadFile(filename.toStdString());

  js->exec();
#endif
}

void
CQSVGJSDialog::
loadVariables()
{
#ifdef CSVG_JAVASCRIPT
  auto *js = window_->svg()->js();

  variablesList_->clear();

  std::vector<std::string> names = js->getVariableNames();

  for (const auto &n : names)
    variablesList_->addItem(n.c_str());
#endif
}

void
CQSVGJSDialog::
loadFunctions()
{
#ifdef CSVG_JAVASCRIPT
  auto *js = window_->svg()->js();

  functionsList_->clear();

  std::vector<std::string> names = js->getFunctionNames();

  for (const auto &n : names)
    functionsList_->addItem(n.c_str());
#endif
}

QSize
CQSVGJSDialog::
sizeHint() const
{
  QFontMetrics fm(font());

  int w = fm.horizontalAdvance("X")*50;
  int h = fm.height()  *25;

  return QSize(w, h);
}
