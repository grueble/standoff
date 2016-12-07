#include "StandoffApp.hpp"

using namespace StandoffApp_n;

StandoffApp_c::StandoffApp_c() :
{
   mCursor = new SDL_Rect(BOARD_COORD.first + (9 * TIL_WIDTH), 
                          BOARD_COORD.second + (9 * TIL_WIDTH), 
                          TILE_WIDTH, TILE_WIDTH);
}

StandoffApp_c::~StandoffApp_c()
{
}

bool StandoffApp_c::init()
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
      if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
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
         gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
         if( gRenderer == NULL )
         {
            printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
            success = false;
         }

         
      }
   }

   return success;
}

bool Standoffapp_c::loadMedia()
{
   // initialization flag
   bool success = true;

   mResourceManager = new ResourceManager_n::ResourceManager_c(gRenderer);

   if (!mResourceManager)
   {
      printf("Failed to initialize the resource manager!\n");
      success = false;
   }
   else
   {
      if (!mResourceManager.loadTextures())
      {
         printf("Failed to load image files!\n");
         success = false;
      }
   }

   return success;
}

int StandoffApp_c::run()
{
   // main loop flag
   bool quit = false;

   // event handler
   SDL_Event e;

   while (!quit)
   {
      while(SDL_PollEvebt(&e) != 0)
      {
         switch(e.type)
         {
            // user requests quit
            case SDL_QUIT :
            {
               quit = true;
               break;
            }
            // user presses a key
            case SDL_KEYDOWN :
            {
               switch(e.key.keysym.sym)
               {
                  case SDLK_UP :
                  {
                     gCurrentSurface = gKeyPressSurfaces[ KEY_PRESS_SURFACE_UP ];
                     break;
                  }
                  case SDLK_DOWN :
                  {
                     gCurrentSurface = gKeyPressSurfaces[ KEY_PRESS_SURFACE_DOWN ];
                     break;
                  }
                  case SDLK_LEFT :
                  {
                     gCurrentSurface = gKeyPressSurfaces[ KEY_PRESS_SURFACE_LEFT ];
                     break;
                  }
                  case SDLK_RIGHT :
                  {
                     gCurrentSurface = gKeyPressSurfaces[ KEY_PRESS_SURFACE_RIGHT ];
                     break;
                  }
                  default :
                  {
                     gCurrentSurface = gKeyPressSurfaces[ KEY_PRESS_SURFACE_DEFAULT ];
                     break;
                  }
               }
            }
         }

         // render texture to screen
         SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);

         // update screen
         SDL_RenderPresent(gRenderer);
      }
   }
}

void StandoffApp_c::close()
{
   delete mResourceManager;
   mResourceManager = NULL;

   // destroy loaded texture
   SDL_DestroyTexture( gTexture );
   gTexture = NULL;

   // destroy window
   SDL_DestroyWindow(gWindow);
   gWindow = NULL;

   // quit SDL subsystems
   SDL_QUIT();
}

SDL_Surface* Standoffapp_c::getImage(ResourceManager_n::ImageType_e image_type)
{
   return mResourceManager->getTexture(image_type);
}