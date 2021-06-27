#include <CQSVGWindow.h>
#include <CQSVG.h>
#include <CQApp.h>
#include <CQStyle.h>

#include <QTimer>

int
main(int argc, char **argv)
{
  bool    debug         = false;
  bool    check_viewbox = false;
  bool    image         = false;
  QString imageDir      = "";
  bool    print         = false;
  bool    nofilter      = false;
  int     dpi           = -1;

  CQSVGWindow::Files files;
  CQSVGWindow::Ids   ids;

  for (int i = 1; i < argc; i++) {
    if (argv[i][0] == '-') {
      auto arg = std::string(&argv[i][1]);

      if      (arg == "debug")
        debug = true;
      else if (arg == "check_viewbox")
        check_viewbox = true;
      else if (arg == "image")
        image = true;
      else if (arg == "image_dir") {
        ++i;

        if (i < argc)
          imageDir = argv[i];
      }
      else if (arg == "print")
        print = true;
      else if (arg == "nofilter")
        nofilter = true;
      else if (arg == "dpi") {
        ++i;

        if (i < argc)
          dpi = atoi(argv[i]);
      }
      else if (arg == "id") {
        ++i;

        if (i < argc)
          ids.push_back(argv[i]);
      }
      else
        std::cerr << "Invalid option: " << argv[i] << "\n";
    }
    else
      files.push_back(argv[i]);
  }

  //---

  CQApp app(argc, argv);

  app.setStyle(new CQStyle);

  if (dpi > 0)
    CScreenUnitsMgrInst->setDpi(dpi);

  auto *window = new CQSVGWindow;

  window->svg()->setIgnoreFilter(nofilter);

  window->svg()->setDebug(debug);

  window->svg()->setCheckViewBox(check_viewbox);

  if (image)
    window->setIsImage(image);

  if (imageDir != "")
    window->setImageDir(imageDir);

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
