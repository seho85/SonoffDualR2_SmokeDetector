#include "sonoff_dual_r2_smoke_detector.h"

#include <ESP8266WiFi.h>
#include "EspMQTTClient.h"
#include "config.h"

#define PIN_LED_BLUE 13
#define PIN_BUTTON0 0
#define PIN_BUTTON1 9
#define PIN_BUTTON_CASE 10
#define PIN_RELAY1 12
#define PIN_RELAY2 5


enum class OperationState
{
	Normal, Alarm, WaitingForConfirmation
};

#ifdef USE_MQTT_CREDENTIALS
EspMQTTClient mqttClient(WIFI_SSID, WIFI_PASSWORD, MQTT_BROKER_IP, MQTT_BROKER_USER, MQTT_BROKER_PASSWORD, MQTT_BROKER_ID, MQTT_BROKER_PORT);
#else
EspMQTTClient mqttClient(WIFI_SSID, WIFI_PASSWORD, MQTT_BROKER_IP, MQTT_BROKER_ID, MQTT_BROKER_PORT);
#endif


OperationState eState = OperationState::Normal;

ICACHE_RAM_ATTR void smokeAlarmRaisingInterruptHandler()
{
	digitalWrite(PIN_RELAY1, 0);
	digitalWrite(PIN_RELAY2, 0);

	eState = OperationState::Alarm;

	Serial.println("Smoke alarm Interrupt!");
}

void setup()
{
	pinMode(PIN_LED_BLUE, OUTPUT);
	digitalWrite(PIN_LED_BLUE, 1);

	pinMode(PIN_RELAY1, OUTPUT);
	digitalWrite(PIN_RELAY1, RELAY1_POWER_ON_STATE);

	pinMode(PIN_RELAY2, OUTPUT);
	digitalWrite(PIN_RELAY2, RELAY2_POWER_ON_STATE);

	pinMode(PIN_BUTTON1, INPUT);
	attachInterrupt(digitalPinToInterrupt(PIN_BUTTON1), smokeAlarmRaisingInterruptHandler, RISING);

	pinMode(PIN_BUTTON_CASE, INPUT);

	Serial.begin(115200);
}

bool payloadStringToBool(const String &payload)
{
	bool bState = (payload == "ON") || (payload == "on") || payload == "1";

	return bState;
}

void publishRelayStates()
{
	bool bRelay1State = digitalRead(PIN_RELAY1);
	bool bRelay2State = digitalRead(PIN_RELAY2);

	Serial.print("publishRelayState Relay1:");
	Serial.print(String(bRelay1State));
	Serial.println();

	Serial.print("publishRelayState Relay2:");
	Serial.print(String(bRelay2State));
	Serial.println();

	mqttClient.publish(MQTT_BASE_TOPIC"/"RELAY1_TOPIC"/"STATE_TOPIC, bRelay1State ? "ON" : "OFF", true);
	mqttClient.publish(MQTT_BASE_TOPIC"/"RELAY2_TOPIC"/"STATE_TOPIC, bRelay2State ? "ON" : "OFF", true);
}


void onConnectionEstablished()
{
	publishRelayStates();

	mqttClient.subscribe(MQTT_BASE_TOPIC"/"RELAY1_TOPIC"/"CMD_TOPIC, [](const String &payload)
	{
		Serial.println("Relay1: " + payload);
		if(eState == OperationState::Normal)
			digitalWrite(PIN_RELAY1, payloadStringToBool(payload));
		publishRelayStates();
	});

	mqttClient.subscribe(MQTT_BASE_TOPIC"/"RELAY2_TOPIC"/"CMD_TOPIC, [](const String &payload)
	{
		Serial.println("Relay2: " + payload);
		if(eState == OperationState::Normal)
			digitalWrite(PIN_RELAY2, payloadStringToBool(payload));
		publishRelayStates();
	});

}

void loop()
{
	switch (eState)
	{
		case OperationState::Normal:
		{
//			bool bAlarm = digitalRead(PIN_BUTTON1) == HIGH;
//			if (bAlarm)
//			{
//				eState = OperationState::Alarm;
//				break;
//			}
			mqttClient.loop();

			if (mqttClient.isConnected())
				digitalWrite(PIN_LED_BLUE, 0);
			else
				digitalWrite(PIN_LED_BLUE, 1);

			break;
		}

		case OperationState::Alarm:
		{
			bool bAlarmGone = digitalRead(PIN_BUTTON1) == LOW;

			digitalWrite(PIN_RELAY1, 0);
			digitalWrite(PIN_RELAY2, 0);

			if (bAlarmGone)
				eState = OperationState::WaitingForConfirmation;
			else
			{
				bool bLEDState = digitalRead(PIN_LED_BLUE) == HIGH;

				digitalWrite(PIN_LED_BLUE, !bLEDState);
				delay(100);
			}

			break;
		}

		case OperationState::WaitingForConfirmation:
		{
			bool bConfirmed = digitalRead(PIN_BUTTON_CASE) == LOW;

			if (bConfirmed)
			{
				eState = OperationState::Normal;
				Serial.println("Alarm confirmed - switching back to normal operation...");
			}
			else
			{
				bool bLEDState = digitalRead(PIN_LED_BLUE) == HIGH;

				digitalWrite(PIN_LED_BLUE, !bLEDState);
				delay(250);
			}
			break;
		}
	}
}
