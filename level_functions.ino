void generate_level(){
  //LEVEL GENERATION
  level++;
  score_multiplier = pow(level, 0.8);
  level_score = 0;
  generate_asteroids = level * 2;
  generate_asteroids = constrain(generate_asteroids, 0 ,32);
  asteroid_count = 0;
  index = 0;
  alternate_sides = 160;
  invincibility = 75;
  is_invincible = true;
  powerup_cooldown = random(400, 800);
  triple_shot_active = false;
  bullet_sound = 0;
  max_bullets = 3;

  if(!(level % 4)){
    lives++;
  }

  while(generate_asteroids){
    if(generate_asteroids % 8 == 0){
      asteroid[index].spawn(4, (random(-300, 300) / 100) * (1 + float(level) / 8),(random(-300, 300) / 100) * (1 + float(level) / 8), random(200, 280) + alternate_sides, random(20, 300));
      generate_asteroids -= 8;
      asteroid_count += 15;
      index++;
    }else if(generate_asteroids % 4 == 0){
      asteroid[index].spawn(3, (random(-300, 300) / 100) * (1 + float(level) / 8), (random(-300, 300) / 100) * (1 + float(level) / 8), random(200, 280) + alternate_sides, random(20, 300));
      generate_asteroids -= 4;
      asteroid_count += 7;
      index++;
    }else if (generate_asteroids % 2 == 0){
      asteroid[index].spawn(2, (random(-300, 300) / 100) * (1 + float(level) / 8), (random(-300, 300) / 100) * (1 + float(level) / 8), random(200, 280) + alternate_sides, random(20, 300));
      generate_asteroids -= 2;
      asteroid_count += 3;
      index++;
    }else{
      asteroid[index].spawn(1, (random(-300, 300) / 100) * (1 + float(level) / 8), (random(-300, 300) / 100) * (1 + float(level) / 8), random(200, 280) + alternate_sides, random(20, 300));
      generate_asteroids--;
      asteroid_count++;
      index++;
    }
    alternate_sides *= -1;
  }
}

void print_level(){
  tft.fillScreen(BLACK);
  tft.setCursor(140, 130);
  tft.setTextColor(WHITE);
  tft.setTextSize(4);
    
  tft.print("Level: "); tft.print(level);
  tft.setCursor(140, 180);
  tft.setTextSize(2);
  tft.print("Level score: "); tft.println(level_score);
  tft.setCursor(140, 200);
  tft.print("Time taken: "); tft.print(int(level_time / 50)); tft.println(" seconds");
  tft.setCursor(140, 220);
  tft.print("bonus points: "); tft.println(int(time_score));
  score += time_score;
  score += level_score;
  tft.setCursor(140, 240);
  tft.print("Total score: "); tft.println(score);
  while(!digitalRead(MOVE)){};

  tft.fillScreen(BLACK);
  
}

void game_over(){

score += level_score;  
  tft.fillScreen(BLACK);
  tft.setCursor(130, 100);
  tft.setTextColor(RED);
  tft.setTextSize(4);
    
  tft.print("GAME OVER");
  tft.setCursor(120, 150);
  tft.setTextSize(2);
  tft.setTextColor(WHITE);
  tft.print("FINAL SCORE: "); tft.println(score);

  if(high_score < score){
    tft.setCursor(120, 170);
    tft.setTextColor(GREEN);
    tft.print("You beat your highscore!");
    tft.setCursor(120, 190);
    tft.setTextColor(WHITE);
    tft.print("Previous highscore: "); tft.print(high_score);
    high_score = score;
  }else{
    tft.setCursor(120, 170);    
    tft.setTextColor(WHITE);
    tft.print("Current highscore: "); tft.print(high_score);
  }
  
  while(!digitalRead(MOVE)){};

  tft.fillScreen(BLACK);
}