
install:
	cl connectFour.c linkedList.c doublyLinkedList.c stack.c
	cl /c linkedList.c doublyLinkedList.c stack.c
	lib linkedList.obj doublyLinkedList.obj stack.obj
	link connectFour.obj linkedList.lib 

delete:
	del *.c *.obj *.exe *.lib
	
clear:
	del *.obj *.exe *.lib

run:
	connectFour.exe