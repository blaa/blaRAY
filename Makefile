# (C) 2007 by Tomasz bla Fortuna
# License: GNU GPL3+

##
# Configuration
##
CC=g++
CFLAGS=-Wall -O1 -ggdb -I.
#CFLAGS=-pipe -Wall -O3 -I. -march=athlon64 -fomit-frame-pointer -mmmx  -msse  -msse2 -msse3 -m3dnow
CPPFLAGS=$(CFLAGS)
LDFLAGS=-lSDL
MAKEDEPS=./makedeps

# Source files
IO=	Graphics/Screen.cc Graphics/Image.cc
MATH=	Math/Matrix.cc Math/Transform.cc Math/Vector.cc 
SCENE=	Scene/Object.cc Scene/Plane.cc Scene/Color.cc \
	Scene/Texture.cc Scene/Material.cc \
	Scene/Sphere.cc Scene/Light.cc Scene/Camera.cc \
	Scene/Scene.cc 
RENDER=	Render/Ray.cc Render/Photon.cc Render/Raytracer.cc
MISC=	General/Testcases.cc
SOURCES=$(IO) $(MATH) $(SCENE) $(RENDER) $(MISC) blaray.cc

OBJECTS=$(SOURCES:.cc=.o)
DEPS=$(SOURCES:.cc=.d)
EXEC=blaray

.PHONY: main clean docclean distclean doc 

main: $(EXEC)
-include $(DEPS)


###
# Dependencies
###
%.d: %.cc
	@echo '(DEP) $< -> $@'
	@$(MAKEDEPS) $<

##
# Building rules 
##
%.o: %.cc
	@echo '(OBJ) $< -> $@'
	@$(CC) -c $(CFLAGS) -o $@ $<

$(EXEC): $(OBJECTS)
	@echo 'Linking $@...'
	@$(CC) $(CFLAGS) $(LDFLAGS) -o $(EXEC) $(OBJECTS)

##
# Docs / Stats
##
doc: 
	doxygen
	(cd Docs/latex; make)
stats:
	wc *.cc */*.cc */*.hh
	ls *.cc */*.cc */*.hh | wc -l

###
# Cleaning facilities
###
clean:
	rm -f $(OBJECTS)

docclean:
	rm -rf Docs/html Docs/latex

distclean: clean docclean
	rm -f blaray $(DEPS) tags

