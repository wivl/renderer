#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "SDL.h"
#include "tinytga.h"
#include "tinyobj.h"
#include "matrix.h"
#include "geometry.h"
#include "render.h"

// SDL check code
void scc(int code) {
	if (code < 0) {
		fprintf(stderr, "SDL ERROR: %s\n", SDL_GetError());
		exit(1);
	}
}

// SDL check pointer
void *scp(void *ptr) {
	if (ptr == NULL) {
		fprintf(stderr, "SDL ERROR: %s\n", SDL_GetError());
		exit(1);
	}
	return ptr;
}

bool quit = false;

#define WINDOW_HEIGHT 600
#define WINDOW_WIDTH 800

// 事件处理
void EventHandler(SDL_Event *event) {
    while (SDL_PollEvent(event)) {
			switch (event->type) {
			case SDL_QUIT:
				quit = true;
				break;
		}
	}
}

// 显示一帧
void Present(SDL_Renderer *renderer) {
	SDL_RenderPresent(renderer);
}

// 每帧渲染前清屏
void ClearScreen(SDL_Renderer *renderer) {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
}

// 更新数据
void LogicUpdate() {

}

// 渲染
void Render(SDL_Renderer *renderer) {
	ClearScreen(renderer);
    // 使用 RGBA
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 0);
    


	Present(renderer);
}



int main(void) {
	scc(SDL_Init(SDL_INIT_VIDEO));

	SDL_Window *window = scp(SDL_CreateWindow("renderer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN));
	SDL_Renderer *renderer = scp(SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE)); // SDL_WINDOW_ACCELERATED will cause splash 

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);	// 允许透明

	SDL_Event event = {0};
	while (!quit) {	// 一次循环绘制一帧
		EventHandler(&event);
		LogicUpdate();
		Render(renderer);
        

		
		Present(renderer);
	}

	scc(SDL_RenderClear(renderer));
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	return 0;
}
