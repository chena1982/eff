all : SurfaceShaderParser.exe


SurfaceShaderParser.exe : lex.yy.obj
	link lex.yy.obj /SUBSYSTEM:CONSOLE /LIBPATH:"$(LIBPATH)" 

lex.yy.obj : lex.yy.c
	cl lex.yy.c /I "$(VCINSTALLDIR)\Include"


