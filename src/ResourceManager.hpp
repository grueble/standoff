#ifndef _RESOURCE_MANAGER_HPP_
#define _RESOURCE_MANAGER_HPP_

#include <vector>
#include <string>
#include "LTexture.hpp"

namespace ResourceManager_n
{
   // the width of (both) the border tiles (and the margin)
   static const int TILE_WIDTH = 60; 
   static const int BORDER_WIDTH = 20;

   // screen dimension constants
   static const int SCREEN_WIDTH = ResourceManager_n::TILE_WIDTH * 19;
   static const int SCREEN_HEIGHT = ResourceManager_n::TILE_WIDTH * 11;

   static const int NUM_SPRITESHEET_COL = 8;

   enum Sprite_e
   {
      BOARD,
      P2_PAWN,
      P2_GUN,
      P2_SLINGER,
      P1_PAWN,
      P1_GUN,
      P1_SLINGER,
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

   struct Sprite_s
   {
      Sprite_s(Sprite_e sprite_id, SDL_Rect clip) :
         mSpriteId(sprite_id),
         mClip(clip)
      {
      
      }

      Sprite_e mSpriteId;
      SDL_Rect mClip; 
   };

   class ResourceManager_c
   {
   public:
      // default constructor
      ResourceManager_c();

      // default destructor
      ~ResourceManager_c();

      // initializes the SDL2 functionality
      bool init();

      // loads all required media to memory
      bool loadMedia();

      // unloads all SDL2 functionality
      void close();

      // getter for the SDL_Renderer member
      SDL_Renderer* getRenderer();

      // renders the appropriate sprite at the designated screen tile position
      void renderSpriteAt(
         Sprite_e sprite_id, const std::pair<int, int>& screen_tile_coord, 
         double degrees = 0);

   protected:
      // create a map of sprites and their associated render data
      // - uses Sprite_e value as the key 
      void createSpriteMap();

      // the window to render to
      SDL_Window* gWindow;

      // the window renderer
      SDL_Renderer* gRenderer;

      // storage for necessary textures
      LTexture gBoardTexture;
      LTexture gSpritesheetTexture;

      std::vector<Sprite_s> mSprites;
   };
}

#endif
