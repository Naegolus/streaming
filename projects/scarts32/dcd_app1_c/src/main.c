/* ******************************************************** */
/*                                                          */
/*                                                          */
/*   ____-_-_-_-_-_-_-_-________________________            */
/*                                                          */
/*                                                          */
/*   File:          main.c                                  */
/*   Description:   Bootstrap loader                        */
/*   Autor:         Johannes Natter                         */
/*   Date:          07.08.2008                              */
/*                                                          */
/*   Notes:                                                 */
/*                                                          */
/*                                                          */
/* ******************************************************** */

#include "cstdlib.h"
#include "hardware.h"
#include "gdb_stub.h"
#include "net.h"
#include "servo.h"

/* ******************** LOCAL FUNCTIONS ******************* */
static  void    system_init                 (void);
static  void    system_shutdown             (void);
/* ******************************************************** */

/* *************************************************** */
/* BSL IP SLAVE: Mainloop                              */
/* *************************************************** */
int main(void)
{
    UINT1 brunning = 1;

    system_init();

    signal_app(); /* Hardware initialized */

    while(brunning)
    {
        net_cycl();
    }

    system_shutdown();

    return 0;
}

/* *************************************************** */
/* BSL IP SLAVE: Initialize system                     */
/* *************************************************** */
static void system_init(void)
{
    hw_init();
    stub_init();
    net_init();
    servo_enable_pwm(SERVO_PWM_ID0);
}

/* *************************************************** */
/* BSL IP SLAVE: Shutdown system                       */
/* *************************************************** */
static void system_shutdown(void)
{
}
