#ifndef CQSVGJSDialog_H
#define CQSVGJSDialog_H

#include <QFrame>

class CQSVGWindow;
class QTextEdit;
class CQHistoryLineEdit;
class QListWidget;
class QListWidgetItem;

class CQSVGJSDialog : public QFrame {
  Q_OBJECT

 public:
  CQSVGJSDialog(CQSVGWindow *window);

  CQSVGWindow *window() const { return window_; }

  QSize sizeHint() const;

 private slots:
  void historySlot(QListWidgetItem *item);

  void execCmd(const QString &cmd);

  void loadFile();

  void loadVariables();
  void loadFunctions();

 private:
  CQSVGWindow       *window_        { 0 };
  QTextEdit         *log_           { 0 };
  CQHistoryLineEdit *input_         { 0 };
  QListWidget       *history_       { 0 };
  QListWidget       *variablesList_ { 0 };
  QListWidget       *functionsList_ { 0 };
};

#endif
