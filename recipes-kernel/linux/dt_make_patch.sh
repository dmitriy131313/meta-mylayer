#!/bin/bash
cd ~/EL/poky/build/tmp/work-shared/orange-pi-zero/kernel-source
git diff --no-index ~/EL/poky/meta-mylayer/recipes-kernel/linux/sun8i-h2-plus-orangepi-zero_orig.dts arch/arm/boot/dts/sun8i-h2-plus-orangepi-zero.dts > ~/EL/poky/meta-mylayer/recipes-kernel/linux/DT_en_7789LCD_uart1.patch