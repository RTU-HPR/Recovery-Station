#include <Arduino.h>

#include <config.h>
#include <communication.h>

Config config;
Communication communication;

void setup()
{
  // Set the pin modes
  pinMode(config.BUZZER_PIN, OUTPUT_12MA);
  tone(config.BUZZER_PIN, 1000, 250);
  delay(2000);

  // Start the pc Serial
  Serial.begin(config.PC_BAUDRATE);

  // SPI0
  if (SPI.setRX(config.SPI0_RX) && SPI.setTX(config.SPI0_TX) && SPI.setSCK(config.SPI0_SCK))
  {
    SPI.begin();
    // Serial.println("SPI0 communication bus initialized");
  }
  else
  {
    digitalWrite(config.BUZZER_PIN, HIGH);
  }

  // SPI1
  if (SPI1.setRX(config.SPI1_RX) && SPI1.setTX(config.SPI1_TX) && SPI1.setSCK(config.SPI1_SCK))
  {
    SPI1.begin();
    // Serial.println("SPI1 communication bus initialized");
  }
  else
  {
    digitalWrite(config.BUZZER_PIN, HIGH);
  }

  // Start the first radio
  if (!communication.beginRadioOne(config.radio_config_one))
  {
    digitalWrite(config.BUZZER_PIN, HIGH);
  }

  // Start the second radio
  if (!communication.beginRadioTwo(config.radio_config_two))
  {
    digitalWrite(config.BUZZER_PIN, HIGH);
  }

  // Start the ranging
  if (!communication.beginRangingLora(config.ranging_mode, config.ranging_device))
  {
    digitalWrite(config.BUZZER_PIN, HIGH);
  }
}

void loop()
{
  // Run the ranging
  communication.runRanging(config);

  // RADIO ONE
  // Variables for the message
  byte *msg = new byte[256];
  uint16_t msg_length = 0;
  float rssi = 0;
  float snr = 0;
  double frequency = 0;
  bool checksum_good = false;

  if (communication.receiveRadioOne(msg, msg_length, rssi, snr, frequency))
  {
    // Check if checksum matches
    if (check_crc_16_cciit_of_ccsds_packet(msg, msg_length))
    {
      checksum_good = true;
    }

    if (checksum_good)
    {
      // Append RSSI and SNR to message
      Converter rssi_converter;
      rssi_converter.f = rssi;
      msg[msg_length - 2] = rssi_converter.b[3];
      msg[msg_length - 1] = rssi_converter.b[2];
      msg[msg_length + 0] = rssi_converter.b[1];
      msg[msg_length + 1] = rssi_converter.b[0];

      Converter snr_converter;
      snr_converter.f = snr;
      msg[msg_length + 2] = snr_converter.b[3];
      msg[msg_length + 3] = snr_converter.b[2];
      msg[msg_length + 4] = snr_converter.b[1];
      msg[msg_length + 5] = snr_converter.b[0];

      msg_length += 6 + 2; // Added 6 bytes for RSSI and SNR, 2 bytes for new checksum

      // Add the new checksum to the message
      add_crc_16_cciit_to_ccsds_packet(msg, msg_length);

      Serial.write(msg, msg_length);

      // Parse the message
      // uint16_t apid = 0;
      // uint16_t sequence_count = 0;
      // uint32_t gps_epoch_time = 0;
      // uint16_t subseconds = 0;
      // byte *packet_data = new byte[msg_length];
      // uint16_t packet_data_length = 0;
      // parse_ccsds_telemetry(msg, apid, sequence_count, gps_epoch_time, subseconds, packet_data, packet_data_length);

      // // Print the message
      // if (apid == 100 || apid == 200)
      // {
      //   Converter data_values[6];
      //   extract_ccsds_data_values(packet_data, data_values, "float,float,float,float,uint32,uint32");

      //   Serial.println("Apid: " + String(apid) + " | Sequence Count: " + String(sequence_count) + " | GPS Epoch: " + String(gps_epoch_time) + "." + String(subseconds) + " | RSSI: " + String(rssi) + " | SNR: " + String(snr) + " | Frequency: " + String(frequency, 8));
      //   Serial.println("Latitude: " + String(data_values[0].f, 6) + " | Longitude: " + String(data_values[1].f, 6) + " | Altitude: " + String(data_values[2].f));
      //   Serial.println("Baro Altitude: " + String(data_values[3].f) + " | Satellites: " + String(data_values[4].i32) + " | Info/Error in Queue: " + (data_values[5].i32 == 0 ? "False" : "True"));
      // }
      // else
      // {
      //   Serial.println("Received telemetry with APID: " + String(apid));
      // }
      // Free memory
      // delete[] packet_data;
    }
    else if (!checksum_good)
    {
      // Serial.println("Packet with invalid checksum received!");
    }
    // Serial.println();
  }

  delete[] msg;

  // RADIO TWO
  // Reset variables
  byte *msg_2 = new byte[256];
  msg_length = 0;
  rssi = 0;
  snr = 0;
  frequency = 0;
  checksum_good = false;

  if (communication.receiveRadioTwo(msg_2, msg_length, rssi, snr, frequency))
  {
    // Check if checksum matches
    if (check_crc_16_cciit_of_ccsds_packet(msg_2, msg_length))
    {
      checksum_good = true;
    }

    if (checksum_good)
    {
      // Append RSSI and SNR to message
      Converter rssi_converter;
      rssi_converter.f = rssi;
      msg_2[msg_length - 2] = rssi_converter.b[3];
      msg_2[msg_length - 1] = rssi_converter.b[2];
      msg_2[msg_length + 0] = rssi_converter.b[1];
      msg_2[msg_length + 1] = rssi_converter.b[0];

      Converter snr_converter;
      snr_converter.f = snr;
      msg_2[msg_length + 2] = snr_converter.b[3];
      msg_2[msg_length + 3] = snr_converter.b[2];
      msg_2[msg_length + 4] = snr_converter.b[1];
      msg_2[msg_length + 5] = snr_converter.b[0];

      msg_length += 6 + 2; // Added 6 bytes for RSSI and SNR, 2 bytes for new checksum

      // Add the new checksum to the message
      add_crc_16_cciit_to_ccsds_packet(msg_2, msg_length);
      Serial.write(msg_2, msg_length);
      // // Parse the message
      // uint16_t apid = 0;
      // uint16_t sequence_count = 0;
      // uint32_t gps_epoch_time = 0;
      // uint16_t subseconds = 0;
      // byte *packet_data = new byte[msg_length];
      // uint16_t packet_data_length = 0;
      // parse_ccsds_telemetry(msg_2, apid, sequence_count, gps_epoch_time, subseconds, packet_data, packet_data_length);

      // // Print the message
      // if (apid == 100 || apid == 200)
      // {
      //   Converter data_values[6];
      //   extract_ccsds_data_values(packet_data, data_values, "float,float,float,float,uint32,uint32");

      //   Serial.println("Apid: " + String(apid) + " | Sequence Count: " + String(sequence_count) + " | GPS Epoch: " + String(gps_epoch_time) + "." + String(subseconds) + " | RSSI: " + String(rssi) + " | SNR: " + String(snr) + " | Frequency: " + String(frequency, 8));
      //   Serial.println("Latitude: " + String(data_values[0].f, 6) + " | Longitude: " + String(data_values[1].f, 6) + " | Altitude: " + String(data_values[2].f));
      //   Serial.println("Baro Altitude: " + String(data_values[3].f) + " | Satellites: " + String(data_values[4].i32) + " | Info/Error in Queue: " + (data_values[5].i32 == 0 ? "False" : "True"));
      // }
      // else
      // {
      //   Serial.println("Received telemetry with APID: " + String(apid));
      // }
      // Free memory
      // delete[] packet_data;
    }
    else if (!checksum_good)
    {
      // Serial.println("Packet with invalid checksum received!");
    }
    // Serial.println();
  }

  // Free memory
  delete[] msg_2;

  // Check for any messages from Serial
  int packetSize = Serial.available();
  if (packetSize)
  {
    // Read the packet into packetBuffer
    byte packetBuffer[packetSize];
    Serial.readBytes(packetBuffer, packetSize);

    // Create a temporary array to store the packetBuffer without last byte
    // When sending a string over UDP, the last character always got corrupted
    // So a sacrifical char is added to the end of the string and then removed here
    // To stay consistent with the UDP implementation, the last byte is also removed if received over Serial
    byte tempArray[packetSize - 1];
    for (size_t i = 0; i < packetSize - 1; i++)
    {
      tempArray[i] = packetBuffer[i];
    }

    // Print the received message
    // Serial.print("TELECOMMAND PACKET | HEX: ");
    // for (size_t i = 0; i < packetSize; i++)
    // {
    //   Serial.print(packetBuffer[i], HEX);
    //   Serial.print(" ");
    // }
    // Serial.println();

    // Parse CCSDS telecommand
    packetSize += 2; // +2 for the checksum added later
    byte *packet = new byte[packetSize];
    memcpy(packet, packetBuffer, packetSize - 2);
    byte *packet_data = new byte[packetSize];
    uint16_t apid = 0;
    uint16_t sequence_count = 0;
    uint16_t packet_data_length = 0;
    uint16_t packet_id = 0;
    parse_ccsds_telecommand(packetBuffer, apid, sequence_count, packet_id, packet_data, packet_data_length);

    // YAMCS telecommands are generated without a checksum
    // So the checksum must be added here
    add_crc_16_cciit_to_ccsds_packet(packet, packetSize);

    // Serial.print("Adding cheksum! Packet with added checksum: ");
    // for (size_t i = 0; i < packetSize; i++)
    // {
    //   Serial.print(packet[i], HEX);
    //   Serial.print(" ");
    // }
    // Serial.println();

    // According to the apid do the appropriate action
    if (apid == 10)
    {
      // Serial.println("Payload command received. Sending to Payload flight computer");
      communication.sendRadioOne(packet, packetSize);
    }
    else if (apid == 20)
    {
      // Serial.println("Balloon command received. Sending to Balloon flight computer");
      communication.sendRadioTwo(packet, packetSize);
    }
    else
    {
      // Serial.println("Unknown telecommand! Telecommand discarded!");
    }

    // Serial.println();
    // Free memory
    delete[] packet;
    delete[] packet_data;
  }
}