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

### 5/1/2016
 + Improve path interpolation
 + Text on path
 + Improve handling clip transformed clip path
 + Add alpha bump map support for diffuse, specular lighting
 + Improve mask handing
 + Improve UTF8 support for text rendering
 + More object properties
 + Checkerboard drawing for SVG background

### 5/3/2016
 + Add offset to Qt draw
 + Move draw code into buffer
 + Fix bad matrix transform read/write
 + Move displacement map code to CImageLib
 + Handle relative move to path command correctly

### 5/4/2016
 + Improve font size selection
 + Move CImagePtr calls to CSVGBuffer

### 5/8/2016
 + Support iterating through list of svg files
 + Improve no color default for fill/stroke
 + Move more CImagePtr calls to CSVGBuffer
 + Fix filter print

### 5/15/2016
 + Use new CScreenUnits class for units conversion
 + improve block scale handling

### 5/16/2016
 + handle slice in preserveAspectRatio

### 5/22/2016
 + add mirror path part beziers
 + fix issues in absolute horizontal/vertical path parts
 + support global opacity
 + support gradient fill for stroke
 + add animation controls to gui

### 5/30/2016
 + Move more image handling into buffer and renderer
 + Speed up gaussian blur by only applying to object region
 + buffers render to internal temp buffer instead of SourceGraphic
 + Add translate for animateTransform
 + Add support for enable-background
 + Debug support for filter box and gradient control points
 + Add animation properties to dialog
 + Add path parts to properites dialog

### 6/5/2016
 + combine stroke, fill, clip into single struct and push/pop rather that update/reset
 + put css style data into single struct and simplify code
 + Add color-profile tag
 + Update fill/stroke color handling so none and currentColor types are part of color class
 + Add marker clipping
 + Add css marker support
 + Improve handling of global (per buffer) transparency
 + Add CSS Tree to Qt viewer

### 6/12/2016
 + cleanup up handling of nested svg elements
 + add font def style update
 + add support for animation values
 + Add StrokePaint and FillPaint buffers
 + Add separate x/y turbulence frequency support
 + Improve handling of filter buffer in/out sharing
 + Add svg image file type
 + Add used color output to test program

### 6/18/2016
 + Improve FillPaint and FillStroke handling
 + Improve filter and filter element region handling

### 6/23/2016
 + add busy indicator
 + add print to flat svg
 + add abstract image data class
 + implement abstract image data using QImage for Qt app

### 6/25/2016
 + fix issues with print and print flat losing information
 + update use so doesn't need to overwrite transform

### 7/4/2016
 + Add JavaScript library integratation
 + Abstact font to CFontObj virtual class
 + Base em size calc on current font
 + Use QFont for CFontObj in CQSVG

### 7/10/2016
 + Add JavaScript Console Dialog
 + Add JavaScript Object Transform Objects
