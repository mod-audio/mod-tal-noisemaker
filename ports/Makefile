# -----------------------------------------
# Compile all the Ports

all: libs lv2 vst

plugins: lv2 vst

# -----------------------------------------
# libs

libs:
	$(MAKE) -C ../libs/drowaudio
	$(MAKE) -C ../libs/juce
	$(MAKE) -C ../libs/juced
	$(MAKE) -C ../libs/lv2-ttl-generator

# -----------------------------------------
# lv2

lv2: libs
	install -d ../bin/lv2
ifeq ($(LINUX_EMBED),true)
	$(MAKE) -C tal-noisemaker/LV2
else
	$(MAKE) -C tal-noisemaker/LV2
endif

# -----------------------------------------
# vst

vst: libs
	install -d ../bin/vst
	$(MAKE) -C tal-noisemaker/VST

# -----------------------------------------
# clean

clean:
	# LV2
	$(MAKE) clean -C tal-noisemaker/LV2

	# VST
	$(MAKE) clean -C tal-noisemaker/VST

	rm -rf */LV2/intermediate
	rm -rf */VST/intermediate

distclean: clean
	rm -rf */LV2/*.lv2
	rm -f */*/Makefile
	rm -f */*/*.make
