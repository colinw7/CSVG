#ifndef CQSVGMain_H
#define CQSVGMain_H

#include <QMainWindow>
#include <CHRTimer.h>

class CQSVGCanvas;
class CQSVG;
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

  Q_PROPERTY(bool    isImage  READ isImage  WRITE setIsImage )
  Q_PROPERTY(QString imageDir READ imageDir WRITE setImageDir)
  Q_PROPERTY(QString rootId   READ rootId   WRITE setRootId  )
  Q_PROPERTY(bool    print    READ isPrint  WRITE setPrint   )

 public:
  using Files = std::vector<QString>;
  using Ids   = std::vector<QString>;

 public:
  CQSVGWindow();
 ~CQSVGWindow();

  //---

  CQSVGCanvas *canvas() const { return canvas_; }

  CQPropertyTree *propertiesTree() const { return propTree_; }

  //---

  CQSVG *svg() const;

  //---

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

  QSize sizeHint() const override;

 private:
  void addMenus();

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
  CQSVGCanvas*        canvas_        { nullptr };
  CQPropertyTree*     propTree_      { nullptr };
  QLabel*             posLabel_      { nullptr };
  QLabel*             zoomLabel_     { nullptr };
  QToolButton*        busyButton_    { nullptr };
  CQSVGPropertiesDlg* propertiesDlg_ { nullptr };
  CQSVGBufferView*    bufferView_    { nullptr };
  CQSVGJSDialog*      jsDlg_         { nullptr };

  QAction*  nextAction_       { nullptr };
  QAction*  prevAction_       { nullptr };
  QMenu*    animateMenu_      { nullptr };
  QAction*  playAction_       { nullptr };
  QAction*  pauseAction_      { nullptr };
  QAction*  stepAction_       { nullptr };
  QAction*  bstepAction_      { nullptr };
  QAction*  propertiesAction_ { nullptr };
  QAction*  buffersAction_    { nullptr };
  QAction*  jsAction_         { nullptr };
  QToolBar* animateToolBar_   { nullptr };

  QLineEdit* timeEdit_ { nullptr };

  bool    image_ { false };
  QString imageDir_;

  QString rootId_;
  bool    print_          { false };
  Files   files_;
  int     fileInd_        { -1 };
  Ids     rootIds_;
  int     rootInd_        { -1 };
  bool    initialized_    { false };

  CHRTime startTime_;
  CHRTime busyTime_;
  CHRTime endTime_;
};

#endif
