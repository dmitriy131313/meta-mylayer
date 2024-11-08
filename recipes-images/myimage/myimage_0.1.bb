SUMMARY = "bitbake-layers recipe"
DESCRIPTION = "Recipe created by bitbake-layers"
LICENSE = "MIT"

#require recipes-core/images/core-image-base.bb
require recipes-graphics/images/core-image-x11.bb

#WiFi
IMAGE_INSTALL += "xradio"
IMAGE_INSTALL += "iw"
IMAGE_INSTALL += "wpa-supplicant"

#Periphery
#IMAGE_INSTALL += "i2c-tools"
#IMAGE_INSTALL += "spitools"
#IMAGE_INSTALL += "python3-smbus"
#IMAGE_INSTALL += "python3-opi-gpio"
#IMAGE_INSTALL += "python3-threading"
#IMAGE_INSTALL += "python3-logging"
#IMAGE_INSTALL += "python3-pynput"
#IMAGE_INSTALL += "python3-six"
#IMAGE_INSTALL += "python3-python-xlib"

# IMAGE_INSTALL += "python3-uinput"
# IMAGE_INSTALL += "python3-pyserial"

#Programs Games
#IMAGE_INSTALL += "mc"
#IMAGE_INSTALL += "vim"
IMAGE_INSTALL += "zdoom"
IMAGE_INSTALL += "quake3"
IMAGE_INSTALL += "freedoom-1"
#IMAGE_INSTALL += "joystick"
IMAGE_INSTALL += "xbox360gamepadnative"
# IMAGE_INSTALL += "xbox360gamepadpython"

# IMAGE_INSTALL += "libsuinput"
# IMAGE_INSTALL += "libstdc++"
# IMAGE_INSTALL += "libglu"
# IMAGE_INSTALL += "libsdl"

#Support Debug
#IMAGE_INSTALL += "cpupower"
#IMAGE_INSTALL += "cpufrequtils"
#IMAGE_INSTALL += "powertop"
#IMAGE_INSTALL += "chkservice"
#IMAGE_INSTALL += "python3-debugpy"
#IMAGE_INSTALL += "rt-tests"

#FOR SDK
#IMAGE_INSTALL += "kernel-devsrc"
TOOLCHAIN_TARGET_TASK += "kernel-devsrc"
TOOLCHAIN_TARGET_TASK += "libstdc++-staticdev"

#Graphics
#IMAGE_INSTALL += "mesa-demos"
#IMAGE_INSTALL += "kmscube"
#IMAGE_INSTALL += "piglit"
#IMAGE_INSTALL += "glmark2"
#IMAGE_INSTALL += "libdrm"
#IMAGE_INSTALL += "libdri2"

python do_display_banner() {
    bb.plain("***********************************************");
    bb.plain("*                                             *");
    bb.plain("*             My image recipe                 *");
    bb.plain("*                                             *");
    bb.plain("***********************************************");
}

addtask display_banner before do_build
