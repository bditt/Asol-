#include "c_movement.h"
#include "c_entity.h"

void c_movement::handle( void ) {
	/* todo: make this shit have keybind handling */
	//speed( ctx.m_player );
	//flight( ctx.m_player );
	sprint(ctx.m_player);

}

void c_movement::sprint(c_entity* entity)
{
	if (!entity->is_sprinting())
	{
		entity->set_sprint(true);
	}
}

void c_movement::speed(c_entity* e)
{
	if (e->is_alive() && e->is_valid())
	{
		float currentx = e->get_motionx();
		float newx = currentx + 1.0;
		float currentz = e->get_motionz();
		float newz = currentz + 1.0;

		e->set_motionx(newx);
		e->set_motionz(newz);
	}
}

void c_movement::step( c_entity* entity ) {
	entity->set_step_height( 0.5f );
}

void c_movement::flight( c_entity* entity ) {
	entity->set_flight( true );
}