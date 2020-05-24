// 1-channel LoRa Gateway for ESP8266 and ESP32
// Copyright (c) 2016-2020 Maarten Westenberg version for ESP8266 and ESP32
//
// based on work done by Thomas Telkamp for Raspberry PI 1ch gateway
// and many other contributors.
//
// All rights reserved. This program and the accompanying materials
// are made available under the terms of the MIT License
// which accompanies this distribution, and is available at
// https://opensource.org/licenses/mit-license.php
//
// NO WARRANTY OF ANY KIND IS PROVIDED
//
// Author: Maarten Westenberg (mw12554@hotmail.com)
//
// This file contains a number of compile-time settings and declarations that are'
// specific to the LoRa rfm95, sx1276, sx1272 radio of the gateway.
//
//
// ------------------------------------------------------------------------------------

// --- BEGIN USER SETTINGS ---
// RADIO
#define _LFREQ 915
#define _SPREADING SF9
// Comment out _SPREADING and uncomment _CAD to listen on all SF's at cost of sensitivity
// #define _CAD 1

// HARDWARE
#define _DUSB 1
#define _CLASS "A"
#define _PIN_OUT 6
#define OLED 0
#define CFG_sx1276_radio
#define SPIFF_FORMAT 0

// TTN SERVER
#define A_SERVER 1
#define A_REFRESH 1
#define A_SERVERPORT 80
#define A_MAXBUFSIZE 192

#define _TTNSERVER "router.eu.thethings.network"
#define _TTNPORT 1700

// NAMETAG
#define _DESCRIPTION "Project DEIMOS - ESP Gateway"
#define _EMAIL "jssia@uwaterloo.ca"
#define _PLATFORM "ESP8266"
#define _LAT 43.46779
#define _LON 80.56199
#define _ALT 350

// ACCESS POINT
#define _WIFIMANAGER 0
#define AP_NAME "DEIMOS-ESP8266"
#define AP_PASSWD "shawnthedawn"

// --- END USER SETTINGS ---

// It is possible to use the gateway as a normal sensor node also. In this case,
// substitute the node info below.
#if _GATEWAYNODE==1
	// Valid coding for internal sensors are LCODE and RAW.
	// Make sure to only select one.
#	define _LCODE 1
//#	define _RAW 1
#	define _CHECK_MIC 1
#	define _SENSOR_INTERVAL 300

	// Sensor and app address information
#define _DEVADDR { 0xAA, 0xAA, 0xAA, 0xAA }
#define _APPSKEY { 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB }
#define _NWKSKEY { 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC }

	// For ESP32 based T_BEAM/TTGO boards these two are normally included
	// If included make value 1, else if not, make them 0
#	define _GPS 1
#	define _BATTERY 1
#endif //_GATEWAYNODE


#if _TRUSTED_NODES >= 1
struct nodex {
	uint32_t id;				// This is the LoRa ID (coded in 4 bytes uint32_t
	char nm[32];				// Name of the node
};

// Add all your named and trusted nodes to this list
nodex nodes[] = {
	{ 0x260116BD , "lora-34 PIR node" },						// F=0
	{ 0x26011152 , "lora-35 temp+humi node" },					// F=0
	{ 0x2601148C , "lora-36 test node"  },						// F=0
};

#endif //_TRUSTED_NODES


// In some cases we like to decode the lora message at the single channel gateway.
// In thisase, we need the NkwSKey and the AppsSKey of the node so that we can decode
// its messages.
// Although this is probably overkill in normal gateway situations, it greatly helps
// in debugging the node messages before they reach the TTN severs.
//
#if _LOCALSERVER==1
struct codex  {
	uint32_t id;				// This is the device ID (coded in 4 bytes uint32_t
	unsigned char nm[32];		// A name string which is free to choose
	uint8_t nwkKey[16];			// The Network Session Key of 16 bytes
	uint8_t appKey[16];			// The Application Session Key of 16 bytes
};

// Sometimes we want to decode the sensor completely as we do in the TTN server
// This means that for all nodes we want to view the data of, we need to provide
// the AppsSKey and the NwkSKey

// Definition of all nodes that we want to decode locally on the gateway.
//
codex decodes[] = {
	{ 0xAAAAAAAA , "lora-EE", 	// F=0
		{ 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB },
		{ 0xDD, 0xDD, 0xDD, 0xDD, 0xDD, 0xDD, 0xDD, 0xDD, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC } 
	},
	{ 0xBBBBBBBB , "lora-FF", 	// F=0
		{ 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB, 0xBB },
		{ 0xDD, 0xDD, 0xDD, 0xDD, 0xDD, 0xDD, 0xDD, 0xDD, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC, 0xCC } 
	},
	{ 0x00000000 , "lora-00",	// F=0
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
		{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }
	}	
};
#endif //_LOCALSERVER


// Wifi definitions
// WPA is an array with SSID and password records. Set WPA size to number of entries in array
// When using the WiFiManager, we will overwrite the first entry with the 
// accesspoint we last connected to with WifiManager
// NOTE: Structure needs at least one (empty) entry.
//		So WPASIZE must be >= 1
struct wpas {
	char login[32];							// Maximum Buffer Size (and allocated memory)
	char passw[64];
};


// Please fill in at least ONE valid SSID and password from your own WiFI network
// below. This is needed to get the gateway working
//
wpas wpa[] = {
	{ "The Wifi Network", "S0meP@$$w0rd" },
	{ "The Wifi Network-5G", "S0meP@$$w0rd" }
};


// If you have a second back-end server defined such as Semtech or loriot.io
// your can define _THINGPORT and _THINGSERVER with your own value.
// If not, make sure that you do not define these, which will save CPU time
// Port is UDP port in this program
//
// Default for testing: Switched off
// #define _THINGSERVER "westenberg.org"		// Server URL of the LoRa-udp.js handler
// #define _THINGPORT 1700						// Port 1700 is old compatibility


// Define the name of the accesspoint if the gateway is in accesspoint mode (is
// getting WiFi SSID and password using WiFiManager). 
// If you do not need them, comment out.
//#define AP_NAME "Gway-Things4U"
//#define AP_PASSWD "ttnAutoPw"


// For asserting and testing the following defines are used.
//
#if !defined(CFG_noassert)
#define ASSERT(cond) if(!(cond)) gway_failed(__FILE__, __LINE__)
#else
#define ASSERT(cond) /**/
#endif
