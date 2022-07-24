#include <CQHistoryLineEdit.h>
#include <CHistory.h>
#include <QKeyEvent>

CQHistoryLineEdit::
CQHistoryLineEdit(QWidget *parent) :
 QLineEdit(parent)
{
  history_ = std::make_unique<CHistory>();

  connect(this, SIGNAL(returnPressed()), this, SLOT(execSlot()));
}

CQHistoryLineEdit::
~CQHistoryLineEdit()
{
}

void
CQHistoryLineEdit::
execSlot()
{
  QString str = text();

  emit exec(str);

  QString str1 = str.trimmed();

  if (str1.length())
    history_->addCommand(str1.toStdString());

  setText("");
}

void
CQHistoryLineEdit::
keyPressEvent(QKeyEvent *event)
{
  int key = event->key();

  switch (key) {
    case Qt::Key_Up: {
      std::string cmd;

      if (history_->getPrevCommand(cmd))
        setText(cmd.c_str());

      break;
    }
    case Qt::Key_Down: {
      std::string cmd;

      if (history_->getNextCommand(cmd))
        setText(cmd.c_str());

      break;
    }
    default:
      QLineEdit::keyPressEvent(event);

      break;
  }
}
