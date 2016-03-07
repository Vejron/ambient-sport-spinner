const uint32_t ESP_ID = ESP.getChipId();

WiFiClient espClient;
PubSubClient mqttClient(espClient);
Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, NEO_PIN, NEO_RGB + NEO_KHZ800);
AccelStepper stepper(AccelStepper::DRIVER, STEP_PIN, DIRECTION_PIN, 2, 2);

int reconnects = 0;

// Controller
bool g_gotoStart = false;
int  g_frames = 0;
int  g_delay = 500;

// Timers
long lastReconnectAttempt = 0;
long lastBeat = 0;



