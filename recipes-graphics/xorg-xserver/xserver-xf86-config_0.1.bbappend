FILESEXTRAPATHS:prepend := "${THISDIR}/files:"

SRC_URI += "file://10-modesetting.conf"

#SAVED_DIR := "${THISDIR}"

do_install:append() {
    install -d ${D}${datadir}/X11/xorg.conf.d
    install -m 0755 ${WORKDIR}/10-modesetting.conf \
        ${D}${datadir}/X11/xorg.conf.d/
}

FILES:${PN} += "${datadir}/X11/xorg.conf.d/10-modesetting.conf"

#datadir