# Das große GEOS Buch (Auflage 3)_text.pdf, S. 197
# geoEyes - xeyes lookalike for GEOS using VIC-II sprites

all: eyes.cvt

eyes.cvt: eyes.cvt.c eyes-resource.grc
	grc65 -tgeos eyes-resource.grc
	cl65 -tgeos -Os -o eyes.cvt eyes.cvt.c eyes-resource.s

eyes-comp.cvt: eyes.cvt.c eyes-resource.grc
	grc65 -tgeos eyes-resource.grc
	cl65 -tgeos -Os -DSPR_EXP_Y -o eyes-comp.cvt eyes.cvt.c eyes-resource.s

clean:
	rm -f eyes.cvt eyes-resource.s eyes-resource.o eyes.cvt.o

buch:
	tr '[:upper:]' '[:lower:]' <clock.bas | petcat -w2 -o clock.prg
