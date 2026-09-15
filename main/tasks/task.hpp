#ifndef TASK_HPP_
#define TASK_HPP_

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

template <typename Derived>
class Task {
public:
    void start(const char* name, uint32_t stackSize = 2048,
               UBaseType_t priority = 5) {
        xTaskCreate(&Task::taskEntry, name, stackSize, this, priority, nullptr);
    }

protected:
    static constexpr uint8_t kMaxInitRetries = 5;

private:
    static void taskEntry(void *parameter) {
        // Pointer to the task passed in the template type
        auto *self = static_cast<Derived *>(parameter);

        uint8_t attempts = 0;
        bool ok = false;
        do {
            ok = self->init();
            attempts++;
        } while (!ok && attempts < kMaxInitRetries);

        if (!ok) {
            ESP_LOGE("TASK: ", "Init failed after %d attempts", attempts);
        }

        self->run();
        
        // FreeRTOS task must never return
        vTaskDelete(nullptr);
    }

};

#endif // TASK_HPP_