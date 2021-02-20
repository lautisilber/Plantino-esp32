String processor_info(const String& var)
{
  if (var == "SD_STATUS")
    return (sdStatus ? "g" : "b");
  else if (var == "SPIFFS_STATUS")
    return (spiffsStatus ? "g" : "b");
  return String();
}

void OnWsEvent(AsyncWebSocket * server, AsyncWebSocketClient * client, AwsEventType type, void * arg, uint8_t *data, size_t len)
{
  // from: https://github.com/luisllamasbinaburo/ESP32-Examples/blob/main/25_Json_AsyncWebsocket/ESP32_Utils_AWS.hpp
  if(type == WS_EVT_CONNECT){
    Serial.printf("ws[%s][%u] connect\n", server->url(), client->id());
    //client->ping();
  } else if(type == WS_EVT_DISCONNECT){
    Serial.printf("ws[%s][%u] disconnect: %u\n", server->url(), client->id());
  } else if(type == WS_EVT_ERROR){
    //Serial.printf("ws[%s][%u] error(%u): %s\n", server->url(), client->id(), *((uint16_t*)arg), (char*)data);
  } else if(type == WS_EVT_PONG){
    //Serial.printf("ws[%s][%u] pong[%u]: %s\n", server->url(), client->id(), len, (len)?(char*)data:"");
  } else if(type == WS_EVT_DATA){
    AwsFrameInfo * info = (AwsFrameInfo*)arg;
    String msg = "";
    if(info->final && info->index == 0 && info->len == len){
      if(info->opcode == WS_TEXT){
        for(size_t i=0; i < info->len; i++) {
          msg += (char) data[i];
        }
      } else {
        char buff[3];
        for(size_t i=0; i < info->len; i++) {
          sprintf(buff, "%02x ", (uint8_t) data[i]);
          msg += buff ;
        }
      }

      if(info->opcode == WS_TEXT)
      ProcessRequest(client, msg);
    
    } else {
      //message is comprised of multiple frames or the frame is split into multiple packets
      if(info->opcode == WS_TEXT){
        for(size_t i=0; i < len; i++) {
          msg += (char) data[i];
        }
      } else {
        char buff[3];
        for(size_t i=0; i < len; i++) {
          sprintf(buff, "%02x ", (uint8_t) data[i]);
          msg += buff ;
        }
      }
      //Serial.printf("%s\n",msg.c_str());

      if((info->index + len) == info->len){
        if(info->final){
          if(info->message_opcode == WS_TEXT)
            ProcessRequest(client, msg);
        }
      }
    }
  }
}

void ProcessRequest(AsyncWebSocketClient* client, String request)
{
  StaticJsonDocument<200> doc;
  DeserializationError error = deserializeJson(doc, request);
  if (error)
    return;
  const char* com = doc["com"];
  if (strcmp(com, "setparams") == 0)
  {
    if (doc.containsKey("lt"))
      logTime = doc["lt"];
    else if (doc.containsKey("Mh"))
      maxHumidity = doc["Mh"];
    else if (doc.containsKey("mh"))
      minHumidity = doc["mh"];
    else if (doc.containsKey("Mw"))
      maxWaterValue = doc["Mw"];
    else if (doc.containsKey("mw"))
      minWaterValue = doc["mw"];
    else if (doc.containsKey("pt"))
      pumpCheckTime = doc["pt"];
      
    StaticJsonDocument<200> msg;
    msg["lt"] = logTime;
    msg["Mh"] = maxHumidity;
    msg["mh"] = minHumidity;
    msg["Mw"] = maxWaterValue;
    msg["mw"] = minWaterValue;
    msg["pt"] = pumpCheckTime;
    String response;
    serializeJson(msg, response);

    client->text(response.c_str());   
  }
  else if (strcmp(com, "getsensorvals") == 0)
  {
    UpdateSensors();
    UpdateJson();
    String response;
    serializeJson(jsonDoc, response);

    client->text(response.c_str());
    //ws.textAll(response);
  }
  else if (strcmp(com, "getsensorcount") == 0)
  {
    String response = "{\"sensorcount\": " + String(soilMoistureSensorCount) + "}";
    
    client->text(response.c_str());
    //ws.textAll(response.c_str());
  }
  else if (strcmp(com, "ok?") == 0)
  {
    Serial.printf("received: %s\n", com);
    //String json;
    //serializeJson(jsonDoc, json);
    client->text("ok!");
    //client->text(json.c_str());
  }
  else
  {
    Serial.print("Received unidentified com: ");
    Serial.println(com);
  }
}
