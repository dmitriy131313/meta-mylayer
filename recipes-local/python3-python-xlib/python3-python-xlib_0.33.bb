
SUMMARY = "Python X Library"
HOMEPAGE = "https://github.com/python-xlib/python-xlib"
AUTHOR = "Peter Liljenberg <petli@ctrl-c.liu.se>"
LICENSE = "LGPL-2.0"
LIC_FILES_CHKSUM = "file://LICENSE;md5=8975de00e0aab10867abf36434958a28"

SRC_URI = "https://files.pythonhosted.org/packages/86/f5/8c0653e5bb54e0cbdfe27bf32d41f27bc4e12faa8742778c17f2a71be2c0/python-xlib-0.33.tar.gz"
SRC_URI[md5sum] = "20a687ac84ee632aea9b9861206aa0eb"
SRC_URI[sha256sum] = "55af7906a2c75ce6cb280a584776080602444f75815a7aff4d287bb2d7018b32"

FILESEXTRAPATHS:prepend := "${THISDIR}/patches:"
SRC_URI  += "file://remove_setup_requires.patch"

S = "${WORKDIR}/python-xlib-0.33"

RDEPENDS_${PN} = "python3-six"

inherit setuptools3
