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
	mirror  \
	random  \
	ray  \
	render  \
	shapes  \
	sky  \
	trace_res  \
	transform  \
	vector  \
	main
bin/raytracer: $(patsubst %, obj/%.o, ${RAYTRACER_DEPS})

-include ${DEPENDS}
