#include <WString.h>
#include <SoftwareSerial.h>
#ifndef __SENSORS__

#define __SENSORS__
#define PIN_SERVO 2
#define PIN_DHT 6
#define PIN_BUZZER 10
#define PIN_XBEE_DOUT 5
#define PIN_XBEE_DIN 4
#define PIN_GPS_IN 8
#define PIN_GPS_OUT 7
#define PIN_PITOTTUBE A0
#define PIN_CAMERA 3
#define PIN_VOLTAGE_DIVIDER A1
#define DHT_TYPE 11

enum MSG_TYPES {
	TELEMETRY = 0xFF,
	COMMAND_RESPONSE = 0xEF,
	COMMAND_REQUEST	= 0xDF
};
	// Define pins

	void InitializeSensors(SoftwareSerial&,SoftwareSerial&);

  //The list is not complete
	bool GetTemperatureAndHumidity(double& temp, double& humidity);
	bool GetPressureAndHeight(const double& p0, double& temp, double& pressure, double& height);
  bool GetFlightTime(float& time);
  bool GetVoltage(float&);
  bool GetGPSCoordinates(String& gpsData);	
  bool GetSpeed(float& sp);

  void SavePhoto();
	void TurnServo(const float degree);
	void SetBuzzerState(const bool state);
	// MSG_TYPE/body
	void XBeeSend(MSG_TYPES type,const String& msg);
	void XBeeSend(MSG_TYPES type);

	String XBeeRead();
#endif

