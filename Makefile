# Das große GEOS Buch (Auflage 3)_text.pdf, S. 197
# geoEyes - xeyes lookalike for GEOS using VIC-II sprites
# Based on xeyes (https://commons.wikimedia.org/wiki/File:Xeyes.png,
# public domain, Tizio via Wikimedia Commons)
# Sprite data generated via sp65 from pupil.pcx

SP65 ?= sp65

all: eyes.cvt

eyes.cvt: eyes.cvt.c eyes-resource.grc pupil_data.inc pupil_comp.inc
	grc65 -tgeos eyes-resource.grc
	cl65 -tgeos -Os -o eyes.cvt eyes.cvt.c eyes-resource.s

eyes-comp.cvt: eyes.cvt.c eyes-resource.grc pupil_data.inc pupil_comp.inc
	grc65 -tgeos eyes-resource.grc
	cl65 -tgeos -Os -DSPR_EXP_Y -o eyes-comp.cvt eyes.cvt.c eyes-resource.s

# Regenerate sprite data from PCX (requires sp65 from cc65).
# pupil_comp.inc is manually created (sp65 enforces 21-row sprite height).
gen-sprite: pupil.pcx
	$(SP65) -r $< -c vic2-sprite -w pupil_data.c
	tail -n +5 pupil_data.c > pupil_data.inc
	rm -f pupil_data.c
	@echo "  pupil_data.inc regenerated from pupil.pcx"

clean:
	rm -f eyes.cvt eyes-comp.cvt eyes-resource.s eyes-resource.o eyes.cvt.o

buch:
	tr '[:upper:]' '[:lower:]' <clock.bas | petcat -w2 -o clock.prg
