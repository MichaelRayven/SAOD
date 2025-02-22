#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <stdio.h>

static SDL_Texture *texture = NULL;

void DrawGraph(SDL_Renderer* renderer) {
    int data[] = {20, 20, 30, 40, 50, 60, 70, 80, 90, 100};
    int dataSize = sizeof(data) / sizeof(data[0]);

    int margin = 50;
    int graphWidth = 800 - 2 * margin;
    int graphHeight = 600 - 2 * margin;
    int barWidth = graphWidth / dataSize;

    // Draw axes
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderLine(renderer, margin, margin, margin, 600 - margin); // Y-axis
    SDL_RenderLine(renderer, margin, 600 - margin, 800 - margin, 600 - margin); // X-axis

    

    // Draw bars
    for (int i = 0; i < dataSize; i++) {
        int barHeight = (data[i] * graphHeight) / 100;
        SDL_FRect barRect = {
            margin + i * barWidth,
            600 - margin - barHeight,
            barWidth,
            barHeight
        };
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        SDL_RenderFillRect(renderer, &barRect);
        SDL_RenderTexture(renderer, texture, NULL, &barRect);
    }
}

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Graph Example", 800, 600, 0);
    if (!window) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL);
    if (!renderer) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDL_Color color = { 255, 255, 0, SDL_ALPHA_OPAQUE };
    TTF_Font* Sans = TTF_OpenFont("Sans.ttf", 24);
    SDL_Surface* text = TTF_RenderText_Solid(Sans, "Hello world", 0, color);

    SDL_CreateTextureFromSurface(renderer, text);
    if (text) {
        texture = SDL_CreateTextureFromSurface(renderer, text);
        SDL_DestroySurface(text);
    }
    if (!texture) {
        SDL_Log("Couldn't create text: %s\n", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    // Main loop flag
    int quit = 0;

    // Event handler
    SDL_Event e;

    // While application is running
    while (!quit) {
        // Handle events on queue
        while (SDL_PollEvent(&e)) {
            // User requests quit
            if (e.type == SDL_EVENT_QUIT) {
                quit = 1;
            }
        }

        // Clear screen
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        // Draw the graph here
        DrawGraph(renderer);

        // Update screen
        SDL_RenderPresent(renderer);
    }

    // Destroy renderer and window
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    // Quit SDL subsystems
    SDL_Quit();

    return 0;
}

