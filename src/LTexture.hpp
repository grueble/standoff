#ifndef _L_TEXTURE_HPP_
#define _L_TEXTURE_HPP_

#include <SDL.h>
#include <SDL_image.h>
#include <string>

class LTexture
{
public:
   LTexture();

   ~LTexture();

   bool loadFromFile(SDL_Renderer* renderer, std::string img_path);

   void free();

   void render(SDL_Renderer* renderer, SDL_Rect* dest_rect, 
               SDL_Rect* clip = NULL, double degrees = 0);
private:
   SDL_Texture* mTexture;
};

#endif
