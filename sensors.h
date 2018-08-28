#ifndef __SENSORS__

#define PIN_SERVO 2
#define PIN_DHT 1

#define DHT_TYPE 11

enum MSG_TYPES {
	TELEMETRY 0xFF,
	COMMAND_RESPONSE 0xEF,
	COMMAND_REQUEST	0xDF
};
	// Define pins
  //#define 
	void DefinePinmodes();
	void InitializeSensors();

  //The list is not complete
	bool GetTemperatureAndHumidity(float& temp, float& humidity);
	bool GetPressureAndHeight(const float& p0, const float& temp, float& pressure, float& height);
  	
  
	void TurnServo(const float degree);
	void SetBuzzerState(const bool state);
	// MSG_TYPE/body
	void XBeeSend(MSG_TYPES type,const String& msg);
	void XBeeSend(MSG_TYPES type);

	String* XBeeRead();
#endif

