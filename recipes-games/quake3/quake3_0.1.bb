SUMMARY = "QUAKE3"
HOMEPAGE = "https://ioquake3.org/"
LICENSE = "GPL-2.0"
LIC_FILES_CHKSUM = "file://COPYING.txt;md5=87113aa2b484c59a17085b5c3f900ebf"
SECTION = "games"

FILESEXTRAPATHS:append = "${THISDIR}/files:"

DEPENDS = "libsdl2 libglu"

SRC_URI += "git://github.com/ioquake/ioq3.git;protocol=https;branch=main \
            file://pak0.pk3 \
            file://pak1.pk3 \
            file://pak2.pk3 \
            file://pak3.pk3 \
            file://pak4.pk3 \
            file://pak5.pk3 \
            file://pak6.pk3 \
            file://pak7.pk3 \
            file://pak8.pk3 \
            file://q3config.cfg \
            file://q3 \
            "
SRCREV = "972635ea5a3d1057d9a958c2cb1815dff05ab33b"

S = "${WORKDIR}/git"

inherit features_check pkgconfig

REQUIRED_DISTRO_FEATURES = "opengl x11"

EXTRA_OEMAKE = "'ARCH=arm' 'CC=${CC}' 'COPYDIR=${D}${prefix}/games/ioquake3' 'NO_STRIP=1'"

do_install() {
    oe_runmake copyfiles

    install -d ${D}${prefix}/games/ioquake3/baseq3
    install -m 0755 ${WORKDIR}/pak0.pk3 ${D}${prefix}/games/ioquake3/baseq3
    install -m 0755 ${WORKDIR}/pak1.pk3 ${D}${prefix}/games/ioquake3/baseq3
    install -m 0755 ${WORKDIR}/pak2.pk3 ${D}${prefix}/games/ioquake3/baseq3
    install -m 0755 ${WORKDIR}/pak3.pk3 ${D}${prefix}/games/ioquake3/baseq3
    install -m 0755 ${WORKDIR}/pak4.pk3 ${D}${prefix}/games/ioquake3/baseq3
    install -m 0755 ${WORKDIR}/pak5.pk3 ${D}${prefix}/games/ioquake3/baseq3
    install -m 0755 ${WORKDIR}/pak6.pk3 ${D}${prefix}/games/ioquake3/baseq3
    install -m 0755 ${WORKDIR}/pak7.pk3 ${D}${prefix}/games/ioquake3/baseq3
    install -m 0755 ${WORKDIR}/pak8.pk3 ${D}${prefix}/games/ioquake3/baseq3

    install -d ${D}/home/root/.q3a/baseq3
    install -m 0755 ${WORKDIR}/q3config.cfg ${D}/home/root/.q3a/baseq3

    install -d ${D}${bindir}
    install -m 0755 ${WORKDIR}/q3 ${D}${bindir}
    # ln --relative --symbolic ${D}${prefix}/games/ioquake3/ioquake3.arm ${D}${bindir}/ioquake3

    # cd ${D}${bindir}
    # ln -s /usr/games/ioquake3/ioquake3.arm /usr/bin/ioquake3
    # ln -s ../games/ioquake3/baseq3 baseq3
    # ln -s ../games/ioquake3/missionpack missionpack
    # ln -s ../games/ioquake3/renderer_opengl1_arm.so renderer_opengl1_arm.so
    # ln -s ../games/ioquake3/renderer_opengl2_arm.so renderer_opengl2_arm.so

    # ln -s ../games/ioquake3/ioquake3.arm ioquake3
    # ln -s ../games/ioquake3/baseq3 baseq3
    # ln -s ../games/ioquake3/missionpack missionpack
    # ln -s ../games/ioquake3/renderer_opengl1_arm.so renderer_opengl1_arm.so
    # ln -s ../games/ioquake3/renderer_opengl2_arm.so renderer_opengl2_arm.so
}

#ldflags
INSANE_SKIP:${PN} = "dev-so"
# INHIBIT_PACKAGE_STRIP = "1"
# INHIBIT_SYSROOT_STRIP = "1"
# SOLIBS = ".so"
# FILES_SOLIBSDEV = ""
# INHIBIT_PACKAGE_DEBUG_SPLIT = "1"

FILES:${PN} += "${prefix}/games/ioquake3/*"
# FILES:${PN}-dev += "${prefix}/games/ioquake3/*.so"
# FILES:${PN} += "${prefix}/games/ioquake3/renderer_opengl2_arm.so"
FILES:${PN} += "/home/root/.q3a/baseq3/*"


# ln -s ../games/ioquake3/renderer_opengl1_arm.so renderer_opengl1_arm.so
# ln -s ../games/ioquake3/renderer_opengl2_arm.so renderer_opengl2_arm.so
