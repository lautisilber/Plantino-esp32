// SD functions
void CreateFile(fs::FS &fs, const char* path)
{
  if (!fs.exists(path))
  {
    Serial.printf("Creating file: %s... ", path);
    File file = fs.open(path, FILE_WRITE);
    if (!file)
    {
      Serial.println("failed");
      sdStatus = false;
      return;
    }
    file.close();
    Serial.println("done");
    sdStatus = true;
  }
  else
  Serial.printf("File %s already exists\n", path);
}

void LogJson(fs::FS &fs, const char* path, JsonDocument &doc)
{
  Serial.printf("Logging to file %s... ", path);
  File file = fs.open(path, FILE_APPEND);
  if(!file)
  {
    Serial.println("Failed to open file for appending");
    sdStatus = false;
    return;
  }
  String message = "";
  serializeJson(doc, message);
  message += ",\n";
  if (file.print(message))
  {
    Serial.println("done");
    sdStatus = true;
  }
  else
  {
    Serial.println("Append failed");
    sdStatus = false;
  }
  file.close();
}

void CreateBackup(fs::FS &fs, const char* path)
{
  Serial.printf("Creating backup nr. %i of file %s... " , currBackup, path);
  String pathStr = String(path);
  int index = pathStr.indexOf('.');

  File oFile = SD.open(path, FILE_READ);
  String nFileName = pathStr.substring(0, index) + "_" + String(currBackup) + pathStr.substring(index);
  if (SD.exists(nFileName))
    SD.remove(nFileName);
  CreateFile(SD, nFileName.c_str());
  File nFile = SD.open(nFileName, FILE_WRITE);
  if (!oFile || !nFile)
  {
    Serial.println("Failed to open files for backup!");
    return;
  }
  
  size_t n; 
  uint8_t buf[64];
  while ((n = oFile.read(buf, sizeof(buf))) > 0)
  {
    nFile.write(buf, n);
  }
  currBackup++;
  if (currBackup >= maxBackups)
    currBackup = 0;
  Serial.println("done");
}

void DeleteFile(fs::FS &fs, const char * path){
    Serial.printf("Deleting file: %s\n", path);
    if(fs.remove(path)){
        Serial.println("File deleted");
    } else {
        Serial.println("Delete failed");
    }
}
