/**
 @file GApp.h
 
 @maintainer Morgan McGuire, morgan@graphics3d.com

 @created 2003-11-03
 @edited  2006-03-04
 */

#ifndef G3D_GAPP_H
#define G3D_GAPP_H

#include "G3D/Stopwatch.h"
#include "GLG3D/GFont.h"
#include "G3D/GCamera.h"
#include "GLG3D/ManualCameraController.h"
#include "GLG3D/RenderDevice.h"
#include "GLG3D/TextureManager.h"
#include "G3D/NetworkDevice.h"
#include "GLG3D/GWindow.h"
#include "GLG3D/GModule.h"

namespace G3D {

class RenderDevice;
class UserInput;

/**
  See @link guideapp @endlink for a discussion of GApp and GApplet. 
 */
class GApp {
public:
    friend class GApplet;

	class Settings {
	public:
		GWindow::Settings    window;

		/**
		 If "<AUTO>", G3D will search for the standard
		 data files.  It is recommended that you override this
		 default and set dataDir to a directory relative
		 to your executable (e.g. "./data/")
		 so your programs can be distributed to users who
		 do not have full the G3D data directory.
		 */
		std::string             dataDir;

		/**
		 Can be relative to the G3D data directory (e.g. "font/dominant.fnt")
		 or relative to the current directory.
		 Default is "console-small.fnt"
		 */
		std::string             debugFontName;

		std::string             logFilename;

		/** 
		  When true, GAapp ensures that g3d-license.txt exists in the current
		  directory.  That file is written from the return value of G3D::license() */
		bool                    writeLicenseFile;

		/** When true, the networkDevice is initialized.  Defaults to true. */
		bool                    useNetwork;

		Settings() : dataDir("<AUTO>"), debugFontName("console-small.fnt"), 
			logFilename("log.txt"), writeLicenseFile(true), useNetwork(true) {
		}
	};


private:
    bool                    _debugMode;

    /**
     Tracks whether the debug controller (activated with TAB) was
     active when last we were in debug mode.
     */
    bool                    _debugControllerWasActive;

    /** Called from init. */
    void loadFont(const std::string& fontName);

    GWindow*                _window;
    bool                    _hasUserCreatedWindow;

protected:

    /**
     Called from run.  This is invoked inside of several
     exception handlers so that any G3D uncaught exceptions
     can be logged instead of crashing the application.
     */
    virtual void main() = 0;

    friend class GApplet;
    Stopwatch           m_graphicsWatch;
    Stopwatch           m_logicWatch;
    Stopwatch           m_networkWatch;
    Stopwatch           m_userInputWatch;
    Stopwatch           m_simulationWatch;
    Stopwatch           m_waitWatch;

    GModuleManagerRef   m_moduleManager;

public:

    const Stopwatch& graphicsWatch() const {
        return m_graphicsWatch;
    }

    const Stopwatch& waitWatch() const {
        return m_waitWatch;
    }

    const Stopwatch& logicWatch() const {
        return m_logicWatch;
    }

    const Stopwatch& networkWatch() const {
        return m_networkWatch;
    }

    const Stopwatch& userInputWatch() const {
        return m_userInputWatch;
    }


    const Stopwatch& simulationWatch() const {
        return m_simulationWatch;
    }

    /** Initialized to GApp::Settings::dataDir, or if that is "<AUTO>", 
        to System::demoFindData(). To make your program
        distributable, override the default 
        and copy all data files you need to a local directory.
        Recommended setting is "data/" or "./", depending on where
        you put your data relative to the executable.

        Your data directory must contain the default debugging font, 
        "console-small.fnt", unless you change it.
    */
    std::string             dataDir;
    Log*                    debugLog;
    RenderDevice*           renderDevice;
    NetworkDevice*          networkDevice;

    /**
     NULL if not loaded
     */
    GFontRef                debugFont;
    UserInput*              userInput;
    bool                    endProgram;

    /**
     A global texture manager.
     */
    TextureManager          textureManager;

    /**
     A default camera that is driven by the debugController.
     */
    GCamera					debugCamera;

    /**
     When in debugMode, this allows first person (Quake game-style) control
     using the arrow keys or W,A,S,D and the mouse.
     @deprecated Will become a FirstPersonManipulatorRef in 7.00
     */
    ManualCameraController  debugController;

    /**
     Strings that have been printed with debugPrint.
     */
    Array<std::string>      debugText;

    inline GWindow* window() const {
        return _window;
    }

    /** Returns the state of debugMode.
        All debugX options are only in effect
        when debugMode is also true.  Default is false*/
    bool debugMode() const;

    /**
      Changes the state of debugMode. 
      You must <B>separately</B> activate the debugController 
      if you want events to go to it (by default, the TAB key
      activates it).
     */
    virtual void setDebugMode(bool b);

    /**
     When true and debugMode is true, debugPrintf prints to the screen.
     (default is true)
     */
    bool                    debugShowText;

    /**
     When true and debugMode is true, an SDL_ESCAPE keydown event
     quits the program.
     (default is true)
     */
    bool                    debugQuitOnEscape;

    /**
     When true and debugMode is true, SDL_TAB keydown deactivates
     the camera and restores the mouse cursor.
     (default is true)
     */
    bool                    debugTabSwitchCamera;

    /**
     When debugMode is true and debugShowRenderingStats is true,
     renderDebugInfo prints the frame rate and other data to the screen.
     */
    bool                    debugShowRenderingStats;

    /**
     When true and the window is resizable, automatically
     responds to SDL_RESIZE events by notifying the
     RenderDevice that the window has been resized and
     resetting the viewport to full screen.
     (default is true)
     */
    bool                    autoResize;

    /**
      When true, there is an assertion failure if an exception is 
      thrown during GApp::main().

      Default is true.
      */
    bool                    catchCommonExceptions;

    /**
     If app->debugShowText is true, prints to an on-screen buffer that
     is cleared every frame.
     */
    virtual void debugPrintf(const char* fmt ...);

    /**
     Called from GApplet::run immediately after doGraphics to render
     the debugging text.  Does nothing if debugMode is false.  It
     is not usually necessary to override this method.
     */
    virtual void renderDebugInfo();

    /**
     @param window If null, a SDLWindow will be created for you. This
         argument is useful for substituting a different window
         system (e.g. GlutWindow)
     */
    GApp(const Settings& options = Settings(), GWindow* window = NULL);

    virtual ~GApp();

    /**
     Call this to run the app.  Subclasses should override main(), not run.
     */
    void run();

    /**
     Installs a module.  Actual insertion may be delayed until the next frame.
     See GApplet::addModule for further discussion.

     @beta
     */
    virtual void addModule(const GModuleRef& module, GModuleManager::EventPriority p = GModuleManager::NORMAL_PRIORITY);


    /**
     The actual removal of the module may be delayed until the next frame.
     See GApplet::removeModule for further discussion.
     @beta
     */
    virtual void removeModule(const GModuleRef& module);
};


/**
  A self-contained mode of operation for an application; a "screen".
  One large-scale state of an application.  For example, a game might have one GApplet subclass
  for the title screen, one for the main menu, one for the game itself, and one for the
  credits.

 
  For each frame, the GApplet has several tasks that can be implemented by overriding
  base class methods.  The use of cooperative, round-robbin scheduling avoids the need
  for threads in most applications.  These tasks are:

  <ul>
   <li> doGraphics(G3D::RenderDevice*)
   <li> doUserInput(G3D::UserInput*)
   <li> doLogic()
   <li> doNetwork()
   <li> doSimulation(G3D::RealTime, G3D::SimTime)
   <li> doWait(G3D::RealTime cumulativeTime, G3D::RealTime desiredCumulativeTime)
  </ul>


  To invoke a GApplet and let it control the main loop, call
  run().  To control the main loop explicitly, invoke beginRun on
  initialization, call oneFrame() from the main loop, and call endRun on cleanup.
 */ 
class GApplet {
private:
    GApp*               app;

    /** Used by doSimulation for elapsed time. */
    RealTime            now, lastTime;

    /** Used by doWait for elapsed time. */
    RealTime            lastWaitTime;

    float               m_desiredFrameRate;
    double              m_simTimeRate;
    RealTime            m_realTime;
    SimTime             m_simTime;
    SimTime             m_idealSimTime;

protected:

    GModuleManagerRef   m_moduleManager;

public:

    /**
     Installs a module.  Actual insertion may be delayed until the next frame.

     Modules can be installed either on a GApp or a GApplet.  Those installed
     on the GApp will be run by the GApplet every frame in addition to its own.
     
     By default events have NORMAL_PRIORITY. HIGH_PRIORITY is only intended
     for debugging modules that need to temporarily superceed other components.

     All modules within the same priority class receive events
     with undetermined priority.  All high priority modules
     receive events before.
     
     Event priority is important because a module can consume
     events, preventing other modules from observing them.
     For example, a debugging console might run at high priority 
     so that it can consume arrow keys before they reach a normal priority
     first-person controller.

     @beta
     */
    virtual void addModule(const GModuleRef& module, GModuleManager::EventPriority p = GModuleManager::NORMAL_PRIORITY);


    /**
     The actual removal of the module may be delayed until the next frame.
     @beta
     */
    virtual void removeModule(const GModuleRef& module);

    /** Amount of time that passes in simTime for every second of realTime.
        e.g., 1.0 == real-time, 2.0 == fast, 0.5 == slow, 0.0 = stop time.
        Default is 1.0.
        */
    inline double simTimeRate() const {
        return m_simTimeRate;
    }

    virtual void setSimTimeRate(double s) {
        m_simTimeRate = s;
    }

    /** Accumulated wall-clock time since init was called on this applet. 
        Since this time is accumulated, it may drift from the true
        wall-clock obtained by System::time().*/
    inline RealTime realTime() const {
        return m_realTime;
    }

    virtual void setRealTime(RealTime r) {
        m_realTime = r;
    }

    /** In-simulation time since init was called on this applet.  
        Takes into account simTimeSpeed.  Automatically incremented
        after doSimulation.
      */
    inline SimTime simTime() const {
        return m_simTime;
    }

    virtual void setIdealSimTime(SimTime s) {
        m_idealSimTime = s;
    }

    /**
       Simulation time that is always advanced by precisely the
       desiredFrameDuration * simTimeRate, regardless of the 
       actual frame duration.
     */
    inline SimTime idealSimTime() const {
        return m_idealSimTime;
    }

    virtual void setSimTime(SimTime s) {
        m_simTime = s;
    }

    /** Change to invoke frame limiting via doWait.
        Defaults to inf() */
    virtual void setDesiredFrameRate(float fps) {
        debugAssert(fps > 0);
        m_desiredFrameRate = fps;
    }

    float desiredFrameRate() const {
        return m_desiredFrameRate;
    }

    RealTime desiredFrameDuration() const {
        return 1.0 / m_desiredFrameRate;
    }

    /** @param _app This is usually your own subclass of GApp.*/
    GApplet(GApp* _app);

    /**
     An empty virtual destructor for virtual methods.
    */
    virtual ~GApplet() {}

    /**
      Run until app->endProgram or endApplet is set to true. 
      Calls beginRun(), then oneFrame in a loop, then endRun().
      
      Consider instead calling app->window()->pushLoopBody(applet), which
      allows you to push and pop additional applets.
        
      For use with GWindows for which requiresMainLoop is false.
      On GWindows where requiresMainLoop is true, this will schedule
      the applet to run and return immediately.  (This odd behavior 
      is required for backwards compatibility; in version 7.00 it will
      change).
    */
    virtual void run();

    /**
      Usually you'll call GApplet::run instead.  This is used by GApp.

      Prepare for running.  The default implementation sets endApplet
      to false, calls GApplet::onInit(), copies the debug camera
      position to the debug camera controller,
    */
    virtual void beginRun();

    /** 
      Usually you'll call GApplet::run instead.  This is used by GApp.

      A single frame of rendering, simulation, AI, events, networking,
      etc.  Invokes the onXXX methods.  For use with GWindows that
      require a main loop.  This if your GWindow does not require
      control of the main loop, GApp will call this for you.
    */
    virtual void oneFrame();

    /**
      Usually you'll call GApplet::run instead.  This is used by GApp.

      Invokes cleanup().
     */
    virtual void endRun();

protected:
    

    /** Default implementation poses the managed modules from the GApplet and GApp.*/
    virtual void getPosedModel(
        Array<PosedModelRef>& posedArray, 
        Array<PosedModel2DRef>& posed2DArray);

    /**
     Set to false to break out of the run() loop.
     */
    bool                endApplet;

    /** @deprecated */
    virtual void G3D_DEPRECATED
        doSimulation(RealTime rdt) {
            (void)rdt;
    }

    /**
     Override this with your simulation code.
     Called from GApp::run.
        
     Default implementation does nothing.

     simTime(), idealSimTime() and realTime() are incremented after
     doSimulation is called, so at the beginning of call the current
     time is the end of the previous frame.

     @param rdt Elapsed real-world time since the last call to doSimulation.

     @param sdt Elapsed sim-world time since the last call to
     doSimulation, computed by multiplying the wall-clock time by the
     simulation time rate.

     @param idt Elapsed ideal sim-world time.  Use this for perfectly
     reproducible timing results.  Ideal time always advances by the
     desiredFrameDuration * simTimeRate, no matter how much wall-clock
     time has elapsed.
     */
    virtual void onSimulation(RealTime rdt, SimTime sdt, SimTime idt) {
        (void)idt;
        (void)rdt;
        doSimulation(sdt);
    }


    /**
     Override and implement.  The debugCamera's projection and object to world
     matrices are set by default; you can set other cameras as desired. 
     RenderDevice::beginFrame and endFrame are called for you.
     
	 See <A HREF="../demos/main.cpp">demos/main.cpp</A> for an example of
	 overriding lights.

     @deprecated
     */
    virtual void G3D_DEPRECATED doGraphics();

    /**
     Rendering callback.

     Override and implement.  The debugCamera's projection and object to world
     matrices are set by default; you can set other cameras as desired. 
     RenderDevice::beginFrame and endFrame are called for you.
     
     Use getPosedModel() to obtain the installed GModules to be rendered.

	 See <A HREF="../demos/main.cpp">demos/main.cpp</A> for an example of
	 overriding lights.
     */
    virtual void onGraphics(RenderDevice* rd);


    /**
       @deprecated
     */
    virtual void G3D_DEPRECATED doNetwork() {}

    /**
     For a networked app, override this to implement your
     network message polling.
     */
    virtual void onNetwork();

    /**
      Task to be used for frame rate limiting.  

      Overriding onWait is not recommended.

      Frame rate limiting is useful
      to avoid overloading a maching that is running background tasks and
      for situations where fixed time steps are needed for simulation and there
      is no reason to render faster.

      Default implementation System::sleep()s until cumulativeTime + time
      in wait is greater than or equal to @a frameDuration = 1 / desiredFrameRate.
      */
    virtual void onWait(RealTime cumulativeTime, RealTime frameDuration);

    /**
     Update any state you need to here.  This is a good place for
     AI code, for example.  Called after network and user input,
     before simulation.
     @deprecated
     */
    virtual void G3D_DEPRECATED 
        doLogic() {}

    /**
     Update any state you need to here.  This is a good place for
     AI code, for example.  Called after network and user input,
     before simulation.
     */
    virtual void onLogic() {
        doLogic();
    }

    /**
       @deprecated
     */
    virtual void G3D_DEPRECATED init() {
        m_simTime     = 0;
        m_realTime    = 0;
        m_simTimeRate = 1.0;
        lastWaitTime  = System::time();
    }

    /**
     Invoked every time run is called.  Default implementation
     resets timers and simTimeRate.

     Sublcasses should invoke GApplet::init to reset the timers.
     */
    virtual void onInit() {
        init();
    }

    /**
     Invoked at the end of every run call.  Default implementation
     does nothing.
     @deprecated
     */
    virtual void G3D_DEPRECATED cleanup() {}

    /**
     Invoked at the end of every run call.  Default implementation
     does nothing.
     */
    virtual void onCleanup() {
        cleanup();
    }
    
    /** @deprecated Use onEvent */
    virtual void G3D_DEPRECATED processEvent(const GEvent& event) {
        (void)event;
    }

    /**
     It is recommended to override doUserInput instead of this method.

     Override if you need to explicitly handle events in the order
     they appear.
     
     Note that the userInput contains a record of all
     keys pressed/held, mouse, and joystick state, so 
     you do not have to override this method to handle
     basic input events.

     Return true if the event has been consumed (i.e., no-one else including GApp should process it further).

     The default implementation does nothing.
     */
    virtual bool onEvent(const GEvent& event);

    /**
     Updates the userInput.  Called from run.  Rarely needs to be
     called by user programs.

     Never overriden by a subclass.
     Instead, override GApp::processEvent to handle your own events.

     @deprecated In 7.0, will be private and doUserInput will be a function
      you can override to handle events.
     */
    void G3D_DEPRECATED doUserInput();

    /**
     Routine for processing user input from the previous frame.
     */
    virtual void onUserInput(class UserInput* userInput) {
        (void)userInput;
    }
};

/**
 @deprecated Use GApp::Settings.  GAppSettings will be removed in 7.0 
 */
typedef GApp::Settings GAppSettings;

}

#endif
