void game_player(){
//PLAYER

  //Erases old position
  tft.drawTriangle(int(player.get_player_position(2)), int(player.get_player_position(3)),
                    int(player.get_player_position(6)), int(player.get_player_position(7)),
                    int(player.get_player_position(10)), int(player.get_player_position(11)), BLACK);
  //Draws new position
  if (is_invincible){
    tft.drawTriangle(int(player.get_player_position(0)), int(player.get_player_position(1)),
                      int(player.get_player_position(4)), int(player.get_player_position(5)),
                      int(player.get_player_position(8)), int(player.get_player_position(9)), GREEN);    
  }else if(lives == 3){
    tft.drawTriangle(int(player.get_player_position(0)), int(player.get_player_position(1)),
                      int(player.get_player_position(4)), int(player.get_player_position(5)),
                      int(player.get_player_position(8)), int(player.get_player_position(9)), WHITE);
  }else if(lives == 2){
    tft.drawTriangle(int(player.get_player_position(0)), int(player.get_player_position(1)),
                      int(player.get_player_position(4)), int(player.get_player_position(5)),
                      int(player.get_player_position(8)), int(player.get_player_position(9)), YELLOW);
  }else if(lives == 1){
    tft.drawTriangle(int(player.get_player_position(0)), int(player.get_player_position(1)),
                      int(player.get_player_position(4)), int(player.get_player_position(5)),
                      int(player.get_player_position(8)), int(player.get_player_position(9)), RED);
  }else{
    tft.drawTriangle(int(player.get_player_position(0)), int(player.get_player_position(1)),
                      int(player.get_player_position(4)), int(player.get_player_position(5)),
                      int(player.get_player_position(8)), int(player.get_player_position(9)), BLUE);
  }
    joystick_x = map(analogRead(JOYSTICK_X_PIN), 0, 1023, -1000, 1000)/10000.0;
    joystick_y = map(analogRead(JOYSTICK_Y_PIN), 0, 1023, -1000, 1000)/10000.0;

    if((-0.01 > joystick_y || joystick_y > 0.01) || (-0.01 > joystick_x || joystick_x > 0.01)){
      if (digitalRead(MOVE)){
        player.move(joystick_x, joystick_y);
      }else{
        player.turn(joystick_x, joystick_y);
      }
    }else{
      player.drift();
    }
    
    if (!invincibility){
      is_invincible = false;
      for(index = 0; index < 32; index++){
        if(asteroid[index].is_alive()){
          if(player.check_collision(asteroid[index].get_asteroid_x(), asteroid[index].get_asteroid_y(), asteroid[index].get_asteroid_length())){
            tone(SOUND, 100, 600);
            lives--;
            invincibility = 200;
            is_invincible = true;
            for(explosion_index = 0; explosion_index < 5; explosion_index++){
              if (!explode[explosion_index].is_alive()){
                explode[explosion_index].start_explosion(player.get_player_origin(0) - 20, player.get_player_origin(1) - 20, 4, 3);          
              }
            }
            
            player.reset_player();
          }
        }
      }    
    }else{
      invincibility--;
    }
}
void game_bullet(){
  //BULLET
  if(!cooldown){
    if(digitalRead(SHOOT)){
      if(triple_shot_active){
        for(bullet_index = 0; bullet_index < max_bullets; bullet_index++){
          if(!bullet[bullet_index].is_alive()){
            bullet[bullet_index].respawn(player.get_player_direction(0), player.get_player_position(0), player.get_player_position(1));
            break;
          }
        }
        for(bullet_index = 0; bullet_index < max_bullets; bullet_index++){
          if(!bullet[bullet_index].is_alive()){
            bullet[bullet_index].respawn(player.get_player_direction(0) + (PI/30), player.get_player_position(0), player.get_player_position(1));
            break;
          }
        }
        for(bullet_index = 0; bullet_index < max_bullets; bullet_index++){
          if(!bullet[bullet_index].is_alive()){
            bullet[bullet_index].respawn(player.get_player_direction(0) - (PI/30), player.get_player_position(0), player.get_player_position(1));
            break;
          }
        }
        bullet_sound = 20;
        cooldown = 40;
      }else{
        for(bullet_index = 0; bullet_index < max_bullets; bullet_index++){  
          if(!bullet[bullet_index].is_alive()){
            bullet[bullet_index].respawn(player.get_player_direction(0), player.get_player_position(0), player.get_player_position(1));
            bullet_sound = 10;
            cooldown = 25;
            break;
          }
        }
      }
    }

  }else{
    cooldown--;
  }
  for(bullet_index = 0; bullet_index < 6; bullet_index++){
    if (bullet[bullet_index].is_alive()){
      if(bullet[bullet_index].get_bullet_lifespan() > 75){
        tft.drawPixel(int(bullet[bullet_index].get_bullet_x()), int(bullet[bullet_index].get_bullet_y()), BLACK);
        bullet[bullet_index].die();
      }else{
      bullet[bullet_index].move();
      tft.drawPixel(int(bullet[bullet_index].get_bullet_oldx()), int(bullet[bullet_index].get_bullet_oldy()), BLACK);
      tft.drawPixel(int(bullet[bullet_index].get_bullet_x()), int(bullet[bullet_index].get_bullet_y()), WHITE);
      }
    }
  }

  if(bullet_sound){
    tone(SOUND, 250 + bullet_sound * 10, 20);
    bullet_sound--;
  }

}
void game_asteroids(){

//ASTEROIDS
    for (index = 0; index < 32; index++){
      if(asteroid[index].is_alive()){
        asteroid[index].move();
        for(bullet_index = 0; bullet_index < max_bullets; bullet_index++){
          if (asteroid[index].check_collision(int(bullet[bullet_index].get_bullet_x()), int(bullet[bullet_index].get_bullet_y()))){
            tone(SOUND, 125 - 25 * asteroid[index].get_asteroid_tier(), 300 + 100 * asteroid[index].get_asteroid_tier());
            tft.drawPixel(int(bullet[bullet_index].get_bullet_x()), int(bullet[bullet_index].get_bullet_y()), BLACK);
            bullet[bullet_index].die();
            level_score += 100 * score_multiplier;
            asteroid_count--;
            for(explosion_index = 0; explosion_index < 5; explosion_index++){
              if (!explode[explosion_index].is_alive()){
                explode[explosion_index].start_explosion(asteroid[index].get_asteroid_x(), asteroid[index].get_asteroid_y(), asteroid[index].get_asteroid_tier(), 2);            
              }
            }
            if (asteroid[index].get_asteroid_tier() == 1){
              tft.drawRect(int(asteroid[index].get_asteroid_oldx()), int(asteroid[index].get_asteroid_oldy()), asteroid[index].get_asteroid_length(), asteroid[index].get_asteroid_length(), BLACK);  
              asteroid[index].die();
            }else{
              tft.drawRect(int(asteroid[index].get_asteroid_oldx()), int(asteroid[index].get_asteroid_oldy()), asteroid[index].get_asteroid_length(), asteroid[index].get_asteroid_length(), BLACK);  
              asteroid[index].split((random(-300, 300) / 100) * (1 + float(level) / 8), (random(-300, 300) / 100) * (1 + float(level) / 8));
              new_asteroid = index;

              while (new_asteroid < 32){
                if (!asteroid[new_asteroid].is_alive()){
                  asteroid[new_asteroid].spawn(asteroid[index].get_asteroid_tier(), (random(-300, 300) / 100) * (1 + float(level) / 8), (random(-300, 300) / 100) * (1 + float(level) / 8), asteroid[index].get_asteroid_x(), asteroid[index].get_asteroid_y());
                  break;
                }
                new_asteroid++;
              }
            }
            break;
          }
        } // bullet for loop
          tft.drawRect(int(asteroid[index].get_asteroid_oldx()), int(asteroid[index].get_asteroid_oldy()), asteroid[index].get_asteroid_length(), asteroid[index].get_asteroid_length(), BLACK);  
          tft.drawRect(int(asteroid[index].get_asteroid_x()), int(asteroid[index].get_asteroid_y()), asteroid[index].get_asteroid_length(), asteroid[index].get_asteroid_length(), WHITE);
          continue;
        }
      }
}
void game_explosion(){
  //EXPLOSION
    alternate_frame *= -1;

    for (explosion_index = 0; explosion_index < 5; explosion_index++){
      if (explode[explosion_index].is_alive()){
        if(alternate_frame == 1){
          tft.drawCircle(explode[explosion_index].get_x(), explode[explosion_index].get_y(), explode[explosion_index].get_size(), BLACK);
          explode[explosion_index].explode();
        }else{
          tft.drawCircle(explode[explosion_index].get_x(), explode[explosion_index].get_y(), explode[explosion_index].get_size(), colour[explode[explosion_index].get_colour()]);
        }
      }
    }
}
void game_powerup(){
  if(!powerup_cooldown){
    if(triple_shot.is_alive()){
      if(player.check_collision(triple_shot.get_x(), triple_shot.get_y(), triple_shot.get_size())){
        draw_powerup(triple_shot.get_x(), triple_shot.get_y(), triple_shot.get_size(), 5);
        triple_shot.collect();
        triple_shot_active = true;
        max_bullets = 6;
        powerup_cooldown = random(600, 1200);
        tone(SOUND, 400, 400);
      }else{
        draw_powerup(triple_shot.get_x(), triple_shot.get_y(), triple_shot.get_size(), 6);
      }
    }else{
      triple_shot.spawn(random(20, 460), random(20, 300));
      draw_powerup(triple_shot.get_x(), triple_shot.get_y(), triple_shot.get_size(), 6);
    }

  }else if(triple_shot_active){
    triple_shot.decay();
    if(!triple_shot.get_lifespan()){
      triple_shot_active = false;
      max_bullets = 3;
    }    
  }else{
    powerup_cooldown--;
  }
}
void draw_powerup(int x, int y, int size, int c){
  tft.drawRect(x, y, size, size, colour[c]);
  tft.drawLine(x + 2, y + size / 2, x + size - 2, y + size / 2, colour[c]);
  tft.drawLine(x + 2, y + size / 2, x + size - 4, y + (size / 2) - 8, colour[c]);
  tft.drawLine(x + 2, y + size / 2, x + size - 4, y + (size / 2) + 8, colour[c]);
}