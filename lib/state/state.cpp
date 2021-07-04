#include "state.h"
#include <Arduino.h>

State::State()
{
    _semaphore = xSemaphoreCreateMutex();

    if (_semaphore == NULL) {
        Serial.println("Failed to acquire semaphore");
    }

    xSemaphoreGive(_semaphore);
}

bool State::isWifiConnected()
{
    auto connected = false;

    if (xSemaphoreTake(_semaphore, (TickType_t)50) == pdTRUE) {
        connected = _wifiConnected;
        xSemaphoreGive(_semaphore);
    }

    return connected;
}

bool State::isMqttConnected()
{
    auto connected = false;

    if (xSemaphoreTake(_semaphore, (TickType_t)50) == pdTRUE) {
        connected = _mqttConnected;
        xSemaphoreGive(_semaphore);
    }

    return connected;
}

void State::setWifiConnected(bool state)
{
    if (xSemaphoreTake(_semaphore, (TickType_t)50) == pdTRUE) {
        _wifiConnected = state;
        xSemaphoreGive(_semaphore);
    }
}

void State::setMqttConnected(bool state)
{
    if (xSemaphoreTake(_semaphore, (TickType_t)50) == pdTRUE) {
        _mqttConnected = state;
        xSemaphoreGive(_semaphore);
    }
}

void State::logState()
{
    if (xSemaphoreTake(_semaphore, (TickType_t)50) == pdTRUE) {
        Serial.printf("[STATE] Wifi connected: %d\r\n", _wifiConnected);
        Serial.printf("[STATE] Mqtt connected: %d\r\n", _mqttConnected);
        xSemaphoreGive(_semaphore);
    }
}
