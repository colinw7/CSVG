#include <CSVGAnimation.h>
#include <CSVGObject.h>
#include <CSVG.h>

CSVGAnimation::
CSVGAnimation(CSVGObject *parent) :
 parent_(parent)
{
}

CSVGAnimation::
~CSVGAnimation()
{
  for (auto &o : objects_)
    delete o;
}

void
CSVGAnimation::
addObject(CSVGObject *obj)
{
  objects_.push_back(obj);
}

void
CSVGAnimation::
removeObject(CSVGObject *obj)
{
  objects_.remove(obj);
}

CSVGObject *
CSVGAnimation::
prevObject(CSVGObject *obj) const
{
  CSVGObject *prev = nullptr;

  for (const auto &o : objects_) {
    if (o == obj)
      return prev;

    prev = o;
  }

  return prev; // not added yet so return last
}

void
CSVGAnimation::
setTime(double t)
{
  if (objects_.empty())
    return;

  if (! t_.isValid())
    t_ = parent_->getTransform();
  else
    parent_->setTransform(t_.getValue());

  for (const auto &c : objects())
    c->setTime(t);
}

void
CSVGAnimation::
tick(double dt)
{
  if (objects_.empty())
    return;

  if (! t_.isValid())
    t_ = parent_->getTransform();
  else
    parent_->setTransform(t_.getValue());

  for (const auto &c : objects())
    c->tick(dt);
}

void
CSVGAnimation::
handleEvent(CSVGEventType type, const std::string &id, const std::string &data)
{
  for (const auto &o : objects_)
    o->handleEvent(type, id, data);
}
