#include <WString.h>
#include <SoftwareSerial.h>
#ifndef __SENSORS__

#define __SENSORS__
#define PIN_SERVO_CORRECT 7
#define PIN_SERVO_REVERSED 6
#define PIN_DHT 11
#define PIN_BUZZER 12
#define PIN_XBEE_DOUT 4
#define PIN_XBEE_DIN 3
#define PIN_EMPTY 1
#define PIN_GPS_OUT 10
#define PIN_PITOTTUBE A3
#define PIN_CAMERA 13
#define PIN_VOLTAGE_DIVIDER A1
#define PIN_MICS_PREHEAT 9
#define PIN_MICS_VNOX A2
#define PIN_MICS_VRED A0
#define DHT_TYPE 11
#define BUZZER_COOLDOWN 60000
#define MICS_PREHEAT_SECONDS 5

enum MSG_TYPES {
	TELEMETRY = 0xFF,
	COMMAND_RESPONSE = 0xEF,
	COMMAND_REQUEST	= 0xDF
};
	// Define pins

	void InitializeSensors(SoftwareSerial&,SoftwareSerial&);

  //The list is not complete
	bool GetTemperatureAndHumidity(double& temp, double& humidity);
	bool GetPressureAndHeight(const double& p0, double& temp, double& pressure, double& height,const bool findTemp = false);
  bool GetFlightTime(long& time);
  bool GetVoltage(float&);
  bool GetGPSCoordinates(String& gpsData);	
  bool GetSpeed(float& sp);
  bool GetMISCData(String& miscData);

  void GiveSoundCommand(const unsigned int t,const unsigned int iter = 3); 

  void SavePhoto();
	void TurnServo(const int servoPin,const float degree,const bool doDetach = true, const int timeout = 250);
	void SetBuzzerState(const bool state, long cooldown = 0,bool force = false);
	// MSG_TYPE/body
	void XBeeSend(MSG_TYPES type,const String& msg);
	void XBeeSend(MSG_TYPES type);

	String XBeeRead();
#endif

