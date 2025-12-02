SUMMARY = "mGBA - Game Boy Advance Emulator"
DESCRIPTION = "A fast, accurate, and portable GBA emulator."
LICENSE = "CLOSED"

DEPENDS = "libsdl2 libzip zlib libpng sqlite3"

SRC_URI = "git://github.com/mgba-emu/mgba.git;protocol=https;branch=master"
SRCREV = "b6618c5ebd0f046d7084817df272d6e1ee0c0726"

S = "${WORKDIR}/git"

inherit cmake pkgconfig

EXTRA_OECMAKE = " \
    -DBUILD_SDL=ON \
    -DBUILD_QT=OFF \
    -DBUILD_LIBRETRO=OFF \
    -DUSE_FFMPEG=OFF \
    -DUSE_DISCORD_RPC=OFF \
    -DUSE_MINIZIP=OFF \
    -DCMAKE_BUILD_TYPE=Release \
"

FILES:${PN} += "/usr/bin/mgba"
FILES_${PN} += " \
    ${datadir}/icons/hicolor/* \
"