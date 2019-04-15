######################################
#
# mod-tal-noisemaker
#
######################################

MOD_TAL_NOISEMAKER_SITE_METHOD = local
MOD_TAL_NOISEMAKER_SITE = $($(PKG)_PKGDIR)/


# even though this is a local build, we still need a version number
# bump this number if you need to force a rebuild
MOD_TAL_NOISEMAKER_VERSION = 1.2

# dependencies (list of other buildroot packages, separated by space)
MOD_TAL_NOISEMAKER_DEPENDENCIES =

# LV2 bundles that this package generates (space separated list)
MOD_TAL_NOISEMAKER_BUNDLES = TAL-NoiseMaker-ME.lv2 

# call make with the current arguments and path. "$(@D)" is the build directory.
#CABSIM_BASS_TARGET_MAKE = $(TARGET_MAKE_ENV) $(TARGET_CONFIGURE_OPTS) $(MAKE) -C $(@D)/source
MOD_TAL_NOISEMAKER_TARGET_MAKE = $(TARGET_MAKE_ENV) $(TARGET_CONFIGURE_OPTS) $(MAKE) LINUX_EMBED=true -C $(@D)

define MOD_TAL_NOISEMAKER_CONFIGURE_CMDS
       (cd $(@D); LINUX_EMBED=1 ./scripts/premake-update.sh linux)
endef

define MOD_TAL_NOISEMAKER_BUILD_CMDS
	$(MOD_TAL_NOISEMAKER_TARGET_MAKE) lv2_nogen
endef

define MOD_TAL_NOISEMAKER_INSTALL_TARGET_CMDS
	install -d $(TARGET_DIR)/usr/lib/lv2/
	cp -r $(@D)/bin/lv2/*.lv2/    $(TARGET_DIR)/usr/lib/lv2/
	cp -r $(@D)/data/*    $(TARGET_DIR)/usr/lib/lv2/TAL-NoiseMaker-ME.lv2/
endef

$(eval $(generic-package))
