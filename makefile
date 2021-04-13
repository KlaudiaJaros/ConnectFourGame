
install:
	cl connectFour.c linkedList.c doublyLinkedList.c
	cl /c linkedList.c doublyLinkedList.c
	lib linkedList.obj doublyLinkedList.obj
	link connectFour.obj linkedList.lib 

delete:
	del *.c *.obj *.exe *.lib
	
clear:
	del *.obj *.exe *.lib

run:
	connectFour.exe