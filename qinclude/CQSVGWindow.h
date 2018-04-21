#ifndef CQSVGMain_H
#define CQSVGMain_H

#include <QMainWindow>
#include <CHRTimer.h>

class CQSVG;
class CQSVGCanvas;
class CQSVGPropertiesDlg;
class CQSVGBufferView;
class CQSVGJSDialog;
class CQPropertyTree;
class CSVGObject;
class QLabel;
class QToolButton;
class QLineEdit;

class CQSVGWindow : public QMainWindow {
  Q_OBJECT

  Q_PROPERTY(CQSVG*   svg      READ svg)
  Q_PROPERTY(bool     isImage  READ isImage  WRITE setIsImage )
  Q_PROPERTY(QString  imageDir READ imageDir WRITE setImageDir)
  Q_PROPERTY(QString  rootId   READ rootId   WRITE setRootId  )
  Q_PROPERTY(bool     print    READ isPrint  WRITE setPrint   )

 public:
  typedef std::vector<QString> Files;
  typedef std::vector<QString> Ids;

 public:
  CQSVGWindow();

 ~CQSVGWindow();

  CQSVG *svg() const { return svg_; }

  CQSVGCanvas *canvas() const { return canvas_; }

  CQPropertyTree *propertiesTree() const { return propTree_; }

  bool isImage() const { return image_; }
  void setIsImage(bool b) { image_ = b; }

  const QString &imageDir() const { return imageDir_; }
  void setImageDir(const QString &s) { imageDir_ = s; }

  const QString &rootId() const { return rootId_; }
  void setRootId(const QString &s) { rootId_ = s; }

  bool isPrint() const { return print_; }
  void setPrint(bool b) { print_ = b; }

  void addFiles(const Files &files);
  void addFile(const QString &filename);

  void addRootIds(const Ids &ids);

  void showPos(const QPoint &ppos, const QPointF &wpos);

  void setTime(double t);

  void deselectAllObjects();

  bool isInitialized() const { return initialized_; }
  void setInitialized(bool b) { initialized_ = b; }

  void startBusy();
  void endBusy();

  void updateBusy();

  QSize sizeHint() const;

 private:
  void updateAltRoot();

  void updateTitle();

  void updateState();

  void addProperties();

  void loadCSS();

  void addObjectToTree(const std::string &name, CSVGObject *obj);

  void deselectObjects(CSVGObject *obj);

 public slots:
  void redraw();

  void loadFile();

 private slots:
  void itemSelectedSlot(QObject *obj, const QString &path);

  void print();
  void printFlat();
  void close();

  void nextFile();
  void prevFile();

  void showProperties();
  void showBuffers();
  void showJSDialog();

  void playSlot();
  void pauseSlot();
  void stepSlot();
  void bstepSlot();
  void timeSlot();

 private:
  CQSVG*              svg_            { 0 };
  CQSVGCanvas*        canvas_         { 0 };
  CQPropertyTree*     propTree_       { 0 };
  QLabel*             posLabel_       { 0 };
  QLabel*             zoomLabel_      { 0 };
  QToolButton*        busyButton_     { 0 };
  CQSVGPropertiesDlg* propertiesDlg_  { 0 };
  CQSVGBufferView*    bufferView_     { 0 };
  CQSVGJSDialog*      jsDlg_          { 0 };
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
  QString             imageDir_;
  QString             rootId_;
  bool                print_          { false };
  Files               files_;
  int                 fileInd_        { -1 };
  Ids                 rootIds_;
  int                 rootInd_        { -1 };
  bool                initialized_    { false };
  CHRTime             startTime_;
  CHRTime             busyTime_;
  CHRTime             endTime_;
};

#endif
