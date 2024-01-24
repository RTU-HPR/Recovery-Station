#include <communication.h>

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