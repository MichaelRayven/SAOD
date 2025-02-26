#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <stdio.h>

#define M_PI 3.14159265358979323846

static SDL_Window *window = NULL;
static SDL_Renderer *renderer = NULL;

static SDL_Texture *leftLabelTexture = NULL;
static SDL_Texture *bottomLabelTexture = NULL;

const int WINDOW_HEIGHT = 800;
const int WINDOW_WIDTH = 1000;
const int MARGIN = 100; // Left and bottom offset for the graph


float normalize(float min, float max, float x) {
    return (x - min) / max;
}

void DrawGraphLabels() {
    SDL_FRect dst;

    // Draw bottom label
    SDL_GetTextureSize(bottomLabelTexture, &dst.w, &dst.h);
    dst.x = (WINDOW_WIDTH - dst.w) / 2;
    dst.y = WINDOW_HEIGHT - dst.h;
    SDL_RenderTexture(renderer, bottomLabelTexture, NULL, &dst);

    // Draw left label
    SDL_GetTextureSize(leftLabelTexture, &dst.w, &dst.h);
    dst.x = -(dst.w / 2) + (dst.h / 2);
    dst.y = (WINDOW_HEIGHT - dst.h) / 2;
    SDL_RenderTextureRotated(renderer, leftLabelTexture, NULL, &dst, -90, NULL, SDL_FLIP_NONE);
}

void DrawGraph(int n, int data[], char label[], int min, int max) {
    int graphWidth = WINDOW_WIDTH - 2 * MARGIN;
    int graphHeight = WINDOW_HEIGHT - 2 * MARGIN;
    int barWidth = graphWidth / (n - 1);

    // Draw axes
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderLine(renderer, MARGIN, MARGIN, MARGIN, WINDOW_HEIGHT - MARGIN); // Y-axis
    SDL_RenderLine(renderer, MARGIN, WINDOW_HEIGHT - MARGIN, WINDOW_WIDTH - MARGIN, WINDOW_HEIGHT - MARGIN); // X-axis

    // Draw bars
    for (int i = 1; i < n; i++) {
        float startY = MARGIN + (1.f - normalize(min, max, data[i - 1])) * graphHeight;
        float endY = MARGIN + (1.f - normalize(min, max, data[i])) * graphHeight;
        float startX = MARGIN + (barWidth * (i - 1));
        float endX = MARGIN + (barWidth * i);
        
        int r =  data[0] % 256;
        int g =  (r + 85) % 256;
        int b =  (g + 85) % 256;

        SDL_SetRenderDrawColor(renderer, r, g, b, 255);
        SDL_RenderLine(renderer, startX, startY, endX, endY);

        if (i == (n - 1)) {
            TTF_Font *font = TTF_OpenFont("fonts/OpenSans-Regular.ttf", 12);
            SDL_Color color = { r, g, b, SDL_ALPHA_OPAQUE };
            SDL_Surface* labelSurface = TTF_RenderText_Blended(font, label, 0, color);
            SDL_Texture* labelTexture = NULL;

            if (labelSurface) {
                labelTexture = SDL_CreateTextureFromSurface(renderer, labelSurface);
                SDL_DestroySurface(labelSurface);
            }

            if (labelTexture) {
                SDL_FRect dst = {endX, endY, 0.f, 0.f};
                SDL_GetTextureSize(labelTexture, &dst.w, &dst.h);
                SDL_RenderTexture(renderer, labelTexture, NULL, &dst);
                SDL_DestroyTexture(labelTexture);
            }

            if (font) {
                TTF_CloseFont(font);
            }
        } 
    }
}

void DrawXLabels(int n, int xLabels[]) {
    int graphWidth = WINDOW_WIDTH - 2 * MARGIN;
    int graphHeight = WINDOW_HEIGHT - 2 * MARGIN;
    int barWidth = graphWidth / (n - 1);

    TTF_Font *font = TTF_OpenFont("fonts/OpenSans-Regular.ttf", 12);
    SDL_Color color = { 0, 0, 0, SDL_ALPHA_OPAQUE };

    for (int i = 0; i < n; i++) {
        float x = MARGIN + (barWidth * i);
        
        // Convert number to string
        char str[8];
        sprintf(str, "%d", xLabels[i]);
        
        // Create and render the label
        SDL_FRect dst;
        SDL_Surface* label = TTF_RenderText_Blended(font, str, 0, color);
        SDL_Texture* labelTexture;
        if (label) {
            labelTexture = SDL_CreateTextureFromSurface(renderer, label);
            SDL_DestroySurface(label);
        }

        if (labelTexture) {
            SDL_GetTextureSize(labelTexture, &dst.w, &dst.h);
            dst.x = x - dst.w / 2;
            dst.y = MARGIN + graphHeight;

            SDL_RenderTexture(renderer, labelTexture, NULL, &dst);
            SDL_DestroyTexture(labelTexture);
        }
    }

    if (font) {
        TTF_CloseFont(font);
    }
}

void DrawYLabels(int n, int yLabels[]) {
    int graphWidth = WINDOW_WIDTH - 2 * MARGIN;
    int graphHeight = WINDOW_HEIGHT - 2 * MARGIN;
    int barWidth = graphWidth / (n - 1);
    
    TTF_Font *font = TTF_OpenFont("fonts/OpenSans-Regular.ttf", 12);
    SDL_Color color = { 0, 0, 0, SDL_ALPHA_OPAQUE };
    
    int min = yLabels[0], max = yLabels[n - 1];

    for (int i = 0; i < n; i++) {        
        // Convert number to string
        char str[8];
        sprintf(str, "%d", yLabels[i]);
        
        // Create and render the label
        SDL_FRect dst;
        SDL_Surface* label = TTF_RenderText_Blended(font, str, 0, color);
        SDL_Texture* labelTexture;
        if (label) {
            labelTexture = SDL_CreateTextureFromSurface(renderer, label);
            SDL_DestroySurface(label);
        }

        if (labelTexture) {
            SDL_GetTextureSize(labelTexture, &dst.w, &dst.h);
            dst.x = MARGIN * 0.9 - dst.w;
            dst.y = MARGIN + (1.f - normalize(min, max, yLabels[i])) * graphHeight - dst.h / 2;

            SDL_RenderTexture(renderer, labelTexture, NULL, &dst);
            SDL_DestroyTexture(labelTexture);
        }
    }

    if (font) {
        TTF_CloseFont(font);
    }
}

int GraphInit() {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s\n", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("Hello World", WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_INPUT_FOCUS, &window, &renderer)) {
        SDL_Log("Couldn't create window and renderer: %s\n", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    if (!TTF_Init()) {
        SDL_Log("Couldn't initialise SDL_ttf: %s\n", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    // Open the font
    TTF_Font *font = TTF_OpenFont("fonts/OpenSans-Regular.ttf", 24);
    if (!font) {
        SDL_Log("Couldn't open font: %s\n", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    // Create the text
    SDL_Color color = { 0, 0, 0, SDL_ALPHA_OPAQUE };
    SDL_Surface* leftLabel = TTF_RenderText_Blended(font, "Трудоемкость, T", 0, color);
    SDL_Surface* bottomLabel = TTF_RenderText_Blended(font, "Кол-во данных, N", 0, color);

    if (font) {
        TTF_CloseFont(font);
    }

    if (leftLabel && bottomLabel) {
        leftLabelTexture = SDL_CreateTextureFromSurface(renderer, leftLabel);
        bottomLabelTexture = SDL_CreateTextureFromSurface(renderer, bottomLabel);
        SDL_DestroySurface(leftLabel);
        SDL_DestroySurface(bottomLabel);
    }
    if (!leftLabelTexture && !bottomLabelTexture) {
        SDL_Log("Couldn't create text: %s\n", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    return SDL_APP_CONTINUE;  
}


void GraphQuit()
{
    TTF_Quit();

    // Destroy renderer and window
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    // Quit SDL subsystems
    SDL_Quit();
}

/* This function runs once per frame, and is the heart of the program. 
@param m length of data
@param n length of data[i] and xLabels
@param k length of yLabels
*/
void GraphIterate(int m, int n, int k, int **data, char **dataLabels, int xLabels[], int yLabels[])
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
        
    for (int i = 0; i < m; i++) {
        DrawGraph(n, data[i], dataLabels[i], yLabels[0], yLabels[k - 1]);
    }
    DrawXLabels(n, xLabels);
    DrawYLabels(k, yLabels);
    DrawGraphLabels();
    SDL_RenderPresent(renderer);
}