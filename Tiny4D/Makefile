CFLAGS=-dtypes -I +sdl
LIBS=unix.cmxa graphics.cmxa

#raytracer: raytracer.ml
#	ocamlopt -dtypes -o raytracer unix.cmxa graphics.cmxa raytracer.ml
raytracer: raytracer.ml
	ocamlopt -dtypes -o raytracer -I +site-packages/sdl $(LIBS) raytracer.ml

debug: raytracer.ml
	ocamlc -dtypes -o debug unix.cma graphics.cma -g raytracer.ml
clean:
	rm raytracer *.o *.cmo *.cmx *.cmi
