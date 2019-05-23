/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic startup code for a JUCE application.

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include <memory>
#include "MainComponent.h"
#include "GidiLookAndFeel.h"
#include "AppSettings.h"

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

//==============================================================================
class mdGidiApplication  : public JUCEApplication
{
public:
    //==============================================================================
    mdGidiApplication() {}

    const String getApplicationName() override       { return ProjectInfo::projectName; }
    const String getApplicationVersion() override    { return ProjectInfo::versionString; }
    bool moreThanOneInstanceAllowed() override       { return true; }
    
    std::unique_ptr<GidiLookAndFeel> gidiLAF;

    //==============================================================================
    void initialise (const String& commandLine) override
    {
        SDL_SetMainReady();
        SDL_Init(SDL_INIT_GAMECONTROLLER);
        AppSettings::loadAppSettings();
        mainWindow.reset (new MainWindow (getApplicationName()));
        gidiLAF = std::unique_ptr<GidiLookAndFeel>(new GidiLookAndFeel());
        mainWindow->setLookAndFeel(gidiLAF.get());
    }

    void shutdown() override
    {
        SDL_Quit();
        mainWindow = nullptr; // (deletes our window)
    }

    //==============================================================================
    void systemRequestedQuit() override
    {
        quit();
    }

    void anotherInstanceStarted (const String& commandLine) override
    {
    }

    class MainWindow    : public DocumentWindow
    {
    public:

        MainWindow (String name)  : DocumentWindow (name,
                                                    Desktop::getInstance().getDefaultLookAndFeel()
                                                                          .findColour (ResizableWindow::backgroundColourId),
                                                    DocumentWindow::allButtons)
        {
            setUsingNativeTitleBar (false);
            setContentOwned (new MainComponent(), true);

           #if JUCE_IOS || JUCE_ANDROID
            setFullScreen (true);
           #else
            setResizable (true, true);
            setResizeLimits(400, 286, 800, 572);
            centreWithSize (getWidth(), getHeight());
           #endif

            setVisible (true);
        }

        void closeButtonPressed() override
        {
            // This is called when the user tries to close this window. Here, we'll just
            // ask the app to quit when this happens, but you can change this to do
            // whatever you need.
            JUCEApplication::getInstance()->systemRequestedQuit();
        }

        /* Note: Be careful if you override any DocumentWindow methods - the base
           class uses a lot of them, so by overriding you might break its functionality.
           It's best to do all your work in your content component instead, but if
           you really have to override any DocumentWindow methods, make sure your
           subclass also calls the superclass's method.
        */

    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainWindow)
    };

private:
    std::unique_ptr<MainWindow> mainWindow;
};

//==============================================================================
// This macro generates the main() routine that launches the app.
START_JUCE_APPLICATION (mdGidiApplication)
