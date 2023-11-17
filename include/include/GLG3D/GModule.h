/**
 @file GModule.h

 @maintainer Morgan McGuire, morgan3d@users.sourceforge.net


 @created 2006-04-22
 @edited  2006-04-22
*/

#ifndef GLG3D_GMODULE_H
#define GLG3D_GMODULE_H

#include "G3D/platform.h"
#include "G3D/Array.h"
#include "G3D/ReferenceCount.h"
#include "GLG3D/PosedModel.h"
#include "GLG3D/GWindow.h"

namespace G3D {

class RenderDevice;
class UserInput;

typedef ReferenceCountedPointer<class GModule> GModuleRef;

/**
 Interface for 2D or 3D objects that experience standard
 virtual world events and are rendered.

 GModule is an interface for "widget"-like objects.  You could think of it as a bare-bones scene graph.

 Modules are objects like the FPController, GConsole, and debug text overlay that need to receive almost the same set of events (onXXX methods) as a GApplet and that you would like to be called from the corresponding methods of a GApplet.  They are a way to break large pieces of functionality for UI and debugging off so that they can be mixed and matched.


 @beta
 */
class GModule : public ReferenceCountedObject {
public:

    /** 
     Appends a posed model for this object to the array, if it has a graphic representation.
     The posed model appended is allowed to reference the agent and is allowed to mutate
     if the agent is mutated. 
     */
    virtual void getPosedModel(
        Array<PosedModelRef>& posedArray,
        Array<PosedModel2DRef>& posed2DArray) = 0;

    virtual void onSimulation(RealTime rdt, SimTime sdt, SimTime idt) = 0;

    /** Returning true consumes the event and prevents other GModules from seeing it. */
    virtual bool onEvent(const GEvent& event) = 0;

    virtual void onUserInput(UserInput* ui) = 0;

    virtual void onNetwork() = 0;

    virtual void onLogic() = 0;
};

typedef ReferenceCountedPointer<class GModuleManager> GModuleManagerRef;

/**
 Manages a group of GModules.  This is used internally by GApp and GApplet
 to process their modules.  It also enables use of GModules without
 the GApp infrastructure.

 You can use GModules without this class.
 */
class GModuleManager : public GModule {
public:

    enum EventPriority {LOW_PRIORITY, NORMAL_PRIORITY, HIGH_PRIORITY, NUM_PRIORITY};

private:
    
    /** Module index is the priority */
    Array<GModuleRef>   m_moduleArray[NUM_PRIORITY];

    class Add {
    public:
        GModuleRef      module;
        EventPriority   priority;
        Add() {}
        Add(const GModuleRef& m, EventPriority p) : module(m), priority(p) {}
    };

    Array<Add>          m_addList;

    Array<GModuleRef>   m_removeList;

    /** Number of elements. */
    int                 m_size;

    bool                m_locked;

    /** If true, when the lock is lifted all objects should be removed. */
    bool                m_removeAll;

    GModuleManager();

public:

    static GModuleManagerRef create();

    /** 
      Between beginLock and endLock, add and remove operations are delayed so that 
      iteration is safe.  Locks may not be executed recursively; only one level of
      locking is allowed.
      */
    void beginLock();

    void endLock();

    /** 
        If a lock is in effect, the add may be delayed until the unlock.

        Priorities should generally not be used; they are largely for supporting
        debugging components at HIGH_PRIORITY that intercept events before they
        can hit the regular infrastructure.
      */
    void add(const GModuleRef& m, EventPriority p = NORMAL_PRIORITY);

    /**
       If a lock is in effect the remove will be delayed until the unlock.
     */
    void remove(const GModuleRef& m);

    /**
     Removes all.
     */
    void clear();

    int size() const;

    /** Runs the event handles of each manager interlaced, as if all the modules from a were in b.*/
    static bool onEvent(const GEvent& event, GModuleManagerRef& a, GModuleManagerRef& b);

    const GModuleRef& operator[](int i) const;

    /** Calls getPosedModel on all children.*/
    virtual void getPosedModel(
        Array<PosedModelRef>& posedArray, 
        Array<PosedModel2DRef>& posed2DArray);

    virtual void onSimulation(RealTime rdt, SimTime sdt, SimTime idt);

    virtual bool onEvent(const GEvent& event);

    virtual void onUserInput(UserInput* ui);

    virtual void onNetwork();

    virtual void onLogic();
};


/**
 Exports a coordinate frame, typically in response to user input.
 Examples:
 G3D::ThirdPersonManipulator,
 G3D::FirstPersonManipulator
 */
class Manipulator : public GModule {
public:

    virtual void getFrame(CoordinateFrame& c) const = 0;

    virtual CoordinateFrame frame() const = 0;
};


} // G3D

#endif
