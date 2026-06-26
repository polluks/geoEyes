# geoEyes - xeyes lookalike for GEOS using VIC-II sprites

all: eyes.cvt

eyes.cvt: eyes.cvt.c eyes-resource.grc pupil_data.c eye_icon.raw
	grc65 -tgeos-cbm eyes-resource.grc
	cl65 -tgeos-cbm -Os -o $@ eyes.cvt.c eyes-resource.s

eye_icon.raw: pupil_data.c
	python3 -c "import re; f=open('pupil_data.c'); b=[int(x,16) for x in re.findall(r'0x([0-9A-Fa-f]{2})',f.read())]; open('eye_icon.raw','wb').write(bytes(b))"

pupil_data.c: pupil.pcx
	sp65 -r $< -c vic2-sprite -w $@

clean:
	rm -f eyes.cvt eyes-resource.s eyes-resource.o eyes.cvt.o pupil_data.c

# Das große GEOS Buch (Auflage 3)_text.pdf, S. 197
buch:
	tr '[:upper:]' '[:lower:]' <clock.bas | petcat -w2 -o clock.prg
