FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}:"

SRC_URI += " \
    file://0002-Add-ILI9341-SPI-Fb-display-driver.patch \
    file://0001-Enable-spi-ili9341.patch \
    file://ili9341.cfg \
    file://mgpio.cfg \
"

KERNEL_MODULE_AUTOLOAD += "ili9341_cus mgpio"