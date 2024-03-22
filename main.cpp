#include "raylib.h"
#include <stdlib.h>
#include <math.h>

// dimensiones pantalla
#define screenWidth 1600 // ALTO  (X)
#define screenHeight 900 // ANCHO (Y)

#define MAX_GREEN_BALLS 30    // Maximo de bolas verdes en pantalla
#define GREEN_BALL_RADIUS 70  // radio
#define GREEN_BALL_SPEED 7.0f // velocidad de caida

#define MAX_BROWN_BALLS 10    // Maximo de bolas verdes en pantalla
#define BROWN_BALL_RADIUS 40  // radio
#define BROWN_BALL_SPEED 9.0f // velocidad de caida

#define MAX_YELLOW_BALLS 2
#define YELLOW_BALL_RADIUS 20
#define YELLOW_BALL_SPEED 8.0f

#define MAX_RED_BALLS 1
#define RED_BALL_RADIUS 20
#define RED_BALL_SPEED 9.0f

#define NUM_STARS 200 // Número de estrellas en el fondo
#define STAR_RADIUS 2 // Radio de las estrellas

typedef struct
{
    Vector2 position;
    bool active;
} Ball;

Ball greenBalls[MAX_GREEN_BALLS];
Ball brownBalls[MAX_BROWN_BALLS];
Ball yellowBalls[MAX_YELLOW_BALLS];
Ball RedBalls[MAX_RED_BALLS];

float elapsedTime = 0.0f;         //
const float spawnInterval = 0.3f; // Intervalo de tiempo entre la aparición de esferas verdes

int score = 0;  // inicio del puntaje
int lives = 10; // vidas

void drawMainMenu();
void InitGreenBall(Ball *ball);
void InitBrownBall(Ball *ball);
void InitYellowBall(Ball *ball);
void InitRedBall(Ball *ball);
bool CheckCollision(Vector2 playerPos, float playerRadius, Vector2 ballPos, float playRadius);
void dibujarVerde(float rotation);
void dibujarCafe(float rotation);
void dibujarAmarillo();
void dibujarRojo();
void vidas();
void gameOverInterface();

int main(void)
{
    const int playRadius = 25;     // tamaño del jugador
    const float ballSpeed = 15.0f; // velocidad del jugador
    bool gameOver = false;         // ya esta, solo el texto del game over ponerlo de blanco xd

    InitWindow(screenWidth, screenHeight, "BETA 0.08");
    SetTargetFPS(60);
    Vector2 playPosition = {(float)screenWidth / 2, (float)screenHeight / 1.1f};

    // vector fondo
    Vector2 stars[NUM_STARS];
    for (int i = 0; i < NUM_STARS; i++)
    {
        stars[i].x = GetRandomValue(0, screenWidth);
        stars[i].y = GetRandomValue(0, screenHeight);
    }

    bool isPlaying = false;

    // Rotacion meteoros
    float rotation = 0.0f;

    while (!WindowShouldClose())
    {
        if (!isPlaying) // Si isPlaying es falso vuelve al menu principal
        {
            ClearBackground(BLACK);
            drawMainMenu();
            // STARS
            for (int i = 0; i < NUM_STARS; i++)
            {
                DrawCircleV(stars[i], STAR_RADIUS, WHITE);
            }

            if (IsKeyPressed(KEY_ENTER))
            {
                isPlaying = true;
            }
        }
        else
        {
            // Velocidad de rotacion meteoros
            rotation += 1.5f;

            if (!gameOver)
            {
                // Actualizar temporizador
                elapsedTime += GetFrameTime();

                // Control del jugador
                if (IsKeyDown(KEY_RIGHT) && playPosition.x + playRadius < screenWidth)
                {
                    playPosition.x += ballSpeed;
                }
                if (IsKeyDown(KEY_LEFT) && playPosition.x - playRadius > 0)
                {
                    playPosition.x -= ballSpeed;
                }
                if (IsKeyDown(KEY_UP))
                {
                    playPosition.y -= ballSpeed;
                }
                if (IsKeyDown(KEY_DOWN))
                {
                    playPosition.y += ballSpeed;
                }

                // Generar esferas
                if (elapsedTime >= spawnInterval)
                {
                    for (int i = 0; i < MAX_GREEN_BALLS; i++)
                    {
                        if (!greenBalls[i].active)
                        {
                            InitGreenBall(&greenBalls[i]);
                            break;
                        }
                    }
                    for (int i = 0; i < MAX_BROWN_BALLS; i++)
                    {
                        if (!brownBalls[i].active)
                        {
                            InitGreenBall(&brownBalls[i]);
                            break;
                        }
                    }
                    for (int i = 0; i < MAX_YELLOW_BALLS; i++)
                    {
                        if (!yellowBalls[i].active)
                        {
                            InitYellowBall(&yellowBalls[i]);
                            break;
                        }
                    }
                    for (int i = 0; i < MAX_RED_BALLS; i++)
                    {
                        if (!RedBalls[i].active)
                        {
                            InitRedBall(&RedBalls[i]);
                            break;
                        }
                    }

                    elapsedTime = 0.0f; // Reiniciar el temporizador
                }

                // Actualiza caida de esfera verde
                for (int i = 0; i < MAX_GREEN_BALLS; i++)
                {
                    if (greenBalls[i].active)
                    {
                        greenBalls[i].position.y += GREEN_BALL_SPEED;
                        if (greenBalls[i].position.y > screenHeight + GREEN_BALL_RADIUS * 2)
                        {
                            greenBalls[i].active = false;
                        }

                        // Detectar colisión con jugador
                        if (CheckCollision(playPosition, playRadius, greenBalls[i].position, GREEN_BALL_RADIUS))
                        {
                            greenBalls[i].active = false; // Eliminar la esfera tocada
                            lives--;                      // Pierde una vida
                            if (lives <= 0)
                            {
                                gameOver = true;
                            }
                        }
                    }
                }
                // --------- ESFERA CAFE
                for (int i = 0; i < MAX_BROWN_BALLS; i++)
                {
                    if (brownBalls[i].active)
                    {
                        brownBalls[i].position.y += BROWN_BALL_SPEED;
                        if (brownBalls[i].position.y > screenHeight + BROWN_BALL_RADIUS * 2)
                        {
                            brownBalls[i].active = false;
                        }

                        // Detectar colisión con jugador
                        if (CheckCollision(playPosition, playRadius, brownBalls[i].position, BROWN_BALL_RADIUS))
                        {
                            brownBalls[i].active = false; // Eliminar la esfera tocada
                            lives--;                      // Pierde una vida
                            if (lives <= 0)
                            {
                                gameOver = true;
                            }
                        }
                    }
                }

                // Actualiza caida de esfera amarilla
                for (int i = 0; i < MAX_YELLOW_BALLS; i++)
                {
                    if (yellowBalls[i].active)
                    {
                        yellowBalls[i].position.y += YELLOW_BALL_SPEED;
                        if (yellowBalls[i].position.y > screenHeight + YELLOW_BALL_RADIUS * 2)
                        {
                            yellowBalls[i].active = false;
                        }

                        // Detectar colisión con jugador y aumentar el contador de puntos
                        if (CheckCollision(playPosition, playRadius, yellowBalls[i].position, YELLOW_BALL_RADIUS))
                        {
                            yellowBalls[i].active = false;
                            score += 10; // Aumentar el puntaje
                        }
                    }
                }
                // Actualiza caida de esfera Roja
                for (int i = 0; i < MAX_RED_BALLS; i++)
                {
                    if (RedBalls[i].active)
                    {
                        RedBalls[i].position.y += RED_BALL_SPEED;
                        if (RedBalls[i].position.y > screenHeight + RED_BALL_RADIUS * 2)
                        {
                            RedBalls[i].active = false;
                        }

                        // Detectar colisión con jugador
                        if (CheckCollision(playPosition, playRadius, RedBalls[i].position, RED_BALL_RADIUS))
                        {
                            RedBalls[i].active = false; // Eliminar la esfera tocada
                            lives++;                    // Gana una vida
                        }
                    }
                }
            }

            BeginDrawing();

            // Background -------------------------------------------------------------------22/03
            ClearBackground(BLACK);
            DrawRectangleGradientV(0, 0, screenWidth, screenHeight, DARKGRAY, BLACK);

            // Dibujar las estrellas
            for (int i = 0; i < NUM_STARS; i++)
            {
                DrawCircleV(stars[i], STAR_RADIUS, WHITE);
            }
            // ---^-^-^-^-^-^-^-^-^-^-^-^-^---------- CORNEJO

            // Dibujar vidas
            vidas();
            DrawText(TextFormat("SCORE: %04i", score), screenWidth - 400, 20, 50, WHITE);

            //
            // Dibujar jugador
            DrawCircleV(playPosition, playRadius, MAROON);
            //  FUNCIONES DE DIBUJO DE ESFERAS   // 21/03  10:03 pm
            dibujarVerde(rotation);
            dibujarCafe(rotation);
            dibujarAmarillo();
            dibujarRojo();
            //  ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^ ^// 21/03  10:03 pm

            if (gameOver) // DECICION PARA DESPLEGAR GAME OVER
            {
                gameOverInterface(); // INTERFACE  DE REGRESO O CONTINUAR

                // Reiniciar el juego si se presiona Enter
                if (IsKeyDown(KEY_ENTER))
                {
                    gameOver = false;
                    lives = 5;
                    score = 0;
                }
                if (IsKeyPressed(KEY_Q))
                {
                    isPlaying = false;
                }
            }

            DrawFPS(0, 0);
            EndDrawing();
        }
    }

    CloseWindow();
    return 0;
}

void drawMainMenu()
{
    BeginDrawing();

    const char *title = "COSMIC CHAOS";

    // Calcula la anchura del texto con la fuente
    int titleWidth = MeasureText(title, 80);

    // Dibuja titulo y opciones
    DrawText(title, screenWidth / 2 - titleWidth / 2, screenHeight - (screenHeight * 0.80), 80, GREEN);

    int sizeStartTxt = MeasureText("(Enter) Start", 60);
    int sizeExitText = MeasureText("(ESC) Exit", 60);
    DrawText("(Enter) Start", screenWidth / 2 - sizeStartTxt / 2, screenHeight / 2 + 50, 60, YELLOW);
    DrawText("(ESC) Exit", screenWidth / 2 - sizeExitText / 2, screenHeight / 2 + 120, 60, GREEN);
    EndDrawing();
}

void InitGreenBall(Ball *ball)
{
    ball->position.x = GetRandomValue(0, GetScreenWidth());
    ball->position.y = -GREEN_BALL_RADIUS * 2;
    ball->active = true;
}
void InitBrownBall(Ball *ball)
{
    ball->position.x = GetRandomValue(0, GetScreenWidth());
    ball->position.y = -BROWN_BALL_RADIUS * 2;
    ball->active = true;
}

void InitYellowBall(Ball *ball)
{
    ball->position.x = GetRandomValue(0, GetScreenWidth());
    ball->position.y = -YELLOW_BALL_RADIUS * 2;
    ball->active = true;
}

void InitRedBall(Ball *ball)
{
    ball->position.x = GetRandomValue(0, GetScreenWidth());
    ball->position.y = -RED_BALL_RADIUS * 2;
    ball->active = true;
}

bool CheckCollision(Vector2 playerPos, float playerRadius, Vector2 ballPos, float playRadius)
{
    float distance = sqrt(pow(ballPos.x - playerPos.x, 2) + pow(ballPos.y - playerPos.y, 2));
    return (distance < (playerRadius + playRadius));
}

void dibujarVerde(float rotation) // vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv// 21/03  10:03 pm
{
    // Velocidad de rotacion
    rotation += 2.5f;
    // Dibujar esferas verdes
    for (int i = 0; i < MAX_GREEN_BALLS; i++)
    {
        if (greenBalls[i].active)
        {
            DrawPoly(greenBalls[i].position, 5, GREEN_BALL_RADIUS, rotation, GRAY);
            DrawPolyLines(greenBalls[i].position, 5, GREEN_BALL_RADIUS, rotation, DARKGRAY);
            DrawPolyLinesEx(greenBalls[i].position, 5, GREEN_BALL_RADIUS, rotation, 5, DARKGRAY);
        }
    }
}

void dibujarCafe(float rotation)
{
    // Velocidad de rotacion
    rotation += 2.5f;
    // Dibujar esferas verdes
    for (int i = 0; i < MAX_BROWN_BALLS / 2; i++)
    {
        if (brownBalls[i].active)
        {
            DrawPoly(brownBalls[i].position, 5, BROWN_BALL_RADIUS, rotation, BROWN);
            DrawPolyLines(brownBalls[i].position, 5, BROWN_BALL_RADIUS, rotation, DARKBROWN);
            DrawPolyLinesEx(brownBalls[i].position, 5, BROWN_BALL_RADIUS, rotation, 5, DARKGRAY);
        }
    }
}
void dibujarAmarillo()
{
    // Dibujar esferas amarillas
    for (int i = 0; i < MAX_YELLOW_BALLS; i++)
    {
        if (yellowBalls[i].active)
        {
            DrawCircleV(yellowBalls[i].position, YELLOW_BALL_RADIUS, YELLOW);
        }
    }
}
void dibujarRojo()
{
    // Dibujar esferas Rojas
    for (int i = 0; i < MAX_RED_BALLS; i++)
    {
        if (RedBalls[i].active)
        {
            DrawCircleV(RedBalls[i].position, RED_BALL_RADIUS, RED);
        }
    }
} //^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^  21/03  10:03 pm

void vidas()
{
    DrawText(TextFormat("Vidas: %d", lives), screenWidth - 250, screenHeight - 140, 50, WHITE);
    for (int i = 0; i < lives; i++)
    {
        DrawText("<3 ", screenWidth - 350 + (i * 60), screenHeight - 60, 50, RED); // Corazón lleno
    }
    for (int i = lives; i < 5; i++)
    {
        DrawText(" - ", screenWidth - 350 + (i * 60), screenHeight - 60, 50, RED); // Corazón vacío
    }                                                                              //  Horizontal, Espaciado,         Altura, Tamaño
}
void gameOverInterface()
{
    // Dibujar ventana de "Game Over"
    DrawText("GAME OVER", screenWidth / 2 - MeasureText("GAME OVER", 100) / 2, screenHeight / 2 - 90, 100, RED);
    DrawText(TextFormat("Puntaje: %04i", score), screenWidth / 2 - MeasureText(TextFormat("Puntaje: %04i", score), 70) / 2, screenHeight / 2, 70, RAYWHITE);
    DrawText("Presione Esc para salir", screenWidth / 2 - MeasureText("Presione Esc para salir", 50) / 2, screenHeight / 2 + 80, 50, RAYWHITE);
    DrawText("Enter para reiniciar", screenWidth / 2 - MeasureText("Enter para reiniciar", 50) / 2, screenHeight / 2 + 120, 50, RAYWHITE);
}