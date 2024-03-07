#include <communication.h>

bool Communication::beginRangingLora(Ranging_Wrapper::Mode &ranging_mode, Ranging_Wrapper::Lora_Device &ranging_device)
{
    String status = _ranging.init(ranging_mode, ranging_device);

    if (status != "")
    {
        return false;
    }
    return true;
}

bool Communication::beginRadioOne(RadioLib_Wrapper<radio_module>::Radio_Config &radio_config_one)
{
    // Create a radio object
    _radio_one = new RadioLib_Wrapper<radio_module>(nullptr, 5, "Radio One");

    // Initialize the radio
    if (!_radio_one->begin(radio_config_one))
    {
        return false;
    }

    return true;
}

bool Communication::beginRadioTwo(RadioLib_Wrapper<radio_module>::Radio_Config &radio_config_two)
{
    // Create a radio object
    _radio_two = new RadioLib_Wrapper<radio_module>(nullptr, 5, "Radio Two");

    // Initialize the radio
    if (!_radio_two->begin(radio_config_two))
    {
        return false;
    }

    return true;
}

void Communication::runRanging(Config &config)
{
    if (_ranging.slave_reenable(config.RANGING_LORA_TIMEOUT, _slave))
    {
        // Turn on buzzer
        tone(config.BUZZER_PIN, 1000, 150);
    }
}

bool Communication::sendRadioOne(byte *bytes, size_t size)
{
    // Send the message
    if (!_radio_one->transmit_bytes(bytes, size))
    {
        return false;
    }

    return true;
}

bool Communication::sendRadioTwo(byte *bytes, size_t size)
{
    // Send the message
    if (!_radio_two->transmit_bytes(bytes, size))
    {
        return false;
    }

    return true;
}

bool Communication::receiveRadioOne(byte *&msg, uint16_t &msg_length, float &rssi, float &snr, double &frequency)
{
    // Receive the message
    if (!_radio_one->receive_bytes(msg, msg_length, rssi, snr, frequency))
    {
        return false;
    }

    return true;
}

bool Communication::receiveRadioTwo(byte *&msg, uint16_t &msg_length, float &rssi, float &snr, double &frequency)
{
    // Receive the message
    if (!_radio_two->receive_bytes(msg, msg_length, rssi, snr, frequency))
    {
        return false;
    }

    return true;
}