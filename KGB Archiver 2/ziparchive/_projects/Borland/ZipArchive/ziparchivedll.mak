#
# Borland C++ IDE generated makefile
# Generated 01/04/01 at 12:11:49 
#
.AUTODEPEND


#
# Borland C++ tools
#
IMPLIB  = Implib
BCC32   = Bcc32 +BccW32.cfg
BCC321   = Bcc32 +BccW321.cfg 
TLINK32 = iLink32
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
LinkerLocalOptsAtC32_zipardlib =  -Tpd -ap -c -L$(BCB)\LIB 
ResLocalOptsAtC32_zipardlib = 
BLocalOptsAtC32_zipardlib = 
CompInheritOptsAt_zipardlib =  -I$(BCB)\INCLUDE;\zlib -DNDEBUG;_MBCS;WIN32;_WINDOWS;ZLIB_DLL;_NO_VCL;_ASSERTE;PKZIP_BUG_WORKAROUND;NO_STRICT
LinkerInheritOptsAt_zipardlib = -x 
LinkerOptsAt_zipardlib = $(LinkerLocalOptsAtC32_zipardlib)
ResOptsAt_zipardlib = $(ResLocalOptsAtC32_zipardlib)
BOptsAt_zipardlib = $(BLocalOptsAtC32_zipardlib)

#
# Dependency List
#
Dep_zipar = \
   ziparchive1.lib

zipar : BccW32.cfg $(Dep_zipar)
  echo MakeNode

ziparchive1.lib : ziparchive.dll
  $(IMPLIB) $@ ziparchive.dll


Dep_ziparddll = \
   zutil.obj\
   uncompr.obj\
   adler32.obj\
   trees.obj\
   inftrees.obj\
   inflate.obj\
   inffast.obj\
   infback.obj\
   deflate.obj\
   crc32.obj\
   compress.obj\
   zipstring.obj\
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
   zipautobuffer.obj\
   ziparchive.obj\
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

ziparchive.dll : $(Dep_ziparddll)
  $(TLINK32) @&&|
  $(IDE_LinkFLAGS32) $(LinkerOptsAt_zipardlib) $(LinkerInheritOptsAt_zipardlib) +
$(BCB)\LIB\c0d32.obj+
zutil.obj+
uncompr.obj+
adler32.obj+
trees.obj +
inftrees.obj+
inflate.obj+
inffast.obj+
infback.obj+
deflate.obj+
crc32.obj+
compress.obj+
zipstring.obj+
zipstorage.obj+
zipplatformcomm.obj+
zipplatform_win.obj+
zipplatform_lnx.obj+
zippathcomponent_win.obj+
zippathcomponent_lnx.obj+
zipmemfile.obj+
zipfileheader.obj+
zipfile_stl.obj+
zipfile_mfc.obj+
zipexception.obj+
zipcompatibility.obj+
zipcentraldir.obj+
zipautobuffer.obj+
ziparchive.obj+
byteswriter.obj+
zipextradata.obj+
zipextrafield.obj+
zipcryptograph.obj+
zipcrc32cryptograph.obj+
aes.obj+
hmac.obj+
randompool.obj+
zipaescryptograph.obj+
sha1.obj
$<,$*
$(BCB)\LIB\import32.lib+
$(BCB)\LIB\cw32.lib

|

zutil.obj :  zlib\zutil.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_zipardlib) $(CompInheritOptsAt_zipardlib) -o$@ zlib\zutil.c
|


uncompr.obj :  zlib\uncompr.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_zipardlib) $(CompInheritOptsAt_zipardlib) -o$@ zlib\uncompr.c
|

adler32.obj :  zlib\adler32.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_zipardlib) $(CompInheritOptsAt_zipardlib) -o$@ zlib\adler32.c
|

trees.obj :  zlib\trees.c
  $(BCC32) -P- -c  @&&|
 $(CompOptsAt_zipardlib) $(CompInheritOptsAt_zipardlib) -o$@ zlib\trees.c
|
inftrees.obj :  zlib\inftrees.c
  $(BCC32) -P- -c  @&&|
 $(CompOptsAt_zipardlib) $(CompInheritOptsAt_zipardlib) -o$@ zlib\inftrees.c
|

inflate.obj :  zlib\inflate.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_zipardlib) $(CompInheritOptsAt_zipardlib) -o$@ zlib\inflate.c
|

inffast.obj :  zlib\inffast.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_zipardlib) $(CompInheritOptsAt_zipardlib) -o$@ zlib\inffast.c
|

infback.obj :  zlib\infback.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_zipardlib) $(CompInheritOptsAt_zipardlib) -o$@ zlib\infback.c
|

deflate.obj :  zlib\deflate.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_zipardlib) $(CompInheritOptsAt_zipardlib) -o$@ zlib\deflate.c
|

crc32.obj :  zlib\crc32.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_zipardlib) $(CompInheritOptsAt_zipardlib) -o$@ zlib\crc32.c
|

compress.obj :  zlib\compress.c
  $(BCC32) -P- -c @&&|
 $(CompOptsAt_zipardlib) $(CompInheritOptsAt_zipardlib) -o$@ zlib\compress.c
|

makebcdll.obj :  makebcdll.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_zipardlib) $(CompInheritOptsAt_zipardlib) -o$@ makebcdll.cpp
|

zipstring.obj :  zipstring.cpp
  $(BCC32) -c  @&&|
 $(CompOptsAt_zipardlib) $(CompInheritOptsAt_zipardlib) -o$@ zipstring.cpp
|

zipstorage.obj :  zipstorage.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_zipardlib) $(CompInheritOptsAt_zipardlib) -o$@ zipstorage.cpp
|

zipplatformcomm.obj :  zipplatformcomm.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_zipardlib) $(CompInheritOptsAt_zipardlib) -o$@ zipplatformcomm.cpp
|

zipplatform_win.obj :  zipplatform_win.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_zipardlib) $(CompInheritOptsAt_zipardlib) -o$@ zipplatform_win.cpp
|

zipplatform_lnx.obj :  zipplatform_lnx.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_zipardlib) $(CompInheritOptsAt_zipardlib) -o$@ zipplatform_lnx.cpp
|

zippathcomponent_win.obj :  zippathcomponent_win.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_zipardlib) $(CompInheritOptsAt_zipardlib) -o$@ zippathcomponent_win.cpp
|

zippathcomponent_lnx.obj :  zippathcomponent_lnx.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_zipardlib) $(CompInheritOptsAt_zipardlib) -o$@ zippathcomponent_lnx.cpp
|

zipmemfile.obj :  zipmemfile.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_zipardlib) $(CompInheritOptsAt_zipardlib) -o$@ zipmemfile.cpp
|

zipfileheader.obj :  zipfileheader.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_zipardlib) $(CompInheritOptsAt_zipardlib) -o$@ zipfileheader.cpp
|

zipfile_stl.obj :  zipfile_stl.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_zipardlib) $(CompInheritOptsAt_zipardlib) -o$@ zipfile_stl.cpp
|

zipfile_mfc.obj :  zipfile_mfc.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_zipardlib) $(CompInheritOptsAt_zipardlib) -o$@ zipfile_mfc.cpp
|

zipexception.obj :  zipexception.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_zipardlib) $(CompInheritOptsAt_zipardlib) -o$@ zipexception.cpp
|

zipcompatibility.obj :  zipcompatibility.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_zipardlib) $(CompInheritOptsAt_zipardlib) -o$@ zipcompatibility.cpp
|


zipcentraldir.obj :  zipcentraldir.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_zipardlib) $(CompInheritOptsAt_zipardlib) -o$@ zipcentraldir.cpp
|

zipautobuffer.obj :  zipautobuffer.cpp
  $(BCC32) -c @&&|
 $(CompOptsAt_zipardlib) $(CompInheritOptsAt_zipardlib) -o$@ zipautobuffer.cpp
|

ziparchive.obj :  ziparchive.cpp
  $(BCC32) -c  @&&|
 $(CompOptsAt_zipardlib) $(CompInheritOptsAt_zipardlib) -o$@ ziparchive.cpp
|

byteswriter.obj : byteswriter.cpp
  $(BCC32) -c   @&&|
 $(CompOptsAt_zipardlib) $(CompInheritOptsAt_zipardlib) -o$@ byteswriter.cpp
|

zipextradata.obj : zipextradata.cpp
  $(BCC32) -c   @&&|
 $(CompOptsAt_zipardlib) $(CompInheritOptsAt_zipardlib) -o$@ zipextradata.cpp
|

zipextrafield.obj : zipextrafield.cpp
  $(BCC32) -c   @&&|
 $(CompOptsAt_zipardlib) $(CompInheritOptsAt_zipardlib) -o$@ zipextrafield.cpp
|

zipcryptograph.obj : zipcryptograph.cpp
  $(BCC32) -c   @&&|
 $(CompOptsAt_zipardlib) $(CompInheritOptsAt_zipardlib) -o$@ zipcryptograph.cpp
|

zipcrc32cryptograph.obj : zipcrc32cryptograph.cpp
  $(BCC32) -c   @&&|
 $(CompOptsAt_zipardlib) $(CompInheritOptsAt_zipardlib) -o$@ zipcrc32cryptograph.cpp
|

aes.obj : aes.cpp
  $(BCC32) -c   @&&|
 $(CompOptsAt_zipardlib) $(CompInheritOptsAt_zipardlib) -o$@ aes.cpp
|

hmac.obj : hmac.cpp
  $(BCC32) -c   @&&|
 $(CompOptsAt_zipardlib) $(CompInheritOptsAt_zipardlib) -o$@ hmac.cpp
|

randompool.obj : randompool.cpp
  $(BCC32) -c   @&&|
 $(CompOptsAt_zipardlib) $(CompInheritOptsAt_zipardlib) -o$@ randompool.cpp
|

zipaescryptograph.obj : zipaescryptograph.cpp
  $(BCC32) -c   @&&|
 $(CompOptsAt_zipardlib) $(CompInheritOptsAt_zipardlib) -o$@ zipaescryptograph.cpp
|

sha1.obj : sha1.cpp
  $(BCC32) -c   @&&|
 $(CompOptsAt_zipardlib) $(CompInheritOptsAt_zipardlib) -o$@ sha1.cpp
|

# Compiler configuration file
BccW32.cfg : 
   Copy &&|
-w-
-R
-vi
-v-
-R-
-H-
-5
-a8
-O2
-OS
| $@

BccW321.cfg : 
   Copy &&|
-w-
-R
-vi
-R-
-H-
-5
-a8
-O2
-OS
| $@


