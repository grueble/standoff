#ifndef _RESOURCE_MANAGER_HPP_
#define _RESOURCE_MANAGER_HPP_

#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <vector>
#include <string>
#include <memory>
#include <unordered_map>
#include "LTexture.hpp"

// make sure to include the correct .dll in the running directory for the executable

namespace ResourceManager_n
{
   // the width of (both) the border tiles (and the margin)
   static const int TILE_WIDTH = 60; 
   static const int BORDER_WIDTH = 20;

   // paths to game assets
   static const std::string PATH_TO_ASSETS = "../assets/";
   static const std::vector<std::string> IMG_PATHS =
   {
      "board.png", "spritesheet.png" // "tiles.png", "pieces.png", "special.png"
   };

   // screen dimension constants
   static const int SCREEN_WIDTH = ResourceManager_n::TILE_WIDTH * 19;
   static const int SCREEN_HEIGHT = ResourceManager_n::TILE_WIDTH * 11;

   enum Sprite_e
   {
      BOARD,
      P1_PAWN,
      P1_GUN,
      P1_SLINGER,
      P2_PAWN,
      P2_GUN,
      P2_SLINGER,
      BRIEFCASE,
      CURSOR,
      BASE_TILE,
      EDGE_TILE,
      CORNER_TILE,
      P1_SCORING_TILE,
      P1_MANHOLE_TILE,
      P2_SCORING_TILE,
      P2_MANHOLE_TILE
   };

   class ResourceManager_c
   {
   public:
      // default constructor
      ResourceManager_c();

      // default destructor
      ~ResourceManager_c();

      // \Name: init
      // \Description:
      // - initializes the StandoffApp's SDL2 functionality
      // \Argument:
      // - none
      // \Returns
      // - bool, success of failure
      bool init();

      // \Name: loadMedia
      // \Description:
      // - loads all required media to memory
      // \Argument:
      // - none
      // \Returns
      // - bool, success or failure
      bool loadMedia();

      // \Name: close
      // \Description:
      // - closes the Standoff (client) Application
      // \Argument:
      // - none
      // \Returns
      // - none
      void close();

      // \Name: renderSpriteAt
      // \Description:
      // - returns a texture based on the passed value
      // \Argument:
      // - Sprite_e, the type of image to return
      // \Returns
      // - SDL_Texture*, pointer to the requested texture
      void renderSpriteAt(
         Sprite_e sprite, const std::pair<int, int>& screen_tile_coord, double degrees = 0);

   protected:
      void createSpriteMap();

      // the window to render to
      SDL_Window* gWindow = NULL;

      // the window renderer
      SDL_Renderer* gRenderer = NULL;

      // storage for necessary textures
      LTexture gBoardTexture;
      LTexture gSpritesheetTexture;

      // mapping of sprite type to spritesheet clip
      typedef std::unordered_map<Sprite_e, SDL_Rect> SpriteMap;
      SpriteMap mSpriteMap;
   };
}

#endif _RESOURCE_MANAGER_HPP_