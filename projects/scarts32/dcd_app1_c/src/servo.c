/* ******************************************************** */
/*                                                          */
/*                                                          */
/*   ____-_-_-_-_-_-_-_-________________________            */
/*                                                          */
/*                                                          */
/*   File:          servo.c                                 */
/*   Description:   Servo motor controller                  */
/*   Autor:         Johannes Natter                         */
/*   Date:          07.03.2013                              */
/*                                                          */
/*   Notes:                                                 */
/*                                                          */
/*                                                          */
/* ******************************************************** */

#include "servo.h"
#include "modules.h"

/* ******************** LOCAL FUNCTIONS ******************* */
/* ******************************************************** */

/* **************************************************** */
/* Servo: Enable pwm                                    */
/* **************************************************** */
void servo_enable_pwm(SERVO_PWM_ID_ENUM pwm_id)
{
    switch(pwm_id)
    {
        case SERVO_PWM_ID0: p_scarts->servo.gen_if.usr_conf |= SERVO_REG_CONF_PWM0_ENABLE; break;
        case SERVO_PWM_ID1: p_scarts->servo.gen_if.usr_conf |= SERVO_REG_CONF_PWM1_ENABLE; break;
        case SERVO_PWM_ID2: p_scarts->servo.gen_if.usr_conf |= SERVO_REG_CONF_PWM2_ENABLE; break;
        case SERVO_PWM_ID3: p_scarts->servo.gen_if.usr_conf |= SERVO_REG_CONF_PWM3_ENABLE; break;
        default:
            break;
    }
}

/* **************************************************** */
/* Servo: Disable pwm                                   */
/* **************************************************** */
void servo_disable_pwm(SERVO_PWM_ID_ENUM pwm_id)
{
    switch(pwm_id)
    {
        case SERVO_PWM_ID0: p_scarts->servo.gen_if.usr_conf &= ~SERVO_REG_CONF_PWM0_ENABLE; break;
        case SERVO_PWM_ID1: p_scarts->servo.gen_if.usr_conf &= ~SERVO_REG_CONF_PWM1_ENABLE; break;
        case SERVO_PWM_ID2: p_scarts->servo.gen_if.usr_conf &= ~SERVO_REG_CONF_PWM2_ENABLE; break;
        case SERVO_PWM_ID3: p_scarts->servo.gen_if.usr_conf &= ~SERVO_REG_CONF_PWM3_ENABLE; break;
        default:
            break;
    }
}

/* **************************************************** */
/* Servo: Set width of pwm high pulse                   */
/*        High pulse every 20ms                         */
/* **************************************************** */
void servo_set_high_pulse_width(SERVO_PWM_ID_ENUM pwm_id, uint32_t num_20ns_ticks)
{
    if(pwm_id > SERVO_PWM_ID3)
        return;

    if((SERVO_MIN_TICKS > num_20ns_ticks) ||
       (SERVO_MAX_TICKS < num_20ns_ticks))
        return;

    p_scarts->servo.pwms_20ns_ticks[pwm_id] = num_20ns_ticks;
}

/* **************************************************** */
/* Servo: Get width of pwm high pulse                   */
/* **************************************************** */
uint32_t servo_get_high_pulse_width(SERVO_PWM_ID_ENUM pwm_id)
{
    if(pwm_id > SERVO_PWM_ID3)
        return 0;

    return p_scarts->servo.pwms_20ns_ticks[pwm_id];
}
