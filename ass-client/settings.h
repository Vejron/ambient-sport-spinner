// base topic for pub/sub
#define BASE_PATH     "fredrik40/"
// settings - incomming
#define TOPIC_RUN     BASE_PATH "run/"
// status - outgoing
#define TOPIC_STATUS  BASE_PATH "status/"
// heart beat
#define TOPIC_HEARTBEAT BASE_PATH "beat/"

// Topic structure              |    Payload (JSON)
// <BASE_PATH>/<TOPIC_RUN>/     |    {"frames":<Number>,"delay":<Number>}
// <BASE_PATH>/<TOPIC_STATUS>/  |    {"id":<Number>,"reconnects":<Number>,"time":<Number>}
// <BASE_PATH>/<TOPIC_BEAT>/    |    {"id":<Number>,"beat":<Number>}

// Redefine packet length to allow for longer topics
#define MQTT_MAX_PACKET_SIZE 256

// Input for opto switch
#define OPTO_PIN 12
// Driver pin for neopixels
#define NEO_PIN 13
// Nbr of pixels (ie 12 for ring and 1 for single dot)
#define PIXEL_COUNT 3

// Driver pins
#define DIRECTION_PIN 5
#define STEP_PIN      0
#define ENABLE_PIN    4

// Estetic settings
#define RECONNECT_DELAY 5000
#define TOUCH_RETRANSMIT_RATE 1000
#define HEART_BEAT_RATE 10000

// Update these with values suitable for your network.
const char* ssid = "ssid";
const char* password = "password";
const char* mqtt_broker = "test.mosquitto.org";

