all:
	gcc main.c -Lx86-windows\bin -Ix86-windows\include -lSDL2 -lSDL2_image -o x86-windows\bin\a.exe -g