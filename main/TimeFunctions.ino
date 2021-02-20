void UpdateTime()
{
  if (!timeClient.update())
  {
    for (byte i = 0; i < 10; i++)
    {
      timeClient.forceUpdate();
      if (timeClient.update())
        break;
    }
  }    
  String formattedDate = timeClient.getFormattedDate();
  int splitT = formattedDate.indexOf("T");
  prettyDate = formattedDate.substring(0, splitT);
  prettyDate += " ";
  prettyDate += formattedDate.substring(splitT+1, formattedDate.length()-1);
}
