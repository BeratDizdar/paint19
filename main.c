#include "raylib.h"
#include <stdio.h>
#include <string.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define PALETTE_SIZE 6
#define HEX_INPUT_MAX 7

int main() {
    //SetConfigFlags(FLAG_WINDOW_UNDECORATED);
    SetTraceLogLevel(LOG_NONE);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Paint19");
    SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor()));
    
    // CANVAS BOYUTLARI
    RenderTexture2D canvas = LoadRenderTexture(SCREEN_WIDTH, SCREEN_HEIGHT);
    BeginTextureMode(canvas);
        ClearBackground(BLANK);
    EndTextureMode();
    
    Color brushColor = BLACK;
    float brushSize = 5.0f;
    Vector2 lastMousePos = {-1, -1};
    
    Color palette[PALETTE_SIZE] = {RED, GREEN, BLUE, BLACK, YELLOW, PURPLE};
    Rectangle paletteRects[PALETTE_SIZE];
    for (int i = 0; i < PALETTE_SIZE; i++) {
        paletteRects[i] = (Rectangle){10 + i * 40, SCREEN_HEIGHT - 50, 30, 30};
    }
    
    char hexInput[HEX_INPUT_MAX + 1] = "#000000";
    int hexIndex = 0;
    bool hexMode = false;
    
    while (!WindowShouldClose()) {
        Vector2 mousePos = GetMousePosition();
        
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
            if (mousePos.y < SCREEN_HEIGHT - 60) {
                BeginTextureMode(canvas);
                if (lastMousePos.x >= 0 && lastMousePos.y >= 0) {
                    DrawLineEx(lastMousePos, mousePos, brushSize, brushColor);
                }
                DrawCircleV(mousePos, brushSize / 2, brushColor);
                EndTextureMode();
                lastMousePos = mousePos;
            }
        } else {
            lastMousePos = (Vector2){-1, -1};
        }
        
        if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
            BeginTextureMode(canvas);
            ClearBackground(BLANK);
            EndTextureMode();
        }
        
        if (IsKeyPressed(KEY_UP)) brushSize += 2.0f;
        if (IsKeyPressed(KEY_DOWN)) brushSize -= 2.0f;
        if (brushSize < 2.0f) brushSize = 2.0f;
        
        if (IsKeyPressed(KEY_S)) {
            Image screenshot = LoadImageFromTexture(canvas.texture);
            ImageFlipVertical(&screenshot);
            if(ExportImage(screenshot, "drawing.png"))
            {
                printf("INFO: Canvas saved as \"%s\"\n", "drawing.png");
            }
            UnloadImage(screenshot);
        }
        
        for (int i = 0; i < PALETTE_SIZE; i++) {
            if (CheckCollisionPointRec(mousePos, paletteRects[i]) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                brushColor = palette[i];
            }
        }
        
        int key = GetCharPressed();
        while (key > 0) {
            if (hexMode && ((key >= '0' && key <= '9') || (key >= 'A' && key <= 'F') || (key >= 'a' && key <= 'f')) && hexIndex < HEX_INPUT_MAX - 1) {
                hexInput[hexIndex + 1] = (char)key;
                hexIndex++;
                hexInput[hexIndex + 1] = '\0';
            }
            key = GetCharPressed();
        }
        
        if (IsKeyPressed(KEY_ENTER) && hexMode) {
            int r, g, b;
            if (sscanf(hexInput + 1, "%2x%2x%2x", &r, &g, &b) == 3) {
                brushColor = (Color){r, g, b, 255};
            }
            hexMode = false;
        }
        
        if (IsKeyPressed(KEY_H)) {
            hexMode = true;
            strcpy(hexInput, "#");
            hexIndex = 0;
        }
        
        BeginDrawing();
        ClearBackground(LIGHTGRAY);
        DrawTextureRec(canvas.texture, (Rectangle){0, 0, SCREEN_WIDTH, -SCREEN_HEIGHT}, (Vector2){0, 0}, WHITE);
        
        for (int i = 0; i < PALETTE_SIZE; i++) {
            DrawRectangleRec(paletteRects[i], palette[i]);
            DrawRectangleLinesEx(paletteRects[i], 2, DARKGRAY);
        }
        
        DrawText("UP/DOWN: Brush Size | Right Click: Clear | S: Save | H: Hex Color", 10, 10, 20, DARKGRAY);
        
        if (hexMode) {
            DrawRectangle(10, SCREEN_HEIGHT - 90, 120, 30, WHITE);
            DrawRectangleLines(10, SCREEN_HEIGHT - 90, 120, 30, BLACK);
            DrawText(hexInput, 15, SCREEN_HEIGHT - 85, 20, BLACK);
        }
        
        EndDrawing();
    }
    
    UnloadRenderTexture(canvas);
    CloseWindow();
    return 0;
}
