# Trail Lab

Advanced trail customization tool for Geometry Dash.

## Features

Trail Lab allows you to customize Geometry Dash trails independently for each game mode:

- **Trail Length** - Adjustable fade time with slider and numeric display
- **Trail Width** - Fine-grained stroke width control
- **Trail Opacity** - Adjustable transparency from 0% to 100%
- **Trail Color** - Full RGB color picker per game mode
- **No Pulse** - Disable pulse effects on trails
- **Independent Profiles** - Separate settings for Wave, Ship, Cube, UFO, Ball, Robot, Spider
- **Presets** - Save, load, rename, and delete trail configurations
- **Live Preview** - Optional preview of trail changes (Android)

## Supported Game Modes

- Cube
- Ship
- Wave
- UFO
- Ball
- Robot
- Spider

## Installation

### Prerequisites

1. Geometry Dash 2.208 installed on your Android device
2. Geode Loader installed (compatible with GD 2.208)

### Install Trail Lab

1. Download the latest `trail-lab-v1.0.0.android64.geode` file from the releases page
2. Open Geometry Dash
3. Click the Geode icon in the bottom-left corner of the main menu
4. Click the "Open Folder" button to open your geode mods folder
5. Copy the `.geode` file into the `mods/` subfolder
6. Restart Geometry Dash

### Accessing Settings

1. Open Geometry Dash
2. Click the Geode icon in the bottom-left corner
3. Find "Trail Lab" in your installed mods list
4. Click on it to access the settings menu

## Configuration

Each game mode has independent settings:
- Enable/Disable custom trails
- Trail length (fade duration)
- Trail width (stroke thickness)
- Trail opacity (0-100%)
- Trail color (RGB picker)
- No Pulse toggle

Settings are saved automatically and persist across game restarts.

## Presets

Save your trail configurations as presets for quick switching:
- Save current settings as a named preset
- Load saved presets without restarting
- Delete presets you no longer need
- Default preset "default" cannot be deleted

## Technical Details

- Built with Geode SDK 5.11.2
- Compatible with Geometry Dash 2.208
- Android ARM64 (arm64-v8a)
- Hooks CCMotionStreak for trail customization
- Settings stored via Geode's ModSettingsManager
- Presets stored as JSON in the mod's save directory

## Limitations

- Trail color customization works on trails that support tinting
- Game mode detection during trail creation is currently limited; all modes use the same base configuration
- Live preview on Android requires enabling in settings

## License

MIT License
