#!/bin/bash
cd ~/EL/poky/build/tmp/work-shared/orange-pi-zero/kernel-source
git diff --no-index ~/EL/poky/meta-mylayer/recipes-kernel/linux/drm_mipi_dbi_orig.c drivers/gpu/drm/drm_mipi_dbi.c > ~/EL/poky/meta-mylayer/recipes-kernel/linux/fix_drm_mipi_dbi.patch