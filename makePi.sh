gcc src/main.c -w -lOpenVG -lglut -lluajit -Wl,-rpath='$ORIGIN' -o ./bin/pi/Diököl -L./lib/pi -I./include
