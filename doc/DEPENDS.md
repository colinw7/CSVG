This package depends on other packages in my github directory.

To get all code to successfully build follow the following steps:
 + Download charts into directory
   + git clone "https://github.com/colinw7/CSVG" CSVG
 + cd $ROOT/CSVG/src
   + Check list of paths in Makefile (-I directories in CPPFLAGS variable)
     + for each path download the missing project into the same directory as you downloaded CSVG
   + make
 + cd $ROOT/CSVG/test
   + Check list of libraries in Makefile (-L directories in LFLAGS variable)
     + for each library download the missing project into the same directory as you downloaded CSVG
   + build each downloaded library using its src and/or test directory
     + each library may have additional dependencies in its Makefile
   + make

Additional dependencies are standard UNIX packages:
 + png
   + libpng12.so.0
 + jpeg
   + libjpeg.so.8
 + tre
   + libtre.so.5
 + freetype
   + libfreetype.so.6.17.1
