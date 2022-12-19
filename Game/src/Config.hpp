#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <libconfig.h++>
#include <Log.hpp>

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
private:
	static bool initialized;
	static libconfig::Config cfg;
};

#endif /* CONFIG_HPP */