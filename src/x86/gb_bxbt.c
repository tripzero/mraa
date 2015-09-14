/*
 * Author: Brendan Le Foll <brendan.le.foll@intel.com>
 * Copyright (c) 2015 Intel Corporation.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "common.h"
#include "x86/gb_bxbt.h"

#define PLATFORM_NAME "Gigabyte BXBT"
#define SYSFS_CLASS_GPIO "/sys/class/gpio"
#define I2CNAME "designware"

mraa_board_t*
mraa_gb_bxbt()
{
    mraa_board_t* b = (mraa_board_t*) calloc(1, sizeof(mraa_board_t));
    if (b == NULL) {
        return NULL;
    }

    b->platform_name = PLATFORM_NAME;
    b->phy_pin_count = MRAA_GB_BXBT_PINCOUNT;

    b->adv_func = (mraa_adv_func_t*) calloc(1, sizeof(mraa_adv_func_t));
    if (b->adv_func == NULL) {
        goto error;
    }

    b->pins = (mraa_pininfo_t*) malloc(sizeof(mraa_pininfo_t) * MRAA_GB_BXBT_PINCOUNT);
    if (b->pins == NULL) {
        free(b->adv_func);
        goto error;
    }

    strncpy(b->pins[0].name, "GPIO_S0_78", 8);
    b->pins[0].capabilites = (mraa_pincapabilities_t){ 1, 0, 0, 0, 0, 0, 0, 0 };

    strncpy(b->pins[1].name, "GPIO_S0_82", 8);
    b->pins[1].capabilites = (mraa_pincapabilities_t){ 1, 0, 0, 0, 0, 0, 0, 0 };
    strncpy(b->pins[2].name, "GPIO_S0_79", 8);
    b->pins[2].capabilites = (mraa_pincapabilities_t){ 1, 0, 0, 0, 0, 0, 0, 0 };
    strncpy(b->pins[3].name, "GPIO_S0_83", 8);
    b->pins[3].capabilites = (mraa_pincapabilities_t){ 1, 0, 0, 0, 0, 0, 0, 0 };
    strncpy(b->pins[4].name, "GPIO_S0_80", 8);
    b->pins[4].capabilites = (mraa_pincapabilities_t){ 1, 0, 0, 0, 0, 0, 0, 0 };
    strncpy(b->pins[5].name, "GPIO_S0_84", 8);
    b->pins[5].capabilites = (mraa_pincapabilities_t){ 1, 0, 0, 0, 0, 0, 0, 0 };
    strncpy(b->pins[6].name, "GPIO_S0_81", 8);
    b->pins[6].capabilites = (mraa_pincapabilities_t){ 1, 0, 0, 0, 0, 0, 0, 0 };
    strncpy(b->pins[7].name, "GPIO_S0_85", 8);
    b->pins[7].capabilites = (mraa_pincapabilities_t){ 1, 0, 0, 0, 0, 0, 0, 0 };
    strncpy(b->pins[8].name, "SMB_CLK", 8);
    b->pins[8].capabilites = (mraa_pincapabilities_t){ 1, 0, 0, 0, 0, 0, 0, 0 };
    strncpy(b->pins[9].name, "SMB_SDA", 8);
    b->pins[9].capabilites = (mraa_pincapabilities_t){ 1, 0, 0, 0, 0, 0, 0, 0 };

    strncpy(b->pins[10].name, "+VSS", 8);
    b->pins[10].capabilites = (mraa_pincapabilities_t){ 0, 0, 0, 0, 0, 0, 0, 0 };
    
    strncpy(b->pins[11].name, "GND", 8);
    b->pins[11].capabilites = (mraa_pincapabilities_t){ 0, 0, 0, 0, 0, 0, 0, 0 };
    
    b->i2c_bus_count = 0;
    int i2c_num = -1;
    int i;
    for (i = 0; i < 1; i++) {
        i2c_num = mraa_find_i2c_bus(I2CNAME, i2c_num + 1);
        if (i2c_num == -1) {
            break;
        }
        b->i2c_bus_count++;
        b->i2c_bus[i].bus_id = i2c_num;
        b->i2c_bus[i].sda = 12 + i;
        b->i2c_bus[i].scl = 13 + i;
    }

    if (b->i2c_bus_count > 0) {
        b->def_i2c_bus = b->i2c_bus[0].bus_id;
    }

    return b;

error:
    syslog(LOG_CRIT, "gb-bxbt: Platform failed to initialise");
    free(b);
    return NULL;
}
