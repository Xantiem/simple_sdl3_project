#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#define WINDOW_X 500
#define WINDOW_Y 400

static SDL_Window *window = NULL;
static SDL_Renderer *render = NULL;

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) {
    if (!SDL_CreateWindowAndRenderer("Study Timer", WINDOW_X, WINDOW_Y, SDL_WINDOW_RESIZABLE, &window, &render)) {
        SDL_Log("Could not create renderer or window: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
    if (event->type == SDL_EVENT_KEY_DOWN) { 
        return SDL_APP_SUCCESS;
    }

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate) {
    static Uint64 start_tick = 0;
    static int countdown_seconds = 300; // 5:00 — change as needed
    static bool started = false;

    if (!started) {
        start_tick = SDL_GetTicks();
        started = true;
    }

    Uint64 elapsed_ms = SDL_GetTicks() - start_tick;
    int remaining = countdown_seconds - (int)(elapsed_ms / 1000);
    if (remaining < 0) remaining = 0;

    int minutes = remaining / 60;
    int seconds = remaining % 60;

    char message[8]; // "MM:SS\0" fits comfortably
    SDL_snprintf(message, sizeof(message), "%02d:%02d", minutes, seconds);

    int w = 0, h = 0;
    float x, y;
    float scale = 2;
    SDL_GetRenderOutputSize(render, &w, &h);

    x = ((w / scale) - SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE * (float)SDL_strlen(message)) / 2;
    y = ((h / scale) - SDL_DEBUG_TEXT_FONT_CHARACTER_SIZE) / 2;

    SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
    SDL_RenderClear(render);

    SDL_SetRenderDrawColor(render, 155, 155, 155, 255);
    SDL_RenderDebugText(render, x, y, message);
    SDL_RenderPresent(render);

    if (remaining == 0) {
        return SDL_APP_SUCCESS; // countdown finished, quit app
        // or just fall through and keep showing 00:00 instead
    }

    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result) {
}
