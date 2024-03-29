/* 
    Copyright 2017-2020 Mason Dructor
*/
/*
    This file is part of Gidi.

    Gidi is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Gidi is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Gidi.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include <memory>
#include "MainComponent.h"
#include "GidiLookAndFeel.h"
#include "AppSettings.h"

#define SDL_MAIN_HANDLED
#include <SDL.h>

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

        MainWindow(String name) : DocumentWindow(name,
            Desktop::getInstance().getDefaultLookAndFeel()
            .findColour(ResizableWindow::backgroundColourId),
            DocumentWindow::minimiseButton | DocumentWindow::closeButton)
        {
            setUsingNativeTitleBar (false);
            setContentOwned (new MainComponent(), true);

           #if JUCE_IOS || JUCE_ANDROID
            setFullScreen (true);
           #else
            setResizable (false, true);
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
