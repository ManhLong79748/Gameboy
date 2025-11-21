SUMMARY = "A Recipes to demo how to intergrate qt5 app to yocto"
LICENSE = "CLOSED"

DEPENDS:append = " cmake qtbase qtdeclarative-native"
RDEPENDS:${PN}:append = " qtbase qtdeclarative"

SRC_URI:append = " file://yoctoqt5demo "

S = "${WORKDIR}/yoctoqt5demo"

inherit pkgconfig cmake_qt5

EXTRA_OECMAKE:append = ""