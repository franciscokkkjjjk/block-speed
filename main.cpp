#include "raylib.h"
#include <math.h>
#define MAX_BULDING_VALUE 500
#define DEBUG 0

const int largura_Window = 800;
const int altura_window = 500;
const int playerTam = 100;
const float initPositionX = -6000.0f;


int main(){
  InitWindow(largura_Window, altura_window, "Camera Speed Game");
  int game_over = false;
  //posição inicial do playe em y
  float initPlayeY = altura_window/2;
  //posição inicial do playe em x
  float initPlayeX = initPositionX;
  SetTargetFPS(60);
  //Desenha um retangulo para o player
  Rectangle player = { initPlayeX, initPlayeY, playerTam, playerTam };
  //Cria varios retangulos para o fundo
  Rectangle buildings[MAX_BULDING_VALUE] = { 0 };
  
  //Config do retangulo
  const int width_retangulo = 150;
  const int height_retangulo = 250; 
  const int position_rent_y = (height_retangulo+playerTam)-(altura_window/2);

  //Cria varios retangulos de ostaculos
  Rectangle *obstacules;
  int num_obstacules_jump = 0;
  int score_obstacule = 0;
  const int num_def_obstacules_rend = 100;
  Color *obstacules_colors;
  Vector2 position_jumped_obstacules[num_def_obstacules_rend];

  // Cria cores para o fundo
  Color buildColors[MAX_BULDING_VALUE] = { 0 };
  // espaço entre os retangulos do fundo 
  int spacing = 0;
  //Velocidade do player dependendo da camera
  float speed = 0;
  //Verifica se pulou
  bool jump = false;
  float jumpTimer = 0.001f;
  bool fallJump = false;

  // Inicializa o vetor de struct position_jumped_obstacules
  for(int i = 0; i < num_def_obstacules_rend; ++i) position_jumped_obstacules[i] = { 0 };

  // Cria 100 retangulos para o fundo com valores aleatorios 
  for (int i = 0; i < MAX_BULDING_VALUE; i++) {
        buildings[i].width = (float)GetRandomValue(50, 200);
        buildings[i].height = (float)GetRandomValue(100, largura_Window);
        buildings[i].y = altura_window - 130.0f - buildings[i].height;
        buildings[i].x = initPositionX + spacing;

        spacing += (int)buildings[i].width;

        buildColors[i] = (Color){GetRandomValue(200, 240), GetRandomValue(200, 240), GetRandomValue(200, 250), 255 };
    }
    
  // Alocação dinamica dos obstaculos  
  obstacules = (Rectangle *)MemAlloc(sizeof(Rectangle)*num_def_obstacules_rend);
  obstacules_colors = (Color *)MemAlloc(sizeof(Color)*num_def_obstacules_rend);
  

  // Numeros randomicos para gerar os retangulos obstaculos inicialmente
  obstacules[0] = {(float)GetRandomValue(abs(initPlayeX+500), abs(initPlayeY+800)), position_rent_y, width_retangulo, height_retangulo};
  obstacules_colors[0] = (Color){GetRandomValue(125, 200), GetRandomValue(100, 200), GetRandomValue(100, 150), 255};

  // Pega o anterior e gera depois desse obstaculo
  for(int i = 1; i < num_def_obstacules_rend; ++i) {
    obstacules[i] = {(float)GetRandomValue(abs(obstacules[i-1].x+width_retangulo+(width_retangulo*0.9f)), abs(obstacules[i-1].x+width_retangulo+(width_retangulo*2.0f)+600)), position_rent_y, width_retangulo, height_retangulo};
    obstacules_colors[i] = (Color){GetRandomValue(125, 200), GetRandomValue(100, 200), GetRandomValue(100, 150), 255};
  }

  Camera2D camera = { 0 };
  camera.target = (Vector2){ player.x + (float)(playerTam/2), player.y + (float)playerTam/2 };
  camera.offset = (Vector2){ largura_Window/2.0f, altura_window/2.0f };
  camera.rotation = 0.0f;
  camera.zoom = 1.0f;
  game_over = true;
  while (!WindowShouldClose()){ 
    if(game_over) {
      BeginDrawing();
        ClearBackground(BLACK);
        BeginMode2D(camera);
          for (int i = 0; i < MAX_BULDING_VALUE; i++) DrawRectangleRec(buildings[i], buildColors[i]);
          DrawRectangle((-6000+initPositionX), -12000, 6000, 16150, DARKGRAY);
          DrawRectangle(initPositionX, (altura_window*2/3)+10, MAX_BULDING_VALUE*10000, 8000, DARKGRAY);
          for(int i = 0; i < num_def_obstacules_rend; ++i) DrawRectangleRec(obstacules[i], obstacules_colors[i]);
          DrawRectangleRec(player, GetColor(0xff0000ff));
      EndMode2D();
      DrawRectangle(0, altura_window/3, largura_Window, altura_window/3, GetColor(0xff0000aa));
        DrawText("Game Over", (largura_Window/2)-40, altura_window/2, 20, BLACK);
        DrawText("Precione 'R' para recomecar.", (largura_Window/2)-70, (altura_window/2)+20, 12, BLACK);
        if(IsKeyDown(KEY_R)) {
          game_over = false;
          for(int i = 0; i < num_def_obstacules_rend; ++i) position_jumped_obstacules[i] = { 0 };
          obstacules[0] = {(float)GetRandomValue(abs(initPlayeX+500), abs(initPlayeY+800)), position_rent_y, width_retangulo, height_retangulo};
          obstacules_colors[0] = (Color){GetRandomValue(125, 200), GetRandomValue(100, 200), GetRandomValue(100, 150), 255};
          for(int i = 1; i < num_def_obstacules_rend; ++i) {
            obstacules[i] = {(float)GetRandomValue(abs(obstacules[i-1].x+width_retangulo+(width_retangulo*0.9f)), abs(obstacules[i-1].x+width_retangulo+(width_retangulo*2.0f)+600)), position_rent_y, width_retangulo, height_retangulo};
            obstacules_colors[i] = (Color){GetRandomValue(125, 200), GetRandomValue(100, 200), GetRandomValue(100, 150), 255};
          }
          player = {initPlayeX, initPlayeY, playerTam, playerTam};
          num_obstacules_jump = 0;
          score_obstacule = 0;
          camera.rotation = 0;
        }
        DrawText(TextFormat("Score: %d", score_obstacule), 25, 5, 20, GREEN);
      EndDrawing();
      continue;
    }
    // Zera a contagem quando o usuario pula todos os obstaculos
    if(num_def_obstacules_rend-1 <= num_obstacules_jump) {
      obstacules[num_def_obstacules_rend/2] = {(float)GetRandomValue(abs(obstacules[num_def_obstacules_rend/2-1].x+width_retangulo+(width_retangulo*2.0f)), abs(obstacules[num_def_obstacules_rend/2-1].x+width_retangulo+(width_retangulo*0.9f)+600)), position_rent_y, width_retangulo, height_retangulo};
      for(int i = (num_def_obstacules_rend/2)+1; i < num_def_obstacules_rend; ++i)
        obstacules[i] = {(float)GetRandomValue(abs(obstacules[i-1].x+width_retangulo+(width_retangulo*0.9f)), abs(obstacules[i-1].x+width_retangulo+(width_retangulo*2.0f)+600)), position_rent_y, width_retangulo, height_retangulo};
      num_obstacules_jump = 0;
    }
    /*
      Gera todos os obstaculos
      Pega todos os 20 primeiros e define as posições dele quando passa todos os vinte pega esses vinte e joga pra depois dos outrso vinte usuado a 40 posição como inspiração, depois que passou do quarenta pega os vinte primeiro de emplo e taca pra frente

    */
    if(num_obstacules_jump >= num_def_obstacules_rend/2 && num_obstacules_jump < num_def_obstacules_rend/2+2) {
        obstacules[0] = {(float)GetRandomValue(abs(obstacules[num_def_obstacules_rend-1].x+width_retangulo+(width_retangulo*0.9f)), abs(obstacules[num_def_obstacules_rend-1].x+width_retangulo+(width_retangulo*0.9f)+600)), position_rent_y, width_retangulo, height_retangulo};
        for(int i = 1; i < num_def_obstacules_rend/2; ++i) obstacules[i] = {(float)GetRandomValue(abs(obstacules[i-1].x+width_retangulo+(width_retangulo*0.9f)), abs(obstacules[i-1].x+width_retangulo+(width_retangulo*2.0f)+600)), position_rent_y, width_retangulo, height_retangulo};      
    }
    //Verifica os obstaculos pulados  
    for(int i = 0; i < num_def_obstacules_rend; ++i) if(player.x > obstacules[i].x+width_retangulo && !jump) {
      if(obstacules[i].x == position_jumped_obstacules[i].x)
          continue;
      num_obstacules_jump++;
      score_obstacule++;
      position_jumped_obstacules[i] = {obstacules[i].x, obstacules[i].y};
    }

    camera.target = (Vector2){player.x + (float)(playerTam/2), player.y + (float)playerTam/2};

    camera.zoom += (float)GetMouseWheelMove()*0.05f;
    
    if(camera.zoom > 3.0f) camera.zoom = 3.0f;
    else if(camera.zoom < 0.1f) camera.zoom = 0.1f; 
    
    speed = camera.rotation;

    player.x += speed;
    
    if(IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))  {
      camera.rotation += 0.5f;
    } 

    if(IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT))  {
      camera.rotation -= 0.5f;
    }
    if(IsKeyPressed(KEY_SPACE) && !jump) {
      jump = true;
      player.y = playerTam+1;
    }
    // fallJump = true;
    if(jump && fallJump) player.y += playerTam*0.09;
    
    if(jump && !fallJump) {
         if(jumpTimer >= ((initPlayeY*2)-playerTam)) {
            fallJump = true;
            continue;
         }
        
        jumpTimer += playerTam*0.09;
        player.y -= playerTam*0.09;
    }

    if(player.y >= initPlayeY) {
      jump = false;
      fallJump = false;
      player.y = initPlayeY;
      jumpTimer = 0;
    }

    if(camera.rotation > 30.0f) camera.rotation = 30.0f;
    else if(camera.rotation < -30.0f) camera.rotation = -30.0f;    

    if(player.x < initPositionX) player.x = initPositionX;
    
    for(int i = 0; i < num_def_obstacules_rend; ++i) if(CheckCollisionRecs(player, obstacules[i])) {
        DrawText(TextFormat("O retangulo %d bateu.", i), 400, 10, 20, RED);
        game_over = true;
    }
    BeginDrawing();
      ClearBackground(BLACK);

      BeginMode2D(camera);
          for (int i = 0; i < MAX_BULDING_VALUE; i++) DrawRectangleRec(buildings[i], buildColors[i]);
          //Parede que impede o usuario de ir alem do limite (ilusão)
          DrawRectangle((-6000+initPositionX), -12000, 6000, 16150, DARKGRAY);
          //Gera o chão
          DrawRectangle(initPositionX, (altura_window*2/3)+10, MAX_BULDING_VALUE*10000, 8000, DARKGRAY);
          
          

          for(int i = 0; i < num_def_obstacules_rend; ++i) DrawRectangleRec(obstacules[i], obstacules_colors[i]);

          DrawRectangleRec(player, GetColor(0xff0000ff));
      EndMode2D();
      DrawText(TextFormat("Score: %d", score_obstacule), 25, 5, 20, GREEN);
      #if DEBUG == 1
        DrawText(TextFormat("OBSTACULES JUMP: %d", num_obstacules_jump), 25, 25, 20, RED);
        DrawText(TextFormat("ZOOM: %g", camera.zoom), 25, 45, 20, RED);
        DrawText(TextFormat("Rotacao: %g", camera.rotation), 25, 65, 20, RED);
        DrawText(TextFormat("JUMPTIMER %g", initPlayeY-jumpTimer), 25, 85, 20, RED);
        DrawText(TextFormat("Tela %g", initPlayeY), 25, 105, 20, RED);
        for(int i = 0; i < num_def_obstacules_rend; ++i) DrawText(TextFormat("Obstaculo %d X: %f, Y: %f", i, obstacules[i].x, obstacules[i].y), 25, 130+(i*10), 5, RED);
        DrawText(TextFormat("FPS: %d", GetFPS()), 650, 40, 20, RED);
      #endif

    EndDrawing();
  }
  
  MemFree(obstacules);

  MemFree(obstacules_colors);

  CloseWindow();
  
  return 0;
}
