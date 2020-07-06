x86_64-w64-mingw32-gcc src/main.c -lopengl32 -lglu32 -lgdi32 -lOpenVG -lglut -llua51 -Wl,-subsystem,windows -o bin/windows/Diököl.exe -L./lib/win64 -I./include -w
