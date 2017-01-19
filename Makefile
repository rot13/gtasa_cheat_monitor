all: gtasa_cheat_monitor.exe

gtasa_cheat_monitor.exe : gtasa_cheat_monitor.c
	i686-w64-mingw32-gcc -std=c99 -Os -Wall -Wextra \
	-isystem nuklear -isystem nuklear/demo/gdi \
	gtasa_cheat_monitor.c -o gtasa_cheat_monitor.exe -s -mwindows
clean:
	rm *.exe
