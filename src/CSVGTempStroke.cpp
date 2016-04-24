#include <CSVGTempStroke.h>
#include <CSVGObject.h>
#include <CSVG.h>

CSVGTempStroke::
CSVGTempStroke(CSVGObject &object) :
 object_(object),
 stroke_(object_.getSVG()),
 fill_  (object_.getSVG()),
 clip_  (object_.getSVG())
{
  CSVG &svg = object_.getSVG();

  svg.setStyleObject(&object);

  stroke_ = svg.getStroke();
  fill_   = svg.getFill  ();
  clip_   = svg.getClip  ();

  if (! object.getSelected())
    svg.updateStroke(object_.getStroke());
  else
    svg.setSelectedStroke();

  svg.updateFill(object_.getFill());

  svg.updateClip(object_.getClip());
}

CSVGTempStroke::
~CSVGTempStroke()
{
  CSVG &svg = object_.getSVG();

  svg.setStyleObject(0);

  svg.resetStroke();
  svg.resetFill  ();
  svg.resetClip  ();

  svg.updateStroke(stroke_);
  svg.updateFill  (fill_  );
  svg.updateClip  (clip_  );
}
