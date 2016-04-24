### 3/24/2016
 + C++11 improvements
 + Track all arguments as optional to determine if set or not
 + Add print method to dump out original SVG file

### 3/28/2016
 + Add animation objects

### 4/17/2016
 + Support all filters (stil some bugs)
 + Improve rendere pipeline for filters (intermediate buffers and transform handling)
 + Extend object factory so Qt code can create derived objects for all types with support for Qt properties
 + Add Qt test program to git repository
 + Add separate Qt buffer viewer window
 + Add separate Qt property tree window
 + Add basic zoom in/out

### 4/24/2016
 + Use Matrix Stack so build transform so orignal transform parts can be extracted
 + Improve handling of tspan so text ordering is maintained
 + Add UTF8 support for XML reader
 + Improve CSS handling so support global, type and class matches
 + Add subscript, superscript, overline, underline, strokethrough font styles
 + Improve marker drawing
 + More object properties
 + Fix issues in radial gradient bbox scaling