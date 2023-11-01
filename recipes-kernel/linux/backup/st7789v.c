// SPDX-License-Identifier: GPL-2.0+
/*
 * DRM driver for display panels connected to a Sitronix ST7789V
 * display controller in SPI mode.
 * Based on ST7735S DRM Driver.
 * Copyright 2017 David Lechner <david@lechnology.com>
 * Copyright (C) 2019 Glider bvba
 * Copyright 2021 CNflysky
 */

#include <drm/drm_atomic_helper.h>
#include <drm/drm_drv.h>
#include <drm/drm_fb_helper.h>

#include <drm/drm_gem_atomic_helper.h>
#include <drm/drm_gem_dma_helper.h>
// #include <drm/drm_gem_cma_helper.h>
//  #include <drm/drm_gem_framebuffer_helper.h>
#include <drm/drm_managed.h>
#include <drm/drm_mipi_dbi.h>

#include <linux/backlight.h>
#include <linux/delay.h>
#include <linux/dma-buf.h>
#include <linux/gpio/consumer.h>
#include <linux/module.h>
#include <linux/property.h>
#include <linux/spi/spi.h>
#include <video/mipi_display.h>

#define ST7789V_MY BIT(7)
#define ST7789V_MX BIT(6)
#define ST7789V_MV BIT(5)
#define ST7789V_RGB BIT(3)

/**
 * enum st7789v_command - ST7789V display controller commands
 *
 * @PORCTRL: porch setting
 * @GCTRL: gate control
 * @VCOMS: VCOM setting
 * @VDVVRHEN: VDV and VRH command enable
 * @VRHS: VRH set
 * @VDVS: VDV set
 * @VCMOFSET: VCOM offset set
 * @PWCTRL1: power control 1
 * @PVGAMCTRL: positive voltage gamma control
 * @NVGAMCTRL: negative voltage gamma control
 *
 * The command names are the same as those found in the datasheet to ease
 * looking up their semantics and usage.
 *
 * Note that the ST7789V display controller offers quite a few more commands
 * which have been omitted from this list as they are not used at the moment.
 * Furthermore, commands that are compliant with the MIPI DCS have been left
 * out as well to avoid duplicate entries.
 */
enum st7789v_command {
	PORCTRL = 0xB2,
	GCTRL = 0xB7,
	VCOMS = 0xBB,
	VDVVRHEN = 0xC2,
	VRHS = 0xC3,
	VDVS = 0xC4,
	VCMOFSET = 0xC5,
	PWCTRL1 = 0xD0,
	PVGAMCTRL = 0xE0,
	NVGAMCTRL = 0xE1,
};

#define HSD20_IPS 1

struct st7789v_cfg
{
    const struct drm_display_mode mode;
    unsigned int left_offset;
    unsigned int top_offset;
    unsigned int write_only : 1;
    unsigned int rgb : 1; /* RGB (vs. BGR) */
};

struct st7789v_priv
{
    struct mipi_dbi_dev dbidev; /* Must be first for .release() */
    const struct st7789v_cfg *cfg;
};

static void st7789v_pipe_enable(struct drm_simple_display_pipe *pipe,
                                struct drm_crtc_state *crtc_state,
                                struct drm_plane_state *plane_state)
{
    struct mipi_dbi_dev *dbidev = drm_to_mipi_dbi_dev(pipe->crtc.dev);
    struct st7789v_priv *priv = container_of(dbidev, struct st7789v_priv, dbidev);
    struct mipi_dbi *dbi = &dbidev->dbi;
    int ret, idx;
    uint8_t addr_mode;

    if (!drm_dev_enter(pipe->crtc.dev, &idx))
        return;

    DRM_DEBUG_KMS("\n");
    printk(">>>>>>> st7789v_pipe_enable >>>>>>>\n");

    ret = mipi_dbi_poweron_reset(dbidev);
    if (ret)
    {
        goto out_exit;
    }



	/* turn off sleep mode */
	mipi_dbi_command(dbi, MIPI_DCS_EXIT_SLEEP_MODE);
	mdelay(120);

	/* set pixel format to RGB-565 */
	mipi_dbi_command(dbi, MIPI_DCS_SET_PIXEL_FORMAT, MIPI_DCS_PIXEL_FMT_16BIT);
	if (HSD20_IPS)
		mipi_dbi_command(dbi, PORCTRL, 0x05, 0x05, 0x00, 0x33, 0x33);

	else
		mipi_dbi_command(dbi, PORCTRL, 0x08, 0x08, 0x00, 0x22, 0x22);

	/*
	 * VGH = 13.26V
	 * VGL = -10.43V
	 */
	if (HSD20_IPS)
		mipi_dbi_command(dbi, GCTRL, 0x75);
	else
		mipi_dbi_command(dbi, GCTRL, 0x35);

	/*
	 * VDV and VRH register values come from command write
	 * (instead of NVM)
	 */
	mipi_dbi_command(dbi, VDVVRHEN, 0x01, 0xFF);

	/*
	 * VAP =  4.1V + (VCOM + VCOM offset + 0.5 * VDV)
	 * VAN = -4.1V + (VCOM + VCOM offset + 0.5 * VDV)
	 */
	if (HSD20_IPS)
		mipi_dbi_command(dbi, VRHS, 0x13);
	else
		mipi_dbi_command(dbi, VRHS, 0x0B);

	/* VDV = 0V */
	mipi_dbi_command(dbi, VDVS, 0x20);

	/* VCOM = 0.9V */
	if (HSD20_IPS)
		mipi_dbi_command(dbi, VCOMS, 0x22);
	else
		mipi_dbi_command(dbi, VCOMS, 0x20);

	/* VCOM offset = 0V */
	mipi_dbi_command(dbi, VCMOFSET, 0x20);

	/*
	 * AVDD = 6.8V
	 * AVCL = -4.8V
	 * VDS = 2.3V
	 */
	mipi_dbi_command(dbi, PWCTRL1, 0xA4, 0xA1);

    switch (dbidev->rotation)
    {
    default:
        addr_mode = 0;
        break;
    case 90:
        addr_mode = ST7789V_MX | ST7789V_MV;
        break;
    case 180:
        addr_mode = ST7789V_MX | ST7789V_MY;
        break;
    case 270:
        addr_mode = ST7789V_MY | ST7789V_MV;
        break;
    }
    if (priv->cfg->rgb)
        addr_mode |= ST7789V_RGB;
    mipi_dbi_command(dbi, MIPI_DCS_SET_ADDRESS_MODE, addr_mode);

	/* TE line output is off by default when powering on */

	mipi_dbi_command(dbi, MIPI_DCS_SET_DISPLAY_ON);

	if (HSD20_IPS)
		mipi_dbi_command(dbi, MIPI_DCS_ENTER_INVERT_MODE);

    msleep(50);
    // init seq end
    mipi_dbi_enable_flush(dbidev, crtc_state, plane_state);
out_exit:
    drm_dev_exit(idx);
}

static void st7789v_update_dummy(struct drm_simple_display_pipe *pipe,
			  struct drm_plane_state *old_state)
{
}

static const struct drm_simple_display_pipe_funcs st7789v_pipe_funcs = {
    .mode_valid = mipi_dbi_pipe_mode_valid,
    .enable = st7789v_pipe_enable,
    .disable = mipi_dbi_pipe_disable,
    .update = mipi_dbi_pipe_update,
    // .update = st7789v_update_dummy,
    // .prepare_fb = drm_gem_simple_display_pipe_prepare_fb,
};

static const struct st7789v_cfg st7789v_240x240 = {
    .mode = {DRM_SIMPLE_MODE(240, 240, 26, 26)}, // 2.6cm * 2.6cm
    .write_only = true,
};

static const struct st7789v_cfg st7789v_240x320 = {
    .mode = {DRM_SIMPLE_MODE(240, 320, 33, 44)}, // 3.3cm * 4.4cm
    .write_only = true,
};

DEFINE_DRM_GEM_DMA_FOPS(st7789v_fops);

static struct drm_driver st7789v_driver = {
    .driver_features = DRIVER_GEM | DRIVER_MODESET | DRIVER_ATOMIC,
    .fops = &st7789v_fops,
    DRM_GEM_DMA_DRIVER_OPS_VMAP,
    .debugfs_init = mipi_dbi_debugfs_init,
    .name = "st7789v",
    .desc = "Sitronix ST7789V",
    .date = "20171128",
    .major = 1,
    .minor = 0,
};

static const struct of_device_id st7789v_of_match[] = {
    {.compatible = "sitronix,st7789v_240x240", .data = &st7789v_240x240},
    {.compatible = "sitronix,st7789v_240x320", .data = &st7789v_240x320},
    {},
};
MODULE_DEVICE_TABLE(of, st7789v_of_match);

static const struct spi_device_id st7789v_id[] = {
    {"st7789v_240x240", (uintptr_t)&st7789v_240x240},
    {"st7789v_240x320", (uintptr_t)&st7789v_240x320},
    {},
};
MODULE_DEVICE_TABLE(spi, st7789v_id);

static int st7789v_probe(struct spi_device *spi)
{
    struct device *dev = &spi->dev;
    const struct st7789v_cfg *cfg;
    struct mipi_dbi_dev *dbidev;
    struct st7789v_priv *priv;
    struct drm_device *drm;
    struct mipi_dbi *dbi;
    struct gpio_desc *dc;
    u32 rotation = 0;
    int ret;

    cfg = device_get_match_data(&spi->dev);
    if (!cfg)
        cfg = (void *)spi_get_device_id(spi)->driver_data;

    priv =
        devm_drm_dev_alloc(dev, &st7789v_driver, struct st7789v_priv, dbidev.drm);
    if (IS_ERR(priv))
        return PTR_ERR(priv);

    dbidev = &priv->dbidev;
    priv->cfg = cfg;

    dbi = &dbidev->dbi;
    drm = &dbidev->drm;

    dbi->reset = devm_gpiod_get(dev, "reset", GPIOD_OUT_HIGH);
    if (IS_ERR(dbi->reset))
    {
        DRM_DEV_ERROR(dev, "Failed to get gpio 'reset'\n");
        return PTR_ERR(dbi->reset);
    }

    dc = devm_gpiod_get(dev, "dc", GPIOD_OUT_LOW);
    if (IS_ERR(dc))
    {
        DRM_DEV_ERROR(dev, "Failed to get gpio 'dc'\n");
        return PTR_ERR(dc);
    }

    dbidev->backlight = devm_of_find_backlight(dev);
    if (IS_ERR(dbidev->backlight))
        return PTR_ERR(dbidev->backlight);

    device_property_read_u32(dev, "rotation", &rotation);
    printk(">>>>>>> st7789v_probe; rotation: %d >>>>>>>\n", rotation);
    dbidev->rotation = rotation;

    ret = mipi_dbi_spi_init(spi, dbi, dc);
    if (ret)
        return ret;

    if (cfg->write_only)
        dbi->read_commands = NULL;

    dbidev->left_offset = cfg->left_offset;
    dbidev->top_offset = cfg->top_offset;


    // ret = mipi_dbi_dev_init(dbidev, &st7789v_pipe_funcs, &cfg->mode, rotation);
    // if (ret) return ret;

    static const uint32_t mipi_dbi_formats1[] = {
        DRM_FORMAT_RGB565
    };
    size_t bufsize = cfg->mode.vdisplay * cfg->mode.hdisplay * sizeof(u16) * 2;
    dbidev->drm.mode_config.preferred_depth = 16;
    ret = mipi_dbi_dev_init_with_formats(dbidev, &st7789v_pipe_funcs, mipi_dbi_formats1, ARRAY_SIZE(mipi_dbi_formats1), &cfg->mode, rotation, bufsize);
    if (ret) return ret;

    drm_mode_config_reset(drm);

    ret = drm_dev_register(drm, 0);
    if (ret)
        return ret;

    spi_set_drvdata(spi, drm);

    drm_fbdev_generic_setup(drm, 0);

    return 0;
}

static void st7789v_remove(struct spi_device *spi)
{
    struct drm_device *drm = spi_get_drvdata(spi);

    drm_dev_unplug(drm);
    drm_atomic_helper_shutdown(drm);
}

static void st7789v_shutdown(struct spi_device *spi)
{
    drm_atomic_helper_shutdown(spi_get_drvdata(spi));
}

static struct spi_driver st7789v_spi_driver = {
    .driver =
        {
            .name = "st7735r",
            .of_match_table = st7789v_of_match,
        },
    .id_table = st7789v_id,
    .probe = st7789v_probe,
    .remove = st7789v_remove,
    .shutdown = st7789v_shutdown,
};
module_spi_driver(st7789v_spi_driver);

MODULE_DESCRIPTION("Sitronix ST7789V DRM driver");
MODULE_AUTHOR("David Lechner <david@lechnology.com>");
MODULE_LICENSE("GPL");