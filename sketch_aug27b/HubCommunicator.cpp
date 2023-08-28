#include "HubCommunicator.h"
#include "string.h"



HubCommunicator::HubCommunicator(char hubIpAddress[], char controllerPublicKey[], char controllerPrivateKey[]):
  hubIpAddress_(hubIpAddress),
  controllerPublicKey_(controllerPublicKey),
  controllerPrivateKey_(controllerPrivateKey_),
  ethernetClient_(EthernetClient()),
  client_(HttpClient(ethernetClient_, hubIpAddress_)) // there is port by-default !
{
  byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };  
  while(!Ethernet.begin(mac)){
    Serial.println("ethernet client not started");
    delay(3000);

  }
}

void HubCommunicator::initialize(){
  
  DynamicJsonDocument doc(100);

  doc["messageType"] = "INITIALIZE";
  doc["publicKey"]   = controllerPublicKey_;
  

  int replyCode = sendPost(doc);
  
  

  int statusCode = client_.responseStatusCode();

  if (statusCode != 200){
    delay(initDelay_);
    statusCode = client_.responseStatusCode();
  }

  
}

uint16_t HubCommunicator::sendPost(const DynamicJsonDocument& data){
  DynamicJsonDocument doc(data);

  
  
  

  String jsonString;
  serializeJson(doc, jsonString);
  
  
  char charArray[jsonString.length() + 1];
  jsonString.toCharArray(charArray, jsonString.length());

  client_.beginRequest();
  client_.post("/sendMessage", "application/json", charArray );
  char str[] = "PrivateKey: ";

  client_.sendHeader(strcat(str, controllerPrivateKey_));
  client_.endRequest();
  

  return client_.responseStatusCode();
}

uint16_t HubCommunicator::sendMessage(DynamicJsonDocument data){
  
  return sendPost(data);
}



void HubCommunicator::registerMessageHandler(String messageType, void (*func)(DynamicJsonDocument)){
  // Very bad ... But its is ok for small amount of messageType (alternative std::map)
   for (size_t i = 0; i < maxHandlers; ++i) {
            if (handlers_[i].messageType_ == messageType) {
                handlers_[i].handler_ = func;
                break;
            }
        }
}

