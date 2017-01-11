#include "StandoffApp.hpp"

using namespace StandoffApp_n;

StandoffApp_c::StandoffApp_c()
{

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
         gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_TARGETTEXTURE);
         if( gRenderer == NULL )
         {
            printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
            success = false;
         }

         
      }
   }

   return success;
}

bool StandoffApp_c::loadMedia()
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
      if (!mResourceManager->loadTextures())
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

   // game instance
   Game_n::Game_c game(*mResourceManager);

   // the SDL_RENDERER_TARGETTEXTURE
   SDL_Texture* board_texture = getImage(BOARD);

   // main loop
   while (!quit)
   {
      // event handling
      while(SDL_PollEvent(&e) != 0)
      {
         switch(e.type)
         {
            // user requests quit
            case SDL_QUIT :
            {
               quit = true;
               break;
            }
            case SDL_MOUSEBUTTONDOWN :
            {
               // user presses LMB
               if (e.button == SDL_BUTTON_LEFT)
               {
                  this->handleLmbDown(e, game);
               }
               else
               {
                  // unhandled mouse button
               }
            }
            // user presses a key
            case SDL_KEYDOWN :
            {
               this->handleKeyDown(e, game);
            }
         }

         // render texture to screen
         SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);

         // update screen
         SDL_RenderPresent(gRenderer);
      }
   }

   return 0;
}

void StandoffApp_c::close()
{
   // destroy loaded texture
   SDL_DestroyTexture(gTexture);
   gTexture = NULL;

   // destroy renderer
   SDL_DestroyRenderer(gRenderer);
   gRenderer = NULL;

   // destroy window
   SDL_DestroyWindow(gWindow);
   gWindow = NULL;

   // quit SDL subsystems
   SDL_Quit();
}

void StandoffApp_c::handleLmbDown(const SDL_Event& e, 
                                  Game_n::Game_c& current_game)
{
   /*
    * transform the mouse click event's coordinate in pixels into a
    * "screen tile" coordinate - for reference, the board's upper left
    * hand corner is at (1, 4) in the "screen tile" coordinate system
    */
   std::pair<int, int> screen_tile_coord =
      std::make_pair( e.x / Game_n::TILE_WIDTH , e.y / Game_n::TILE_WIDTH ); // check that integer div

   /*
    * loop through the current player's available pieces (either in reserve 
    * or in play) to determine if the mouse click event selects a piece
    */
   std::vector<Piece_n::Piece_c>& pieces = current_game.getCurrentPlayer().mPieces; 
   std::std::vector<Piece_n::Piece_c>::iterator it;
   for (it = pieces.being(); it != pieces.end(); ++it)
   {
      if (current_game.getCurrentPiece().getPosition() == screen_tile_coord &&
          it->getCurrentPiece().getPlayState() != Piece_n::PlayState_e::DEAD)
      {
         current_game.setCurrentPiece(it);
         break;
      }
   }

   // if we didn't hit a piece with this mouse click event AND a piece is currently selected...
   if (it = pieces.end() && current_game.getCurrentPiece() != NULL)
   {
      current_game.move(screen_tile_coord);
   }
}

void StandoffApp_c::handleKeyDown(const SDL_Event& e,
                                  Game_n::Game_c& current_game)
{
   case SDL_KEYDOWN :
   {
      switch(e.keysym.scancode)
      {
         case SDLK_RETURN :
         {
            if (!current_game.gameOver())
            {
               current_game.emptyCurrentPiece();

               if (current_game.getShootoutFlag())
               {
                  current_game.shootout();
               }

               current_game.nextPlayer();
            }
            else
            {
               // end the game
            }
         }
         case SDLK_BACKSPACE :
         {
            current_game.emptyCurrentPiece();
            current_game.revertMove();
         }
         case SDLK_SPACE :
         {
            current_game.setShootoutFlag();
         }
         case SDLK_DOWN :
         {
            current_game.rotate(Piece_n::DOWN);   
         }
         case SDLK_UP :
         {
            current_game.rotate(Piece_n::UP);
         } 
         case SDLK_LEFT :
         {
            current_game.rotate(Piece_n::LEFT);
         } 
         case SDLK_RIGHT :
         {
            current_game.rotate(Piece_n::RIGHT);
         }  
      }
   }
}

SDL_Texture* StandoffApp_c::getImage(ResourceManager_n::ImageType_e image_type)
{
   return mResourceManager->getTexture(image_type);
}