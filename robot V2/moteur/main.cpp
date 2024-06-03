#include <stdio.h>
#include <hardware/gpio.h>
#include <hardware/pwm.h>
#include <hardware/adc.h>
#include <pico/i2c_slave.h>
#include <pico/time.h>
#include <pico/stdio.h>

static const uint I2C_SLAVE_SDA_PIN = 8;
static const uint I2C_SLAVE_SCL_PIN = 9;

static const uint I2C_SLAVE_ADDRESS = 0x17;
static const uint I2C_BAUDRATE = 400000; // 400 kHz

static volatile long pos = 0L;
static volatile int v = 0.;
absolute_time_t t;
absolute_time_t tp;
static double dt;
static volatile uint64_t tus;
float e;
float s=0;
int dataindex = 0;
uint8_t data[10];
float kp = 10.;
float ki = 1.;

static int i = 0;
static volatile int cons=0;
static volatile int16_t cons_temp = 0;
int c;
char buf[255];
int k=0;
int j=0;
long vmoy;
static volatile uint16_t tension;

void gpio_event_string(char *buf, uint32_t events);

void gpio_callback(uint gpio, uint32_t events) {
    t =  get_absolute_time();
    tus =  to_us_since_boot(t);
    dt =  ((double)absolute_time_diff_us(tp, t)) * 1e-6;
    tp = t;
    bool vA = gpio_get(3);
    bool vB = gpio_get(4);
    if (vA == vB)
    {
        pos++;
        v=1./dt;
    }
    else 
    {
        pos--;
         v=-1./dt;
    }
//    tension = adc_read();
}

static void i2c_slave_handler(i2c_inst_t *i2c, i2c_slave_event_t event) {
    uint8_t b;
    switch (event) {
    case I2C_SLAVE_RECEIVE: // master has written some data
        b = i2c_read_byte_raw(i2c);
        data[dataindex++]=b;
//        printf("received : % u\n", b);
        break;
    case I2C_SLAVE_REQUEST: // master is requesting data
//        printf("request\n");
        i2c_write_byte_raw(i2c, 0xA0);
        break;
    case I2C_SLAVE_FINISH: // master has signalled Stop / Restart
//        printf("finish\n");
        if (dataindex>1)
        {
            cons_temp = data[0];
            cons_temp <<= 8;
            cons_temp |= data[1];
//            printf("cons : %d\n", cons_temp);
            cons= cons_temp;   
        }    
//        printf("dataindex : %d\n", dataindex);
        if (dataindex>9)
        {
            *((uint8_t *)&kp) = data[5];
            *(((uint8_t *)&kp)+1)= data[4];
            *(((uint8_t *)&kp)+2)= data[3];
            *(((uint8_t *)&kp)+3)= data[2];

            *((uint8_t *)&ki) = data[9];
            *(((uint8_t *)&ki)+1)= data[8];
            *(((uint8_t *)&ki)+2)= data[7];
            *(((uint8_t *)&ki)+3)= data[6];

 //           printf("kp : %f, ki : %f\n", kp, ki);
        }    
        dataindex=0;  
        break;
    default:
        break;
    }
}

int main()
{
    stdio_init_all();

    adc_init();

    adc_gpio_init(26);
    adc_select_input(0);

    gpio_init(I2C_SLAVE_SDA_PIN);
    gpio_set_function(I2C_SLAVE_SDA_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SLAVE_SDA_PIN);

    gpio_init(I2C_SLAVE_SCL_PIN);
    gpio_set_function(I2C_SLAVE_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SLAVE_SCL_PIN);

    i2c_init(i2c0, I2C_BAUDRATE);
    // configure I2C0 for slave mode
    i2c_slave_init(i2c0, I2C_SLAVE_ADDRESS, &i2c_slave_handler);

    gpio_init(1);
    gpio_set_dir(1, true);
    gpio_put(1, false);

    gpio_init(2);
    gpio_set_dir(2, true);
    gpio_put(2, true);

    gpio_init(3);
    gpio_set_dir(3, false);
    gpio_init(4);
    gpio_set_dir(4, false);

    gpio_set_irq_enabled_with_callback(3, GPIO_IRQ_EDGE_RISE | GPIO_IRQ_EDGE_FALL, true, &gpio_callback);
   

    const uint pwm_pin = 0; 

    gpio_set_function(pwm_pin, GPIO_FUNC_PWM); // fonction PWM sur GP8   
    uint slice = pwm_gpio_to_slice_num(pwm_pin); // slice associée à GP8   
    uint channel = pwm_gpio_to_channel(pwm_pin);  // channel associée à GP8 

    pwm_set_phase_correct (slice, false); // mode phase-correct non activé 

    pwm_set_clkdiv_int_frac (slice, 6, 2); // diviseur de fréquence = 100 + 0/16 
    pwm_set_wrap(slice, 10230);  // valeur wrap pour fixer la fréquence 
    pwm_set_chan_level(slice, channel, 0);

    pwm_set_enabled(slice, true); // activer le signal 

    while(1)
    {    
       if ((c = getchar_timeout_us(1)) != PICO_ERROR_TIMEOUT )
        {
            if (c != '\n')
            {
                buf[k]=c;
                k++;
            }
            else
            {
                buf[k]=0;
                printf("%s\n", buf);
                sscanf(buf, "%d", &cons);
                k=0;
            }

        }
        e=cons-v;
        if ((i < 10230) && ( i > -10230))
            s+=ki*e;
        i=kp*e+s;
        if (i>=0)
        {
            gpio_put(1, true);
            gpio_put(2, false);
        }
        else
        {
            gpio_put(1, false);
            gpio_put(2, true);
            i=-i;
        }
        if (i>10230)
            i=10230;
        pwm_set_chan_level(slice, channel, i);
        printf("%d;", v);
        printf("%d\n", cons);

        sleep_ms(10);
    }
}
