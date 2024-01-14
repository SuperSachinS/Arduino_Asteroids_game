//red, black, green, white, yellow, blue, grey, orange, purple

#include <TFT_eSPI.h>
#include "data/asteroids.h"
#include "data/asteroids.cpp"
TFT_eSPI tft = TFT_eSPI();

float joystick_x, joystick_y, current_time = 0, score_multiplier = 1;
int resetCount = 0, index = 0, new_asteroid = 0, explosion_index = 0, level = 0, asteroid_count = 0, generate_asteroids = 0, lives = 0, max_bullets = 6;
int invincibility = 0, high_score, alternate_sides = 1, bullet_index = 0, cooldown = 10, alternate_frame = 1, powerup_cooldown, bullet_sound = 0;
const int  refresh_rate = 20;
long unsigned int score = 0, level_score = 0;
float start_time, level_time, time_score = 0;
bool is_invincible = false, triple_shot_active = false;
Player player;
Bullet bullet[6];
Asteroids asteroid[32];
Explosion explode[5];
Powerup triple_shot;

void menu();
void game();
void tutorial();
void delete_entities();
void generate_level();
void print_level();
void game_over();
void game_player();
void game_bullet();
void game_asteroids();
void game_explosion();
void game_powerup();
void draw_powerup(int x, int y, int size, int c);

#define JOYSTICK_X_PIN A6
#define JOYSTICK_Y_PIN A7
#define JOYSTICK_BUTTON_PIN 13
#define MOVE 10
#define SHOOT 11
#define SOUND 12
#define PI 3.14159265

#define BLACK 0x0000
#define BLUE 0x001F
#define RED 0xF800
#define GREEN 0x07E0
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define WHITE 0xFFFF
#define GRAY 0xBDF7

unsigned int colour[] = {WHITE, YELLOW, RED, BLUE, GREEN, BLACK, MAGENTA};

void setup() {
  Serial.begin(9600);
  tft.begin();
  tft.setRotation(1);
  tft.fillScreen(BLACK);
  randomSeed(analogRead(A0));
  
  pinMode(JOYSTICK_BUTTON_PIN, INPUT);
  pinMode(MOVE, INPUT);
  pinMode(SHOOT, INPUT);
  digitalWrite(JOYSTICK_BUTTON_PIN, HIGH);

}

void loop() {
  menu();
}

void menu(){
  tft.fillScreen(BLACK);
  tft.setCursor(0, 60);
  tft.setTextSize(4);
  tft.setTextColor(GREEN);
  tft.print("TRIANGLES");
  tft.setTextColor(WHITE);
  tft.print(" vs ");
  tft.setTextColor(RED);
  tft.print("SQUARES");
  tft.setCursor(0, 200);
  tft.setTextColor(WHITE);
  tft.setTextSize(2);
  tft.print("Press left button to ");
  tft.setTextSize(3);
  tft.setTextColor(GREEN);
  tft.println("PLAY");
  tft.setTextColor(WHITE);
  tft.setTextSize(2);
  tft.print("Press right button for ");
  tft.setTextSize(3);
  tft.setTextColor(RED);
  tft.print("TUTORIAL");

  while(1){
    if(digitalRead(MOVE)){
      lives = 3;
      level = 0;
      score = 0;
      level_score = 0;
      level_time = 0;
      time_score = 0;
      alternate_frame = 1;
      while (lives){
        game();
      }
      break;      
    }else if(digitalRead(SHOOT)){
      tutorial();
      break;
    }
  }
}

void game(){

  generate_level();
  player.reset_player();
  tft.fillScreen(BLACK);

  level_time = 0;
  while (asteroid_count){  
    current_time = millis();
    game_player();
  
    if (!lives){
      game_over();
      break;
    }    

    game_bullet();
    game_asteroids();
    game_explosion();
    game_powerup();

    level_time++;
    while(current_time + refresh_rate > millis()){}
  }

  delete_entities();

  tone(SOUND, 200, 150);
  delay(200);
  tone(SOUND, 250, 150);
  delay(200);
  tone(SOUND, 300, 150);
  
  time_score = (1500 + (level * 1000) - level_time) / 10;
  time_score *= score_multiplier;
  if(time_score < 0){
    time_score = 0;
  }
  if (lives){
    print_level();
  }
}

void tutorial(){
  tft.fillScreen(BLACK);  
  tft.setCursor(160, 0);
  tft.setTextColor(WHITE);
  tft.setTextSize(4);
  tft.print("TUTORIAL");
  tft.setCursor(20, 120);
  tft.setTextColor(GREEN);
  tft.setTextSize(2);
  tft.print("This is You!");
  tft.setCursor(20, 180);
  tft.setTextColor(WHITE);
  tft.setTextSize(1);
  tft.println("use joystick to turn");
  tft.print("press left button to ");
  tft.setTextColor(GREEN);
  tft.println("move");
  tft.setTextColor(WHITE);
  tft.print("press right button to ");  
  tft.setTextColor(RED);
  tft.println("shoot");


  tft.setTextSize(2);
  tft.setTextColor(RED);
  tft.setCursor(320, 80);
  tft.print("Destroy This");
  tft.setCursor(350, 160);
  tft.setTextColor(WHITE);
  tft.setTextSize(1);  
  tft.print("if you get hit by");
  tft.setCursor(350, 170);
  tft.print("these guys 3 times");
  tft.setTextColor(RED);
  tft.setTextSize(2);
  tft.setCursor(350, 185);
  tft.print("YOU LOSE!");  

  tft.setCursor(190, 230);
  tft.setTextColor(MAGENTA);
  tft.setTextSize(2);
  tft.print("This is a powerup");
  tft.setCursor(200, 300);
  tft.setTextColor(WHITE);
  tft.setTextSize(1);  
  tft.print("fly over it to pick it up");

  player.reset_player(80, 160);
  asteroid[0].spawn(2, 0, 0, 400, 120, false);
  triple_shot.spawn(270, 260);
  asteroid_count = 3;
  lives = 3;
  while (asteroid_count){  
    current_time = millis();
    game_player();
  
    if (lives != 3){
      lives = 3;
    }

    game_bullet();
    game_asteroids();
    game_explosion();
    game_powerup();

    while(current_time + refresh_rate > millis()){}
  }
  
  delete_entities();

  
  tft.fillScreen(BLACK);
  tft.setCursor(120, 120);
  tft.setTextColor(GREEN);
  tft.setTextSize(4);
  tft.print("GREAT JOB!");
  tft.setTextColor(WHITE);
  tft.setTextSize(2);
  tft.setCursor(130, 160);
  tft.print("Now get out there");
  tft.setCursor(90, 180);
  tft.print("and destroy those ");
  tft.setTextColor(RED);
  tft.print("SQUARES");

  delay(3000);

}
void delete_entities(){
  for (index = 0; index < 32; index++){
    asteroid[index].die();
  }
  for (explosion_index = 0; explosion_index < 5; explosion_index++){
    explode[explosion_index].stop_explosion();
  }
  for (bullet_index = 0; bullet_index < max_bullets; bullet_index++){
    bullet[bullet_index].die();
  }
  draw_powerup(triple_shot.get_x(), triple_shot.get_y(), triple_shot.get_size(), 5);
  triple_shot.despawn();
}