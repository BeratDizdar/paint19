#include "raylib.h"
#include <stdio.h>
#include <string.h>
#include "src/fopen.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define PALETTE_SIZE 6
#define HEX_INPUT_MAX 7

int main() {
    //SetConfigFlags(FLAG_WINDOW_UNDECORATED);
    SetTraceLogLevel(LOG_NONE);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Paint19");
    SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor()));
    
    Image app_icon = LoadImage("image/downscaled_app_icon.png");
    SetWindowIcon(app_icon);

    //TODO: CANVAS BOYUTLARI
    RenderTexture2D canvas = LoadRenderTexture(SCREEN_WIDTH, SCREEN_HEIGHT);
    BeginTextureMode(canvas);
        ClearBackground(BLANK);
    EndTextureMode();
    
    Color brushColor = BLACK;
    float brushSize = 20.0f;
    Vector2 lastMousePos = {-1, -1};
    
    Image temp = LoadImage("image/body.png");
    ImageFlipHorizontal(&temp);
    Texture tex_body = LoadTextureFromImage(temp);
    temp = LoadImage("image/brush.png");
    ImageFlipHorizontal(&temp);
    Texture tex_brush = LoadTextureFromImage(temp);

    Color palette[PALETTE_SIZE] = {RED, GREEN, BLUE, BLACK, YELLOW, PURPLE};
    Rectangle paletteRects[PALETTE_SIZE];
    for (int i = 0; i < PALETTE_SIZE; i++) {
        paletteRects[i] = (Rectangle){10 + i * 40, SCREEN_HEIGHT - 50, 30, 30};
    }
    
    char hexInput[HEX_INPUT_MAX + 1] = "#000000";
    int hexIndex = 0;
    bool hexMode = false;
    
    HideCursor();
    while (!WindowShouldClose()) {
        Vector2 mousePos = GetMousePosition();
        Vector2 brush_pos = mousePos;
        brush_pos.x -= 5;
        brush_pos.y -= 5;
        
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
        
        float wheel_y = GetMouseWheelMove();
        if (IsKeyDown(KEY_LEFT_CONTROL) && wheel_y > 0.0f) {
            brushSize += 2.0f;
            
        }
        if (IsKeyDown(KEY_LEFT_CONTROL) && wheel_y < 0.0f) {
            brushSize -= 2.0f;
            
        }
        if (brushSize < 2.0f) brushSize = 2.0f;
        
        if (!IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_S)) {
            char savePath[MAX_PATH];
            if (SaveFileDialog(savePath, MAX_PATH)) {
                printf("Kaydedilecek dosya: %s\n", savePath);
                Image screenshot = LoadImageFromTexture(canvas.texture);
                ImageFlipVertical(&screenshot);
                if(ExportImage(screenshot, savePath))
                {
                    printf("INFO: Canvas saved as \"%s\"\n", savePath);
                }
                UnloadImage(screenshot);
            } else {
                printf("Kaydetme işlemi iptal edildi.\n");
            }
        }

        if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_S)) {
            char savePath[MAX_PATH];
            if (SaveFileDialog(savePath, MAX_PATH)) {
                printf("Kaydedilecek dosya: %s\n", savePath);
                Image screenshot = LoadImageFromTexture(canvas.texture);
                ImageFlipVertical(&screenshot);
                ImageResize(&screenshot, 32, 32);
                if(ExportImage(screenshot, savePath))
                {
                    printf("INFO: Canvas saved as \"%s\"\n", savePath);
                }
                UnloadImage(screenshot);
            } else {
                printf("Kaydetme işlemi iptal edildi.\n");
            }
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
        
        if (IsKeyPressed(KEY_O)) {
            char filePath[MAX_PATH] = {0};
            if (OpenFileDialog(filePath, MAX_PATH)) {
                Image loadedImage = LoadImage(filePath);
                ImageFlipVertical(&loadedImage);
                if (loadedImage.data != NULL) {
                    BeginTextureMode(canvas);
                    DrawTextureRec(LoadTextureFromImage(loadedImage), 
                                   (Rectangle){0, 0, loadedImage.width, -loadedImage.height}, 
                                   (Vector2){0, 0}, WHITE);
                    EndTextureMode();
                    UnloadImage(loadedImage);
                }
            }
        }

        BeginDrawing();
            ClearBackground(LIGHTGRAY);
            DrawTextureRec(canvas.texture, (Rectangle){0, 0, SCREEN_WIDTH, -SCREEN_HEIGHT}, (Vector2){0, 0}, WHITE);
            
            // PALETI YAZDIR.
            for (int i = 0; i < PALETTE_SIZE; i++) {
                DrawRectangleRec(paletteRects[i], palette[i]);
                DrawRectangleLinesEx(paletteRects[i], 2, DARKGRAY);
            }
            
            // YARDIMCI YAZILAR
            DrawText("Right Click: Clear | O: Open | S: Save | CTRL+S: Save(32x32)", 10, 10, 20, DARKGRAY);
            DrawText("MWHEEL: Brush Size | H: Hex Color", 10, 40, 20, DARKGRAY);
            DrawText(TextFormat("Brush Size: %.0f", brushSize), 140, SCREEN_HEIGHT - 85, 20, DARKGRAY);

            // GIRDI YAZISI
            if (hexMode) {
                DrawRectangle(10, SCREEN_HEIGHT - 90, 120, 30, WHITE);
                DrawRectangleLines(10, SCREEN_HEIGHT - 90, 120, 30, BLACK);
                DrawText(hexInput, 15, SCREEN_HEIGHT - 85, 20, BLACK);
            }
            
            // FIRÇAYI BOYA
            DrawTextureEx(tex_body, brush_pos, 0.0f, 1.0f, WHITE);
            DrawTextureEx(tex_brush, brush_pos, 0.0f, 1.0f, brushColor);
        EndDrawing();
    }
    
    UnloadTexture(tex_body);
    UnloadTexture(tex_brush);
    UnloadRenderTexture(canvas);
    CloseWindow();
    return 0;
}
