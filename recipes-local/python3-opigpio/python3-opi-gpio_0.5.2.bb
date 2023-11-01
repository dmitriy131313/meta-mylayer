
SUMMARY = "A drop-in replacement for RPi.GPIO for the Orange Pi Zero"
HOMEPAGE = "https://github.com/rm-hull/OPi.GPIO"
AUTHOR = "Richard Hull <richard.hull@destructuring-bind.org>"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://LICENSE.rst;md5=e5d1797028eec84df9bb5ad3b213a6c9"

SRC_URI = "https://files.pythonhosted.org/packages/4f/53/ace1498028690d02864a4d057587ebdecae2329dc2d06bcb52b36b24b5ff/OPi.GPIO-0.5.2.tar.gz"
SRC_URI[md5sum] = "63bea917ea1d70a55ba9ede05fdaa0d1"
SRC_URI[sha256sum] = "8c8a32e1da559010880034f8810844c5321912b26d35b637d359e1d75368dba9"

S = "${WORKDIR}/OPi.GPIO-0.5.2"

RDEPENDS_${PN} = ""

inherit setuptools3
