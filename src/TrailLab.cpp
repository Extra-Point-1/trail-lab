#include "TrailLab.hpp"
#include <Geode/Geode.hpp>
#include <Geode/modify/Modify.hpp>
#include <Geode/loader/Mod.hpp>
#include <Geode/loader/Log.hpp>
#include <Geode/cocos/misc_nodes/CCMotionStreak.h>
#include <cocos2d.h>

using namespace geode::prelude;

// Apply trail config to a motion streak object
static inline void applyTrailConfig(cocos2d::CCMotionStreak* streak, TrailConfig const& config) {
    if (!streak || !config.enabled) return;

    // Apply width (stroke)
    streak->setStroke(config.width);

    // Apply opacity via RGBA on the node
    streak->setOpacity(static_cast<unsigned char>(config.opacity * 255.0f));

    // Apply color
    streak->tintWithColor(config.color);

    // No Pulse: disable opacity fading if requested
    streak->setDontOpacityFade(config.noPulse);
}

// Hook CCMotionStreak::initWithFade to customize trails at initialization time.
// CCMotionStreak::create() calls initWithFade internally, so hooking initWithFade
// catches all trail creation paths.
class $modify(TrailMotionStreak, cocos2d::CCMotionStreak) {
    bool initWithFade(
        float fade, float minSeg, float stroke, cocos2d::ccColor3B const& color, char const* path
    ) {
        if (!CCMotionStreak::initWithFade(fade, minSeg, stroke, color, path)) {
            return false;
        }

        auto settings = TrailLabSettings::get();
        if (!settings->isEnabled()) return true;

        // Apply trail settings for Cube mode as baseline.
        // Game mode-specific config will be refined when we can determine
        // the active gameplay mode from PlayLayer context.
        auto& cfg = settings->getConfig(GameMode::Cube);
        if (cfg.enabled) {
            applyTrailConfig(this, cfg);
        }

        return true;
    }

    bool initWithFade(
        float fade, float minSeg, float stroke, cocos2d::ccColor3B const& color, cocos2d::CCTexture2D* texture
    ) {
        if (!CCMotionStreak::initWithFade(fade, minSeg, stroke, color, texture)) {
            return false;
        }

        auto settings = TrailLabSettings::get();
        if (!settings->isEnabled()) return true;

        auto& cfg = settings->getConfig(GameMode::Cube);
        if (cfg.enabled) {
            applyTrailConfig(this, cfg);
        }

        return true;
    }
};

// Initialize settings on mod load
$execute {
    auto settings = TrailLabSettings::get();
    settings->reloadSettings();

    geode::log::info("[TrailLab] Mod loaded successfully - version 1.0.0");
}
