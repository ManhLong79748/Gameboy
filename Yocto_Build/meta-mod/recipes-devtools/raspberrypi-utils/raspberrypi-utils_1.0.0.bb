FILESEXTRAPATHS:prepend := "${THISDIR}/files2:"

SUMMARY = "Recipe file for a new raspberry pi tools"
LICENSE = "CLOSED"

RDEPENDS:${PN}:append = " bash "
DEPENDS += "dtc"

SRC_URI = "git://github.com/raspberrypi/utils.git;protocol=https;branch=master"
SRC_URI += " file://git/CMakeLists.txt "

SRCREV = "c3d0bfe3a8fc3404bebf2fe149b1bf595ee0f30b"

S = "${WORKDIR}/git"

inherit cmake pkgconfig

do_install:append(){
	install -d ${D}${bindir}
	install -m 0755 ${B}/pinctrl/pinctrl ${D}${bindir}
    install -m 0755 ${B}/dtmerge/dtmerge ${D}${bindir}
    install -m 0755 ${B}/dtmerge/dtoverlay ${D}${bindir}
	install -m 0755 ${S}/raspinfo/raspinfo ${D}${bindir}
}

FILES:${PN} += " /usr/share/bash-completion/completions/pinctrl"
FILES:${PN} += " ${bindir}/pinctrl"
FILES:${PN} += " ${bindir}/raspinfo"
FILES:${PN} += " ${bindir}/dtoverlay"
FILES:${PN} += " ${bindir}/dtmerge"

