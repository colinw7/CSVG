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
  stroke_ = object_.getSVG().getStroke();
  fill_   = object_.getSVG().getFill  ();
  clip_   = object_.getSVG().getClip  ();

  if (! object.getSelected())
    object_.getSVG().updateStroke(object_.getStroke());
  else
    object_.getSVG().setSelectedStroke();

  object_.getSVG().updateFill(object_.getFill());

  object_.getSVG().updateClip(object_.getClip());
}

CSVGTempStroke::
~CSVGTempStroke()
{
  object_.getSVG().resetStroke();
  object_.getSVG().resetFill  ();
  object_.getSVG().resetClip  ();

  object_.getSVG().updateStroke(stroke_);
  object_.getSVG().updateFill  (fill_  );
  object_.getSVG().updateClip  (clip_  );
}
