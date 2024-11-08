FILESEXTRAPATHS:prepend := "${THISDIR}/files:"

SAVED_DIR := "${THISDIR}"

SRC_URI  += "file://zdoom.ini \
             file://doom \
             "

do_install:append() {
    install -d ${D}${sysconfdir}/zdoom
    install -m 0755 ${SAVED_DIR}/files/zdoom.ini ${D}${sysconfdir}/zdoom

    install -d ${D}${bindir}
    install -m 0755 ${WORKDIR}/doom ${D}${bindir}
}

FILES:${PN} += "${sysconfdir}/zdoom/zdoom.ini \
                ${bindir}/doom \
                "