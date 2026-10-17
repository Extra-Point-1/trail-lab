#include "TrailLab.hpp"
#include <Geode/Geode.hpp>
#include <Geode/utils/file.hpp>
#include <Geode/utils/JsonValidation.hpp>
#include <matjson.hpp>

using namespace geode::prelude;

TrailLabSettings* TrailLabSettings::get() {
    static TrailLabSettings instance;
    return &instance;
}

TrailLabSettings::TrailLabSettings() {
    loadPresetsFromDisk();
}

TrailLabSettings::~TrailLabSettings() = default;

bool TrailLabSettings::isEnabled() const {
    return m_enabled;
}

bool TrailLabSettings::isLivePreviewEnabled() const {
    return m_livePreview;
}

std::string const& TrailLabSettings::currentPreset() const {
    return m_currentPreset;
}

TrailConfig const& TrailLabSettings::getConfig(GameMode mode) const {
    return m_configs[static_cast<size_t>(mode)];
}

void TrailLabSettings::reloadSettings() {
    auto mod = Mod::get();

    m_enabled = mod->getSettingValue<bool>("mod-enabled");
    m_livePreview = mod->getSettingValue<bool>("live-preview");
    m_currentPreset = mod->getSettingValue<std::string>("current-preset");

    // Load configs for each game mode
    for (int i = 0; i < static_cast<int>(GameMode::Count); ++i) {
        auto mode = static_cast<GameMode>(i);
        auto prefix = modePrefix(mode);
        auto& cfg = m_configs[i];

        cfg.enabled = mod->getSettingValue<bool>(std::string(prefix) + "-enabled");
        cfg.length = static_cast<float>(mod->getSettingValue<double>(std::string(prefix) + "-length"));
        cfg.width = static_cast<float>(mod->getSettingValue<double>(std::string(prefix) + "-width"));
        cfg.opacity = static_cast<float>(mod->getSettingValue<double>(std::string(prefix) + "-opacity"));

        // Color3B setting returns ccColor3B directly
        cfg.color = mod->getSettingValue<cocos2d::ccColor3B>(std::string(prefix) + "-color");

        cfg.noPulse = mod->getSettingValue<bool>(std::string(prefix) + "-no-pulse");
    }

    // Load preset if one is selected
    if (!m_currentPreset.empty() && m_currentPreset != "default") {
        loadPreset(m_currentPreset);
    }
}

void TrailLabSettings::resetToDefaults() {
    // Reset all configs to defaults
    for (int i = 0; i < static_cast<int>(GameMode::Count); ++i) {
        m_configs[i] = TrailConfig{};
    }
    m_enabled = true;
    m_livePreview = false;
    m_currentPreset = "default";

    auto mod = Mod::get();
    mod->setSettingValue<bool>("mod-enabled", true);
    mod->setSettingValue<bool>("live-preview", false);
    mod->setSettingValue<std::string>("current-preset", "default");

    cocos2d::ccColor3B white = {255, 255, 255};
    for (int i = 0; i < static_cast<int>(GameMode::Count); ++i) {
        auto mode = static_cast<GameMode>(i);
        auto prefix = modePrefix(mode);
        mod->setSettingValue<bool>(std::string(prefix) + "-enabled", false);
        mod->setSettingValue<double>(std::string(prefix) + "-length", 0.5);
        mod->setSettingValue<double>(std::string(prefix) + "-width", 5.0);
        mod->setSettingValue<double>(std::string(prefix) + "-opacity", 1.0);
        mod->setSettingValue<cocos2d::ccColor3B>(std::string(prefix) + "-color", white);
        mod->setSettingValue<bool>(std::string(prefix) + "-no-pulse", false);
    }

    savePresetsToDisk();
}

void TrailLabSettings::savePreset(std::string const& name) {
    if (name.empty()) return;

    std::array<TrailConfig, static_cast<size_t>(GameMode::Count)> copy;
    for (int i = 0; i < static_cast<int>(GameMode::Count); ++i) {
        copy[i] = m_configs[i];
    }
    m_presets[name] = copy;
    m_currentPreset = name;

    auto mod = Mod::get();
    mod->setSettingValue<std::string>("current-preset", name);

    savePresetsToDisk();
}

bool TrailLabSettings::loadPreset(std::string const& name) {
    auto it = m_presets.find(name);
    if (it == m_presets.end()) {
        // Try loading from disk
        loadPresetsFromDisk();
        it = m_presets.find(name);
        if (it == m_presets.end()) {
            return false;
        }
    }

    for (int i = 0; i < static_cast<int>(GameMode::Count); ++i) {
        m_configs[i] = it->second[i];
    }
    m_currentPreset = name;

    auto mod = Mod::get();
    mod->setSettingValue<std::string>("current-preset", name);

    for (int i = 0; i < static_cast<int>(GameMode::Count); ++i) {
        auto mode = static_cast<GameMode>(i);
        auto prefix = modePrefix(mode);
        auto& cfg = m_configs[i];

        mod->setSettingValue<bool>(std::string(prefix) + "-enabled", cfg.enabled);
        mod->setSettingValue<double>(std::string(prefix) + "-length", cfg.length);
        mod->setSettingValue<double>(std::string(prefix) + "-width", cfg.width);
        mod->setSettingValue<double>(std::string(prefix) + "-opacity", cfg.opacity);
        mod->setSettingValue<cocos2d::ccColor3B>(std::string(prefix) + "-color", cfg.color);
        mod->setSettingValue<bool>(std::string(prefix) + "-no-pulse", cfg.noPulse);
    }

    return true;
}

bool TrailLabSettings::deletePreset(std::string const& name) {
    if (name == "default") return false; // Don't allow deleting default

    auto it = m_presets.find(name);
    if (it == m_presets.end()) return false;

    m_presets.erase(it);
    if (m_currentPreset == name) {
        m_currentPreset = "default";
        auto mod = Mod::get();
        mod->setSettingValue<std::string>("current-preset", "default");
    }

    savePresetsToDisk();
    return true;
}

std::vector<std::string> TrailLabSettings::listPresets() const {
    std::vector<std::string> result;
    result.push_back("default");
    for (auto const& [name, _] : m_presets) {
        result.push_back(name);
    }
    return result;
}

void TrailLabSettings::loadPresetsFromDisk() {
    m_presets.clear();

    auto mod = Mod::get();
    auto saveDir = mod->getSaveDir();
    auto path = saveDir / "trail_presets.json";

    if (!file::exists(path)) return;

    auto json = matjson::parse(file::readFile(path));
    if (!json.isObject()) return;

    if (json.has("presets") && json["presets"].isObject()) {
        for (auto const& [key, val] : json["presets"].obj().items()) {
            if (!val.isObject()) continue;

            auto& cfgArray = m_presets[key];
            for (int i = 0; i < static_cast<int>(GameMode::Count); ++i) {
                auto mode = static_cast<GameMode>(i);
                auto prefix = modePrefix(mode);
                auto& cfg = cfgArray[i];

                auto modeVal = val[prefix];
                if (!modeVal.isObject()) continue;

                cfg.enabled = modeVal["enabled"].unwrapOr(false);
                cfg.length = static_cast<float>(modeVal["length"].unwrapOr(0.5));
                cfg.width = static_cast<float>(modeVal["width"].unwrapOr(5.0));
                cfg.opacity = static_cast<float>(modeVal["opacity"].unwrapOr(1.0));
                cfg.noPulse = modeVal["noPulse"].unwrapOr(false);

                if (modeVal["color"].isArray() && modeVal["color"].size() >= 3) {
                    cfg.color.r = static_cast<unsigned char>(modeVal["color"][0].unwrapOr(255));
                    cfg.color.g = static_cast<unsigned char>(modeVal["color"][1].unwrapOr(255));
                    cfg.color.b = static_cast<unsigned char>(modeVal["color"][2].unwrapOr(255));
                }
            }
        }
    }
}

void TrailLabSettings::savePresetsToDisk() {
    auto mod = Mod::get();
    auto saveDir = mod->getSaveDir();
    auto path = saveDir / "trail_presets.json";

    matjson::Value root = matjson::Value::object();
    matjson::Value presets = matjson::Value::object();

    for (auto const& [name, configs] : m_presets) {
        matjson::Value modeObj = matjson::Value::object();
        for (int i = 0; i < static_cast<int>(GameMode::Count); ++i) {
            auto mode = static_cast<GameMode>(i);
            auto prefix = modePrefix(mode);
            auto& cfg = configs[i];

            matjson::Value cfgObj = matjson::Value::object();
            cfgObj["enabled"] = cfg.enabled;
            cfgObj["length"] = cfg.length;
            cfgObj["width"] = cfg.width;
            cfgObj["opacity"] = cfg.opacity;
            cfgObj["noPulse"] = cfg.noPulse;
            cfgObj["color"] = matjson::Value::array({
                static_cast<double>(cfg.color.r),
                static_cast<double>(cfg.color.g),
                static_cast<double>(cfg.color.b)
            });
            modeObj[prefix] = cfgObj;
        }
        presets[name] = modeObj;
    }
    root["presets"] = presets;

    file::writeFile(path, root.dump(4));
}
