#include <inttypes.h>

#include "driver/ledc.h"
#include "esp_err.h"

typedef uint32_t dutycycle_t;

const dutycycle_t phase_max = 0xFFFF;
#define phase_count 28
// Lookup table mapping lunar day -> duty cycles
const dutycycle_t phases[phase_count][5] = {
    {0x0000, 0x0000, 0x0000, 0x0000, 0x0000},
    {0x1249, 0x0000, 0x0000, 0x0000, 0x0000},
    {0x2492, 0x0000, 0x0000, 0x0000, 0x0000},
    {0xFFFF, 0x03A8, 0x0000, 0x0000, 0x0000},
    {0xFFFF, 0x15F1, 0x0000, 0x0000, 0x0000},
    {0xFFFF, 0x283A, 0x0000, 0x0000, 0x0000},
    {0xFFFF, 0xFFFF, 0x0750, 0x0000, 0x0000},
    {0xFFFF, 0xFFFF, 0x1999, 0x0000, 0x0000},
    {0xFFFF, 0xFFFF, 0x2BE2, 0x0000, 0x0000},
    {0xFFFF, 0xFFFF, 0xFFFF, 0x0AF8, 0x0000},
    {0xFFFF, 0xFFFF, 0xFFFF, 0x1D41, 0x0000},
    {0xFFFF, 0xFFFF, 0xFFFF, 0x2F8A, 0x0000},
    {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x0EA0},
    {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x20E9},
    {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF},
    {0x20E9, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF},
    {0x0EA0, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF},
    {0x0000, 0x2F8A, 0xFFFF, 0xFFFF, 0xFFFF},
    {0x0000, 0x1D41, 0xFFFF, 0xFFFF, 0xFFFF},
    {0x0000, 0x0AF8, 0xFFFF, 0xFFFF, 0xFFFF},
    {0x0000, 0x0000, 0x2BE2, 0xFFFF, 0xFFFF},
    {0x0000, 0x0000, 0x1999, 0xFFFF, 0xFFFF},
    {0x0000, 0x0000, 0x0750, 0xFFFF, 0xFFFF},
    {0x0000, 0x0000, 0x0000, 0x283A, 0xFFFF},
    {0x0000, 0x0000, 0x0000, 0x15F1, 0xFFFF},
    {0x0000, 0x0000, 0x0000, 0x03A8, 0xFFFF},
    {0x0000, 0x0000, 0x0000, 0x0000, 0x2492},
    {0x0000, 0x0000, 0x0000, 0x0000, 0x1249},
};

#define for_channels(varname) for (int varname = 0; varname < 5; varname++)

ledc_channel_config_t ledc_channel[] = {
    {
        .channel    = LEDC_CHANNEL_0,
        .duty       = 0,
        .gpio_num   = CONFIG_MOON_ONE_GPIO,
        .speed_mode = LEDC_HIGH_SPEED_MODE,
        .hpoint     = phase_max,
        .timer_sel  = LEDC_TIMER_0
    },
    {
        .channel    = LEDC_CHANNEL_1,
        .duty       = 0,
        .gpio_num   = CONFIG_MOON_TWO_GPIO,
        .speed_mode = LEDC_HIGH_SPEED_MODE,
        .hpoint     = phase_max,
        .timer_sel  = LEDC_TIMER_0
    },
    {
        .channel    = LEDC_CHANNEL_2,
        .duty       = 0,
        .gpio_num   = CONFIG_MOON_THREE_GPIO,
        .speed_mode = LEDC_HIGH_SPEED_MODE,
        .hpoint     = phase_max,
        .timer_sel  = LEDC_TIMER_0
    },
    {
        .channel    = LEDC_CHANNEL_3,
        .duty       = 0,
        .gpio_num   = CONFIG_MOON_FOUR_GPIO,
        .speed_mode = LEDC_HIGH_SPEED_MODE,
        .hpoint     = phase_max,
        .timer_sel  = LEDC_TIMER_0
    },
    {
        .channel    = LEDC_CHANNEL_4,
        .duty       = 0,
        .gpio_num   = CONFIG_MOON_FIVE_GPIO,
        .speed_mode = LEDC_HIGH_SPEED_MODE,
        .hpoint     = phase_max,
        .timer_sel  = LEDC_TIMER_0
    },
};

/// Sets up the LED hardware
void led_init() {
    ledc_timer_config_t ledc_timer = {
        .duty_resolution = LEDC_TIMER_13_BIT, // resolution of PWM duty
        .freq_hz = 5000,                      // frequency of PWM signal
        .speed_mode = LEDC_HIGH_SPEED_MODE,           // timer mode
        .timer_num = LEDC_TIMER_0,            // timer index
        .clk_cfg = LEDC_AUTO_CLK,              // Auto select the source clock
    };
    ledc_timer_config(&ledc_timer);

    for_channels(ch) {
        ledc_channel_config(&ledc_channel[ch]);
    }
}

/// Updates the state of the LEDs to match the given lunar day
void set_moon_phase(int phase) {
    int phaseidx = phase % phase_count;

    for_channels(ch) {
        ledc_set_duty_and_update(
            ledc_channel[ch].speed_mode, ledc_channel[ch].channel,
            phases[phaseidx][ch], ledc_channel[ch].hpoint
        );
    }
}
