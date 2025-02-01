/*
 * Por: Israel Falcão Jesus 
 * 
 * Estecodigo é adaptação do codigo do professo (Wilton Lacerda Silva) do SDK Pico
 * para a utilização da matriz de LEDs WS2812 do BitDogLab para 
 * cocnluir o exercico proposta na aula 27/01/25.
 * 
 * A seleÃ§Ã£o de LEDs acesos Ã© feita por meio de um buffer de LEDs, onde
 * cada posiÃ§Ã£o do buffer representa um LED da matriz 5x5.
 * 
 * Original em:
 * https://github.com/raspberrypi/pico-examples/tree/master/pio/ws2812
 */

#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "ws2812.pio.h"

#define LED_PIN 13
#define BUTTON_A_PIN 5  // Pino do botão A
#define BUTTON_B_PIN 6 // pino do botão B
#define IS_RGBW false
#define NUM_PIXELS 25
#define WS2812_PIN 7
#define TEMPO 400

// Variável global para armazenar a cor (Entre 0 e 255 para intensidade)
uint8_t led_r = 0;   // Intensidade do vermelho
uint8_t led_g = 0;   // Intensidade do verde
uint8_t led_b = 200; // Intensidade do azul

// Buffers para armazenar os desenhos da matriz 5x5
bool led_buffer[NUM_PIXELS] = {
    0, 1, 1, 1, 0, 
    0, 1, 0, 1, 0, 
    0, 1, 0, 1, 0, 
    0, 1, 0, 1, 0, 
    0, 1, 1, 1, 0
};
bool led_buffer1[NUM_PIXELS] = {
    0, 0, 1, 0, 0, 
    0, 0, 1, 0, 0, 
    0, 0, 1, 0, 0, 
    0, 0, 1, 0, 0, 
    0, 0, 1, 1, 0
};
bool led_buffer2[NUM_PIXELS] = {
    0, 1, 1, 1, 0, 
    0, 1, 0, 0, 0, 
    0, 1, 1, 1, 0, 
    0, 0, 0, 1, 0, 
    0, 1, 1, 1, 0
};
bool led_buffer3[NUM_PIXELS] = {
    0, 1, 1, 1, 0, 
    0, 0, 0, 1, 0, 
    0, 1, 1, 1, 0, 
    0, 0, 0, 1, 0, 
    0, 1, 1, 1, 0
};
bool led_buffer4[NUM_PIXELS] = {
    0, 1, 0, 0, 0, 
    0, 0, 0, 1, 0, 
    0, 1, 1, 1, 0, 
    0, 1, 0, 1, 0, 
    0, 1, 0, 1, 0
};
bool led_buffer5[NUM_PIXELS] = {
    0, 1, 1, 1, 0, 
    0, 0, 0, 1, 0, 
    0, 1, 1, 1, 0, 
    0, 1, 0, 0, 0, 
    0, 1, 1, 1, 0
};
bool led_buffer6[NUM_PIXELS] = {
    0, 1, 1, 1, 0, 
    0, 1, 0, 1, 0, 
    0, 1, 1, 1, 0, 
    0, 1, 0, 0, 0, 
    0, 1, 1, 1, 0
};
bool led_buffer7[NUM_PIXELS] = {
    0, 1, 0, 0, 0, 
    0, 0, 0, 1, 0, 
    0, 1, 0, 0, 0, 
    0, 0, 0, 1, 0, 
    0, 1, 1, 1, 0
};
bool led_buffer8[NUM_PIXELS] = {
    0, 1, 1, 1, 0, 
    0, 1, 0, 1, 0, 
    0, 1, 1, 1, 0, 
    0, 1, 0, 1, 0, 
    0, 1, 1, 1, 0
};
bool led_buffer9[NUM_PIXELS] = {
    0, 1, 1, 1, 0, 
    0, 0, 0, 1, 0, 
    0, 1, 1, 1, 0, 
    0, 1, 0, 1, 0, 
    0, 1, 1, 1, 0
};

// Array de buffers para facilitar a troca de desenhos
bool* buffers[10] = {
    led_buffer, led_buffer1, led_buffer2, led_buffer3, led_buffer4,
    led_buffer5, led_buffer6, led_buffer7, led_buffer8, led_buffer9
};

static inline void put_pixel(uint32_t pixel_grb) {
    pio_sm_put_blocking(pio0, 0, pixel_grb << 8u);
}

static inline uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b) {
    return ((uint32_t)(r) << 8) | ((uint32_t)(g) << 16) | (uint32_t)(b);
}

void set_one_led(uint8_t r, uint8_t g, uint8_t b, bool* buffer) {
    // Define a cor com base nos parâmetros fornecidos
    uint32_t color = urgb_u32(r, g, b);

    // Define todos os LEDs com a cor especificada
    for (int i = 0; i < NUM_PIXELS; i++) {
        if (buffer[i]) {
            put_pixel(color); // Liga o LED com true no buffer
        } else {
            put_pixel(0);     // Desliga os LEDs com false no buffer
        }
    }
}

int main() {
    // Inicializa os pinos dos botões como entrada com pull-up
    gpio_init(BUTTON_A_PIN);
    gpio_set_dir(BUTTON_A_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_A_PIN);

    gpio_init(BUTTON_B_PIN);
    gpio_set_dir(BUTTON_B_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_B_PIN);

    // Inicializa o pino do LED vermelho como saída
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    // Configura a matriz de LEDs WS2812
    PIO pio = pio0;
    int sm = 0;
    uint offset = pio_add_program(pio, &ws2812_program);
    ws2812_program_init(pio, sm, offset, WS2812_PIN, 800000, IS_RGBW);

    int current_drawing = 0; // Índice do desenho atual
    bool button_a_pressed = false; // Evita múltiplas leituras do botão A
    bool button_b_pressed = false; // Evita múltiplas leituras do botão B
    uint32_t last_button_time = 0; // Armazena o tempo do último pressionamento

    while (true) { // Loop infinito
        uint32_t current_time = to_ms_since_boot(get_absolute_time());

        // Verifica se o botão A foi pressionado (estado LOW)
        if (gpio_get(BUTTON_A_PIN) == false) {
            if (current_time - last_button_time > 50 && !button_a_pressed) { // Debounce
                button_a_pressed = true;
                current_drawing = (current_drawing + 1) % 10; // Incrementa o índice
                last_button_time = current_time;
            }
        } else {
            button_a_pressed = false; // Reseta o estado do botão A
        }

        // Verifica se o botão B foi pressionado (estado LOW)
        if (gpio_get(BUTTON_B_PIN) == false) {
            if (current_time - last_button_time > 50 && !button_b_pressed) { // Debounce
                button_b_pressed = true;
                current_drawing = (current_drawing - 1 + 10) % 10; // Decrementa o índice
                last_button_time = current_time;
            }
        } else {
            button_b_pressed = false; // Reseta o estado do botão B
        }

        // Exibe o desenho atual
        set_one_led(led_r, led_g, led_b, buffers[current_drawing]);

        // Pisca o LED no pino 13 (opcional)
        gpio_put(LED_PIN, true); // Acende o LED (HIGH)
        sleep_ms(100);           // Mantém aceso por 100 ms
        gpio_put(LED_PIN, false); // Apaga o LED (LOW)
        sleep_ms(100);           // Mantém apagado por 100 ms

      
    }


    return 0;
}