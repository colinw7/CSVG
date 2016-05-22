#ifndef CQSVGMain_H
#define CQSVGMain_H

#include <QMainWindow>

class CQSVG;
class CQSVGCanvas;
class CQSVGPropertiesDlg;
class CQSVGBufferView;
class CQPropertyTree;
class CSVGObject;
class QLabel;
class QLineEdit;

class CQSVGWindow : public QMainWindow {
  Q_OBJECT

 public:
  CQSVGWindow();

  CQSVG *svg() const { return svg_; }

  CQSVGCanvas *canvas() const { return canvas_; }

  CQPropertyTree *tree() const { return tree_; }

  bool isImage() const { return image_; }
  void setImage(bool b) { image_ = b; }

  const std::string &imageDir() const { return imageDir_; }
  void setImageDir(const std::string &v) { imageDir_ = v; }

  bool isPrint() const { return print_; }
  void setPrint(bool b) { print_ = b; }

  void addFile(const std::string &filename);

  void loadFile();

  void showPos(const QPoint &ppos, const QPointF &wpos);

  void setTime(double t);

  void deselectAllObjects();

  QSize sizeHint() const;

 private:
  void updateState();

  void addProperties();

  void addObjectToTree(const std::string &name, CSVGObject *obj);

  void deselectObjects(CSVGObject *obj);

 public slots:
  void redraw();

 private slots:
  void itemSelectedSlot(QObject *obj, const QString &path);

  void print() const;

  void nextFile();
  void prevFile();

  void showProperties();
  void showBuffers();

  void playSlot();
  void pauseSlot();
  void stepSlot();
  void bstepSlot();
  void timeSlot();

 private:
  typedef std::vector<std::string> Files;

  CQSVG*              svg_            { 0 };
  CQSVGCanvas*        canvas_         { 0 };
  CQPropertyTree*     tree_           { 0 };
  QLabel*             posLabel_       { 0 };
  QLabel*             zoomLabel_      { 0 };
  CQSVGPropertiesDlg* propertiesDlg_  { 0 };
  CQSVGBufferView*    bufferView_     { 0 };
  QAction*            nextAction_     { 0 };
  QAction*            prevAction_     { 0 };
  QMenu*              animateMenu_    { 0 };
  QAction*            playAction_     { 0 };
  QAction*            pauseAction_    { 0 };
  QAction*            stepAction_     { 0 };
  QAction*            bstepAction_    { 0 };
  QToolBar*           animateToolBar_ { 0 };
  QLineEdit*          timeEdit_       { 0 };
  bool                image_          { false };
  std::string         imageDir_;
  bool                print_          { false };
  Files               files_;
  int                 ind_            { -1 };
};

#endif
