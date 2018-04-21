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

  QVBoxLayout *layout = new QVBoxLayout(this);
  layout->setMargin(0); layout->setSpacing(2);

  //---

  QTabWidget *tab = new QTabWidget;

  log_ = new QTextEdit;

  log_->setReadOnly(true);

  tab->addTab(log_, "Log");

  history_ = new QListWidget;

  connect(history_, SIGNAL(itemClicked(QListWidgetItem *)),
          this, SLOT(historySlot(QListWidgetItem *)));

  tab->addTab(history_, "History");

  //----

  QFrame *variables = new QFrame;

  QVBoxLayout *variablesLayout = new QVBoxLayout(variables);
  variablesLayout->setMargin(0); variablesLayout->setSpacing(2);

  variablesList_ = new QListWidget;

  variablesLayout->addWidget(variablesList_);

  QPushButton *variablesLoad = new QPushButton("Load");

  connect(variablesLoad, SIGNAL(clicked()), this, SLOT(loadVariables()));

  variablesLayout->addWidget(variablesLoad);

  tab->addTab(variables, "Variables");

  //----

  QFrame *functions = new QFrame;

  QVBoxLayout *functionsLayout = new QVBoxLayout(functions);
  functionsLayout->setMargin(0); functionsLayout->setSpacing(2);

  functionsList_ = new QListWidget;

  functionsLayout->addWidget(functionsList_);

  QPushButton *functionsLoad = new QPushButton("Load");

  connect(functionsLoad, SIGNAL(clicked()), this, SLOT(loadFunctions()));

  functionsLayout->addWidget(functionsLoad);

  tab->addTab(functions, "Functions");

  //---

  layout->addWidget(tab);

  //---

  QFrame *entryFrame = new QFrame;

  layout->addWidget(entryFrame);

  QHBoxLayout *entryLayout = new QHBoxLayout(entryFrame);
  entryLayout->setMargin(0); entryLayout->setSpacing(2);

  input_ = new CQHistoryLineEdit;

  entryLayout->addWidget(input_);

  connect(input_, SIGNAL(exec(const QString &)), this, SLOT(execCmd(const QString &)));

  QPushButton *load = new QPushButton("Load");

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

  CSVGJavaScript *js = window_->svg()->js();

  js->loadString(cmd.toStdString());

  CJValueP value = js->exec();

  if (value) {
    std::string str = value->toString();

    QTextCursor cursor = log_->textCursor();

    cursor.insertText(str.c_str());
    cursor.insertText("\n");
  }
}

void
CQSVGJSDialog::
loadFile()
{
  QString title  = "Load File";
  QString cwd    = QDir::currentPath();
  QString filter = "JavaScript Files (*.js)";

  QString filename = QFileDialog::getOpenFileName(this, title, cwd, filter);

  if (filename == "")
    return;

  CSVGJavaScript *js = window_->svg()->js();

  js->loadFile(filename.toStdString());

  js->exec();
}

void
CQSVGJSDialog::
loadVariables()
{
  CSVGJavaScript *js = window_->svg()->js();

  variablesList_->clear();

  std::vector<std::string> names = js->getVariableNames();

  for (const auto &n : names)
    variablesList_->addItem(n.c_str());
}

void
CQSVGJSDialog::
loadFunctions()
{
  CSVGJavaScript *js = window_->svg()->js();

  functionsList_->clear();

  std::vector<std::string> names = js->getFunctionNames();

  for (const auto &n : names)
    functionsList_->addItem(n.c_str());
}

QSize
CQSVGJSDialog::
sizeHint() const
{
  QFontMetrics fm(font());

  int w = fm.width("X")*50;
  int h = fm.height()  *25;

  return QSize(w, h);
}
