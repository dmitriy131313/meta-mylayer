FILESEXTRAPATHS:prepend := "${THISDIR}/files:"

SRC_URI  += "file://add_source_config_st7789_c_in_kernel.patch \
             file://en_ST7789V_tinydrm.cfg \
             file://en_st7789v_DT.patch \
             file://fix_drm_mipi_dbi.patch \
            "

#file://panel_st7789v.cfg
#file://panel_st7789v_DT.patch

SAVED_DIR := "${THISDIR}"

#file://en_st7789v_DT.patch
#file://add_source_config_st7789_c_in_kernel.patch
#file://en_ST7789V_tinydrm.cfg
       
#file://en_fb_7789.cfg
#file://en_st7789v.patch

#file://activate-spi1-i2c.patch
#file://fragment.cfg
#file://enable_tft_lcd_7789.cfg
#file://my_drm.cfg 
#file://fragment_debug_on.cfg
#file://fragment_enable_perf.cfg
#file://patch-5.4.161-rt67-rc1.patch

#KERNEL_MODULE_AUTOLOAD += "i2c-dev"
#KERNEL_MODULE_AUTOLOAD += "spidev"

do_configure:prepend() {
    cp -f ${SAVED_DIR}/files/st7789v.c ${S}/drivers/gpu/drm/tiny/
}
