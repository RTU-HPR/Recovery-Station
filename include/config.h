#pragma once

// Main libraries
#include <Arduino.h>
#include <SPI.h>

// Our libraries
#include <RadioLib_wrapper.h>
#include <ranging_wrapper.h>
#include <Ccsds_packets.h>

// Used radio module
#define radio_module SX1276
#define radio_module_family Sx127x
#define radio_module_rf_switching Disabled

class Config
{
public:
  // CONFIG STRUCT DECLARATIONS
  // CONFIG DEFINITIONS
  // RADIOS
  RadioLib_Wrapper<radio_module>::Radio_Config radio_config_one{
      .frequency = 434, // Frequency
      .cs = 13,         // Chip select
      .dio0 = 8,        // Busy
      .dio1 = 9,        // Interrupt action
      .family = RadioLib_Wrapper<radio_module>::Radio_Config::Chip_Family::radio_module_family,
      .rf_switching = RadioLib_Wrapper<radio_module>::Radio_Config::Rf_Switching::radio_module_rf_switching,
      // If using GPIO pins for RX/TX switching, define pins used for RX and TX control
      .rx_enable = -1,
      .tx_enable = -1,
      .reset = 14,
      .sync_word = 0xF4,
      .tx_power = 17,
      .spreading = 11,
      .coding_rate = 8,
      .signal_bw = 62.5,
      .frequency_correction = false,
      .spi_bus = &SPI1 // SPI bus used by radio
  };

  RadioLib_Wrapper<radio_module>::Radio_Config radio_config_two{
      .frequency = 434.5, // Frequency
      .cs = 13,           // Chip select
      .dio0 = 8,          // Busy
      .dio1 = 9,          // Interrupt action
      .family = RadioLib_Wrapper<radio_module>::Radio_Config::Chip_Family::radio_module_family,
      .rf_switching = RadioLib_Wrapper<radio_module>::Radio_Config::Rf_Switching::radio_module_rf_switching,
      // If using GPIO pins for RX/TX switching, define pins used for RX and TX control
      .rx_enable = -1,
      .tx_enable = -1,
      .reset = 14,
      .sync_word = 0xF4,
      .tx_power = 17,
      .spreading = 11,
      .coding_rate = 8,
      .signal_bw = 62.5,
      .frequency_correction = false,
      .spi_bus = &SPI1 // SPI bus used by radio
  };

  // Create radio objects and pass error function if not passed will use serial print
  RadioLib_Wrapper<radio_module> radio_one = RadioLib_Wrapper<radio_module>(nullptr, 5);
  RadioLib_Wrapper<radio_module> radio_two = RadioLib_Wrapper<radio_module>(nullptr, 5);

  // RANGING
  Ranging_Wrapper::Mode ranging_mode = Ranging_Wrapper::Mode::SLAVE;
  const int RANGING_LORA_TIMEOUT = 2000;
  Ranging_Wrapper::Lora_Device ranging_device =
      {
          .FREQUENCY = 2405.6,
          .CS = 17,
          .DIO0 = 22, // busy
          .DIO1 = 21,
          .RESET = 20,
          .SYNC_WORD = 0xF5,
          .TXPOWER = 14,
          .SPREADING = 10,
          .CODING_RATE = 7,
          .SIGNAL_BW = 406.25,
          .SPI = &SPI,
  };

  // SPI
  const int SPI0_RX = 16;
  const int SPI0_TX = 19;
  const int SPI0_SCK = 18;

  const int SPI1_RX = 12;
  const int SPI1_TX = 11;
  const int SPI1_SCK = 10;

  // PC Serial
  const int PC_BAUDRATE = 115200;

  // Buzzer
  const int BUZZER_PIN = 3;
};