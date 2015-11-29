#
# Borland C++ IDE generated makefile
# Generated 11/04/01 at 19:22:16 
#
.AUTODEPEND


#
# Borland C++ tools
#
!ifndef BCB
BCB = $(MAKEDIR)\..
!endif

IMPLIB  = Implib
BCC32   = Bcc32 +BccW32.cfg 
TLINK32 = TLink32
TLIB    = TLib
BRC32   = Brc32
TASM32  = Tasm32
#
# IDE macros
#


#
# Options
#
IDE_LinkFLAGS32 =  -L$(BCB)\LIB
LinkerLocalOptsAtC32_ziparddll =  -L$(BCB)\LIB -Tpe -ap -c
ResLocalOptsAtC32_ziparddll = 
BLocalOptsAtC32_ziparddll = 
CompInheritOptsAt_ziparddll = -I$(BCB)\INCLUDE -DWIN32 -u -tWM
LinkerInheritOptsAt_ziparddll = -x
LinkerOptsAt_ziparddll = $(LinkerLocalOptsAtC32_ziparddll)
ResOptsAt_ziparddll = $(ResLocalOptsAtC32_ziparddll)
BOptsAt_ziparddll = $(BLocalOptsAtC32_ziparddll) 

#
# Dependency List
#
Dep_zipar = \
   ziparchive.lib

ziparchive : BccW32.cfg $(Dep_zipar)
  echo MakeNode

Dep_ziparddll = \   
   ziparchive.obj\
   zutil.obj\
   uncompr.obj\
   adler32.obj\
   inftrees.obj\
   inflate.obj\
   inffast.obj\
   infback.obj\
   trees.obj\
   deflate.obj\
   crc32.obj\
   compress.obj\
   zipstring.obj\
   zipautobuffer.obj\
   zipplatform_win.obj\
   zipplatform_lnx.obj\
   zipstorage.obj\
   zipplatformcomm.obj\
   zippathcomponent_win.obj\
   zippathcomponent_lnx.obj\
   zipmemfile.obj\
   zipfileheader.obj\
   zipfile_stl.obj\
   zipfile_mfc.obj\
   zipexception.obj\
   zipcompatibility.obj\
   zipcentraldir.obj\   
   byteswriter.obj\
   zipextradata.obj\
   zipextrafield.obj\
   zipcryptograph.obj\
   zipcrc32cryptograph.obj\
   aes.obj\
   hmac.obj\
   randompool.obj\
   zipaescryptograph.obj\
   sha1.obj


ziparchive.lib : $(Dep_ziparddll)
  $(TLIB) $< $(IDE_BFLAGS) /P32  $(BOptsAt_ziparddll) @&&|
-+zutil.obj &
-+uncompr.obj &
-+adler32.obj &
-+trees.obj &
-+inftrees.obj &
-+inflate.obj &
-+inffast.obj &
-+infback.obj &
-+deflate.obj &
-+crc32.obj &
-+compress.obj &
-+makebcdll.obj &
-+zipstring.obj &
-+zipautobuffer.obj &
-+zipstorage.obj &
-+zipplatformcomm.obj &
-+zipplatform_win.obj &
-+zipplatform_lnx.obj &
-+zippathcomponent_win.obj &
-+zippathcomponent_lnx.obj &
-+zipmemfile.obj &
-+zipfileheader.obj &
-+zipfile_stl.obj &
-+zipfile_mfc.obj &
-+zipexception.obj &
-+zipcompatibility.obj &
-+zipcentraldir.obj &
-+ziparchive.obj &
-+byteswriter.obj &
-+zipextradata.obj &
-+zipextrafield.obj &
-+zipcryptograph.obj &
-+zipcrc32cryptograph.obj &
-+aes.obj &
-+hmac.obj &
-+randompool.obj &
-+zipaescryptograph.obj &
-+sha1.obj
|

zutil.obj :  zlib\zutil.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_ziparddll) $(CompInheritOptsAt_ziparddll) -DZLIB_DLL -o$@ zlib\zutil.c
|

uncompr.obj :  zlib\uncompr.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_ziparddll) $(CompInheritOptsAt_ziparddll) -DZLIB_DLL -o$@ zlib\uncompr.c
|

adler32.obj :  zlib\adler32.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_ziparddll) $(CompInheritOptsAt_ziparddll) -DZLIB_DLL  -o$@ zlib\adler32.c
|

trees.obj :  zlib\trees.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_ziparddll) $(CompInheritOptsAt_ziparddll) -DZLIB_DLL  -o$@ zlib\trees.c
|

inftrees.obj :  zlib\inftrees.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_ziparddll) $(CompInheritOptsAt_ziparddll) -DZLIB_DLL  -o$@ zlib\inftrees.c
|

inflate.obj :  zlib\inflate.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_ziparddll) $(CompInheritOptsAt_ziparddll) -DZLIB_DLL  -o$@ zlib\inflate.c
|

inffast.obj :  zlib\inffast.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_ziparddll) $(CompInheritOptsAt_ziparddll) -DZLIB_DLL  -o$@ zlib\inffast.c
|

infback.obj :  zlib\infback.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_ziparddll) $(CompInheritOptsAt_ziparddll) -DZLIB_DLL  -o$@ zlib\infback.c
|


deflate.obj :  zlib\deflate.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_ziparddll) $(CompInheritOptsAt_ziparddll) -DZLIB_DLL  -o$@ zlib\deflate.c
|

crc32.obj :  zlib\crc32.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_ziparddll) $(CompInheritOptsAt_ziparddll) -DZLIB_DLL  -o$@ zlib\crc32.c
|

compress.obj :  zlib\compress.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_ziparddll) $(CompInheritOptsAt_ziparddll) -DZLIB_DLL  -o$@ zlib\compress.c
|

zipstring.obj :  zipstring.cpp
  $(BCC32) -c  @&&|
 $(CompOptsAt_ziparddll) $(CompInheritOptsAt_ziparddll) -o$@ zipstring.cpp
|

zipstorage.obj :  zipstorage.cpp
  $(BCC32) -c  @&&|
 $(CompOptsAt_ziparddll) $(CompInheritOptsAt_ziparddll) -o$@ zipstorage.cpp
|

zipplatformcomm.obj :  zipplatformcomm.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_ziparddll) $(CompInheritOptsAt_ziparddll) -o$@ zipplatformcomm.cpp
|

zipplatform_win.obj :  zipplatform_win.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_ziparddll) $(CompInheritOptsAt_ziparddll) -o$@ zipplatform_win.cpp
|

zipplatform_lnx.obj :  zipplatform_lnx.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_ziparddll) $(CompInheritOptsAt_ziparddll) -o$@ zipplatform_lnx.cpp
|

zippathcomponent_win.obj :  zippathcomponent_win.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_ziparddll) $(CompInheritOptsAt_ziparddll) -o$@ zippathcomponent_win.cpp
|

zippathcomponent_lnx.obj :  zippathcomponent_lnx.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_ziparddll) $(CompInheritOptsAt_ziparddll) -o$@ zippathcomponent_lnx.cpp
|

zipmemfile.obj :  zipmemfile.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_ziparddll) $(CompInheritOptsAt_ziparddll) -o$@ zipmemfile.cpp
|

zipfileheader.obj :  zipfileheader.cpp
  $(BCC32) -c  @&&|
 $(CompOptsAt_ziparddll) $(CompInheritOptsAt_ziparddll) -o$@ zipfileheader.cpp
|

zipfile_stl.obj :  zipfile_stl.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_ziparddll) $(CompInheritOptsAt_ziparddll) -o$@ zipfile_stl.cpp
|

zipfile_mfc.obj :  zipfile_mfc.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_ziparddll) $(CompInheritOptsAt_ziparddll) -o$@ zipfile_mfc.cpp
|

zipexception.obj :  zipexception.cpp
  $(BCC32)  -c @&&|
 $(CompOptsAt_ziparddll) $(CompInheritOptsAt_ziparddll) -o$@ zipexception.cpp
|

zipcompatibility.obj :  zipcompatibility.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_ziparddll) $(CompInheritOptsAt_ziparddll) -o$@ zipcompatibility.cpp
|


zipcentraldir.obj :  zipcentraldir.cpp
  $(BCC32) -c   @&&|
 $(CompOptsAt_ziparddll) $(CompInheritOptsAt_ziparddll) -o$@ zipcentraldir.cpp
|

zipautobuffer.obj :  zipautobuffer.cpp
  $(BCC32) -c   @&&|
 $(CompOptsAt_ziparddll) $(CompInheritOptsAt_ziparddll) -o$@ zipautobuffer.cpp
|

byteswriter.obj : byteswriter.cpp
  $(BCC32) -c   @&&|
 $(CompOptsAt_ziparddll) $(CompInheritOptsAt_ziparddll) -o$@ byteswriter.cpp
|

zipextradata.obj : zipextradata.cpp
  $(BCC32) -c   @&&|
 $(CompOptsAt_ziparddll) $(CompInheritOptsAt_ziparddll) -o$@ zipextradata.cpp
|

zipextrafield.obj : zipextrafield.cpp
  $(BCC32) -c   @&&|
 $(CompOptsAt_ziparddll) $(CompInheritOptsAt_ziparddll) -o$@ zipextrafield.cpp
|

zipcryptograph.obj : zipcryptograph.cpp
  $(BCC32) -c   @&&|
 $(CompOptsAt_ziparddll) $(CompInheritOptsAt_ziparddll) -o$@ zipcryptograph.cpp
|

zipcrc32cryptograph.obj : zipcrc32cryptograph.cpp
  $(BCC32) -c   @&&|
 $(CompOptsAt_ziparddll) $(CompInheritOptsAt_ziparddll) -o$@ zipcrc32cryptograph.cpp
|

aes.obj : aes.cpp
  $(BCC32) -c   @&&|
 $(CompOptsAt_ziparddll) $(CompInheritOptsAt_ziparddll) -o$@ aes.cpp
|

hmac.obj : hmac.cpp
  $(BCC32) -c   @&&|
 $(CompOptsAt_ziparddll) $(CompInheritOptsAt_ziparddll) -o$@ hmac.cpp
|

randompool.obj : randompool.cpp
  $(BCC32) -c   @&&|
 $(CompOptsAt_ziparddll) $(CompInheritOptsAt_ziparddll) -o$@ randompool.cpp
|

zipaescryptograph.obj : zipaescryptograph.cpp
  $(BCC32) -c   @&&|
 $(CompOptsAt_ziparddll) $(CompInheritOptsAt_ziparddll) -o$@ zipaescryptograph.cpp
|

sha1.obj : sha1.cpp
  $(BCC32) -c   @&&|
 $(CompOptsAt_ziparddll) $(CompInheritOptsAt_ziparddll) -o$@ sha1.cpp
|

ziparchive.obj :  ziparchive.cpp
  $(BCC32) -c   @&&|
 $(CompOptsAt_ziparddll) $(CompInheritOptsAt_ziparddll) -o$@ ziparchive.cpp
|



# Compiler configuration file
BccW32.cfg : 
   Copy &&|
-vi
-Ve
-R-
-H-
-5
-OS
-w-
-O2  -X- -a8 -b -k-  
| $@


