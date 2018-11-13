CC=g++


CFLAGS=-std=c++11 -O3 -Wall -g
LINKFLAGS=-O3

#debug = true
ifdef debug
	CFLAGS +=-g
	LINKFLAGS += -flto
endif

INCDIR= -Iinclude 

LIBDIR=-L/usr/X11R6 -L/usr/local/lib

LIBS=

OS_NAME:=$(shell uname -s)

ifeq ($(OS_NAME),Darwin)
	LIBS += `pkg-config --static --libs glfw3 gl glew`
endif
ifeq ($(OS_NAME),Linux)
	LIBS += `pkg-config --static --libs glfw3 gl glew`
endif

SRCDIR=./src

SRCLIST=$(wildcard $(SRCDIR)/*cpp) 

HEADERDIR=./src

OBJDIR=./obj

OBJLIST=$(addprefix $(OBJDIR)/,$(notdir $(SRCLIST:.cpp=.o))) 

EXECUTABLE=boilerplate.out

all: buildDirectories $(EXECUTABLE) 

$(EXECUTABLE): $(OBJLIST)
	$(CC) $(LINKFLAGS) $(OBJLIST) -o $@ $(LIBS) $(LIBDIR)


$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CC) -c $(CFLAGS) -I$(HEADERDIR) $(INCDIR) $(LIBDIR) $< -o $@


.PHONY: buildDirectories
buildDirectories:
	mkdir -p $(OBJDIR)

.PHONY: clean
clean:
	rm -f *.out $(OBJDIR)/*.o; rmdir obj;
