#ifndef CQInkscapeWindow_H
#define CQInkscapeWindow_H

#include <QFrame>

#include <CPoint2D.h>

class CSVG;
class CSVGObject;
class CQSVGRenderer;
class CQSVGBufferView;

class QMenuBar;
class QLabel;
class QVBoxLayout;

namespace CQInkscape {

class Canvas;
class MouseToolBar;
class ModeToolBar;
class ColorBar;
class StatusBar;
class FillStrokePalette;
class ObjectProperties;
class TransformPalette;
class PaletteArea;
class Palette;
class Console;
class Settings;

class Window : public QFrame {
  Q_OBJECT

 public:
  enum class Mode {
    SELECT_OBJECT,
    SELECT_POINT,
    ZOOM,
    CREATE_RECT,
    CREATE_ELLIPSE,
    CREATE_PATH,
    CREATE_TEXT
  };

 public:
  Window();

  CSVG *svg() const;

  Canvas *canvas() const { return canvas_; }

  void addFiles(const QStringList &files);

  QString currentFile() const { return currentFile_; }

  void updateCurrentObject();

  CSVGObject *currentObject() const { return currentObj_; }
  void setCurrentObject(CSVGObject *obj);

  void setMousePos(const CPoint2D &p);

  void redraw(bool update);

  bool isCheckerboard() const { return checkerboard_; }
  void setCheckerboard(bool b);

  bool isShowViewBox() const { return showViewBox_; }
  void setShowViewBox(bool b);

  const Mode &mode() const { return mode_; }
  void setMode(const Mode &v);

  bool isConsoleVisible() const;
  void setConsoleVisible(bool show);

  bool isBufferViewVisible() const;
  void setBufferViewVisible(bool show);

  void loadTclFile(const QString &file);

  void selectAll();
  void selectNone();
  void selectParent();

  void resizeEvent(QResizeEvent *) override;

  QSize sizeHint() const override { return QSize(2000, 1600); }

  void updatePlacement();

 private:
  void updateStatus();

 private slots:
  void loadSlot();
  void saveAsSlot();
  void printSlot();
  void quitSlot();

  void zoomInSlot();
  void zoomOutSlot();
  void zoomResetSlot();

  void settingsSlot();
  void consoleSlot();

  void selectAllSlot();
  void selectNoneSlot();
  void selectParentSlot();

  void ungroupSlot();

  void helpSlot();

 private:
  QMenuBar*     menuBar_      { nullptr };
  MouseToolBar* mouseToolBar_ { nullptr };
  ModeToolBar*  modeToolBar_  { nullptr };
  ColorBar*     colorBar_     { nullptr };
  StatusBar*    statusBar_    { nullptr };
  Canvas*       canvas_       { nullptr };

  using Palettes = std::vector<Palette *>;

  PaletteArea* paletteArea_ { nullptr };

  FillStrokePalette* fillStrokePalette_       { nullptr };
  ObjectProperties*  objectPropertiesPalette_ { nullptr };
  TransformPalette*  transformPalette_        { nullptr };

  Console*  console_  { nullptr };
  Settings* settings_ { nullptr };

  CQSVGBufferView *bufferView_ { nullptr };

  QString     currentFile_;
  CSVGObject* currentObj_ { nullptr };

  bool checkerboard_ { false };
  bool showViewBox_  { true };

  Mode mode_ { Mode::SELECT_OBJECT };
};

}

#endif
