/**
  @file ManualCameraController.h

  @maintainer Morgan McGuire, morgan@cs.brown.edu

  @created 2002-07-28
  @edited  2006-02-28
*/

#ifndef G3D_ManualCameraController_H
#define G3D_ManualCameraController_H

#include "G3D/platform.h"
#include "G3D/Vector3.h"
#include "G3D/CoordinateFrame.h"
#include "GLG3D/GModule.h"

namespace G3D {

class CoordinateFrame;

typedef ReferenceCountedPointer<class FirstPersonManipulator> FirstPersonManipulatorRef;

/**
 Uses a First Person (Quake- or World of Warcraft style) mapping to translate keyboard and mouse input
 into a flying camera position.  The result is an Euler-angle 
 camera controller suitable for games and fly-throughs.  

  To use without G3D::GApp:
  <OL>
    <LI> Create a G3D::RenderDevice
    <LI> Create a UserInput object (set the keyboard controls when creating it)
    <LI> Create a ManualCameraController
    <LI> Call ManualCameraController::setActive(true)
    <LI> Invoke ManualCameraController::doSimulation every time simulation is invoked (e.g. once per rendering iteration)
    <LI> Use ManualCameraController::getCoordinateFrame() to set the camera's position
  </OL>

 */
class FirstPersonManipulator : public Manipulator {
public:
    /**
     MOUSE_DIRECT              = Shooter/Quake style (default), mouse cursor is hidden and mouse controls yaw/pitch
     MOUSE_DIRECT_RIGHT_BUTTON = RPG/World of Warcraft style, on right mouse button cursor is hidden and mouse controls yaw/pitch
     MOUSE_SCROLL_AT_EDGE      = Leaves mouse cursor visible and rotates while mouse is near the window edge
     MOUSE_PUSH_AT_EDGE        = Leaves mouse cursor visible and rotates when the mouse is actively pushing against the window edge
     */
    enum MouseMode {
        MOUSE_DIRECT,
        MOUSE_DIRECT_RIGHT_BUTTON,
        MOUSE_SCROLL_AT_EDGE,
        MOUSE_PUSH_AT_EDGE
    };

private:

	/** m/s */
	double                      maxMoveRate;

	/** rad/s */
	double                      maxTurnRate;

	double                      yaw;
    double                      pitch;
	Vector3                     translation;

    bool                        _active;

    class UserInput*            userInput;

    MouseMode                   m_mouseMode;

public:

    /** @deprecated Use create. */
	G3D_DEPRECATED FirstPersonManipulator();

    /** Creates and initializes.
        @deprecated Use create.
      */
	G3D_DEPRECATED FirstPersonManipulator(class RenderDevice*, class UserInput*);
    
    static FirstPersonManipulatorRef create();
        

    /** You need to call setActive(true) before the controller will work. 
        @deprecated Not needed anymore
      */
    void G3D_DEPRECATED init(class RenderDevice* device, class UserInput* input);

    /** Deactivates the controller */
    virtual ~FirstPersonManipulator();

    /** When active, the FirstPersonManipulator takes over the mouse.  It turns
        off the mouse cursor and switches to first person controller style.
        Use this to toggle between your menu system and first person camera control.

        When deactivated, the mouse cursor is restored and the mouse is located
        where it was when the camera controller was activated.

        In release mode, the cursor movement is restricted to the window
        while the controller is active.  This does not occur in debug mode because
        you might hit a breakpoint while the controller is active and it
        would be annoying to not be able to move the mouse.*/
    void setActive(bool a);

    MouseMode mouseMode() const;

    void setMouseMode(MouseMode m);

    bool active() const;

    /** Initial value is 10 */
    void setMoveRate(double metersPerSecond);

    /** Initial value is PI / 2 */
    void setTurnRate(double radiansPerSecond);

    /** Invoke immediately before entering the main game loop. */
    void reset();

	/**
	 Increments the ManualCameraController's orientation and position.
     Invoke once per simulation step.
     @deprecated
	 */
	void G3D_DEPRECATED doSimulation(
        double                  elapsedTime);

	void setPosition(const Vector3& t) {
		translation = t;
	}

    void lookAt(const Vector3& position);

    double getYaw() const {
        return yaw;
    }

    double getPitch() const {
        return pitch;
    }

	const Vector3& getPosition() const {
		return translation;
	}

	Vector3 getLookVector() const {
		return getCoordinateFrame().getLookVector();
	}

    /** Right vector */
	Vector3 getStrafeVector() const {
		return getCoordinateFrame().getRightVector();
	}

    /** @deprecated Use frame */
	CoordinateFrame G3D_DEPRECATED getCoordinateFrame() const;

    /** @deprecated Use getFrame.
      */
	void G3D_DEPRECATED getCoordinateFrame(CoordinateFrame& c) const;


    /** @deprecated Use setFrame */
    void G3D_DEPRECATED setCoordinateFrame(const CoordinateFrame& c);

    /**
      Sets to the closest legal controller orientation to the coordinate frame.
     */
    void setFrame(const CoordinateFrame& c);

    // Inherited from Manipulator
	virtual void getFrame(CoordinateFrame& c) const;
    virtual CoordinateFrame frame() const;

    // Inherited from GModule
    virtual void getPosedModel(Array<PosedModelRef>& p3d, Array<PosedModel2DRef>& p2d);
    virtual void onNetwork();
    virtual void onLogic();
    virtual void onSimulation(RealTime rdt, SimTime sdt, SimTime idt);
    virtual void onUserInput(UserInput* ui);
    virtual bool onEvent(const GEvent& event);
};

/** Use FirstPersonManipulator instead 
    @deprecated */
typedef FirstPersonManipulator FPCameraController;

/** Use FirstPersonManipulator instead 
    @deprecated */
typedef FirstPersonManipulator ManualCameraController;

}
#endif
