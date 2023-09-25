
SUMMARY = "Python bindings for Linux SMBus access through i2c-dev"
HOMEPAGE = "https://i2c.wiki.kernel.org/index.php/I2C_Tools"
AUTHOR = "Mark M. Hoffman <mhoffman@lightlink.com>"
LICENSE = "smbus"
LIC_FILES_CHKSUM = "file://COPYING;md5=751419260aa954499f7abaabaa882bbe"

SRC_URI = "https://files.pythonhosted.org/packages/4d/5c/70e14aa4f0c586efc017e1d1aa6e2f7921eefc7602fc2d03368ff912aa91/smbus-1.1.post2.tar.gz"
SRC_URI[md5sum] = "7c47970e21b8c73dad5e62ffaeb41f50"
SRC_URI[sha256sum] = "f96d345e0aa10053a8a4917634f1dc37ba1f656fa5cace7629b71777e90855c6"

S = "${WORKDIR}/smbus-1.1.post2"

RDEPENDS_${PN} = ""

inherit setuptools3
