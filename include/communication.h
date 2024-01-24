#pragma once
#include <config.h>

class Communication
{
public:
  RadioLib_Wrapper<radio_module> *_radio_one;
  RadioLib_Wrapper<radio_module> *_radio_two;

  /**
   * @brief Initialise the Communication Radio
   * @param config Radio config object
   */
  bool beginRadioOne(RadioLib_Wrapper<radio_module>::Radio_Config &radio_config_one);

  /**
   * @brief Initialise the Communication Radio
   * @param config Radio config object
   */
  bool beginRadioTwo(RadioLib_Wrapper<radio_module>::Radio_Config &radio_config_two);

  /**
   * @brief Sends the provided message using LoRa
   * @param bytes The message to send
   * @param size The size of the message
   * @return Whether the message was sent successfully
   */
  bool sendRadioOne(byte *bytes, size_t size);

  /**
   * @brief Sends the provided message using LoRa
   * @param bytes The message to send
   * @param size The size of the message
   * @return Whether the message was sent successfully
   */
  bool sendRadioTwo(byte *bytes, size_t size);

  /**
   * @brief Receives a message using LoRa
   * @param msg The message to receive
   * @param msg_length The size of the message
   * @param rssi The RSSI of the received message
   * @param snr The SNR of the received message
   * @param frequency The frequency of the received message
  */
  bool receiveRadioOne(byte *&msg, uint16_t &msg_length, float &rssi, float &snr, double &frequency);

  /**
   * @brief Receives a message using LoRa
   * @param msg The message to receive
   * @param msg_length The size of the message
   * @param rssi The RSSI of the received message
   * @param snr The SNR of the received message
   * @param frequency The frequency of the received message
  */
  bool receiveRadioTwo(byte *&msg, uint16_t &msg_length, float &rssi, float &snr, double &frequency);
};
