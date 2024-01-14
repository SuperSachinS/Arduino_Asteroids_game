#ifndef asteroids_h
#define asteroids_h
class Asteroids {
public:
	Asteroids();

	float get_asteroid_x() const;
	float get_asteroid_y() const;
	float get_asteroid_oldx() const;
	float get_asteroid_oldy() const;
	int get_asteroid_length() const;
	int get_asteroid_tier() const;
	bool is_alive() const;
	
	void spawn(int size, float new_velocity_x, float new_velocity_y, float new_position_x, float new_position_y, bool no_zero = true);
	void split(float new_velocity_x, float new_velocity_y);
	void die();
	void move();
	void reset();
	bool check_collision(int bullet_x, int bullet_y);
	
private:
	float position_x, position_y, old_x, old_y;
	int length, tier;
	float velocity_x, velocity_y;
	bool alive;

};

class Player {
public:
	Player();

	float get_player_position(int index) const;
	float get_player_origin(int index) const;
	
	float get_player_vx() const;
	float get_player_vy() const;
	float get_player_v() const;
	int get_player_colour() const;
	float get_player_direction(int index) const;
	
	void set_player_colour(int c);
	void move(float joystick_x,float joystick_y);
	void drift();
	void turn(float joystick_x, float joystick_y);
	void reset_player(int x = 240, int y = 160);
	bool check_collision(int asteroid_x, int asteroid_y, int asteroid_length);
	
private:
	void update_model();
	int colour;
	float position[12];
	/*
	head position (0-3): x, y, oldx, oldy
	left position (4-7): x, y, oldx, oldy
	right position (8-11): x, y, oldx, oldy
	triangle dimensions
	
	*/
	float origin[4];
	// 0 = x position, 1 = y position, 2 = oldx, 3 = oldy
	float velocity_x, velocity_y, velocity, velocity_direction;
	float orientation_x, orientation_y, direction[2]; // 0 = direction, 1 = old direction
	float air_resistance = 0.005, max_speed = 5, hitbox;
	
};

class Bullet {
public:
	Bullet();
	
	void respawn(float player_direction, float player_x, float player_y);
	void move();
	void die();
	float get_bullet_x() const;
	float get_bullet_y() const;
	float get_bullet_oldx() const;
	float get_bullet_oldy() const;
	int get_bullet_lifespan() const;
	bool is_alive() const;
	
		
private:
	float position_x, position_y, old_x, old_y;
	float velocity_x, velocity_y;
	int bullet_lifespan;
	bool alive;
	
};

class Explosion {
public:
	Explosion();
	int get_x() const;
	int get_y() const;
	int get_size() const;
	int get_colour() const;
	bool is_alive() const;
	start_explosion(int x, int y, int tier, int c);
	stop_explosion();
	explode();

private:
	int position_x, position_y, lifespan, size, life_limit, colour;
	bool alive;

	
	
};

class Powerup{
public:
	Powerup();
	int get_x() const;
	int get_y() const;
	int get_size() const;
	int get_lifespan() const;
	bool is_alive()const;
	
	void spawn(int x, int y);
	void decay();
	void despawn();
	void collect();
private:
	int position_x, position_y, size, lifespan;
	bool alive;
	
};
#endif