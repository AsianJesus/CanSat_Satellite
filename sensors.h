#ifndef __SENSORS__
  
  // Define pins
  //#define 
  void DefinePinmodes();
  void InitializeSensors();

  //The list is not complete
  bool GetTemperatureAndHumidity(float& temp, float& humidity);
  bool GetPressureAndHeight(const float& p0, float& pressure, float& height);
  
  
  void TurnServo(const float degree);
  void SetBuzzerState(const bool state);
    
#endif

