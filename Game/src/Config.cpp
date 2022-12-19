#include "Config.hpp"
#include <Log.hpp>

libconfig::Config Config::cfg;

void Config::init() {
    Log::logStatus("Loading settings.cfg...", ConsoleColor::DarkBlue);

    try {
        cfg.readFile("settings.cfg");
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
}