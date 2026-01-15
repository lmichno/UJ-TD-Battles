# UJ-TD-Battles

Simple tower-defense style project built with C++ and SFML (Graphics). This repository contains a small game prototype where "shooters" defend against waves of enemies. It was created as a student project for demonstrations and reporting.

## Features
- Multiple waves with timed spawn patterns
- Placeable shooters (buy from side panel)
- Shooters fire bullets at enemies
- Enemies can fire red bullets back at shooters
- Simple UI/menu with responsive tiles

## Requirements
- C++ compiler (MSVC/Visual Studio recommended)
- SFML 3.x (Graphics module)
- Assets (fonts and images) must be present in the runtime working directory:
  - `ScienceGothic-Medium.ttf`, `LuckiestGuy-Regular.ttf`
  - `jaguar1.png`, `jaguar2.png`, `jaguar3.png`, `jaguar4.png`
  - `wrog1.png`, `wrog2.png`, `wrog3.png`
  - `bullet.png`, `sidePanel.png`, `background.png`, `background1.png`, `background2.png`

## Build & Run (Visual Studio)
1. Install SFML 3.x and configure include/lib paths in your Visual Studio project.
2. Open the project/solution in Visual Studio.
3. Build (Release or Debug) and run. Ensure the asset files are copied to the executable folder.

## Controls
- Mouse: navigate menu, click "PLAY" or "EXIT"
- In-game: click side-panel buttons to buy shooters (cost shown), shooters auto-fire

