#include "Config.hpp"
#include <Log.hpp>

// Static member variables
libconfig::Config Config::cfg;
bool Config::initialized = false;

void Config::init() {
    Log::logStatus("Loading settings.cfg...", ConsoleColor::DarkBlue);

    // As of now, a failure to load settings.cfg is a complete failure, this will change later
    try {
        cfg.readFile("./res/settings.cfg");
    }
    catch (const libconfig::FileIOException& ex) {
        Log::logError("IO error reading settings.cfg");
        std::exit(-1);
    }
    catch (const libconfig::ParseException& ex) {
        Log::logError(std::string("Parse error: ") + ex.getFile() + std::string(":") + std::to_string(ex.getLine()) + std::string("-") + ex.getError());
        std::exit(-1);
    }

    Log::logStatus("Settings.cfg loaded", ConsoleColor::DarkBlue);

    initialized = true;
}