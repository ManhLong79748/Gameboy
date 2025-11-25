#ifndef _ILI9341_DISPLAY_H
#define _ILI9341_DISPLAY_H

#include <linux/types.h>

#include "ili9341.h"

#define ILI9341_HEIGHT              320
#define ILI9341_WIDTH               240
#define ILI9341_BYTES_PER_PIXEL     2
#define ILI9341_BITS_PER_PIXEL      (8 * ILI9341_BYTES_PER_PIXEL)
#define ILI9341_COLOR_LENGTH_RED_BLUE 5
#define ILI9341_COLOR_LENGTH_GREEN  6
#define ILI9341_BUFFER_SIZE         (ILI9341_HEIGHT * ILI9341_WIDTH * ILI9341_BYTES_PER_PIXEL)
#define ILI9341_LINE_LENGTH         (ILI9341_WIDTH * ILI9341_BYTES_PER_PIXEL)

#define ILI9341_RED_OFFSET          11
#define ILI9341_GREEN_OFFSET        5
#define ILI9341_BLUE_OFFSET         0

int ili9341_init(struct device_data *dev_data);
int ili9341_send_display_buff(struct device_data *dev_data);

#endif /* _ILI9341_DISPLAY_H */