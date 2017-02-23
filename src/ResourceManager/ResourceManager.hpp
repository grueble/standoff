#ifndef _RESOURCE_MANAGER_HPP_
#define _RESOURCE_MANAGER_HPP_

#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <map>
#include <unordered_map>
#include <string>

// make sure to include the correct .dll in the running directory for the executable

namespace ResourceManager_n
{
   enum ImageType_e
   {
      BOARD,
      BORDER,
      EMPTY_TILE,
      P1_SCORING_TILE,
      P1_MANHOLE,
      P1_PAWN,
      P1_GUN,
      P1_SLINGER,
      P2_SCORING_TILE,
      P2_MANHOLE,
      P2_PAWN,
      P2_GUN,
      P2_SLINGER,
      CURSOR,
      BRIEFCASE
   };

   // configuration properties
   static const std::string PATH_TO_ASSETS = "../assets/";
   static const std::map<ImageType_e, std::string> IMAGE_PATHS = { {BOARD, "board.png"} };

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

      // \Name: loadTextures
      // \Description:
      // - loads all required textures to memory
      // \Argument:
      // - none
      // \Returns
      // - bool, success or failure
      bool loadTextures();

   protected:
      // the window renderer
      SDL_Renderer* gRenderer = NULL;

      // container mapping the loaded SDL_Textures to an indentifying enumeration
      typedef std::unordered_map<ImageType_e, SDL_Texture*> TextureMap;
      TextureMap mTextures;
   };
}

#endif _RESOURCE_MANAGER_HPP_