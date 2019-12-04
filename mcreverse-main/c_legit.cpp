#include "c_legit.h"
#include "c_gameinstance.h"
#define _USE_MATH_DEFINES
#include <math.h>

class MathHelper
{
public:
	static double radtodeg(double x) {
		return x * 180.0 / M_PI;
	}

	static double degtorad(double x) {
		return x * M_PI / 180.0;
	}

	static double distance(double x, double y) {
		return sqrt(pow(x, 2) + pow(y, 2));
	}

	static double distance(double x1, double y1, double z1, double x2, double y2, double z2) {
		return distance(y1 - y2, distance(x1 - x2, z1 - z2));
	}

	static double direction(double x1, double y1, double x2, double y2) {
		return radtodeg(atan2(y2 - y1, x2 - x1));
	}

	static double* direction(double x1, double y1, double z1, double x2, double y2, double z2) {
		double dx = x2 - x1;
		double dy = y2 - y1;
		double dz = z2 - z1;
		double yaw = radtodeg(atan2(dz, dx)) - 90;
		double pitch = -radtodeg(atan2(dy, distance(dx, dz)));
		return new double[2]{ yaw, pitch };
	}
};


#define CONST_RadToUnrRot                                        10430.3783504704527


void c_legit::handle( void ) {
	// lambda function to recognise window titles.
	auto get_window_title = [ & ] ( void ) {
		// buffer
		char title [ 256 ];

		// get current window handle
		HWND m_hwnd = GetForegroundWindow( );
		GetWindowTextA( m_hwnd, title, sizeof( title ) );

		std::string m_title = title;
		return m_title;
	};

	/* todo: make this shit have keybind handling */

	/* functions that need you to be in minecraft window */
	if ( get_window_title( ).find("Minecraft") != std::string::npos && !minecraft->game->get_screen( ) ) {
		auto_clicker( );
		aimbot();
	}
	
	//handle_players( );

}

void c_legit::handle_players( ) {
	for ( int i = 0; i < ctx.m_world->get_entities( ); i++ ) {
		c_entity* e = ctx.m_world->get_entity( i );

		if ( !e->is_valid( ) || e->is_item( ) || e->index( ) == ctx.m_player->index( ) || !e->is_alive( ) )
			continue;

		misplace( e );
	}
}

std::vector<std::string> EntityBlacklist = { "Experience Orb", "Arrow", "unknown", "entity.ThrownEnderpearl.name" };
std::vector<std::string> EntityHostiles = { "" };
std::vector<std::string> EntityWhitelist = { "Skeleton", "Zombie", "Zombie Pigman", "Pig", "Cow", "Chicken", "Sheep", "Witch", "Enderman", "Slime", "Spider", "Creeper", "Silverfish", "Blaze", "Magma Cube", "Villager" };

void c_legit::aimbot()
{
	if (GetAsyncKeyState(VK_XBUTTON1))
	{
		c_entity* closest = NULL;
		double dist = 10;
		for (int i = 0; i < ctx.m_world->get_entities(); i++) {
			bool IsWhitelisted = true;
			c_entity* e = ctx.m_world->get_entity(i);

			if (!e->is_valid() || e->is_item() || e->index() == ctx.m_player->index() || !e->is_alive())
				continue;

			for (auto Name : EntityBlacklist)
			{
				if (e->name() == Name)
				{
					IsWhitelisted = false;
					break;
				}
			}

			if (IsWhitelisted)
			{
				double newDist = MathHelper::distance(e->position().x, e->position().y, e->position().z, ctx.m_player->position().x, ctx.m_player->position().y, ctx.m_player->position().z);
				if (newDist < dist)
				{
					dist = newDist;
					closest = e;
				}
			}
		}
		if (closest != NULL)
		{
			std::cout << "Closest Entity: " << closest->name() << std::endl;
			double* rotation = MathHelper::direction(ctx.m_player->position().x, ctx.m_player->position().y, ctx.m_player->position().z, closest->position().x, closest->position().y, closest->position().z);
			ctx.m_player->set_yaw(rotation[0]);
			ctx.m_player->set_pitch(rotation[1]);
		}
	}
	//Sleep(1000 / 60);
}

void c_legit::auto_clicker( ) {
	if (GetAsyncKeyState(VK_XBUTTON2))
	{
		srand(time(0));
		auto click_left = []() {
			INPUT Input = { 0 };

			// left down
			Input.type = INPUT_MOUSE;
			Input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
			SendInput(1, &Input, sizeof(INPUT));

			// left up
			RtlSecureZeroMemory(&Input, sizeof(INPUT));
			Input.type = INPUT_MOUSE;
			Input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
			SendInput(1, &Input, sizeof(INPUT));
		};

		auto timer = []() {
			static double start_time = GetTickCount64();
			double current_time = GetTickCount64() - start_time;

			int cps = rand() % 20 + 5;

			if (current_time >= (1000 / cps)) {
				start_time = GetTickCount64();
				return true;
			}
			return false;
		};

		if (timer()) {
			click_left();
		}
	}
}

void c_legit::misplace( c_entity* e ) {

}