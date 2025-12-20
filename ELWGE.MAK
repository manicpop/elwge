###############################################################################
#####               This makefile generates ELWGE.DLL                     #####
###############################################################################
!include $(WGDEV)\src\gcrules.mak

PARSEOPTS = $(PARSEOPTS) -DIS30

DEFILE = 

VIRFILES =

DLLLIB =              \
     $(DLLLIB)        \
     wgserver.lib     \
     galme.lib        \
     galgsbl.lib

OBJFILES =          \
     gemain.obj     \
     gefuncs.obj    \
     gecmds.obj     \
     gecybs.obj     \
     geplanet.obj   \
     gedroids.obj   \
     gelib.obj   

start:              \
     $(GCBUILD)     \
     elwge.dll

elwge.dll:         \
     elwge.h      \
     elwgehlp.h      \
     elwgeshp.h      \
     $(OBJFILES)    \
     $(DLLLIB)      \
     $(DEFILE)      \
     $(VIRFILES)
     $(LINK) $(LIBCMD) @&&|
$(PRODLL) $(OBJFILES)
$(RUNDIR)\$&
$(RUNDIR)\$&
$(DLLLIB) $(LINKDLL)
$(DEFILE)
|

