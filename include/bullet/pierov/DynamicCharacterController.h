#ifndef DYNAMIC_CHARACTER_CONTROLLER_H
#define DYNAMIC_CHARACTER_CONTROLLER_H

#include <BulletDynamics/Dynamics/btActionInterface.h>
class btCapsuleShape;
class btRigidBody;

/**
 * Character controller for a rigid body with locked rotations.
 *
 * This class is hardcoded for Z-up characters.
 */
class DynamicCharacterController : public btActionInterface
{
public:
	/**
	 * Setup the controller.
	 * \param body The rigid body that will be controlled. It needs to exist
	 * as long as the controller is active, and it will be modified.
	 */
	DynamicCharacterController(btRigidBody *body, const btCapsuleShape *shape);
	virtual ~DynamicCharacterController() = default;

	void updateAction(btCollisionWorld *collisionWorld, btScalar deltaTimeStep);
	void debugDraw(btIDebugDraw *debugDrawer);

	/**
	 * Set the movement direction
	 * \param walkDirection The direction. Its magnitude will be ignored, as
	 * well as the z direction
	 */
	void setMovementDirection(const btVector3 &walkDirection);

	/**
	 * Get the movement direction in local coordinates
	 * \return The direction
	 */
	const btVector3 &getMovementDirection() const;

	/// Reset the movement status, but not the position.
	void resetStatus();

	/// Tell if the character can jump
	bool canJump() const;

	/**
	 * Set to jump in the next update
	 * \param dir The jump direction in local coordinates.
	 * If zero, up (Z) will be used
	 */
	void jump(const btVector3 &dir = btVector3(0, 0, 0));

	/**
	 * Get the controlled rigid body e.g. to perform collision tests.
	 * \return the controlled rigid body
	 */
	const btRigidBody *getBody() const;

	/**
	 * The squared maximum speed that the character can walk.
	 * By default, 15km/h.
	 */
	btScalar mMaxLinearVelocity2 = pow(15 / 3.6, 2);

	/// The acceleration to use when starting walking
	btScalar mWalkAccel = 25.0;

	/**
	 * The initial speed for the jump.
	 * The default is about 1m starting still, with g=-9.81
	 */
	btScalar mJumpSpeed = 4.5;

	/// The damp to use to stop movements
	btScalar mSpeedDamping = 0.1;

	/// The maximum step to automatically climb to
	btScalar mMaxStepHeight = 0.5;

	/// The speed to step stairs
	btScalar mSteppingSpeed = 3;

	/// The radius of the capsule
	btScalar mShapeRadius;

	/// Half of the height of the capsule
	btScalar mShapeHalfHeight;

	/**
	 * On ramps, if already on ground, the character tends to slowly go down.
	 * btRigidBody::clearGravity is not enough to fix this, but setting no
	 * gravity sorta works, and then when the raycast tells that the character
	 * is not on ground anymore, adding again the gravity works.
	 *
	 * By default this value is populated when setting up the rigid body, but it
	 * can be changed, if necessary, to achieve peculiar effects.
	 */
	btVector3 mGravity;

	/**
	 * Ground detection works with collision checking.
	 * We take all the collisions of the rigid body and we convert them to
	 * spherical coordinates (with theta = 0 as up vector, like ISO standard).
	 *
	 * We check for the radius and the theta angle of that point, or, to be more
	 * specific, at its cosine. The character is considered on ground if these
	 * values are within a certain threshold.
	 * For the angle, by default, it must be between 0.75 pi and pi.
	 *
	 * This cosine is expected to be negative.
	 */
	btScalar mMaxCosGround = -SIMDSQRT12;

	/**
	 * The threshold on the radius, to tell if a character is on ground.
	 * By default, 1cm (if using 1unit as 1m).
	 * \sa mMaxCosGround
	 */
	btScalar mRadiusThreshold = 1e-2;

protected:

	/**
	 * Default constructor, to be used by child classes.
	 * \warning The child classes must set the rigid body when using this, and
	 * set the shape dimensions.
	 */
	DynamicCharacterController();

	/// Setup the internally pointed body
	void setupBody();

	/**
	 * Update the character velocity
	 * \param dt The time elapsed since the last step
	 */
	void updateVelocity(float dt);

	/**
	 * Make the character step up stairs
	 * \param dt The time elapsed since the last step
	 */
	void stepUp(float dt);

	/// Cancel auto stepping
	inline void cancelStep();

	/// The controlled rigid body
	btRigidBody *mRigidBody;

	/**
	 * The walking direction. It must be an unitary vector or 0.
	 * It can be set through setWalkDirection, that will normalize, or child
	 * classes can set it directly, as long as they keep it normalized.
	 */
	btVector3 mMoveDirection;

	/// Tell whether to jump at next update
	bool mJump;

	/// Tell the jump direction (should be normalized)
	btVector3 mJumpDir;

	/// Tell whether the character is on the ground
	bool mOnGround = false;

	/**
	 * The contact point of the character to ground
	 * This vector is valid only when mOnGround is true.
	 */
	btVector3 mGroundPoint;

	/// Tells whether the character is auto stepping
	bool mStepping;

	/// Tells the point the character is stepping to
	btVector3 mSteppingTo;

	/// The normal of the step we are stepping to, seen from the stepping point
	btVector3 mSteppingInvNormal;

	/// Tells the flag to restore to make the character dynamic again after step
	int mPrestepFlags;
};

#endif
