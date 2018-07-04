# Microsoft Developer Studio Generated NMAKE File, Based on diabhack.dsp
!IF "$(CFG)" == ""
CFG=diabhack - Win32 Release
!MESSAGE No configuration specified. Defaulting to diabhack - Win32 Release.
!ENDIF 

!IF "$(CFG)" != "diabhack - Win32 Release" && "$(CFG)" != "diabhack - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "diabhack.mak" CFG="diabhack - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "diabhack - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "diabhack - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "diabhack - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# Begin Custom Macros
OutDir=.\Release
# End Custom Macros

ALL : "$(OUTDIR)\diabhack.exe"


CLEAN :
	-@erase "$(INTDIR)\__vcpch.obj"
	-@erase "$(INTDIR)\data.obj"
	-@erase "$(INTDIR)\datfile.obj"
	-@erase "$(INTDIR)\debug.obj"
	-@erase "$(INTDIR)\diabhack.pch"
	-@erase "$(INTDIR)\diabhack.res"
	-@erase "$(INTDIR)\dialog.obj"
	-@erase "$(INTDIR)\hack.obj"
	-@erase "$(INTDIR)\hacklist.obj"
	-@erase "$(INTDIR)\hacknode.obj"
	-@erase "$(INTDIR)\listview.obj"
	-@erase "$(INTDIR)\main.obj"
	-@erase "$(INTDIR)\proc.obj"
	-@erase "$(INTDIR)\registry.obj"
	-@erase "$(INTDIR)\toolbar.obj"
	-@erase "$(INTDIR)\utils.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\diabhack.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /G5 /ML /W3 /GX /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /Fp"$(INTDIR)\diabhack.pch" /Yu"diabhack.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x809 /fo"$(INTDIR)\diabhack.res" /d "NDEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\diabhack.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=comctl32.lib version.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /version:1.0 /subsystem:windows /incremental:no /pdb:"$(OUTDIR)\diabhack.pdb" /machine:I386 /out:"$(OUTDIR)\diabhack.exe" 
LINK32_OBJS= \
	"$(INTDIR)\__vcpch.obj" \
	"$(INTDIR)\data.obj" \
	"$(INTDIR)\datfile.obj" \
	"$(INTDIR)\debug.obj" \
	"$(INTDIR)\dialog.obj" \
	"$(INTDIR)\hack.obj" \
	"$(INTDIR)\hacklist.obj" \
	"$(INTDIR)\hacknode.obj" \
	"$(INTDIR)\listview.obj" \
	"$(INTDIR)\main.obj" \
	"$(INTDIR)\proc.obj" \
	"$(INTDIR)\registry.obj" \
	"$(INTDIR)\toolbar.obj" \
	"$(INTDIR)\utils.obj" \
	"$(INTDIR)\diabhack.res"

"$(OUTDIR)\diabhack.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "diabhack - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# Begin Custom Macros
OutDir=.\Debug
# End Custom Macros

ALL : "$(OUTDIR)\diabhack.exe" "$(OUTDIR)\diabhack.bsc"


CLEAN :
	-@erase "$(INTDIR)\__vcpch.obj"
	-@erase "$(INTDIR)\__vcpch.sbr"
	-@erase "$(INTDIR)\data.obj"
	-@erase "$(INTDIR)\data.sbr"
	-@erase "$(INTDIR)\datfile.obj"
	-@erase "$(INTDIR)\datfile.sbr"
	-@erase "$(INTDIR)\debug.obj"
	-@erase "$(INTDIR)\debug.sbr"
	-@erase "$(INTDIR)\diabhack.pch"
	-@erase "$(INTDIR)\diabhack.res"
	-@erase "$(INTDIR)\dialog.obj"
	-@erase "$(INTDIR)\dialog.sbr"
	-@erase "$(INTDIR)\hack.obj"
	-@erase "$(INTDIR)\hack.sbr"
	-@erase "$(INTDIR)\hacklist.obj"
	-@erase "$(INTDIR)\hacklist.sbr"
	-@erase "$(INTDIR)\hacknode.obj"
	-@erase "$(INTDIR)\hacknode.sbr"
	-@erase "$(INTDIR)\listview.obj"
	-@erase "$(INTDIR)\listview.sbr"
	-@erase "$(INTDIR)\main.obj"
	-@erase "$(INTDIR)\main.sbr"
	-@erase "$(INTDIR)\proc.obj"
	-@erase "$(INTDIR)\proc.sbr"
	-@erase "$(INTDIR)\registry.obj"
	-@erase "$(INTDIR)\registry.sbr"
	-@erase "$(INTDIR)\toolbar.obj"
	-@erase "$(INTDIR)\toolbar.sbr"
	-@erase "$(INTDIR)\utils.obj"
	-@erase "$(INTDIR)\utils.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\diabhack.bsc"
	-@erase "$(OUTDIR)\diabhack.exe"
	-@erase "$(OUTDIR)\diabhack.ilk"
	-@erase "$(OUTDIR)\diabhack.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /G4 /MLd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\diabhack.pch" /Yu"diabhack.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x809 /fo"$(INTDIR)\diabhack.res" /d "_DEBUG" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\diabhack.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\__vcpch.sbr" \
	"$(INTDIR)\data.sbr" \
	"$(INTDIR)\datfile.sbr" \
	"$(INTDIR)\debug.sbr" \
	"$(INTDIR)\dialog.sbr" \
	"$(INTDIR)\hack.sbr" \
	"$(INTDIR)\hacklist.sbr" \
	"$(INTDIR)\hacknode.sbr" \
	"$(INTDIR)\listview.sbr" \
	"$(INTDIR)\main.sbr" \
	"$(INTDIR)\proc.sbr" \
	"$(INTDIR)\registry.sbr" \
	"$(INTDIR)\toolbar.sbr" \
	"$(INTDIR)\utils.sbr"

"$(OUTDIR)\diabhack.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=comctl32.lib version.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)\diabhack.pdb" /debug /machine:I386 /out:"$(OUTDIR)\diabhack.exe" 
LINK32_OBJS= \
	"$(INTDIR)\__vcpch.obj" \
	"$(INTDIR)\data.obj" \
	"$(INTDIR)\datfile.obj" \
	"$(INTDIR)\debug.obj" \
	"$(INTDIR)\dialog.obj" \
	"$(INTDIR)\hack.obj" \
	"$(INTDIR)\hacklist.obj" \
	"$(INTDIR)\hacknode.obj" \
	"$(INTDIR)\listview.obj" \
	"$(INTDIR)\main.obj" \
	"$(INTDIR)\proc.obj" \
	"$(INTDIR)\registry.obj" \
	"$(INTDIR)\toolbar.obj" \
	"$(INTDIR)\utils.obj" \
	"$(INTDIR)\diabhack.res"

"$(OUTDIR)\diabhack.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("diabhack.dep")
!INCLUDE "diabhack.dep"
!ELSE 
!MESSAGE Warning: cannot find "diabhack.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "diabhack - Win32 Release" || "$(CFG)" == "diabhack - Win32 Debug"
SOURCE=.\__vcpch.cpp

!IF  "$(CFG)" == "diabhack - Win32 Release"

CPP_SWITCHES=/nologo /G5 /ML /W3 /GX /O1 /Ob2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /Fp"$(INTDIR)\diabhack.pch" /Yc"diabhack.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\__vcpch.obj"	"$(INTDIR)\diabhack.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "diabhack - Win32 Debug"

CPP_SWITCHES=/nologo /G4 /MLd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\diabhack.pch" /Yc"diabhack.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\__vcpch.obj"	"$(INTDIR)\__vcpch.sbr"	"$(INTDIR)\diabhack.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\data.cpp

!IF  "$(CFG)" == "diabhack - Win32 Release"

CPP_SWITCHES=/nologo /G5 /ML /W3 /GX /O1 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /Fp"$(INTDIR)\diabhack.pch" /Yu"diabhack.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\data.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\diabhack.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "diabhack - Win32 Debug"

CPP_SWITCHES=/nologo /G4 /MLd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\diabhack.pch" /Yu"diabhack.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\data.obj"	"$(INTDIR)\data.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\diabhack.pch"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 

SOURCE=.\datfile.cpp

!IF  "$(CFG)" == "diabhack - Win32 Release"


"$(INTDIR)\datfile.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\diabhack.pch"


!ELSEIF  "$(CFG)" == "diabhack - Win32 Debug"


"$(INTDIR)\datfile.obj"	"$(INTDIR)\datfile.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\diabhack.pch"


!ENDIF 

SOURCE=.\debug.cpp

!IF  "$(CFG)" == "diabhack - Win32 Release"


"$(INTDIR)\debug.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\diabhack.pch"


!ELSEIF  "$(CFG)" == "diabhack - Win32 Debug"


"$(INTDIR)\debug.obj"	"$(INTDIR)\debug.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\diabhack.pch"


!ENDIF 

SOURCE=.\diabhack.rc

"$(INTDIR)\diabhack.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\dialog.cpp

!IF  "$(CFG)" == "diabhack - Win32 Release"


"$(INTDIR)\dialog.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\diabhack.pch"


!ELSEIF  "$(CFG)" == "diabhack - Win32 Debug"


"$(INTDIR)\dialog.obj"	"$(INTDIR)\dialog.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\diabhack.pch"


!ENDIF 

SOURCE=.\hack.cpp

!IF  "$(CFG)" == "diabhack - Win32 Release"


"$(INTDIR)\hack.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\diabhack.pch"


!ELSEIF  "$(CFG)" == "diabhack - Win32 Debug"


"$(INTDIR)\hack.obj"	"$(INTDIR)\hack.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\diabhack.pch"


!ENDIF 

SOURCE=.\hacklist.cpp

!IF  "$(CFG)" == "diabhack - Win32 Release"


"$(INTDIR)\hacklist.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\diabhack.pch"


!ELSEIF  "$(CFG)" == "diabhack - Win32 Debug"


"$(INTDIR)\hacklist.obj"	"$(INTDIR)\hacklist.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\diabhack.pch"


!ENDIF 

SOURCE=.\hacknode.cpp

!IF  "$(CFG)" == "diabhack - Win32 Release"


"$(INTDIR)\hacknode.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\diabhack.pch"


!ELSEIF  "$(CFG)" == "diabhack - Win32 Debug"


"$(INTDIR)\hacknode.obj"	"$(INTDIR)\hacknode.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\diabhack.pch"


!ENDIF 

SOURCE=.\listview.cpp

!IF  "$(CFG)" == "diabhack - Win32 Release"


"$(INTDIR)\listview.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\diabhack.pch"


!ELSEIF  "$(CFG)" == "diabhack - Win32 Debug"


"$(INTDIR)\listview.obj"	"$(INTDIR)\listview.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\diabhack.pch"


!ENDIF 

SOURCE=.\main.cpp

!IF  "$(CFG)" == "diabhack - Win32 Release"


"$(INTDIR)\main.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\diabhack.pch"


!ELSEIF  "$(CFG)" == "diabhack - Win32 Debug"


"$(INTDIR)\main.obj"	"$(INTDIR)\main.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\diabhack.pch"


!ENDIF 

SOURCE=.\proc.cpp

!IF  "$(CFG)" == "diabhack - Win32 Release"


"$(INTDIR)\proc.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\diabhack.pch"


!ELSEIF  "$(CFG)" == "diabhack - Win32 Debug"


"$(INTDIR)\proc.obj"	"$(INTDIR)\proc.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\diabhack.pch"


!ENDIF 

SOURCE=.\registry.cpp

!IF  "$(CFG)" == "diabhack - Win32 Release"


"$(INTDIR)\registry.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\diabhack.pch"


!ELSEIF  "$(CFG)" == "diabhack - Win32 Debug"


"$(INTDIR)\registry.obj"	"$(INTDIR)\registry.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\diabhack.pch"


!ENDIF 

SOURCE=.\toolbar.cpp

!IF  "$(CFG)" == "diabhack - Win32 Release"


"$(INTDIR)\toolbar.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\diabhack.pch"


!ELSEIF  "$(CFG)" == "diabhack - Win32 Debug"


"$(INTDIR)\toolbar.obj"	"$(INTDIR)\toolbar.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\diabhack.pch"


!ENDIF 

SOURCE=.\utils.cpp

!IF  "$(CFG)" == "diabhack - Win32 Release"


"$(INTDIR)\utils.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\diabhack.pch"


!ELSEIF  "$(CFG)" == "diabhack - Win32 Debug"


"$(INTDIR)\utils.obj"	"$(INTDIR)\utils.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\diabhack.pch"


!ENDIF 


!ENDIF 

