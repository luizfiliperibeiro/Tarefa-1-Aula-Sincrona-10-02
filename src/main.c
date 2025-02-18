#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "hardware/adc.h"
#include "hardware/gpio.h"
#include "hardware/pwm.h"
#include "ssd1306.h"

// **Definição dos Pinos**
#define LED_R 11  // LED Vermelho (PWM - Eixo X)
#define LED_G 12  // LED Verde (Alternado pelo botão do Joystick e pelo botão A)
#define LED_B 13  // LED Azul (PWM - Eixo Y)
#define BOTAO_JOYSTICK 22  // Botão do Joystick
#define BOTAO_A 5  // Botão A
#define JOYSTICK_X 26  // ADC0 - Eixo X
#define JOYSTICK_Y 27  // ADC1 - Eixo Y

// **Definição do Display OLED (I2C)**
#define I2C_SDA 14
#define I2C_SCL 15
#define I2C_PORT i2c1
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define SQUARE_SIZE 8

// **Variáveis de Estado**
volatile bool estado_led_verde = false;  // LED Verde começa APAGADO
volatile bool leds_ativos = true;
volatile int estilo_borda = 0;
ssd1306_t display;
int ultimo_x = -1, ultimo_y = -1;

// **Configuração do PWM**
void setup_pwm(uint gpio) {
    gpio_set_function(gpio, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(gpio);
    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, 4.0f);
    pwm_init(slice_num, &config, true);
}

// **Controle do LED Verde e Borda**
void alternar_led_verde_e_borda() {
    estado_led_verde = !estado_led_verde;
    gpio_put(LED_G, estado_led_verde);
    
    estilo_borda = (estilo_borda + 1) % 3;
    ssd1306_fill(&display, 0);
    
    if (estilo_borda == 0) {  
        ssd1306_hline(&display, 0, SCREEN_WIDTH - 1, 0, 1);
        ssd1306_hline(&display, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1, 1);
        ssd1306_vline(&display, 0, 0, SCREEN_HEIGHT - 1, 1);
        ssd1306_vline(&display, SCREEN_WIDTH - 1, 0, SCREEN_HEIGHT - 1, 1);
    } else if (estilo_borda == 1) {  
        for (int i = 0; i < SCREEN_WIDTH; i += 4) {
            ssd1306_hline(&display, i, i+1, 0, 1);
            ssd1306_hline(&display, i, i+1, SCREEN_HEIGHT - 1, 1);
        }
        for (int i = 0; i < SCREEN_HEIGHT; i += 4) {
            ssd1306_vline(&display, 0, i, i+1, 1);
            ssd1306_vline(&display, SCREEN_WIDTH - 1, i, i+1, 1);
        }
    } else {  
        ssd1306_rect(&display, 2, 2, SCREEN_WIDTH - 4, SCREEN_HEIGHT - 4, 1, 0);
    }

    ssd1306_send_data(&display);
}

// **Função para Alternar TODOS os LEDs (PWM e LED Verde)**
void alternar_leds_pwm() {
    leds_ativos = !leds_ativos;
    if (leds_ativos) {
        pwm_set_gpio_level(LED_R, 32767);
        pwm_set_gpio_level(LED_B, 32767);
        gpio_put(LED_G, 1);  // Liga o LED Verde
    } else {
        pwm_set_gpio_level(LED_R, 0);
        pwm_set_gpio_level(LED_B, 0);
        gpio_put(LED_G, 0);  // Desliga o LED Verde
    }
}

// **Interrupções dos Botões**
void botao_joystick_irq(uint gpio, uint32_t events) {
    static uint32_t ultimo_tempo = 0;
    uint32_t tempo_atual = to_ms_since_boot(get_absolute_time());
    if (tempo_atual - ultimo_tempo < 200) return;
    ultimo_tempo = tempo_atual;

    alternar_led_verde_e_borda();
}

void botao_a_irq(uint gpio, uint32_t events) {
    static uint32_t ultimo_tempo = 0;
    uint32_t tempo_atual = to_ms_since_boot(get_absolute_time());
    if (tempo_atual - ultimo_tempo < 200) return;
    ultimo_tempo = tempo_atual;

    alternar_leds_pwm();
}

// **Mapeamento de Valores**
int map_value(int value, int in_min, int in_max, int out_min, int out_max) {
    return (value - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

// **Configuração do ADC**
void setup_adc() {
    adc_init();
    adc_gpio_init(JOYSTICK_X);
    adc_gpio_init(JOYSTICK_Y);
}

// **Leitura do Joystick**
uint16_t read_adc(uint channel) {
    adc_select_input(channel);
    return adc_read();
}

int main() {
    stdio_init_all();

    // **Configuração de Hardware**
    setup_pwm(LED_R);
    setup_pwm(LED_B);
    setup_adc();

    gpio_init(LED_G);
    gpio_set_dir(LED_G, GPIO_OUT);
    gpio_put(LED_G, estado_led_verde);  // LED Verde inicia APAGADO

    gpio_init(BOTAO_JOYSTICK);
    gpio_set_dir(BOTAO_JOYSTICK, GPIO_IN);
    gpio_pull_up(BOTAO_JOYSTICK);
    gpio_set_irq_enabled_with_callback(BOTAO_JOYSTICK, GPIO_IRQ_EDGE_FALL, true, &botao_joystick_irq);

    gpio_init(BOTAO_A);
    gpio_set_dir(BOTAO_A, GPIO_IN);
    gpio_pull_up(BOTAO_A);
    gpio_set_irq_enabled_with_callback(BOTAO_A, GPIO_IRQ_EDGE_FALL, true, &botao_a_irq);

    printf("Sistema iniciado.\n");

    while (1) {
        uint16_t x_value = read_adc(0);
        uint16_t y_value = read_adc(1);
        pwm_set_gpio_level(LED_R, abs(x_value - 2048) * 65535 / 2048);
        pwm_set_gpio_level(LED_B, abs(y_value - 2048) * 65535 / 2048);
        sleep_ms(50);
    }

    return 0;
}