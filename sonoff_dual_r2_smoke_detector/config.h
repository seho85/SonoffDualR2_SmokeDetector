/*
 * config.h
 *
 *  Created on: 17.04.2022
 *      Author: sebastian
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#define WIFI_SSID "Your WiFi SSID"
#define WIFI_PASSWORD "YourHopefullySecurePassword"

#define MQTT_BROKER_IP "1.2.3.4"
#define MQTT_BROKER_PORT 1883
#define MQTT_BROKER_ID "SonoffR2Dual_3DPrinter"

//#define USE_MQTT_CREDENTIALS

#ifdef USE_MQTT_CREDENTIALS

#define MQTT_BROKER_USER "MyMqttBrokerUser"
#define MQTT_BROKER_PASSWORD "MyMqttUserPassword"

#endif

#define MQTT_BASE_TOPIC "/3DPrinter"
#define RELAY1_TOPIC "Relay1"
#define RELAY2_TOPIC "Relay2"
#define STATE_TOPIC "State"
#define CMD_TOPIC "Cmd"

#define RELAY1_POWER_ON_STATE 0
#define RELAY2_POWER_ON_STATE 0


#endif /* CONFIG_H_ */
