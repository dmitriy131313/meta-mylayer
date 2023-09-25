#!/bin/bash
cd ~/EL/poky/build/tmp/work-shared/orange-pi-zero/kernel-source
git diff --no-index ~/EL/poky/meta-mylayer/recipes-kernel/linux/drm_print_orig.c drivers/gpu/drm/drm_print.c > ~/EL/poky/meta-mylayer/recipes-kernel/linux/en_drm_debug.patch