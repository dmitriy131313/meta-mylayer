SUMMARY = "bitbake-layers recipe"
DESCRIPTION = "Recipe created by bitbake-layers"
LICENSE = "MIT"

#require recipes-core/images/core-image-base.bb
require recipes-graphics/images/core-image-x11.bb

#IMAGE_INSTALL += "xradio"
#IMAGE_INSTALL += "i2c-tools"
#IMAGE_INSTALL += "spitools"
IMAGE_INSTALL += "mc"
#IMAGE_INSTALL += "cpupower"
#IMAGE_INSTALL += "cpufrequtils"
#IMAGE_INSTALL += "powertop"
#IMAGE_INSTALL += "chkservice"
#IMAGE_INSTALL += "python3"
#IMAGE_INSTALL += "python3-smbus"
#IMAGE_INSTALL += "python3-debugpy"

#IMAGE_INSTALL += "rt-tests"
#IMAGE_INSTALL += "kernel-devsrc"
#IMAGE_INSTALL += "mesa-demos"
#IMAGE_INSTALL += "kmscube"
#IMAGE_INSTALL += "piglit"
IMAGE_INSTALL += "glmark2"
IMAGE_INSTALL += "libdrm"

#IMAGE_INSTALL += "iw"
#IMAGE_INSTALL += "wpa-supplicant"
#TOOLCHAIN_TARGET_TASK_append = " kernel-devsrc"
#TOOLCHAIN_TARGET_TASK += "kernel-devsrc"

python do_display_banner() {
    bb.plain("***********************************************");
    bb.plain("*                                             *");
    bb.plain("*             My image recipe                 *");
    bb.plain("*                                             *");
    bb.plain("***********************************************");
}

addtask display_banner before do_build
