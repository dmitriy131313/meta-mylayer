LICENSE = "CLOSED"

inherit systemd

FILESEXTRAPATHS:append = "${THISDIR}/files:"

SRC_URI = " \
		file://xbox360gamepad.py \
		file://xbox360gamepad.service \
"

DEPENDS = " \
	udev \
    libsuinput \
"

# RDEPENDS:${PN} = " \
# 	udev \
#     libsuinput \
# "

#file://xbox360gamepad.py

S = "${WORKDIR}"

do_install () {
	install -d ${D}${systemd_system_unitdir}
	install -m 0644 ${WORKDIR}/xbox360gamepad.service ${D}${systemd_system_unitdir}

	install -d ${D}${bindir}
	install -m 0755 ${WORKDIR}/xbox360gamepad.py ${D}${bindir}
}

# FILES:${PN} = " \
# 		${systemd_system_unitdir}/xbox360gamepad.service \
# 		${bindir}/xbox360virtual_gamepad_daemon.bin \
# "

SYSTEMD_AUTO_ENABLE = "enable"
SYSTEMD_SERVICE:${PN} = "xbox360gamepad.service"