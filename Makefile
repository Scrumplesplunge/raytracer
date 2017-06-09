include config.mk
include rules.mk

.PHONY: all

BINARIES =  \
	raytracer
all: $(patsubst %, bin/%, ${BINARIES})

RAYTRACER_DEPS =  \
	camera  \
	csg  \
	die  \
	diffuse  \
	glass  \
	image  \
	light  \
	material  \
	matrix  \
	plane  \
	primitive  \
	random  \
	ray  \
	render  \
	shape  \
	sky  \
	sphere  \
	trace_res  \
	vector  \
	main
bin/raytracer: $(patsubst %, obj/%.o, ${RAYTRACER_DEPS})

-include ${DEPENDS}
