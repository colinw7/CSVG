#ifndef CQSVGPropertiesDlg_H
#define CQSVGPropertiesDlg_H

#include <QFrame>

class CQSVGWindow;
class CQPropertyTree;

class CQSVGPropertiesDlg : public QFrame {
 public:
  CQSVGPropertiesDlg(CQSVGWindow *window);

  CQSVGWindow *window() const { return window_; }

  CQPropertyTree *tree() const { return tree_; }

  QSize sizeHint() const;

 private:
  CQSVGWindow    *window_ { 0 };
  CQPropertyTree *tree_   { 0 };
};

#endif
