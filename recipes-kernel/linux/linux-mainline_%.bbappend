FILESEXTRAPATHS:prepend := "${THISDIR}/files:"

SRC_URI  += "file://add_source_config_st7789_c_in_kernel.patch \
             file://7789tinyDRM_uinput_xbox_gpio.cfg \
             file://DT_en_7789LCD_uart1.patch \
             file://fix_drm_mipi_dbi.patch \
            "

#file://7789tinyDRM_uinput_xbox_gpio.cfg - enable 7789v_Tiny_DRM :: enable uinput and xbox360 gamepad :: enable gpio sysfs :: increase CMA buffer to 32mb :: enable backtrace when oops

#file://en_st7789v_DT.patch
#file://en_ST7789V_tinydrm.cfg

SAVED_DIR := "${THISDIR}"

#file://en_fb_7789.cfg
#file://en_st7789v.patch

#KERNEL_MODULE_AUTOLOAD += "i2c-dev"
#KERNEL_MODULE_AUTOLOAD += "spidev"
#KERNEL_MODULE_AUTOLOAD += "uinput"

do_configure:prepend() {
    cp -f ${SAVED_DIR}/files/st7789v.c ${S}/drivers/gpu/drm/tiny/
}
