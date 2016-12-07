#ifndef _RESOURCE_MANAGER_HPP_
#define _RESOURCE_MANAGER_HPP_

#include <SDL.h>
#undef main
#include <SDL_image.h>
#include <stdio>
#include <map>
#include <unordered_map>
#include <string>

// make sure to include the correct .dll in the running directory for the executable

namespace ResourceManager_n
{
   // configuration properties
   static const std::string PATH_TO_ASSETS = "../assets/";
   static const std::map<ImageType_e, std::string> IMAGES_PATHS = { {BOARD, "board.png"} };

   enum ImageType_e
   {
      BOARD,
      EMPTY_TILE,
      SCORING_TILE,
      MANHOLE,
      PAWN,
      GUN,
      SLINGER,
      CURSOR,
      BRIEFCASE
   };

   class ResourceManager_c
   {
   public:
      // default constructor
      ResourceManager_c(SDL_Renderer* renderer);

      // default destructor
      ~ResourceManager_c();

      // \Name: getTexture
      // \Description:
      // - returns a texture based on the passed value
      // \Argument:
      // - Imagetype_e, the type of image to return
      // \Returns
      // - SDL_Texture*, pointer to the requested texture
      SDL_Texture* getTexture(ImageType_e image_type);

   protected:
      // \Name: loadTextures
      // \Description:
      // - loads all required textures to memory
      // \Argument:
      // - none
      // \Returns
      // - bool, success or failure
      bool loadTextures();

      // the window renderer
      SDL_Renderer* gRenderer = NULL;

      // container mapping the loaded SDL_Textures to an indentifying enumeration
      std::unordered_map<ImageType_e, SDL_Texture*> mTextures;
   };
}

#endif _RESOURCE_MANAGER_HPP_