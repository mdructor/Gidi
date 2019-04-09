# Gidi
![Screenshot](https://i.ibb.co/0BfvW3x/screnshot.png)

## Available Platforms

- linux (tested and developed on)
- windows (built, not fully tested)
- MacOS/ioS (haven't built tested, need to)

## Dependencies

Gidi relies on heavily on the JUCE framework for midi processing and GUI/application processing.  We also depend on SDL2 for gamepad input and nhlohmann/json to parse map files and general app data settings.

That leaves you with needing:

- JUCE /w Projucer
- SDL2 development libraries and runtime library
- nhlohhman/json library. just need to grab the includes and throw it in your normal include path

Once you have these things sorted, you can move on to...

## Building

First step on all platforms is open up the project file in the main directory with Projucer. From here you can find the exporter for your preferred platform/IDE and export
it out to that and build from there.  If you need to make your own exporter, make sure you are linking to the SDL2 library somewhere (i.e. -lSDL2 in linker flags)