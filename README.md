# CSVG

SVG Parser and Renderer

Work in Progress but renderes most simple SVG files.

## Directories

### src
source files for SVG library which draws using an abstract renderer interface

### test
test program to print parsed file and renderer to an image

### qtest
Qt test program to renderer to Qt Widget with debugging widgets for
editing individual object parameters and viewing intermediate rendered
images.

## Notes

This package depends on a number of other packages.

You can see which packages it depends on in the CPPFLAGS in the Makefile.

Download all the packages in the same root directory and it should build.
