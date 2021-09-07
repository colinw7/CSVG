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
class ModeToolBar;
class ColorBar;
class StatusBar;
class FillStroke;
class ObjectProperties;
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
    CREATE_PATH,
    CREATE_TEXT
  };

 public:
  Window();

  CSVG *svg() const;

  Canvas *canvas() const { return canvas_; }

  void addFiles(const QStringList &files);

  QString currentFile() const { return currentFile_; }

  CSVGObject *currentObject() const { return currentObj_; }
  void setCurrentObject(CSVGObject *obj);

  void setMousePos(const CPoint2D &p);

  void redraw(bool update);

  bool isCheckerboard() const { return checkerboard_; }
  void setCheckerboard(bool b);

  bool isShowViewBox() const { return showViewBox_; }
  void setShowViewBox(bool b);

  const Mode &mode() const { return mode_; }
  void setMode(const Mode &v) { mode_ = v; }

  bool isConsoleVisible() const;
  void setConsoleVisible(bool show);

  bool isBufferViewVisible() const;
  void setBufferViewVisible(bool show);

  void loadTclFile(const QString &file);

  void selectNone();

  void resizeEvent(QResizeEvent *) override;

  QSize sizeHint() const override { return QSize(1600, 1600); }

 private:
  void updatePlacement();

  void updateStatus();

 private slots:
  void loadSlot();
  void settingsSlot();
  void quitSlot();

  void selectNoneSlot();

  void helpSlot();

 private:
  QMenuBar*    menuBar_      { nullptr };
  QFrame*      mouseToolBar_ { nullptr };
  ModeToolBar* modeToolBar_  { nullptr };
  ColorBar*    colorBar_     { nullptr };
  StatusBar*   statusBar_    { nullptr };
  Canvas*      canvas_      { nullptr };

  using Palettes = std::vector<Palette *>;

  QFrame*      palettesArea_   { nullptr };
  QVBoxLayout* palettesLayout_ { nullptr };

  FillStroke*       fillStrokePalette_       { nullptr };
  ObjectProperties* objectPropertiesPalette_ { nullptr };
  Palettes          palettes_;

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
