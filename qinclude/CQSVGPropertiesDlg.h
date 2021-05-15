#ifndef CQSVGPropertiesDlg_H
#define CQSVGPropertiesDlg_H

#include <QFrame>

class CQSVGWindow;
class CQPropertyTree;
class CQSVGStyleTree;

class CQSVGPropertiesDlg : public QFrame {
  Q_OBJECT

 public:
  CQSVGPropertiesDlg(CQSVGWindow *window);

  CQSVGWindow *window() const { return window_; }

  CQPropertyTree *propertiesTree() const { return propertiesTree_; }

  CQSVGStyleTree *cssTree() const { return cssTree_; }

  void loadCSS();

  QSize sizeHint() const override;

 private:
  CQSVGWindow    *window_         { 0 };
  CQPropertyTree *propertiesTree_ { 0 };
  CQSVGStyleTree *cssTree_        { 0 };
};

#endif
