#include <linux/module.h>
#include <linux/gpio.h>
#include <linux/platform_device.h>
#include <linux/gpio/consumer.h>
#include <linux/of.h>
#include <linux/interrupt.h>
#include <linux/input.h>
#include <linux/jiffies.h>

#define DRIVER_AUTHOR "long79748@gmail.com"
#define DRIVER_DESC   "Button driver using interrupt as input device"

struct button_data {
    struct gpio_desc *gpiod;
    int irq;
    unsigned int keycode;
    struct input_dev *input;
    unsigned long last_time; 
    unsigned int debounce;   // Debounce interval (ms)
};

static struct button_data buttons[6];

static const struct of_device_id gpiod_dt_ids[] = {
    { .compatible = "gpio_base_myself", },
    { /* sentinel */ }
};

static irqreturn_t button_handler(int irq, void *dev_id) {
    struct button_data *btn = dev_id;
    unsigned long now = jiffies;
    
    if (time_before(now, btn->last_time + msecs_to_jiffies(btn->debounce))) {
        return IRQ_HANDLED; // Ignore bounce/noise
    }
    btn->last_time = now;

    int value = gpiod_get_value(btn->gpiod);

    pr_info("Raw GPIO value: %d\n", value);
    int pressed = !value; // ACTIVE_LOW: low=pressed
    pr_info("Button %d %s\n", btn->keycode, pressed ? "Pressed" : "Released");
    input_report_key(btn->input, btn->keycode, pressed);
    input_sync(btn->input);
    return IRQ_HANDLED;
}

static int my_pdrv_probe(struct platform_device *pdev) {
    struct device *dev = &pdev->dev;
    struct input_dev *input;
    int i, ret;
    u32 debounce_ms = 200; // Default 200ms

    of_property_read_u32(dev->of_node, "debounce-interval", &debounce_ms);

    // Create input device
    input = devm_input_allocate_device(dev);
    if (!input) return -ENOMEM;
    input->name = "RPi Buttons";
    input->phys = "gpio-buttons/input0";
    input->id.bustype = BUS_HOST;
    input->id.vendor = 0x0001;
    input->id.product = 0x0001;
    input->id.version = 0x0100;
    // SỬA: Set parent cho input device
    input->dev.parent = dev;
    set_bit(EV_KEY, input->evbit); // Support key events

    // Map buttons: up=GPIO4, down=5, left=6, right=12, enter=13, esc=16
    const char *btn_names[6] = {"button_up", "button_down", "button_left", "button_right", "button_enter", "button_esc"};
    unsigned int keycodes[6] = {KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT, KEY_ENTER, KEY_ESC};

    for (i = 0; i < 6; i++) {
        buttons[i].gpiod = gpiod_get(dev, btn_names[i], GPIOD_IN);
        if (IS_ERR(buttons[i].gpiod)) {
            ret = PTR_ERR(buttons[i].gpiod);
            goto err_cleanup;
        }

        buttons[i].irq = gpiod_to_irq(buttons[i].gpiod);
        buttons[i].keycode = keycodes[i];
        buttons[i].input = input;
        buttons[i].debounce = debounce_ms;
        buttons[i].last_time = 0; // Init debounce time

        set_bit(keycodes[i], input->keybit); // Enable keycode

        ret = request_irq(buttons[i].irq, button_handler, IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING | IRQF_SHARED,
                          "button_irq", &buttons[i]);
        if (ret) {
            gpiod_put(buttons[i].gpiod);
            goto err_cleanup;
        }
    }

    ret = input_register_device(input);
    if (ret) goto err_cleanup;

    platform_set_drvdata(pdev, input);
    pr_info("Probe successful\n");
    return 0;

err_cleanup:
    while (--i >= 0) {
        free_irq(buttons[i].irq, &buttons[i]);
        gpiod_put(buttons[i].gpiod);
    }
    return ret;
}

static int my_pdrv_remove(struct platform_device *pdev) {
    struct input_dev *input = platform_get_drvdata(pdev);
    int i;

    for (i = 0; i < 6; i++) {
        free_irq(buttons[i].irq, &buttons[i]);
        gpiod_put(buttons[i].gpiod);
    }

    input_unregister_device(input);
    pr_info("Remove successful\n");
    return 0;
}

static struct platform_driver mypdrv = {
    .probe = my_pdrv_probe,
    .remove = my_pdrv_remove,
    .driver = {
        .name = "descriptor-based",
        .of_match_table = of_match_ptr(gpiod_dt_ids),
        .owner = THIS_MODULE,
    },
};

module_platform_driver(mypdrv);

MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_VERSION("1.0");
