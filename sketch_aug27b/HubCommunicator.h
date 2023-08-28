#pragma once

#include <ArduinoJson.h>
#include <ArduinoHttpClient.h>
#include <SPI.h>
#include <Ethernet.h>



class HubCommunicator{
  static constexpr size_t maxHandlers = 100; // Max count of massage types
  struct MessageHandler {
        String messageType_;
        void (*handler_)(DynamicJsonDocument);
    };
    MessageHandler handlers_[HubCommunicator::maxHandlers];

public:
  HubCommunicator(char hubIpAddress[], char controllerPublicKey[], char controllerPrivateKey[]);

  void initialize();
  uint16_t sendMessage(DynamicJsonDocument data);

  void registerMessageHandler(String messageType, void (*func)(DynamicJsonDocument));
private:
  const char* hubIpAddress_;
  const char* controllerPublicKey_;
  const char* controllerPrivateKey_;
  HttpClient client_;
  EthernetClient ethernetClient_;
  bool initialized_ = false;
  const int16_t initDelay_ = 10*1000;

  uint16_t sendPost(const DynamicJsonDocument& data);



};