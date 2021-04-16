
install:
	cl connectFour.c gameHistory.c stack.c
	cl /c gameHistory.c stack.c
	lib gameHistory.obj stack.obj
	link connectFour.obj gameHistory.lib 

delete:
	del *.c *.obj *.exe *.lib
	
clear:
	del *.obj *.exe *.lib

run:
	connectFour.exe