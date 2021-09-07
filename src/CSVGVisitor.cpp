#include <CSVG.h>

CSVGVisitor::
CSVGVisitor()
{
}

CSVGVisitor::
~CSVGVisitor()
{
}

void
CSVGVisitor::
exec(CSVGObject *object)
{
  preVisit(object);

  object->accept(this);

  postVisit(object);

  for (const auto &c : object->children())
    exec(c);

  for (const auto &o : object->getAnimation().objects())
    exec(o);

  postExec(object);
}
