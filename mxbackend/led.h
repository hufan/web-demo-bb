#ifndef LED_H
#define LED_H

#define DEBUG 1

#ifdef DEBUG
#define dbg_printf(fmt, args...) printf(fmt, ##args)
#define dbg_perror(msg) (perror(msg))
#else
#define dbg_printf(fmt, args...)
#define dbg_perror(msg)
#endif


int set_led_brightness(const char * led, int on);
void  get_led_list(char * led_list);
void led_init(void);
#endif