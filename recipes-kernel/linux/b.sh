#!/bin/bash
cd ~/EL/poky/build/tmp/work-shared/orange-pi-zero/kernel-source
git diff --no-index ~/EL/poky/meta-mylayer/recipes-kernel/linux/sunxi-h3-h5_orig.dtsi arch/arm/boot/dts/sunxi-h3-h5.dtsi > ~/EL/poky/meta-mylayer/recipes-kernel/linux/panel_st7789v_DT1.patch


#rm -rf /home/user/EL/poky/build/tmp/work-shared/orange-pi-zero/kernel-source/arch/arm/boot/dts/patches
#rm -f /home/user/EL/poky/build/tmp/work-shared/orange-pi-zero/kernel-source/arch/arm/boot/dts/sun8i-h2-plus-orangepi-zero.dts
#cp /home/user/EL/poky/meta-mylayer/recipes-kernel/linux/sun8i-h2-plus-orangepi-zero_orig.dts /home/user/EL/poky/build/tmp/work-shared/orange-pi-zero/kernel-source/arch/arm/boot/dts/sun8i-h2-plus-orangepi-zero.dts
#cd /home/user/EL/poky/build/tmp/work-shared/orange-pi-zero/kernel-source/arch/arm/boot/dts
#quilt new mypatch.patch
#quilt add sun8i-h2-plus-orangepi-zero.dts
#rm -f /home/user/EL/poky/build/tmp/work-shared/orange-pi-zero/kernel-source/arch/arm/boot/dts/sun8i-h2-plus-orangepi-zero.dts
#cp /home/user/EL/poky/meta-mylayer/recipes-kernel/linux/sun8i-h2-plus-orangepi-zero_mod.dts /home/user/EL/poky/build/tmp/work-shared/orange-pi-zero/kernel-source/arch/arm/boot/dts/sun8i-h2-plus-orangepi-zero.dts
#quilt refresh
#mv /home/user/EL/poky/build/tmp/work-shared/orange-pi-zero/kernel-source/arch/arm/boot/dts/patches/mypatch.patch /home/user/EL/poky/meta-mylayer/recipes-kernel/linux/patch/panel_st7789v_DT.patch


#( cd /home/user/EL/poky/build/tmp/work-shared/orange-pi-zero/kernel-source ; git format-patch -1 )
#mv /home/user/EL/poky/build/tmp/work-shared/orange-pi-zero/kernel-source/0001-init.patch /home/user/EL/poky/meta-mylayer/recipes-kernel/linux/patch/panel_st7789v_DT.patch