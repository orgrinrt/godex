#pragma once

#include "../../components/component.h"
#include "../../storage/steady_storage.h"
#include <BulletDynamics/Dynamics/btRigidBody.h>
#include <btBulletCollisionCommon.h>

/// Structure that contains the shape information describing the stance.
struct PawnShape {
private:
	real_t pawn_height = 1.8;
	real_t pawn_radius = 0.3;
	real_t margin = 0.04;

public:
	/// Main shape.
	btCapsuleShape main_shape = btCapsuleShape(0.3, 1.5);
	/// Little enflated shape used for unstuck.
	btCapsuleShape margin_shape = btCapsuleShape(0.3, 1.5);

	PawnShape();

	void set_pawn_height(real_t p_pawn_height);
	real_t get_pawn_height() const;

	void set_pawn_radius(real_t p_pawn_radius);
	real_t get_pawn_radius() const;

	void update_shapes_dimention();
};

enum Stance : int {
	STANCE_STANDING,
	STANCE_CROUCHING,
};

/// The Pawn is a component that allow a Kienamtic Body to walk around:
/// up and down slopes, move up and down stairs.
///
/// # How to use it
/// To make the body walk, you just need to set the velocity, from a system
/// that does it according the Player inputs.
///
/// # Forces & Impulese
/// Forces and Impulses are also taken into account.
///
/// # Reference plane
/// It's possible to change the walking algorithm reference plane. This feature
/// is a lot useful when you want that your Pawn walks all around a sphere.
struct BtPawn {
	COMPONENT_CUSTOM_CONSTRUCTOR(BtPawn, SteadyStorage)

	static void _bind_methods();
	static void _get_storage_config(Dictionary &r_config);

	/// The pawn stances describes the height and radius the Pawn has depending
	/// on its current stance.
	/// The pawn can have up to 2 stances: Standing, Crouching.
	PawnShape stances[2];

	btRigidBody body = btRigidBody(0.0, nullptr, &stances[0].main_shape);

	Stance current_stance = STANCE_STANDING;

	/// Current pawn linear velocity and direction.
	Vector3 velocity;

	/// The step height the Pawn will be able to step up.
	real_t step_height = 0.2;

	/// The ground direction the Pawn will be computed.
	Basis ground_direction;

	/// Control if the Pawn should snap to the ground. Set this to false, when
	/// your pawn is falling or jumping, so to have a more natural motion.
	bool snap_to_ground = true;

	bool need_body_reload = false;

private:
	uint32_t layer = 1;
	uint32_t mask = 1;

public:
	BtPawn();

	void set_layer(uint32_t p_layer);
	uint32_t get_layer() const;

	void set_mask(uint32_t p_mask);
	uint32_t get_mask() const;
};
