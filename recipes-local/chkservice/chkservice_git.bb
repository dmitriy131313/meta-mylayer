SUMMARY = "Chkservice is a tool for managing systemd units in terminal"
LICENSE = "GPLv3"
LIC_FILES_CHKSUM = "file://LICENSE;md5=a7799ceb308a6ce6f33f50ea1a2b1ac5"

inherit cmake pkgconfig

FILESEXTRAPATHS:prepend := "${THISDIR}/patch:"

DEPENDS += "ncurses systemd"

SRCREV = "ea40b14f4dcddeb3dd607f0260a2453fb9e0100e"

SRC_URI = "git://github.com/linuxenko/chkservice.git;protocol=https \
           file://0001-fix_CXX_FLAGS.patch"

S = "${WORKDIR}/git"
