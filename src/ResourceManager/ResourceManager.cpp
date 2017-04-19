#include "ResourceManager.hpp"
#include <iostream>

using namespace ResourceManager_n;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// - Constructor and Destructor
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ResourceManager_c::ResourceManager_c()
{

}

ResourceManager_c::~ResourceManager_c()
{

}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// - Memory Management
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
bool ResourceManager_c::init()
{
   // initialization flag
   bool success = true;

   if (SDL_Init(SDL_INIT_VIDEO) < 0)
   {
      printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
      success = false;
   }
   else
   {
      // set texture filtering to linear
      if(!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
      {
         printf( "Warning: Linear texture filtering not enabled!" );
      }

      // create window
      gWindow = SDL_CreateWindow("Standoff", SDL_WINDOWPOS_UNDEFINED, 
                                 SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);     
      if (gWindow == NULL)
      {
         printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
         success = false;
      }
      else
      {
         // create renderer for window
         gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);// SDL_RENDERER_TARGETTEXTURE);
         if( gRenderer == NULL )
         {
            printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
            success = false;
         }
         else 
         {
            //Initialize renderer color
            SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF );

            //Initialize PNG loading
            int imgFlags = IMG_INIT_PNG;
            if( !( IMG_Init( imgFlags ) & imgFlags ) )
            {
               printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
               success = false;
            }
         }
      }
   }

   return success;
}

bool ResourceManager_c::loadMedia()
{
   bool success = true;
   
   if (!gBoardTexture.loadFromFile(gRenderer, PATH_TO_ASSETS + "board.png"))
   {
      printf("Failed to load Board texture image!\n");
      success = false;
   }
   
   if (!gSpritesheetTexture.loadFromFile(gRenderer, PATH_TO_ASSETS + "spritesheet.png"))
   {
      printf("Failed to load Spritesheet texture image!\n");
      success = false;
   }
   else 
   {
      createSpriteMap();
   }

   return success;
}

void ResourceManager_c::close()
{
   gBoardTexture.free();
   gSpritesheetTexture.free();

   // destroy renderer
   SDL_DestroyRenderer(gRenderer);
   gRenderer = NULL;

   // destroy window
   SDL_DestroyWindow(gWindow);
   gWindow = NULL;

   // quit SDL subsystems
   IMG_Quit();
   SDL_Quit();
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// - getRenderer
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
SDL_Renderer* ResourceManager_c::getRenderer()
{
   return gRenderer;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// - renderSpriteAt
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void ResourceManager_c::renderSpriteAt(Sprite_e sprite_id, 
                                       const std::pair<int, int>& screen_tile_coord, 
                                       double degrees)
{
   SDL_Rect dest_rect;
   if (sprite_id != BOARD)
   {  
      dest_rect = { 
         screen_tile_coord.first * TILE_WIDTH, 
         screen_tile_coord.second * TILE_WIDTH,
         TILE_WIDTH, 
         TILE_WIDTH
      };
   
      /*SpriteMap::iterator sprite_it = mSpriteMap.find(sprite);
      if (sprite_it != mSpriteMap.end())
      {
         gSpritesheetTexture.render(gRenderer, &dest_rect, &sprite_it->second, degrees);
      }*/

      std::vector<Sprite_s>::iterator it;
      for (it = mSprites.begin(); it != mSprites.end(); ++it)
      {
         if (it->mSpriteId == sprite_id)
         {
            gSpritesheetTexture.render(gRenderer, &dest_rect, &it->mClip, degrees);
         }
      }
   }
   else // special case for drawing the board
   {
      dest_rect = {
         (screen_tile_coord.first * TILE_WIDTH) - BORDER_WIDTH,
         (screen_tile_coord.second * TILE_WIDTH) - BORDER_WIDTH,
         (BORDER_WIDTH * 2) + (TILE_WIDTH * 9),
         (BORDER_WIDTH * 2) + (TILE_WIDTH * 9)
      };

      gBoardTexture.render(gRenderer, &dest_rect, NULL, degrees);
   }
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// - createSpriteMap
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void ResourceManager_c::createSpriteMap()
{
   SDL_Rect clip = { 0, 0, TILE_WIDTH, TILE_WIDTH };

   for (int sprite_int = P2_PAWN; sprite_int != P2_MANHOLE_TILE; ++sprite_int)
   {
      /*std::pair<Sprite_e, SDL_Rect> sprite = std::pair<Sprite_e, SDL_Rect>(static_cast<Sprite_e>(sprite_int), clip);
      mSpriteMap.insert(sprite);*/

      mSprites.push_back(Sprite_s(static_cast<Sprite_e>(sprite_int), clip));

      if (clip.x < TILE_WIDTH * 8) // spritesheet.png contains 8 sprites/row
      {
         clip.x = clip.x + TILE_WIDTH;
      }
      else 
      {
         clip.x = 0;
         clip.y = clip.y + TILE_WIDTH;
      }
   }
}