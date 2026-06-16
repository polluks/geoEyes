# Das große GEOS Buch (Auflage 3)_text.pdf, S. 197
# geoEyes - xeyes lookalike for GEOS using VIC-II sprites

all: eyes.cvt

eyes.cvt: eyes.cvt.c eyes-resource.grc pupil_comp.inc
	grc65 -tgeos-cbm eyes-resource.grc
	cl65 -tgeos-cbm -Os -o eyes.cvt eyes.cvt.c eyes-resource.s

# Regenerate sprite data from PCX (requires sp65 from cc65).
gen-sprite: pupil.pcx
	$(SP65) -r $< -c vic2-sprite -w pupil_data.c
	tail -n +5 pupil_data.c > pupil_data.inc
	rm -f pupil_data.c
	python3 -c "import re; b=[int(x,16) for x in re.findall(r'0x[0-9A-Fa-f]+',open('pupil_data.inc').read())]; open('pupil_comp.inc','w').write('\n'.join('\t'+','.join('0x{:02X}'.format(x) for x in sum((b[i*3:(i+1)*3] for i in range(0,21,2)),[])[i:i+16])+',' for i in range(0,33,16))+'\n')"
	@echo "  pupil_comp.inc regenerated from pupil.pcx"

clean:
	rm -f eyes.cvt eyes-resource.s eyes-resource.o eyes.cvt.o

buch:
	tr '[:upper:]' '[:lower:]' <clock.bas | petcat -w2 -o clock.prg
