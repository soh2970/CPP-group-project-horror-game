# Group Project: Audio-Based Gameplay Challenge System (Qt)

## Overview

This group project is a Qt-based interactive game developed in C++ for CS3307A – Object-Oriented Design and Analysis. The game was designed to feature periodic challenges that engage the player while navigating the environment. It integrates real-time player input, graphical elements, and attempts at audio interaction.

## My Contributions

As part of the development team, I was primarily responsible for the gameplay pop-ups and challenge system:

- Implemented the typing challenge pop-up that interrupts gameplay at intervals.
- Designed UI components for countdown, challenge phrase, input, and result feedback.
- Originally attempted to integrate voice-to-text functionality using Vosk and PocketSphinx, but due to integration difficulties, the feature was replaced with a keyboard input alternative.
- Developed fallback logic for incorrect inputs (e.g., CS meme jumpscare pop-ups).
- Authored and contributed to:
  - `voicechallenge.cpp/.h`
  - `inputhandler.cpp/.h`
  - Portions of `audiomanager.cpp/.h`
  - Minor edits in `main.cpp` for integration and initialization

## Features

- Periodic in-game typing challenges:
  - Players must correctly type a tongue-twister phrase within a countdown.
  - Success allows continued movement.
  - Failure results in a health penalty and jumpscare overlay.
- Modular challenge management with start/stop functionality.
- Dynamic UI rendering of overlays, input fields, success animations, and meme imagery.
- Real-time key event handling via custom `InputHandler`.

## Key Files

| File                 | Purpose                                                   |
|----------------------|-----------------------------------------------------------|
| `voicechallenge.cpp/h` | Manages challenge timing, text input, feedback, and jumpscares |
| `inputhandler.cpp/h`   | Handles player movement (WASD) and input routing         |
| `audiomanager.cpp/h`   | Initializes Qt audio input system (originally for voice detection) |
| `main.cpp`             | Application entry point and initialization               |

## Technical Stack

- Qt Framework (Widgets, GraphicsScene, Multimedia)
- C++ with OOP principles (signals/slots, encapsulation, modularization)
- GraphicsScene-based UI elements (e.g., challenge pop-ups, text, and buttons)
- Failed integration attempt with:
  - Vosk API for real-time speech recognition
  - PocketSphinx for offline speech parsing

## Known Limitations

- Audio input pipeline was initialized (AudioManager) but full speech recognition was not successfully integrated.
- Current solution uses keyboard-based fallback input instead of voice-based gameplay.
- GraphicsScene UI positioning is static and may require dynamic scaling improvements.

## Screenshots

Included in the repository is a visual snapshot of the project folder and challenge overlay.

## License

This project is part of CS3307A – Object-Oriented Design and Analysis at Western University. For academic use only.
