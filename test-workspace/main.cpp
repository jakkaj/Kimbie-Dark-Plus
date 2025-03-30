#ifdef ARDUINO
#include <Arduino.h>
#include <ESP_IOExpander_Library.h>
#include <Wire.h>
#endif

#include "./filesystem/filesystem.h"
#ifdef WAVESHARE45_DEVICE
#include "./filesystem/waveshare45_filesystem.h"
#endif
#include "./location/locations.h"
#include "./location/locations_data_loader.h"
#include "./tools/conversions/UnitConverter.h"
#include "hal/display_hal.h"
#include "hal/gps_hal.h"
#include "location/gps_cache.h"
#ifdef ESP32
#include "tasks/gps_task.h" // GPS task for ESP32
#else
#include <chrono>   // For timing in simulator
#include <unistd.h> // For usleep in simulator
#endif
#include "logging.h"
#include "ui/ui.h"

// Forward declaration for native GPS polling
#ifndef ESP32
void poll_gps_and_update_cache();
#endif

// Extender Pin define
#define TP_RST 1
#define LCD_BL 2
#define LCD_RST 3
#define SD_CS 4
#define USB_SEL 5

// I2C Pin define
#define I2C_MASTER_NUM 0
#define I2C_MASTER_SDA_IO 8
#define I2C_MASTER_SCL_IO 9

#define ESP_IO_EXPANDER_I2C_CH422G_ADDRESS_000 (0x24)

// Function declarations
void demo_nearest_location();
void demo_location_bearing();
void demo_gps_data();

// ESP_IOExpander_CH422G *expander;

// void BLset(byte state) { expander->digitalWrite(LCD_BL, state); }

// void ExpanderInit() {
//   Wire.begin(I2C_MASTER_SDA_IO, I2C_MASTER_SCL_IO);
//   log_info("IO expander init...");
//   expander = new ESP_IOExpander_CH422G((i2c_port_t)I2C_MASTER_NUM,
//                                        ESP_IO_EXPANDER_I2C_CH422G_ADDRESS_000);
//   expander->init();
//   expander->begin();
//   Serial.println("Set the IO0-7 pin to output mode.");
//   expander->enableAllIO_Output();
//   expander->digitalWrite(TP_RST, HIGH);
//   expander->digitalWrite(LCD_RST, HIGH);
//   expander->digitalWrite(LCD_BL, HIGH);

//   // Use extended GPIO for SD card
//   expander->digitalWrite(SD_CS, LOW);

//   // Turn off backlight
//   // expander->digitalWrite(LCD_BL, HIGH);

//   expander->digitalWrite(USB_SEL, HIGH);

//   // When USB_SEL is HIGH, it enables FSUSB42UMX chip and gpio19, gpio20
//   wired
//   // CAN_TX CAN_RX, and then don't use USB Function
//   // expander->digitalWrite(USB_SEL, HIGH);
//   // BLset(LOW);
// }

void setup()
{
#ifdef ARDUINO                                   // Ensure Serial is available
    Serial.begin(115200);                        // Initialize Serial at the expected baud rate
    delay(1000);                                 // Wait a moment for serial connection to establish
    Serial.println("--- Basic Serial Test ---"); // Basic test print

    // Report which device flags are defined
    Serial.println("--- Device Flag Check ---");
#ifdef T5EPAPER_DEVICE
    Serial.println("T5EPAPER_DEVICE is defined");
#else
    Serial.println("T5EPAPER_DEVICE is NOT defined");
#endif

#ifdef WAVESHARE45_DEVICE
    Serial.println("WAVESHARE45_DEVICE is defined");
#else
    Serial.println("WAVESHARE45_DEVICE is NOT defined");
#endif

    Serial.println("--- End Flag Check ---");
#endif

    log_init(115200); // Initialize logging with a baud rate of 115200
    log_info("Application starting up");
    log_info("DEBUG-1: Starting setup");

    // Also log device flags with log_info
    log_info("--- Device Flag Check ---");
#ifdef T5EPAPER_DEVICE
    log_info("T5EPAPER_DEVICE is defined");
#else
    log_info("T5EPAPER_DEVICE is NOT defined");
#endif

#ifdef WAVESHARE45_DEVICE
    log_info("WAVESHARE45_DEVICE is defined");
#else
    log_info("WAVESHARE45_DEVICE is NOT defined");
#endif
    log_info("--- End Flag Check ---");

#ifdef T5EPAPER_DEVICE
    // Specific setup for T5 E-Paper
    log_info("T5 E-Paper Device Detected");
    // Add any other T5 specific setup here if needed in the future
#endif

    // Initialize GPS Cache (create mutex)
    log_info("DEBUG-2: Initializing GPS cache");
    gps_cache_init();
    log_info("DEBUG-3: GPS cache initialized");

#ifdef ARDUINO
    // vTaskDelay(pdMS_TO_TICKS(2000));
    // ExpanderInit();
    log_info("DEBUG-4: Setting up I2C");
    Wire.end();
    // try to read a i2c device at 0x5D
    Wire.begin(I2C_MASTER_SDA_IO, I2C_MASTER_SCL_IO);
    log_info("DEBUG-5: I2C initialized");
    // Wire.beginTransmission(93);
    // if (Wire.endTransmission() == 0) {
    //   log_info("I2C device found at 0x5D");
    // } else {
    //   log_error("I2C device not found at 0x5D");
    // }
#endif

    // Initialize GPS HAL
    log_info("DEBUG-6: Initializing GPS HAL");
    GPSHAL::getInstance().init();
    log_info("DEBUG-7: GPS HAL initialized");

#ifndef ESP32
    // Poll GPS immediately in simulator setup
    log_info("Performing initial GPS poll for simulator...");
    poll_gps_and_update_cache();
#endif // !ESP32

#ifdef ESP32
    // Start the GPS polling task on ESP32
    log_info("DEBUG-8: Starting GPS task");
    gps_task_start();
    log_info("DEBUG-9: GPS task started");
#endif

    // Example of loading version.txt file using the filesystem abstraction
    log_info("DEBUG-10: Loading version information");
    FileSystem &fs = FileSystem::getInstance();
    log_info("DEBUG-11: FileSystem instance obtained");

#ifdef WAVESHARE45_DEVICE
    vTaskDelay(pdMS_TO_TICKS(1000)); // 1 second delay
    log_info("Attempting to initialize WaveShare45 SD card");
    // Get the WaveShare45FileSystem instance directly instead of using
    // dynamic_cast
    WaveShare45FileSystem &ws45_fs = WaveShare45FileSystem::getInstance();
    log_info("Testing WaveShare45 SD card with diagnostic function");
    bool test_result = ws45_fs.testSDCard();
    if (test_result)
    {
        log_info("WaveShare45 SD card test passed successfully");
    }
    else
    {
        log_error("WaveShare45 SD card test failed");
    }
#endif

#ifdef ARDUINO
    FileResult versionInfo = fs.readFileAsString("/version.txt");
    if (versionInfo.success)
    {
        log_info("Version info:");
        log_info(versionInfo.content.c_str());
    }
    else
    {
        log_error("Failed to read version.txt");
    }
#else
    std::optional<std::string> versionInfo = fs.readFileAsString("version.txt");
    if (versionInfo)
    {
        log_info("Version info:");
        log_info(versionInfo->c_str());
    }
    else
    {
        log_error("Failed to read version.txt");
    }
#endif

    // Initialize Display HAL and UI for all relevant devices
    log_info("DEBUG-12: Starting display HAL initialization...");
#ifdef T5EPAPER_DEVICE
    log_info("DEBUG-13: T5EPAPER_DEVICE is defined - initializing LilyGo T5 ePaper HAL");
#endif
#ifdef WAVESHARE45_DEVICE
    log_info("DEBUG-14: WAVESHARE45_DEVICE is defined - initializing Waveshare 4.5\" HAL");
#endif

    if (!DisplayHAL::getInstance().init())
    {
        log_error("DEBUG-15: Failed to initialize display HAL");
// Decide how to handle HAL init failure - maybe loop forever?
#ifdef ARDUINO
        log_error("Display HAL init failed - halting");
        while (1)
        {
            delay(1000);
        }
#else
        log_error("Display HAL init failed - halting");
        while (1)
        { /* Halt */
        }
#endif
    }
    log_info("DEBUG-16: Display HAL initialized successfully");

    log_info("DEBUG-17: Starting UI initialization...");
    ui_init();
    log_info("DEBUG-18: UI initialized successfully");
    // End of removed conditional block

    log_info("DEBUG-19: Setup complete");

    auto response = UnitConverter::parseAndConvert("5km to mi");
    if (response.isValid)
    {
        char message[100];
        log_info("Conversion successful");
        sprintf(message, "Input value: %.2f", response.results[0].inputValue);
        log_info(message);
        sprintf(message, "Output value: %.2f", response.results[0].outputValue);
        log_info(message);
    }
    else
    {
        log_error("Conversion failed");
    }

    std::string result = UnitConverter::parseAndConvertToString("5km to ");
    log_info("Conversion as string:");
    log_info(result.c_str());

    // Example of finding a location by name using the LocationsManager
    log_info("Searching for location: jump");

    // Initialize locations from CSV file
    log_info("Initializing locations from CSV file");
    initLocationsData();

    // Get the LocationsManager instance
    LocationsManager &locManager = LocationsManager::GetInstance();

    // Test searches for "J", "JP", and "JPP"
    log_info("------------- TESTING SEARCH PATTERNS -------------");
    log_info("Searching for 'J'");
    std::vector<Location *> locations_j = locManager.FindByName("J");
    if (!locations_j.empty())
    {
        log_info("Found locations starting with 'J':");
        char debug_msg[100];
        sprintf(debug_msg, "Found %zu locations", locations_j.size());
        log_info(debug_msg);

        // Display first few matches
        for (size_t i = 0; i < std::min(size_t(3), locations_j.size()); ++i)
        {
            sprintf(debug_msg, "%zu. %s (%s)", i + 1, locations_j[i]->name, locations_j[i]->code);
            log_info(debug_msg);
        }
    }
    else
    {
        log_error("No locations found starting with 'J'");
    }

    log_info("Searching for 'JP'");
    std::vector<Location *> locations_jp = locManager.FindByNameOrCode("JP");
    if (!locations_jp.empty())
    {
        log_info("Found locations starting with 'JP':");
        char debug_msg[100];
        sprintf(debug_msg, "Found %zu locations", locations_jp.size());
        log_info(debug_msg);

        // Display first few matches
        for (size_t i = 0; i < std::min(size_t(3), locations_jp.size()); ++i)
        {
            sprintf(debug_msg, "%zu. %s (%s)", i + 1, locations_jp[i]->name, locations_jp[i]->code);
            log_info(debug_msg);
        }
    }
    else
    {
        log_error("No locations found starting with 'JP'");
    }

    log_info("Searching for 'JPP'");
    std::vector<Location *> locations_jpp = locManager.FindByNameOrCode("JPP");
    if (!locations_jpp.empty())
    {
        log_info("Found locations starting with 'JPP':");
        char debug_msg[100];
        sprintf(debug_msg, "Found %zu locations", locations_jpp.size());
        log_info(debug_msg);

        // Display all matches (likely only a few for a specific code like JPP)
        for (size_t i = 0; i < locations_jpp.size(); ++i)
        {
            sprintf(debug_msg, "%zu. %s (%s)", i + 1, locations_jpp[i]->name, locations_jpp[i]->code);
            log_info(debug_msg);
        }
    }
    else
    {
        log_error("No locations found starting with 'JPP'");
    }

    // Print first 20 locations to see what we have
    log_info("------------- FIRST 20 LOCATIONS -------------");
    for (size_t i = 0; i < std::min(size_t(20), locManager.GetLocationCount());
         ++i)
    {
        if (locations_vfr[i].name != nullptr)
        {
            char debug_msg[100];
            sprintf(debug_msg, "Location %zu: %s (Code: %s)", i,
                    locations_vfr[i].name, locations_vfr[i].code);
            log_info(debug_msg);
        }
    }

    // Original search
    log_info("------------- ORIGINAL SEARCH -------------");
    std::vector<Location *> locations = locManager.FindByNameOrCode("jpp");
    if (!locations.empty())
    {
        log_info("Locations found:");
        char message[100];
        sprintf(message, "Found %zu locations matching 'jpp'", locations.size());
        log_info(message);

        // Display details for each location found
        for (size_t i = 0; i < locations.size(); ++i)
        {
            Location *location = locations[i];
            log_info("---------------------------------");
            sprintf(message, "Location %zu:", i + 1);
            log_info(message);
            sprintf(message, "Name: %s", location->name);
            log_info(message);
            sprintf(message, "State: %s", location->state ? location->state : "N/A");
            log_info(message);
            sprintf(message, "Code: %s", location->code);
            log_info(message);

            // Use the new FormatCoordinates method
            std::string coordStr = locManager.FormatCoordinates(location);
            sprintf(message, "Coordinates: %s", coordStr.c_str());
            log_info(message);

            // Also show DMS format
            std::string dmsCoords =
                locManager.FormatCoordinates(location, CoordinateFormat::DMS);
            sprintf(message, "Coordinates (DMS): %s", dmsCoords.c_str());
            log_info(message);
        }
    }
    else
    {
        log_error("No locations found matching 'jpp'");
    }

    // Test FindNearest function with DMS coordinates
    log_info("Testing FindNearest with coordinates: 364500S,1441600E");

    // Convert DMS coordinates to decimal
    double lat = locManager.DMSToDecimal("364500S");
    double lon = locManager.DMSToDecimal("1441600E");

    char debug_msg[100];
    log_debug("Converting DMS: 274400S");
    sprintf(debug_msg, "Decimal latitude: %.6f", lat);
    log_debug(debug_msg);
    log_debug("Converting DMS: 1532700E");
    sprintf(debug_msg, "Decimal longitude: %.6f", lon);
    log_debug(debug_msg);

    // Find the nearest location to these coordinates
    Location *nearest = locManager.FindNearest(lat, lon);

    if (nearest)
    {
        char message[100];
        log_info("Nearest location found:");
        sprintf(message, "Name: %s", nearest->name);
        log_info(message);
        sprintf(message, "State: %s", nearest->state);
        log_info(message);
        sprintf(message, "Code: %s", nearest->code);
        log_info(message);

        // Show the coordinates
        std::string coordStr = locManager.FormatCoordinates(nearest);
        sprintf(message, "Coordinates: %s", coordStr.c_str());
        log_info(message);

        // Calculate and display the distance
        double distance = locManager.CalculateDistance(lat, lon, nearest->latitude,
                                                       nearest->longitude);
        sprintf(message, "Distance: %.2f km", distance);
        log_info(message);
    }
    else
    {
        log_error("No nearby location found");
    }

    // Test FindNNearest function to find 5 nearest locations
    log_info("Testing FindNNearest to get 5 nearest locations:");
    std::vector<Location *> nearestLocations = locManager.FindNNearest(lat, lon, 5);

    if (!nearestLocations.empty())
    {
        char message[100];
        sprintf(message, "Found %zu nearest locations:", nearestLocations.size());
        log_info(message);

        // Display each location and its distance
        for (size_t i = 0; i < nearestLocations.size(); ++i)
        {
            Location *loc = nearestLocations[i];
            double dist = locManager.CalculateDistance(lat, lon, loc->latitude, loc->longitude);

            log_info("-------------------------");
            sprintf(message, "%zu. Name: %s", i + 1, loc->name);
            log_info(message);
            sprintf(message, "   Code: %s", loc->code);
            log_info(message);
            sprintf(message, "   State: %s", loc->state ? loc->state : "N/A");
            log_info(message);
            sprintf(message, "   Distance: %.2f km", dist);
            log_info(message);

            // Show the coordinates
            std::string coordStr = locManager.FormatCoordinates(loc);
            sprintf(message, "   Coordinates: %s", coordStr.c_str());
            log_info(message);
        }
    }
    else
    {
        log_error("No nearby locations found");
    }

    // Test Demo for distance calculation
    log_info("Testing nearest location search demo");
    demo_nearest_location();

    // Test Demo for bearing calculation
    log_info("Testing bearing calculation demo");
    demo_location_bearing();

#ifndef ARDUINO
    // Removed premature return for native build to allow main loop execution
#endif
}

// Demo showing nearby locations
void demo_nearby_locations()
{
    // Existing function code
    // ...
}

// Demo showing nearest location
void demo_nearest_location()
{
    log_info("Starting nearest location demo...");

    // Get LocationsManager instance
    LocationsManager &locMgr = LocationsManager::GetInstance();

    // Define test coordinates (Sydney Airport area)
    double testLat = -33.9399;
    double testLon = 151.1753;

    // Find the nearest location
    Location *nearest = locMgr.FindNearest(testLat, testLon);

    if (nearest)
    {
        char message[100];
        log_info("Nearest location found:");
        sprintf(message, "Name: %s", nearest->name);
        log_info(message);
        sprintf(message, "Code: %s", nearest->code);
        log_info(message);
        if (nearest->state)
        {
            sprintf(message, "State: %s", nearest->state);
            log_info(message);
        }

        // Calculate and show distance
        double distance = locMgr.CalculateDistance(testLat, testLon,
                                                   nearest->latitude, nearest->longitude);
        sprintf(message, "Distance: %.2f km", distance);
        log_info(message);

        // Show coordinates
        std::string coordStr = locMgr.FormatCoordinates(nearest);
        sprintf(message, "Coordinates: %s", coordStr.c_str());
        log_info(message);
    }
    else
    {
        log_error("No nearest location found");
    }
}

// Demo showing bearing between locations
void demo_location_bearing()
{
    log_info("Starting bearing calculation demo...");

    // Get LocationsManager instance
    LocationsManager &locMgr = LocationsManager::GetInstance();

    // Define locations explicitly by coordinates for more reliable demo
    // Sydney coordinates
    double sydney_lat = -33.8667;
    double sydney_lon = 151.2089; // Corrected for proper East longitude

    // Brisbane coordinates
    double brisbane_lat = -27.4698;
    double brisbane_lon = 153.0251; // Corrected for proper East longitude

    char message[100];
    sprintf(message, "Using Sydney at %.4f, %.4f", sydney_lat, sydney_lon);
    log_info(message);

    sprintf(message, "Using Brisbane at %.4f, %.4f", brisbane_lat, brisbane_lon);
    log_info(message);

    // Calculate bearing from Sydney to Brisbane
    double bearing = locMgr.CalculateBearing(sydney_lat, sydney_lon, brisbane_lat, brisbane_lon);
    sprintf(message, "Bearing from Sydney to Brisbane: %.1f degrees", bearing);
    log_info(message);

    // Also calculate distance
    double distance = locMgr.CalculateDistance(sydney_lat, sydney_lon, brisbane_lat, brisbane_lon);
    sprintf(message, "Distance from Sydney to Brisbane: %.1f km", distance);
    log_info(message);

    demo_gps_data();
}

void demo_gps_data()
{
    log_info("GPS Data Demo");

    GPSHAL &gps = GPSHAL::getInstance();

    // Update GPS data (not necessary for hardcoded values but good practice)
    gps.update();

    char buffer[100];

    log_info("Current GPS Position:");

    sprintf(buffer, "  Latitude:  %.6f", gps.getLatitude());
    log_info(buffer);

    sprintf(buffer, "  Longitude: %.6f", gps.getLongitude());
    log_info(buffer);

    sprintf(buffer, "  Altitude:  %.1f meters", gps.getAltitude());
    log_info(buffer);

    sprintf(buffer, "  Ground Speed: %.1f knots", gps.getGroundSpeed());
    log_info(buffer);

    sprintf(buffer, "  Heading: %.1f degrees", gps.getHeading());
    log_info(buffer);
}

// --- Non-ESP32 Polling Logic ---
#ifndef ESP32
static auto last_gps_poll_time = std::chrono::steady_clock::now();
const auto gps_poll_interval = std::chrono::milliseconds(10000); // 10 seconds

void poll_gps_and_update_cache()
{
    GPSHAL &gps = GPSHAL::getInstance();
    gps.update(); // Update HAL

    // Read data
    double lat = gps.getLatitude();
    double lon = gps.getLongitude();
    double alt = gps.getAltitude();
    double speed = gps.getGroundSpeed();
    double heading = gps.getHeading();
    bool valid = (lat != 0.0 || lon != 0.0); // Basic validity check

    // Update cache (using std::mutex)
    try
    {
        std::lock_guard<std::mutex> lock(g_gps_cache_mutex_std);
        g_gps_cache.latitude = lat;
        g_gps_cache.longitude = lon;
        g_gps_cache.altitude = alt;
        g_gps_cache.groundSpeed = speed;
        g_gps_cache.heading = heading;
        g_gps_cache.fix_valid = valid;
        // Optional: log update
        // if (!valid) log_d("GPS polling (sim): No valid fix.");
    }
    catch (const std::system_error &e)
    {
        log_errorf("std::mutex lock failed during GPS poll (sim): %s", e.what()); // Use log_errorf
    }
}
#endif
// --- End Non-ESP32 Polling Logic ---

void loop()
{
// Start of the main loop cycle
#ifdef T5EPAPER_DEVICE
    static uint32_t loop_counter = 0;
    // Only log every 500 iterations to avoid flooding the serial output
    if (loop_counter++ % 500 == 0)
    {
        log_debug("T5EPAPER main loop iteration");
    }
#endif

// Update the display HAL (calls lv_timer_handler internally)
#ifdef T5EPAPER_DEVICE
    if (loop_counter % 500 == 0)
    {
        log_debug("Calling DisplayHAL::update()");
    }
#endif
    DisplayHAL::getInstance().update();

    // lv_tick_inc is handled within DisplayHAL::update() for native builds
    // lv_tick_inc(5); // Removed redundant call

#ifndef ESP32
    // Poll GPS periodically in the simulator loop
    auto now = std::chrono::steady_clock::now();
    if (now - last_gps_poll_time >= gps_poll_interval)
    {
        last_gps_poll_time = now;
        poll_gps_and_update_cache();
    }
#endif

    // Call the GPS demo (uncomment to use)
    //
#ifdef ARDUINO
    // Minimal delay to prevent watchdog timeout on ESP32 if loop is empty
    delay(10);
#endif
}

#ifndef ARDUINO
int main(int argc, char *argv[])
{
    setup();
    while (true)
    {
        loop();
        // DisplayHAL::update() already includes a small delay, so extra sleep is removed.
        // usleep(10000); // Removed redundant delay
    }
    return 0;
}
#endif
