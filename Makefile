# (C) 2007 by Tomasz bla Fortuna
# License: GNU GPL3+

##
# Configuration
##
CC=g++
CFLAGS=-Wall -O1 -ggdb -I. `pkg-config --cflags libxml-2.0`
#CFLAGS=-pipe -Wall -O3 -I. `pkg-config --cflags libxml-2.0` -march=athlon64 -fomit-frame-pointer -mmmx  -msse  -msse2 -msse3 -m3dnow
CPPFLAGS=$(CFLAGS)
LDFLAGS=-lSDL `pkg-config --libs libxml-2.0`
MAKEDEPS=./makedeps

# Source files
IO=	Graphics/Screen.cc Graphics/Image.cc
MATH=	Math/Matrix.cc Math/Transform.cc Math/Vector.cc 
SCENE=	World/Object.cc World/Plane.cc World/Color.cc \
	World/Texture.cc World/Material.cc \
	World/Sphere.cc World/Light.cc World/Camera.cc \
	World/Scene.cc World/SceneXML.cc
RENDER=	Render/Ray.cc Render/Photon.cc Render/Raytracer.cc
MISC=	General/Testcases.cc
SOURCES=$(IO) $(MATH) $(SCENE) $(RENDER) $(MISC) blaRAY.cc

OBJECTS=$(SOURCES:.cc=.o)
DEPS=$(SOURCES:.cc=.d)
EXEC=blaRAY

.PHONY: main clean docclean distclean doc doxygen

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
doxygen:
	doxygen

doc: doxygen
	(cd Docs/latex; make)
stats:
	@echo -n "File count: "
	@ls *.cc */*.cc */*.hh | wc -l
	@echo "Line, word, bytes stats:"
	@wc *.cc */*.cc */*.hh
	@echo -n "In .hh files: "
	@wc *.hh */*.hh 2>/dev/null  | tail -n1
	@echo -n "In .cc files: "
	@wc *.cc */*.cc 2>/dev/null  | tail -n1

###
# Cleaning facilities
###
clean:
	rm -f $(OBJECTS)

docclean:
	rm -rf Docs/html Docs/latex

distclean: clean docclean
	rm -f blaRAY blaray $(DEPS) tags

