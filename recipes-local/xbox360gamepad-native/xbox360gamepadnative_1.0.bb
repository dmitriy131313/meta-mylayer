LICENSE = "CLOSED"
FILESEXTRAPATHS:append = "${THISDIR}/files:"

inherit systemd

SRC_URI = " \
	file://CVirtualJoy.cpp \
	file://CVirtualJoy.h \
	file://main.cpp \
	file://UART_drv.cpp \
	file://UART_drv.h \
	file://Makefile \
	file://xbox360gamepad.service \
"

S = "${WORKDIR}"

EXTRA_OEMAKE = "'CXX=${CXX}' 'CFLAGS=${CFLAGS}' 'CPPFLAGS=${CPPFLAGS}' 'LDFLAGS=${LDFLAGS}'"

DEPENDS = " \
    libsuinput \
"

do_install(){
	install -d ${D}${systemd_system_unitdir}
	install -m 0644 ${WORKDIR}/xbox360gamepad.service ${D}${systemd_system_unitdir}

   install -d ${D}${bindir}
   install -m 0755 ${WORKDIR}/xbox360virtual_gamepad_daemon.bin ${D}${bindir}
}

SYSTEMD_AUTO_ENABLE = "enable"
SYSTEMD_SERVICE:${PN} = "xbox360gamepad.service"