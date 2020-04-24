/* LwIP SNTP example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <time.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "moonmoon.h"

static const char *TAG = "moonmain";

/* Variable holding number of times ESP32 restarted since first boot.
 * It is placed into RTC memory using RTC_DATA_ATTR and
 * maintains its value when ESP32 wakes from deep sleep.
 */
RTC_DATA_ATTR static int boot_count = 0;


void app_main(void)
{
    ++boot_count;
    ESP_LOGI(TAG, "Boot count: %d", boot_count);

    // Set up LEDs
    led_init();

    // Set local time to Eastern
    setenv("TZ", "EST5EDT,M3.2.0/2,M11.1.0", 1);
    tzset();

    for(;;) {
        time_t now;
        struct tm timeinfo;
        time(&now);
        localtime_r(&now, &timeinfo);
        // Is time set? If not, tm_year will be (1970 - 1900).
        if (timeinfo.tm_year < (2016 - 1900)) {
            ESP_LOGI(TAG, "Time is not set yet. Connecting to WiFi and getting time over NTP.");
            obtain_time();
            // update 'now' variable with current time
            time(&now);
        }

        char strftime_buf[64];

        // Set timezone to Eastern Standard Time and print local time
        localtime_r(&now, &timeinfo);
        strftime(strftime_buf, sizeof(strftime_buf), "%c", &timeinfo);
        ESP_LOGI(TAG, "The current date/time in New York is: %s", strftime_buf);

        int phase = moonPhases(timeinfo.tm_year + 1900, timeinfo.tm_mon + 1, timeinfo.tm_mday);
        ESP_LOGI(TAG, "The current lunar day is: %i", phase);

        const int loop_time_sec = 10;
        ESP_LOGI(TAG, "Waiting for %d seconds", loop_time_sec);
        vTaskDelay(loop_time_sec * 1000 / portTICK_PERIOD_MS);
    }
}
