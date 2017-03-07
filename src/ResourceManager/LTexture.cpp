#include "LTexture.hpp"

LTexture::LTexture()
{

}

LTexture::~LTexture()
{
   free();
}

bool LTexture::loadFromFile(SDL_Renderer* renderer, const std::string& img_path)
{
   free();

   bool success = true;
   SDL_Texture* loaded_texture = NULL;

   loaded_texture = IMG_LoadTexture(renderer, img_path.c_str());
   if (loaded_texture == NULL)
   { 
      printf("Unable to load texture %s! SDL_image Error: %s\n", img_path.c_str(), IMG_GetError());
      success = false;
   }
   else 
   {
      mTexture = loaded_texture;
   }

   return success;

   /*free();

   bool success = true;
   SDL_Texture* new_texture = NULL;
   SDL_Surface* loaded_surface = NULL;

   loaded_surface = IMG_Load(img_path.c_str());
   if (loaded_surface == NULL)
   {
      printf("Unable to load image %s! SDL_image Error: %s\n", img_path.c_str(), IMG_GetError());
      success = false;
   }
   else
   {
      new_texture = SDL_CreateTextureFromSurface(renderer, loaded_surface);
      if (new_texture = NULL)
      {
         printf("Unable to create texture from %s! SDL Error: %s\n", img_path.c_str(), SDL_GetError());
      }

      mTexture = new_texture;

      SDL_FreeSurface(loaded_surface);
   }

   return success;*/
}

void LTexture::free()
{
   if (mTexture != NULL)
   {
      SDL_DestroyTexture(mTexture);
      mTexture = NULL;
   }
}

void LTexture::render(SDL_Renderer* renderer, SDL_Rect* dest_rect, SDL_Rect* clip, double degrees)
{
   if (mTexture == NULL)
   {
      printf("Missing Texture Error!");
   }
   else
   {
      if (degrees != 0)
      {
         SDL_RenderCopyEx(renderer, mTexture, clip, dest_rect, degrees, NULL, SDL_FLIP_NONE);
      }
      else
      {
         SDL_RenderCopy(renderer, mTexture, clip, dest_rect);
      }
   }
}