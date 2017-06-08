include config.mk
include rules.mk

.PHONY: all

BINARIES =  \
	raytracer
all: $(patsubst %, bin/%, ${BINARIES})

RAYTRACER_DEPS =  \
	Camera  \
	CSGComplement  \
	CSG  \
	CSGIntersection  \
	CSGUnion  \
	Die  \
	Diffuse  \
	Glass  \
	Image  \
	Light  \
	Material  \
	Matrix  \
	Plane  \
	Primitive  \
	Random  \
	Ray  \
	Render  \
	Shape  \
	Sky  \
	Sphere  \
	TraceRes  \
	Vector  \
	Main
bin/raytracer: $(patsubst %, obj/%.o, ${RAYTRACER_DEPS})

-include ${DEPENDS}
