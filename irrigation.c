#include <reg51.h>

#define LCD P2

sbit RS   = P3^3;
sbit EN   = P3^4;
sbit PUMP = P3^5;

unsigned char pump_state = 0;

void delay(unsigned int ms)
{
    unsigned int i, j;

    for (i = 0; i < ms; i++)
        for (j = 0; j < 120; j++);
}

void lcd_cmd(unsigned char c)
{
    RS = 0;
    LCD = c;
    EN = 1;
    delay(2);
    EN = 0;
    delay(2);
}

void lcd_data(unsigned char d)
{
    RS = 1;
    LCD = d;
    EN = 1;
    delay(2);
    EN = 0;
    delay(2);
}

void lcd_init(void)
{
    delay(20);

    lcd_cmd(0x38);
    lcd_cmd(0x0C);
    lcd_cmd(0x06);
    lcd_cmd(0x01);
    delay(5);
}

void lcd_str(char *s)
{
    while (*s)
        lcd_data(*s++);
}

void lcd_num(unsigned int n)
{
    if (n >= 100)
        lcd_data((n / 100) + '0');

    if (n >= 10)
        lcd_data(((n / 10) % 10) + '0');
    else if (n >= 100)
        lcd_data('0');

    lcd_data((n % 10) + '0');
}

void main(void)
{
    unsigned char adc;
    unsigned char pct;

    P1 = 0xFF;
    PUMP = 0;

    lcd_init();

    while (1)
    {
        adc = P1;

        pct = (unsigned char)
              (((unsigned int)(255 - adc) * 100) / 255);

        if (pct < 40)
            pump_state = 1;
        else if (pct > 60)
            pump_state = 0;

        PUMP = pump_state;

        lcd_cmd(0x80);
        lcd_str("ADC:");
        lcd_num(adc);
        lcd_str(" M:");
        lcd_num(pct);
        lcd_data('%');
        lcd_str("  ");

        lcd_cmd(0xC0);

        if (pump_state == 1)
            lcd_str("Pump: ON ");
        else
            lcd_str("Pump: OFF");

        delay(200);
    }
	}