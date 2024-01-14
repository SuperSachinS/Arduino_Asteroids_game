#include "asteroids.h"
#include <math.h>;
#define PI 3.14159265
Asteroids::Asteroids(){
	position_x = -1000;
	position_y = -1000;
	velocity_x = 0;
	velocity_y = 0;
	tier = 2;
	length = tier * 8 + 4;
	alive = false;
}

float Asteroids::get_asteroid_x() const{
	return position_x;
}
float Asteroids::get_asteroid_y() const{
	return position_y;
}
float Asteroids::get_asteroid_oldx() const{
	return old_x;
}
float Asteroids::get_asteroid_oldy() const{
	return old_y;
}
int Asteroids::get_asteroid_length() const{
	return length;
}
int Asteroids::get_asteroid_tier() const{
	return tier;
}
bool Asteroids::is_alive() const{
	return alive;
}
void Asteroids::spawn(int size, float new_velocity_x, float new_velocity_y, float new_position_x, float new_position_y, bool no_zero = true){
	if (no_zero){
		if((-0.25 < new_velocity_x && new_velocity_x < 0.25) && (-0.25 < new_velocity_y && new_velocity_y < 0.25)){
			new_velocity_x += 1;
			new_velocity_y += 1;
		}
	}
	tier = size;
	length = tier * 8 + 4;
	position_x = new_position_x;
	position_y = new_position_y;
	velocity_x = new_velocity_x;
	velocity_y = new_velocity_y;
	alive = true;
}
void Asteroids::split(float new_velocity_x, float new_velocity_y){
	if((-0.25 < new_velocity_x && new_velocity_x < 0.25) && (-0.25 < new_velocity_y && new_velocity_y < 0.25)){
		new_velocity_x += 1;
		new_velocity_y += 1;
	}
	
	tier--;
	length = tier * 8 + 4;
	velocity_x = new_velocity_x;
	velocity_y = new_velocity_y;	
	
}
void Asteroids::die(){
	position_x = -1000;
	position_y = -1000;
	velocity_x = 0;
	velocity_y = 0;
	tier = 0;
	length = tier * 8 + 4;
	alive = false;
}
void Asteroids::move(){
	old_x = position_x;
	old_y = position_y;
	position_x += velocity_x;
	position_y += velocity_y;
	
	if(position_x > 480){
		position_x -= 480;
	}else if(position_x + length < 0){
		position_x += 480 + length;
	}
	if(position_y > 320){
		position_y -= 320;
	}else if(position_y + length < 0){
		position_y += 320 + length;
	}
}
void Asteroids::reset(){
	position_x = 300;
	position_y = 200;
}
bool Asteroids::check_collision(int bullet_x, int bullet_y){
	if (bullet_x > position_x - 3 && bullet_x < position_x + length + 3){
		if (bullet_y > position_y - 3 && bullet_y < position_y + length + 3){
			return true;
		}
	}
	return false;
}


/*

-----------------------------------------------------------

*/
Player::Player(){
	
	origin[0] = 240;
	origin[1] = 160;
	velocity_x = 0;
	velocity_y = 0;
	velocity = 0;
	orientation_x = 0;
	orientation_y = 90;
	direction[0] = 0;
	hitbox = 8;
	colour = 0;
	
	position[0] = origin[0] + 10 * cos(direction[0]);
	position[1] = origin[1] + 10 * sin(direction[0]);
	
	position[4] = origin[0] + 10 * cos(direction[0] + (140 * PI / 180));
	position[5] = origin[1] + 10 * sin(direction[0] + (140 * PI / 180));
	
	position[8] = origin[0] + 10 * cos(direction[0] - (140 * PI / 180));
	position[9] = origin[1] + 10 * sin(direction[0] - (140 * PI / 180));
	
	
}

float Player::get_player_position(int index) const{
	return position[index];
}

float Player::get_player_origin(int index) const{
	return origin[index];
}

float Player::get_player_vx() const{
	return velocity_x;
}
float Player::get_player_vy() const{
	return velocity_y;
}
float Player::get_player_v() const{
	return velocity;
}
int Player::get_player_colour() const{
	return colour;
}
float Player::get_player_direction(int index) const{
	return direction[index];
}

void Player::set_player_colour(int c){
	colour = c;
}
void Player::move(float joystick_x,float joystick_y){
	velocity_x += joystick_x;
	velocity_y += joystick_y;
	
	direction[1] = direction[0];
	
	direction[0] = atan(joystick_y / joystick_x);
	if ((joystick_x < 0 && joystick_y > 0) || (joystick_x < 0 && joystick_y < 0)){
		direction[0] += PI;
	}
	
	//find magnitude and direction
	velocity = sqrt(pow(velocity_x, 2) + pow(velocity_y, 2));
	if (velocity_x == 0)
		velocity_direction = PI / 2;
	else
		velocity_direction = atan(velocity_y / velocity_x);
	update_model();
	
}

void Player::drift(){	
	update_model();
}

void Player::turn(float joystick_x, float joystick_y){
	
	direction[1] = direction[0];
	
	direction[0] = atan(joystick_y / joystick_x);
	if ((joystick_x < 0 && joystick_y > 0) || (joystick_x < 0 && joystick_y < 0)){
		direction[0] += PI;
	}

	update_model();
}

void Player::reset_player(int x = 240, int y = 160){
	
	origin[0] = x;
	origin[1] = y;
	velocity_x = 0;
	velocity_y = 0;
	velocity = 0;
	orientation_x = 0;
	orientation_y = 90;
	direction[0] = 0;
	
	position[0] = origin[0] + 10 * cos(direction[0]);
	position[1] = origin[1] + 10 * sin(direction[0]);
	
	position[4] = origin[0] + 10 * cos(direction[0] + (140 * PI / 180));
	position[5] = origin[1] + 10 * sin(direction[0] + (140 * PI / 180));
	
	position[8] = origin[0] + 10 * cos(direction[0] - (140 * PI / 180));
	position[9] = origin[1] + 10 * sin(direction[0] - (140 * PI / 180));
	
}

bool Player::check_collision(int asteroid_x, int asteroid_y, int asteroid_length){
	//hitbox - square - 8x8
	if (origin[0] > asteroid_x - hitbox && origin[0] < asteroid_x + asteroid_length + hitbox){
		if (origin[1] > asteroid_y - hitbox && origin[1] < asteroid_y + asteroid_length + hitbox){
			return true;
		}
	}
	return false;
}

void Player::update_model(){
	
	//head
	position[2] = position[0];
	position[3] = position[1];
	//left
	position[6] = position[4];
	position[7] = position[5];
	//right
	position[10] = position[8];
	position[11] = position[9];
	
	origin[2] = origin[0];
	origin[3] = origin[1];
	direction[1] = direction[0]; 
	
	
	
	velocity = sqrt(pow(velocity_x, 2) + pow(velocity_y, 2));

	//apply limitation parameters
	velocity -= air_resistance;
	velocity = constrain(velocity, 0, max_speed);
	

	if (velocity_x > 0 && velocity_y > 0){
		velocity_x = velocity * cos(velocity_direction);
		velocity_y = velocity * sin(velocity_direction);
	}else if (velocity_x < 0 && velocity_y > 0){
		velocity_x = -velocity * cos(velocity_direction);
		velocity_y = -velocity * sin(velocity_direction);		
	}else if (velocity_x > 0 && velocity_y < 0){
		velocity_x = velocity * cos(velocity_direction);
		velocity_y = velocity * sin(velocity_direction);
	}else{
		velocity_x = -velocity * cos(velocity_direction);
		velocity_y = -velocity * sin(velocity_direction);
	}

	origin[0] += velocity_x;
	origin[1] += velocity_y;
	
	if (origin[0] > 480){
		origin[0] -= 480;
	}else if (origin[0] < 0){
		origin[0] += 480;
	}else if (origin[1] > 320){
		origin[1] -= 320;
	}else if (origin[1] < 0){
		origin[1] += 320;
	}
	
	position[0] = origin[0] + 10 * cos(direction[0]);
	position[1] = origin[1] + 10 * sin(direction[0]);
	
	position[4] = origin[0] + 10 * cos(direction[0] + (140 * PI / 180));
	position[5] = origin[1] + 10 * sin(direction[0] + (140 * PI / 180));
	
	position[8] = origin[0] + 10 * cos(direction[0] - (140 * PI / 180));
	position[9] = origin[1] + 10 * sin(direction[0] - (140 * PI / 180));
	
}


/*

----------------------------------------------

*/
Bullet::Bullet(){
	velocity_x = 0;
	velocity_y = 0;
	old_x = 1000;
	old_y = 1000;
	position_x = 1000;
	position_y = 1000;
	alive = false;
}

void Bullet::respawn(float player_direction, float player_x, float player_y){
	velocity_x = 6 * cos(player_direction);
	velocity_y = 6 * sin(player_direction);
	bullet_lifespan = 0;
	position_x = player_x;
	position_y = player_y;
	alive = true;
}
void Bullet::move(){
	old_x = position_x;
	old_y = position_y;
	position_x += velocity_x;
	position_y += velocity_y;
	
	if(position_x > 480){
		position_x -= 480;
	}else if(position_x < 0){
		position_x += 480;
	}
	if(position_y > 320){
		position_y -= 320;
	}else if(position_y < 0){
		position_y += 320;
	}
	
	
	
	bullet_lifespan++;
}

void Bullet::die(){
	velocity_x = 0;
	velocity_y = 0;
	position_x = 1000;
	position_y = 1000;
	alive = false;
}

float Bullet::get_bullet_x() const{
	return position_x;
}
float Bullet::get_bullet_y() const{
	return position_y;
}
float Bullet::get_bullet_oldx() const{
	return old_x;
}
float Bullet::get_bullet_oldy() const{
	return old_y;
}
int Bullet::get_bullet_lifespan() const{
	return bullet_lifespan;	
}
bool Bullet::is_alive() const{
	return alive;
}

/*

----------------------------------------------

*/

Explosion::Explosion(){
	position_x = 0;
	position_y = 0;
	lifespan = 0;
	size = 0;
	colour = 2;
	alive = false;
}
int Explosion::get_x() const{
	return position_x;
}
int Explosion::get_y() const{
	return position_y;
}
int Explosion::get_size() const{
	return size;
}
int Explosion::get_colour() const{
	return colour;
}
bool Explosion::is_alive() const{
	return alive;
}

Explosion::start_explosion(int x, int y, int tier, int c){
	lifespan = 0;
	size = 2 + tier;
	life_limit = 3 + tier;
	colour = c;
	alive = true;
	position_x = x + tier * 5;
	position_y = y + tier * 5;
}
Explosion::explode(){
	lifespan++;
	size++;
	if (lifespan > life_limit)
		stop_explosion();
	
}
Explosion::stop_explosion(){
	position_x = 0;
	position_y = 0;
	lifespan = 0;
	size = 0;
	alive = false;
}

/*

----------------------------------------------

*/
Powerup::Powerup(){
	position_x = 1000;
	position_y = 1000;
	size = 20;	
	alive = false;
}

int Powerup::get_x() const{
	return position_x;
}
int Powerup::get_y() const{
	return position_y;
}
int Powerup::get_size() const{
	return size;
}
int Powerup::get_lifespan() const{
	return lifespan;
}
bool Powerup::is_alive() const{
	return alive;
}

void Powerup::spawn(int x, int y){
	position_x = x;
	position_y = y;
	alive = true;
}

void Powerup::decay(){
	lifespan--;
}

void Powerup::despawn(){
	position_x = 1000;
	position_y = 1000;
	lifespan = 0;
	alive = false;
}

void Powerup::collect(){
	despawn();
	lifespan = 500;
}