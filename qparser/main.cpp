#include <CQSVGParser.h>
#include <QApplication>
#include <iostream>

int
main(int argc, char **argv)
{
  QCoreApplication app(argc, argv);

  std::vector<QString> args;

  bool    dump      = false;
  bool    path      = false;
  bool    header    = false;
  bool    transform = false;
  QString id;
  QString angle;

  for (int i = 1; i < argc; ++i) {
    if (argv[i][0] == '-') {
      auto opt = QString(&argv[i][1]);

      if      (opt == "dump")
        dump = true;
      else if (opt == "path")
        path = true;
      else if (opt == "header")
        header = true;
      else if (opt == "id") {
        ++i;

        if (i < argc)
          id = argv[i];
      }
      else if (opt == "transform") {
        transform = true;

        ++i;

        if (i < argc)
          angle = argv[i];
      }
      else {
        std::cerr << "Invalid option '" << argv[i] << "'\n";
      }
    }
    else {
      args.push_back(argv[i]);
    }
  }

  CQSVGParser parser;

  parser.setDump(dump);
  parser.setPath(path);
  parser.setXmlHeader(header);
  parser.setTransform(transform, id, angle);

  for (const auto &arg : args)
    parser.parse(arg);

  return 0;
}
