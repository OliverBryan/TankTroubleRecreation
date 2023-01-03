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

sf::Keyboard::Key Config::getPlayerKey(const std::string& name, std::size_t index) {
    const auto& root = cfg.getRoot();

    try {
        // TODO: merge these calls into one line
        const auto& players = root["players"];

        // TODO: see above
        const auto& keybinds = players[index]["keybinds"];

        const std::string& key = keybinds.lookup(name);

        return getKeyFromString(key);
    }
    catch (const libconfig::SettingNotFoundException& ex) {
        Log::logError(std::string("Could not read \"players\" section of settings.cfg (details: ") + ex.getPath() + ")");
        std::exit(-1);
    }
    catch (libconfig::SettingRangeException& ex) {
        Log::logError("Invalid index for getPlayerKey() (Config.cpp)");
        std::exit(-1);
    }
}

sf::Color Config::getPlayerColor(std::size_t index) {
    const auto& root = cfg.getRoot();

    try {
        // TODO: merge these calls into one line
        const auto& players = root["players"];

        // TODO: see above
        const auto& player = players[index];

        const std::string& color = player.lookup("color");

        return getColorFromString(color);
    }
    catch (const libconfig::SettingNotFoundException& ex) {
        Log::logError(std::string("Could not read \"players\" section of settings.cfg (details: ") + ex.getPath() + ")");
        std::exit(-1);
    }
    catch (libconfig::SettingRangeException& ex) {
        Log::logError("Invalid index for getPlayerColor() (Config.cpp)");
        std::exit(-1);
    }
}

sf::Keyboard::Key Config::getKeyFromString(const std::string& str) {
	if (str == "A") return sf::Keyboard::A;
	if (str == "B") return sf::Keyboard::B;
	if (str == "C") return sf::Keyboard::C;
	if (str == "D") return sf::Keyboard::D;
	if (str == "E") return sf::Keyboard::E;
	if (str == "F") return sf::Keyboard::F;
	if (str == "G") return sf::Keyboard::G;
	if (str == "H") return sf::Keyboard::H;
	if (str == "I") return sf::Keyboard::I;;		
	if (str == "J") return sf::Keyboard::J;
	if (str == "K") return sf::Keyboard::K;
	if (str == "L") return sf::Keyboard::L;
	if (str == "M") return sf::Keyboard::M;
	if (str == "N") return sf::Keyboard::N;
	if (str == "O") return sf::Keyboard::O;
	if (str == "P") return sf::Keyboard::P;
	if (str == "Q") return sf::Keyboard::Q;
	if (str == "R") return sf::Keyboard::R;
	if (str == "S") return sf::Keyboard::S;
	if (str == "T") return sf::Keyboard::T;
	if (str == "U") return sf::Keyboard::U;
	if (str == "V") return sf::Keyboard::V;
	if (str == "W") return sf::Keyboard::W;
	if (str == "X") return sf::Keyboard::X;
	if (str == "Y") return sf::Keyboard::Y;
	if (str == "Z") return sf::Keyboard::Z;
	if (str == "0") return sf::Keyboard::Num0;
	if (str == "1") return sf::Keyboard::Num1;
	if (str == "2") return sf::Keyboard::Num2;
	if (str == "3") return sf::Keyboard::Num3;
	if (str == "4") return sf::Keyboard::Num4;
	if (str == "5") return sf::Keyboard::Num5;
	if (str == "6") return sf::Keyboard::Num6;
	if (str == "7") return sf::Keyboard::Num7;
	if (str == "8") return sf::Keyboard::Num8;
	if (str == "9") return sf::Keyboard::Num9;
	if (str == "Up") return sf::Keyboard::Up;
	if (str == "Down") return sf::Keyboard::Down;
	if (str == "Left") return sf::Keyboard::Left;
	if (str == "Right") return sf::Keyboard::Right;
	if (str == "LShift") return sf::Keyboard::LShift;
	if (str == "RShift") return sf::Keyboard::RShift;

	// not valid
	Log::logError("Invalid key string: " + str);
	return sf::Keyboard::Escape;
}

// TODO: add more colors to this
sf::Color Config::getColorFromString(const std::string& str) {    
    if (str == "Black") return sf::Color::Black;
    if (str == "Blue") return sf::Color::Blue;
    if (str == "Cyan") return sf::Color::Cyan;
    if (str == "Green") return sf::Color::Green;
    if (str == "Magenta") return sf::Color::Magenta;
    if (str == "Red") return sf::Color::Red;
    if (str == "White") return sf::Color::White;
    if (str == "Yellow") return sf::Color::Yellow;
;}