#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <libconfig.h++>
#include <Log.hpp>
#include <SFML/Graphics.hpp>

// Wrapper over libconfig++ to make loading settings easy
class Config {
public:
	/// <summary>
	/// Must be called before any calls to getSetting
	/// </summary>
	static void init();

	/// <summary>
	/// Loads a setting from settings.cfg
	/// </summary>
	/// <param name="settingName">Name of the setting to retrieve</param>
	/// <param name="defaultValue">Default value if the setting is not found</param>
	/// <returns>Value of the setting </returns>
	template <class T> 
	static T getSetting(const std::string& settingName, const T& defaultValue) {
		if (!initialized)
			throw std::runtime_error("Config must be initialized before usage");

		T out = defaultValue;
		try {
			out = cfg.lookup(settingName);
		}
		catch (const libconfig::SettingNotFoundException& ex) {
			Log::logError("No setting with name " + settingName + " was found");
		}

		return out;
	}

	/// <summary>
	/// Get the keybind for a player from settings.cfg
	/// </summary>
	/// <param name="name">name of the key</param>
	/// <param name="index">index of the player to get keys from</param>
	/// <returns>sfml key as specified in settings.cfg</returns>
	static sf::Keyboard::Key getPlayerKey(const std::string& name, std::size_t index);
	
	/// <summary>
	/// Get a player's color from settings.cfg
	/// </summary>
	/// <param name="index">index of the player to check</param>
	/// <returns>sfml color as specified in settings.cfg</returns>
	static sf::Color getPlayerColor(std::size_t index);

private:
	static bool initialized;
	static libconfig::Config cfg;

	static sf::Keyboard::Key getKeyFromString(const std::string& str);
	static sf::Color getColorFromString(const std::string& str);
};

#endif /* CONFIG_HPP */