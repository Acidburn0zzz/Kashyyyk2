# Contains CLFLAGS for the YYY subprojects

!IF ("$(MODE)"=="RELEASE")
DEBUGMACRO=/DNDEBUG
OPTFLAGS=/O2 /GL
!ENDIF

!IF ("$(MODE)"=="DEBUG")
OPTFLAGS=/Od
LIBSUFFIX=d
!ENDIF

!IF ("$(MODE)"=="RDEBUG")
OPTFLAGS=/O1 /GL
LIBSUFFIX=rd
!ENDIF

CLFLAGS=/nologo $(DEBUGMACRO) $(OPTFLAGS) /M$(CRT)
