# Linux (default)
EXE = diokol
SRC_DIR = src
OBJ_DIR = obj

SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

CFLAGS += -w -Os -fno-common -I./include
LDLIBS = -L./lib -lGL -lGLU -lglut -ldl

#OS = Windows_NT
# Windows (cygwin)
ifeq ($(OS), Windows_NT)
  CC = x86_64-w64-mingw32-gcc
  LD = x86_64-w64-mingw32-gcc
  EXE = diokol.exe
  LDLIBS = -L./lib -lglu32 -lopengl32 -lopenvg -lp5graphics -lluajit
  LDFLAGS = -shared
  CFLAGS = -I./include
else
# OS X
ifeq "$(shell uname)" "Darwin"
# LDLIBS = -L./libs -framework Carbon -framework OpenGL -lopenvg -lp5graphics -lluajit -pagezero_size 10000 -image_base
  LDLIBS = -L./lib -framework Carbon -framework OpenGL -lglfw -lopenvg -lp5graphics -lluajit-5.1.2
 # LDFLAGS = -dynamiclib -undefined suppress -flat_namespace
endif

endif

all: $(EXE)

$(EXE): $(OBJ)
	$(CC) $(LDFLAGS) $^ $(LDLIBS) -o $@
    
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@
    
clean:
	$(RM) $(OBJ)
