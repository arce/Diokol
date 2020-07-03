gcc src/main.c -w -framework OpenGL -framework GLUT -lOpenVG -lluajit -o ./bin/macos/Diököl -L./lib/macos -I./include -pagezero_size 10000 -image_base 10000000
