#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <libconfig.h++>

class Config {
public:
	static void init();

	template <class T> 
	static T getSetting(const std::string& settingName, const T& defaultValue) {
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
	static libconfig::Config cfg;
};

#endif /* CONFIG_HPP */