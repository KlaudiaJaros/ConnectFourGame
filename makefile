
install:
	cl connectFour.c linkedList.c
	cl /c linkedList.c
	lib linkedList.obj
	link connectFour.obj linkedList.lib

delete:
	del *.c *.obj *.exe *.lib
	
clear:
	del *.obj *.exe *.lib

run:
	connectFour.exe