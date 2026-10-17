#pragma once

#include <Geode/Geode.hpp>
#include <Geode/modify/Modify.hpp>
#include <Geode/loader/Mod.hpp>
#include <Geode/loader/SettingV3.hpp>
#include <cocos2d.h>
#include <string>
#include <array>
#include <unordered_map>
#include <vector>

using namespace geode::prelude;

// Trail configuration for a single game mode
struct TrailConfig {
    bool enabled = false;
    float length = 0.5f;      // fade time in seconds
    float width = 5.0f;       // stroke width
    float opacity = 1.0f;     // opacity 0-1
    cocos2d::ccColor3B color = {255, 255, 255};
    bool noPulse = false;     // disable pulse effects
};

// Game mode identifiers matching GD's player object types
enum class GameMode {
    Cube,
    Ship,
    Wave,
    UFO,
    Ball,
    Robot,
    Spider,
    Count
};

class TrailLabSettings {
public:
    static TrailLabSettings* get();

    // Get trail config for a specific game mode
    TrailConfig const& getConfig(GameMode mode) const;

    // Check if mod is globally enabled
    bool isEnabled() const;

    // Check if live preview is enabled
    bool isLivePreviewEnabled() const;

    // Current preset name
    std::string const& currentPreset() const;

    // Reload settings from mod.json / storage
    void reloadSettings();

    // Save current settings as a preset
    void savePreset(std::string const& name);

    // Load settings from a preset
    bool loadPreset(std::string const& name);

    // Delete a preset
    bool deletePreset(std::string const& name);

    // Reset all settings to defaults
    void resetToDefaults();

    // List available presets
    std::vector<std::string> listPresets() const;

private:
    TrailLabSettings();
    ~TrailLabSettings();

    TrailConfig m_configs[static_cast<size_t>(GameMode::Count)];
    bool m_enabled = true;
    bool m_livePreview = false;
    std::string m_currentPreset = "default";

    // Internal preset storage
    void loadPresetsFromDisk();
    void savePresetsToDisk();
    std::unordered_map<std::string, std::array<TrailConfig, static_cast<size_t>(GameMode::Count)>> m_presets;
};

// Helper to convert GameMode to setting key prefix
constexpr char const* modePrefix(GameMode mode) {
    switch (mode) {
        case GameMode::Cube:   return "cube";
        case GameMode::Ship:   return "ship";
        case GameMode::Wave:   return "wave";
        case GameMode::UFO:    return "ufo";
        case GameMode::Ball:   return "ball";
        case GameMode::Robot:  return "robot";
        case GameMode::Spider: return "spider";
        default:               return "";
    }
}
