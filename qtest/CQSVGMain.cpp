#include <CQSVGWindow.h>
#include <CQSVG.h>
#include <CQApp.h>
#include <CQStyle.h>

#include <QTimer>

int
main(int argc, char **argv)
{
  bool    debug    = false;
  bool    nofilter = false;
  bool    image    = false;
  QString imageDir = "";
  bool    print    = false;
  int     dpi      = -1;

  CQSVGWindow::Files files;
  CQSVGWindow::Ids   ids;

  for (int i = 1; i < argc; i++) {
    if (argv[i][0] == '-') {
      if      (strcmp(&argv[i][1], "debug") == 0)
        debug = true;
      else if (strcmp(&argv[i][1], "image") == 0)
        image = true;
      else if (strcmp(&argv[i][1], "image_dir") == 0) {
        ++i;

        if (i < argc)
          imageDir = argv[i];
      }
      else if (strcmp(&argv[i][1], "print") == 0)
        print = true;
      else if (strcmp(&argv[i][1], "nofilter") == 0)
        nofilter = true;
      else if (strcmp(&argv[i][1], "dpi") == 0) {
        ++i;

        if (i < argc)
          dpi = atoi(argv[i]);
      }
      else if (strcmp(&argv[i][1], "id") == 0) {
        ++i;

        if (i < argc)
          ids.push_back(argv[i]);
      }
      else
        std::cerr << "Invalid option: " << argv[i] << std::endl;
    }
    else
      files.push_back(argv[i]);
  }

  //---

  CQApp app(argc, argv);

  app.setStyle(new CQStyle);

  if (dpi > 0)
    CScreenUnitsMgrInst->setDpi(dpi);

  CQSVGWindow *window = new CQSVGWindow;

  if (nofilter)
    window->svg()->setIgnoreFilter(true);

  if (debug)
    window->svg()->setDebug(true);

  if (image)
    window->setIsImage(image);

  if (imageDir != "")
    window->setImageDir(imageDir);

  if (print)
    window->setPrint(print);

  window->addFiles(files);

  window->addRootIds(ids);

  //---

  if (image || print) {
    window->loadFile();

    exit(0);
  }

  //---

  window->show();

  QTimer::singleShot(100, window, SLOT(loadFile()));

  app.exec();

  return 0;
}
