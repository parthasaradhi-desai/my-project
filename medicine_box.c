#include <reg51.h>
#include <intrins.h>

#define LCD_DATA P2
sbit RS  = P3^0;
sbit EN  = P3^1;
sbit SDA = P1^0;
sbit SCL = P1^1;
sbit LED_M = P1^2;
sbit LED_A = P1^3;
sbit LED_N = P1^4;
sbit BUZZER   = P1^5;
sbit STOP_BTN = P3^2;

#define MORNING_H    8
#define MORNING_M    0
#define AFTERNOON_H  14
#define AFTERNOON_M  0
#define NIGHT_H      21
#define NIGHT_M      40

#define INIT_H 12
#define INIT_M 0
#define INIT_S 0

void delay_ms(unsigned int ms)
{
    unsigned int i, j;
    for (i = 0; i < ms; i++)
        for (j = 0; j < 123; j++);
}

void lcd_cmd(unsigned char c)
{
    LCD_DATA = c; RS = 0; EN = 1;
    delay_ms(1); EN = 0; delay_ms(2);
}

void lcd_data(unsigned char d)
{
    LCD_DATA = d; RS = 1; EN = 1;
    delay_ms(1); EN = 0; delay_ms(2);
}

void lcd_init(void)
{
    delay_ms(20);
    lcd_cmd(0x38);
    lcd_cmd(0x0C);
    lcd_cmd(0x06);
    lcd_cmd(0x01);
}

void lcd_str(char *s)
{
    while (*s) lcd_data(*s++);
}

void lcd_2d(unsigned char n)
{
    lcd_data('0' + n / 10);
    lcd_data('0' + n % 10);
}

void i2c_delay(void)
{
    _nop_(); _nop_(); _nop_(); _nop_();
}

void i2c_start(void)
{
    SDA = 1; SCL = 1; i2c_delay();
    SDA = 0; i2c_delay();
    SCL = 0;
}

void i2c_stop(void)
{
    SDA = 0; SCL = 1; i2c_delay();
    SDA = 1; i2c_delay();
}

void i2c_write(unsigned char d)
{
    unsigned char i;
    for (i = 0; i < 8; i++) {
        SDA = (d & 0x80) ? 1 : 0;
        d <<= 1;
        SCL = 1; i2c_delay();
        SCL = 0;
    }
    SDA = 1;
    SCL = 1; i2c_delay();
    SCL = 0;
}

unsigned char i2c_read(unsigned char last)
{
    unsigned char i, d = 0;
    SDA = 1;
    for (i = 0; i < 8; i++) {
        SCL = 1; i2c_delay();
        d = (d << 1) | SDA;
        SCL = 0; i2c_delay();
    }
    SDA = last ? 1 : 0;
    SCL = 1; i2c_delay();
    SCL = 0;
    SDA = 1;
    return d;
}

unsigned char bcd2bin(unsigned char b) { return (b >> 4) * 10 + (b & 0x0F); }
unsigned char bin2bcd(unsigned char b) { return ((b / 10) << 4) | (b % 10); }

void rtc_init(void)
{
    unsigned char sec;

    i2c_start();
    i2c_write(0xD0);
    i2c_write(0x00);
    i2c_start();
    i2c_write(0xD1);
    sec = i2c_read(1);
    i2c_stop();

    if (sec & 0x80) {
        i2c_start();
        i2c_write(0xD0);
        i2c_write(0x00);
        i2c_write(bin2bcd(INIT_S));
        i2c_write(bin2bcd(INIT_M));
        i2c_write(bin2bcd(INIT_H));
        i2c_stop();
    }
}

void rtc_read(unsigned char *h, unsigned char *m, unsigned char *s)
{
    i2c_start();
    i2c_write(0xD0);
    i2c_write(0x00);
    i2c_start();
    i2c_write(0xD1);
    *s = bcd2bin(i2c_read(0) & 0x7F);
    *m = bcd2bin(i2c_read(0));
    *h = bcd2bin(i2c_read(1) & 0x3F);
    i2c_stop();
}

void all_off(void)
{
    LED_M = 1; LED_A = 1; LED_N = 1;
    BUZZER = 0;
}

unsigned char dose_slot(unsigned char h, unsigned char m)
{
    if (h == MORNING_H   && m == MORNING_M)   return 0;
    if (h == AFTERNOON_H && m == AFTERNOON_M) return 1;
    if (h == NIGHT_H     && m == NIGHT_M)     return 2;
    return 0xFF;
}

void led_on(unsigned char slot)
{
    all_off();
    if (slot == 0) LED_M = 0;
    if (slot == 1) LED_A = 0;
    if (slot == 2) LED_N = 0;
}

void main(void)
{
    unsigned char h, m, s, slot;
    unsigned char alarm_slot = 0xFF;
    unsigned char acked_slot = 0xFF;
    unsigned char tick = 0;

    STOP_BTN = 1;
    all_off();
    lcd_init();
    rtc_init();

    while (1) {
        rtc_read(&h, &m, &s);

        lcd_cmd(0x80);
        lcd_2d(h); lcd_data(':'); lcd_2d(m); lcd_data(':'); lcd_2d(s);
        lcd_str("        ");

        slot = dose_slot(h, m);
        if (slot == 0xFF)
            acked_slot = 0xFF;
        else if (slot != acked_slot && alarm_slot == 0xFF)
            alarm_slot = slot;

        lcd_cmd(0xC0);
        if (alarm_slot == 0)      lcd_str("Take MORNING med");
        else if (alarm_slot == 1) lcd_str("Take AFTERNOON  ");
        else if (alarm_slot == 2) lcd_str("Take NIGHT med  ");
        else                      lcd_str("Box ready       ");

        if (alarm_slot != 0xFF) {
            led_on(alarm_slot);
            BUZZER = (tick < 2) ? 1 : 0;
            tick = (tick + 1) % 4;

            if (STOP_BTN == 0) {
                delay_ms(30);
                if (STOP_BTN == 0) {
                    acked_slot = alarm_slot;
                    alarm_slot = 0xFF;
                    all_off();
                    tick = 0;
                }
            }
        }

        delay_ms(100);
    }
}
