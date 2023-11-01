FILESEXTRAPATHS:prepend := "${THISDIR}/files:"

SAVED_DIR := "${THISDIR}"

SRC_URI  += "file://zdoom.ini"

do_install:append() {
    install -d ${D}${sysconfdir}/zdoom
    install -m 0755 ${SAVED_DIR}/files/zdoom.ini ${D}${sysconfdir}/zdoom
}

FILES:${PN} += "${sysconfdir}/zdoom/zdoom.ini"