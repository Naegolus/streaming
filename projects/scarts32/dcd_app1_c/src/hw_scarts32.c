/* ******************************************************** */
/*                                                          */
/*                                                          */
/*   ____-_-_-_-_-_-_-_-________________________            */
/*                                                          */
/*                                                          */
/*   File:          hw_scarts32.c                           */
/*   Description:   SCARTS32 memory write and read          */
/*                  functions                               */
/*   Autor:         Johannes Natter                         */
/*   Date:          12.08.2008                              */
/*                                                          */
/*   Notes:                                                 */
/*                                                          */
/*                                                          */
/* ******************************************************** */

#include "cstdlib.h"
#include "modules.h"
#include "hardware.h"
#include "spi.h"
#include "flash.h"
#include "epcs16.h"
#include "servo.h"

/* LEDs */
#define LED_MAIN_RUNNING                GPIO_REG_LED2

/* Memory ID/IDX */
#define MEM_ID_FLASH                    0
#define MEM_FLASH_IDX_BSL               0
#define MEM_FLASH_IDX_BSL_RODATA        1
#define MEM_FLASH_IDX_FW                2

#define MEM_ID_EPCS16                   1

#define MEM_ID_SERVO                    2

/* ******************** LOCAL FUNCTIONS ******************* */
/* ******************************************************** */

/* *************************************************** */
/* SCARTS32 Hardware initialization                    */
/* *************************************************** */
void hw_init(void)
{
    /* TODO: Disable interrupts */
    /* TODO: Init. IRQ-Vector-Tab */
    spi_init();
}

/* **************************************************** */
/* SCARTS32: Enable interrupts                          */
/* **************************************************** */
void enable_irq(void)
{
    /* TODO: Enable interrupts */
}

/* **************************************************** */
/* SCARTS32: start firmware                             */
/* **************************************************** */
void start_firmware(void)
{
    asm("\
    ldli r13, 0\n\
    jsr r13\n");
}

/* *************************************************** */
/* SCARTS32: write program memory                      */
/* *************************************************** */
UINT1 write_mem(UINT1 *p_data, UINT2 mem_id, UINT2 mem_idx, ULONG4 start_addr, ULONG4 len)
{
    if(MEM_ID_FLASH == mem_id)
    {
        /* TODO: Verify addresses */
        switch(mem_idx)
        {
            case MEM_FLASH_IDX_BSL:
                flash_write_mem(p_data, start_addr, len);
                return 0;
            case MEM_FLASH_IDX_BSL_RODATA:
                flash_write_mem(p_data, start_addr, len);
                return 0;
        }
    }else
    if(MEM_ID_EPCS16 == mem_id)
    {
        epcs16_write_mem(p_data, start_addr, len);
        return 0;
    }else
    if(MEM_ID_SERVO == mem_id)
    {
        if(len != 4)
            return 1;

        servo_set_high_pulse_width((SERVO_PWM_ID_ENUM)mem_idx, *((uint32_t *)p_data));
        return 0;
    }
    return 1; /* Error */
}

/* *************************************************** */
/* SCARTS32: read program memory                       */
/* *************************************************** */
UINT1 read_mem(UINT1 *p_data, UINT2 mem_id, UINT2 mem_idx, ULONG4 start_addr, ULONG4 len)
{
    if(MEM_ID_FLASH == mem_id)
    {
        switch(mem_idx)
        {
            /* TODO: Verify addresses */
            case MEM_FLASH_IDX_BSL:
                flash_read_mem(p_data, start_addr, len);
                return 0;
            case MEM_FLASH_IDX_BSL_RODATA:
                flash_read_mem(p_data, start_addr, len);
                return 0;
        }
    }else
    if(MEM_ID_EPCS16 == mem_id)
    {
        epcs16_read_mem(p_data, start_addr, len);
        return 0;
    }
    if(MEM_ID_SERVO == mem_id)
    {
        if(len != 4)
            return 1;

        *((uint32_t *)p_data) = servo_get_high_pulse_width((SERVO_PWM_ID_ENUM)mem_idx);
        return 0;
    }
    return 1; /* Error */
}

/* *************************************************** */
/* SCARTS32: bsl slave says hello world                */
/* *************************************************** */
void signal_app(void)
{
    p_scarts->gpio.leds |= LED_MAIN_RUNNING;
}

/* *************************************************** */
/* SCARTS32: Get tick count from external timer module */
/* *************************************************** */
UINT2 get_tick_cnt(void)
{
    return p_scarts->time.ticks250us;
}

/* *************************************************** */
/* SCARTS32: Generate trigger pin pulse                */
/* *************************************************** */
void trigger_pin_pulse(void)
{
}
