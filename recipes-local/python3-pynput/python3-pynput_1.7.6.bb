SUMMARY = "Monitor and control user input devices"
HOMEPAGE = "https://github.com/moses-palmer/pynput"
AUTHOR = "Moses Palmér <moses.palmer@gmail.com>"
LICENSE = "LGPL-3.0"
LIC_FILES_CHKSUM = "file://COPYING.LGPL;md5=acef203229a47c1ea4d33721721b9963"

SRC_URI = "https://files.pythonhosted.org/packages/d7/74/a231bca942b1cd9c4bb707788be325a61d26c7998bd25e88dc510d4b55c7/pynput-1.7.6.tar.gz"
SRC_URI[md5sum] = "5fb55a7b67af9b405bc5b466aeaa6efe"
SRC_URI[sha256sum] = "3a5726546da54116b687785d38b1db56997ce1d28e53e8d22fc656d8b92e533c"

FILESEXTRAPATHS:prepend := "${THISDIR}/patches:"
SRC_URI  += "file://remove_setup_requires.patch"

S = "${WORKDIR}/pynput-1.7.6"

#do_compile:prepend(){
#    bbdebug 2 ">>>>>>>>>>>>>>>> ${S}"
#    bbdebug 2 ">>>>>>>>>>>>>>>> ${WORKDIR}"
#}

RDEPENDS:${PN} = "python3-six python3-python-xlib python3-threading"

inherit setuptools3