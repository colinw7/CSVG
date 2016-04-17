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

class CQSVGWindow : public QMainWindow {
  Q_OBJECT

 public:
  CQSVGWindow();

  CQSVG *svg() const { return svg_; }

  CQSVGCanvas *canvas() const { return canvas_; }

  CQPropertyTree *tree() const { return tree_; }

  void loadFile(const std::string &filename, bool image, const std::string &imageDir, bool print);

  void showPos(const QPoint &ppos, const QPointF &wpos);

  void deselectAllObjects();

  QSize sizeHint() const;

 private:
  void addProperties();

  void addObjectToTree(const std::string &name, CSVGObject *obj);

  void deselectObjects(CSVGObject *obj);

 public slots:
  void redraw();

 private slots:
  void itemSelectedSlot(QObject *obj, const QString &path);

  void print() const;

  void showProperties();
  void showBuffers();

 private:
  CQSVG*              svg_           { 0 };
  CQSVGCanvas*        canvas_        { 0 };
  CQPropertyTree*     tree_          { 0 };
  QLabel*             posLabel_      { 0 };
  QLabel*             zoomLabel_     { 0 };
  CQSVGPropertiesDlg* propertiesDlg_ { 0 };
  CQSVGBufferView*    bufferView_    { 0 };
};

#endif
