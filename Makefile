OUTPUT = RSGL-Apps
GXX = g++
SOURCE = source/*.cpp
LIBS = $(shell RSGL-static)

build:
	$(GXX) $(SOURCE) $(LIBS) -o $(OUTPUT)
	
install:
	cp -r res /usr/share/RSGLApps
	cp $(OUTPUT) /usr/bin