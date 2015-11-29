#----- Include the PSDK's WIN32.MAK to pick up defines------------------------------------
!include <win32.mak>

#----- OUTDIR is defined in WIN32.MAK This is the name of the destination directory-------
all: $(OUTDIR)\ShellExt.dll

LINK32_OBJS= \
	$(OUTDIR)\CopyHook.Obj    \
	$(OUTDIR)\CtxMenu.Obj     \
	$(OUTDIR)\IconHdlr.Obj    \
	$(OUTDIR)\PropShet.Obj    \
	$(OUTDIR)\ShellExt.Obj    \
	$(OUTDIR)\ShExInit.Obj    
        
#----- If OUTDIR does not exist, then create directory
$(OUTDIR) :
    if not exist "$(OUTDIR)/$(NULL)" mkdir $(OUTDIR)

# Build rule for DLL
$(OUTDIR)\ShellExt.dll: $(OUTDIR) $(LINK32_OBJS) $(OUTDIR)\ShellExt.res
	$(implib) -machine:$(CPU) -def:ShellExt.Def $(LINK32_OBJS) -out:$(OUTDIR)\ShellExt.Lib
    $(link) $(ldebug) $(dllllflags) \
    $(LINK32_OBJS) $(OUTDIR)\ShellExt.res $(olelibsdll) shell32.Lib $(OUTDIR)\ShellExt.Lib comctl32.Lib $(OUTDIR)\ShellExt.exp \
    -base:0x1C000000 \
    -out:$(OUTDIR)\ShellExt.dll 
    
# Generic rule for building ALL CPP files and placing their OBJ's in the OUTDIR
.cpp{$(OUTDIR)}.obj:
    $(cc) $(cdebug) $(cflags) $(cvarsmt) /Fo"$(OUTDIR)\\" /Fd"$(OUTDIR)\\" $**

# Build rule for resource file
SOURCE=.\ShellExt.rc

$(OUTDIR)\ShellExt.res: $(SOURCE) $(OUTDIR)
	$(rc) $(rcflags) $(rcvars) /fo $(OUTDIR)\ShellExt.res $(SOURCE)

#--------------------- Clean Rule --------------------------------------------------------
# Rules for cleaning out those old files
clean:
        $(CLEANUP)

