/* ******************************************************** */
/*                                                          */
/*                                                          */
/*   ____-_-_-_-_-_-_-_-________________________            */
/*                                                          */
/*                                                          */
/*   File:          servo.h                                 */
/*   Description:   Servo motor controller                  */
/*   Autor:         Johannes Natter                         */
/*   Date:          07.03.2013                              */
/*                                                          */
/*   Notes:                                                 */
/*                                                          */
/*                                                          */
/* ******************************************************** */

#ifndef SERVO_CTRL_H
#define SERVO_CTRL_H

#include <stdint.h>

/* default: all disabled */
typedef enum
{
    SERVO_PWM_ID0 = 0,
    SERVO_PWM_ID1,
    SERVO_PWM_ID2,
    SERVO_PWM_ID3
} SERVO_PWM_ID_ENUM;

#define SERVO_MIN_TICKS     ((uint32_t) 50000)
#define SERVO_MAX_TICKS     ((uint32_t)100000)

void        servo_enable_pwm            (SERVO_PWM_ID_ENUM pwm_id);
void        servo_disable_pwm           (SERVO_PWM_ID_ENUM pwm_id);
void        servo_set_high_pulse_width  (SERVO_PWM_ID_ENUM pwm_id, uint32_t num_20ns_ticks);
uint32_t    servo_get_high_pulse_width  (SERVO_PWM_ID_ENUM pwm_id);

#endif
