# Trail Lab - Installation Instructions

## Prerequisites

Before installing Trail Lab, ensure you have:

1. **Geometry Dash 2.208** installed on your Android device
2. **Geode Loader** installed and working (compatible with GD 2.208)
3. **Unknown sources** enabled in Android settings (to install APKs from outside Play Store)

## Installation Steps

### Step 1: Obtain the Trail Lab Mod File

Download the latest release: `trail-lab-v1.0.0.android64.geode`

### Step 2: Locate Your Geode Mods Folder

1. Open Geometry Dash
2. Tap the **Geode icon** in the bottom-left corner of the main menu
3. Tap **Manage Mods** or browse your installed mods
4. Tap **Open Mods Folder** (or navigate manually to):
   ```
   /storage/emulated/0/Android/data/com.geode.launcher/game/geode/mods/
   ```

### Step 3: Install the Mod

1. Copy `trail-lab-v1.0.0.android64.geode` into the `mods/` folder
2. Restart Geometry Dash completely (close from recents, then reopen)

### Step 4: Verify Installation

1. Open Geometry Dash
2. Tap the Geode icon (bottom-left)
3. You should see **Trail Lab** in your installed mods list
4. Tap on Trail Lab to open its settings

### Step 5: Configure Trail Lab

Each game mode has independent settings:

| Setting | Description | Range |
|---------|-------------|-------|
| Enabled | Turn custom trail on/off for this mode | true/false |
| Length | Trail fade duration | 0.1 - 3.0 seconds |
| Width | Trail stroke thickness | 1.0 - 20.0 |
| Opacity | Trail transparency | 0% - 100% |
| Color | Trail RGB color | Color picker |
| No Pulse | Disable pulse effects | true/false |

## Using Presets

1. **Save Preset**: Save your current configuration under a name
2. **Load Preset**: Apply a previously saved configuration
3. **Delete Preset**: Remove a saved preset (except "default")

Presets persist across game restarts.

## Uninstalling

1. Open the Geode menu in Geometry Dash
2. Find Trail Lab in your mods list
3. Tap the uninstall/remove option
4. Or manually delete `trail-lab.android64.geode` from the mods folder

## Troubleshooting

| Issue | Solution |
|-------|----------|
| Trail Lab not appearing in Geode menu | Ensure the .geode file is in the correct `mods/` folder and restart GD |
| Settings not saving | Ensure Geode has storage permissions on your device |
| Trails not changing in-game | Verify the mod is enabled and the specific game mode is enabled in settings |
| Game crashes on startup | Remove the .geode file from mods folder and restart |

## Support

For issues or feature requests, please visit the Trail Lab GitHub repository.
