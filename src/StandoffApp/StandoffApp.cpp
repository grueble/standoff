#include "StandoffApp.hpp"
#include "iostream"

using namespace StandoffApp_n;
using namespace ResourceManager_n;

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

   mResourceManager = &ResourceManager_c(gRenderer);
   
   return mResourceManager->loadTextures();
}

int StandoffApp_c::run()
{
   // main loop flag
   bool quit = true;

   // event handler
   SDL_Event e;

   // initialize the current game
   mCurrentGame = &Game_n::Game_c(); // *mResourceManager);

   // set the render target (static)
   SDL_SetRenderTarget(gRenderer, gTexture);

   // render initial board state
   draw(DrawType_e::START);

   // main loop
   while (!quit)
   {
      // event handling
      while (SDL_PollEvent(&e) != 0)
      {
         switch (e.type)
         {
            // user requests quit
         case SDL_QUIT:
         {
            quit = true;
            break;
         }
         case SDL_MOUSEBUTTONDOWN:
         {
            // user presses LMB
            if (e.button.button == SDL_BUTTON_LEFT)
            {
               this->handleLmbDown(e);
            }
            else
            {
               // unhandled mouse button
            }
         }
         // user presses a key
         case SDL_KEYDOWN:
         {
            this->handleKeyDown(e);
         }
         }
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

void StandoffApp_c::handleLmbDown(const SDL_Event& e)
{
   /*
    * transform the mouse click event's coordinate in pixels into a
    * "screen tile" coordinate - for reference, the board's upper left
    * hand corner is at (1, 4) in the "screen tile" coordinate system
    */
   std::pair<int, int> screen_tile_coord =
      std::make_pair(e.button.x / TILE_WIDTH, 
                     e.button.y / TILE_WIDTH); // check that integer div

   /*
    * loop through the current player's available pieces (either in reserve 
    * or in play) to determine if the mouse click event selects a piece
    */
   std::vector<Piece_n::Piece_c>& pieces = mCurrentGame->getCurrentPlayer().mPieces; 
   std::vector<Piece_n::Piece_c>::iterator it;
   for (it = pieces.begin(); it != pieces.end(); ++it)
   {
      if (mCurrentGame->getCurrentPiece()->getPosition() == screen_tile_coord &&
          it->getPlayState() != Piece_n::DEAD)
      {
         mCurrentGame->setCurrentPiece(*it);
         break;
      }
   }

   // if we didn't hit a piece with this mouse click event AND a piece is currently selected...
   if (it == pieces.end() && mCurrentGame->getCurrentPiece() != NULL)
   {
      mCurrentGame->move(screen_tile_coord);
      draw(DrawType_e::MOVE);
   }

   // if mDeploymentFlag = true, SDL_WaitEvent e for an arrow key, then call handleKeyDown(e)
}

void StandoffApp_c::handleKeyDown(const SDL_Event& e)
{
   switch(e.key.keysym.scancode)
   {
      case SDLK_RETURN :
      {
         if (!mCurrentGame->gameOver())
         {
            if (mCurrentGame->getShootoutFlag())
            {
               mCurrentGame->shootout();
               draw(DrawType_e::SHOOTOUT);
            }

            mCurrentGame->emptyCurrentPiece();
            mCurrentGame->nextPlayer();
         }
         else
         {
            // end the game
         }
      }
      case SDLK_BACKSPACE :
      {
         draw(DrawType_e::UNDO_MOVE);

         mCurrentGame->emptyCurrentPiece();
         mCurrentGame->revertMove();
      }
      case SDLK_SPACE :
      {
         mCurrentGame->setShootoutFlag();
      }
      case SDLK_UP :
      {
         mCurrentGame->rotate(Piece_n::UP);
         draw(DrawType_e::MOVE);
      } 
      case SDLK_DOWN :
      {
         mCurrentGame->rotate(Piece_n::DOWN);
         draw(DrawType_e::MOVE);  
      }
      case SDLK_LEFT :
      {
         mCurrentGame->rotate(Piece_n::LEFT);
         draw(DrawType_e::MOVE);
      } 
      case SDLK_RIGHT :
      {
         mCurrentGame->rotate(Piece_n::RIGHT);
         draw(DrawType_e::MOVE);
      }  
   }
}

void StandoffApp_c::draw(const DrawType_e& draw_action)
{
   std::cout << "Initiating draw action: " << draw_action << std::endl;

   Sprite_s* sprite = NULL;

   switch (draw_action)
   {
      case START :
      {
         // create the base screen + board texture
         gTexture = SDL_CreateTexture(
            gRenderer,
            SDL_PIXELFORMAT_ARGB8888,
            SDL_TEXTUREACCESS_TARGET,
            ResourceManager_n::SCREEN_WIDTH,
            ResourceManager_n::SCREEN_HEIGHT
         );
         sprite = &mResourceManager->getSprite(ResourceManager_n::BOARD);

         std::cout << "dogy" << std::endl;

         // calculate the board render destination
         SDL_Rect board_dest =
         {
            Game_n::BOARD_COORD.first * ResourceManager_n::TILE_WIDTH,
            Game_n::BOARD_COORD.first * ResourceManager_n::TILE_WIDTH,
            Game_n::BOARD_SIDE_LENGTH * ResourceManager_n::TILE_WIDTH,
            Game_n::BOARD_SIDE_LENGTH * ResourceManager_n::TILE_WIDTH
         };

         SDL_RenderCopy(gRenderer, sprite->mTexture, NULL, &board_dest);

         std::pair<int, int> piece_position;
         SDL_Rect dest_rect;
         dest_rect.w = ResourceManager_n::TILE_WIDTH;
         dest_rect.h = ResourceManager_n::TILE_WIDTH;

         // render all of player 1's pieces in reserve
         std::vector<Piece_n::Piece_c> pieces = mCurrentGame->getPlayer1Pieces();
         std::vector<Piece_n::Piece_c>::iterator p1_it;
         for (p1_it = pieces.begin(); p1_it != pieces.end(); ++p1_it)
         {
            piece_position = p1_it->getPosition();

            dest_rect.x = piece_position.first * ResourceManager_n::TILE_WIDTH;
            dest_rect.y = piece_position.second * ResourceManager_n::TILE_WIDTH;

            switch (p1_it->getPieceType())
            {
               case Piece_n::PAWN :
               {
                  sprite = &mResourceManager->getSprite(ResourceManager_n::P1_PAWN);
               }
               case Piece_n::GUN :
               {
                  sprite = &mResourceManager->getSprite(ResourceManager_n::P1_GUN);
               }
               case Piece_n::SLINGER :
               {
                  sprite = &mResourceManager->getSprite(ResourceManager_n::P1_SLINGER);
               }
            }

            SDL_RenderCopy(gRenderer, sprite->mTexture, sprite->mClip, &dest_rect);
         }

         // render all of player 2's pieces in reserve
         pieces = mCurrentGame->getPlayer2Pieces();
         std::vector<Piece_n::Piece_c>::iterator p2_it;
         for (p2_it = pieces.begin(); p2_it != pieces.end(); ++p2_it)
         {
            piece_position = p2_it->getPosition();

            dest_rect.x = piece_position.first * ResourceManager_n::TILE_WIDTH;
            dest_rect.y = piece_position.second * ResourceManager_n::TILE_WIDTH;

            switch (p2_it->getPieceType())
            {
               case Piece_n::PAWN :
               {
                  sprite = &mResourceManager->getSprite(ResourceManager_n::P2_PAWN);
               }
               case Piece_n::GUN :
               {
                  sprite = &mResourceManager->getSprite(ResourceManager_n::P2_GUN);
               }
               case Piece_n::SLINGER :
               {
                  sprite = &mResourceManager->getSprite(ResourceManager_n::P2_SLINGER);
               }
            }

            SDL_RenderCopy(gRenderer, sprite->mTexture, sprite->mClip, &dest_rect);
         }
      }
      case MOVE :
      {
         drawMove(mCurrentGame->getMovedPiece().getPosition(), 
                  mCurrentGame->getPreMovePieceState().getPosition());
      }
      case UNDO_MOVE :
      {
         drawMove(mCurrentGame->getPreMovePieceState().getPosition(), 
                  mCurrentGame->getMovedPiece().getPosition());
      }
      case SHOOTOUT :
      {
         std::pair<int, int> piece_position;
         SDL_Rect dest_rect;
         dest_rect.w = ResourceManager_n::TILE_WIDTH;
         dest_rect.h = ResourceManager_n::TILE_WIDTH;

         std::vector<Piece_n::Piece_c> pieces = mCurrentGame->getPlayer1Pieces();
         std::vector<Piece_n::Piece_c>::iterator p1_it;
         for (p1_it = pieces.begin(); p1_it != pieces.end();)
         {
            if (p1_it->getPlayState() == Piece_n::DEAD)
            {
               piece_position = p1_it->getPosition();

               // get the base tile texture for the dead piece's position
               dest_rect.x = piece_position.first * ResourceManager_n::TILE_WIDTH;
               dest_rect.y = piece_position.second * ResourceManager_n::TILE_WIDTH;
               renderTileBaseSprite(piece_position, dest_rect);
               pieces.erase(p1_it);
            }
            else
            {
               ++p1_it;
            }
         }
         pieces = mCurrentGame->getPlayer2Pieces();
         std::vector<Piece_n::Piece_c>::iterator p2_it;
         for (p2_it = pieces.begin(); p2_it != pieces.end();)
         {
            if (p2_it->getPlayState() == Piece_n::DEAD)
            {
               piece_position = p2_it->getPosition();

               // get the base tile texture for the dead piece's position
               dest_rect.x = piece_position.first * ResourceManager_n::TILE_WIDTH;
               dest_rect.y = piece_position.second * ResourceManager_n::TILE_WIDTH;
               renderTileBaseSprite(piece_position, dest_rect);
               pieces.erase(p2_it);
            }
            else
            {
               ++p2_it;
            }
         }
      }
   }

   // update screen
   SDL_RenderPresent(gRenderer);
}

void StandoffApp_c::drawMove(const std::pair<int, int>& move_position, const std::pair<int, int>& empty_position)
{
   Piece_n::Piece_c moved_piece = mCurrentGame->getMovedPiece();

   Sprite_s* sprite = NULL;
   switch (moved_piece.getPieceType())
   {
      case Piece_n::PAWN :
      {
         moved_piece.getTeam() == Piece_n::PLAYER_ONE ? 
            sprite = &mResourceManager->getSprite(ResourceManager_n::P1_PAWN) : 
            sprite = &mResourceManager->getSprite(ResourceManager_n::P2_PAWN);
      }
      case Piece_n::GUN :
      {
         moved_piece.getTeam() == Piece_n::PLAYER_ONE ?
            sprite = &mResourceManager->getSprite(ResourceManager_n::P1_GUN) :
            sprite = &mResourceManager->getSprite(ResourceManager_n::P2_GUN);
      }
      case Piece_n::SLINGER :
      {
         moved_piece.getTeam() == Piece_n::PLAYER_ONE ?
            sprite = &mResourceManager->getSprite(ResourceManager_n::P1_SLINGER) :
            sprite = &mResourceManager->getSprite(ResourceManager_n::P2_SLINGER);
      }
   }

   double piece_direction;
   switch (moved_piece.getDirection())
   {
      case Piece_n::DOWN : { piece_direction = 180; }
      case Piece_n::LEFT : { piece_direction = 270; }
      case Piece_n::RIGHT : { piece_direction = 90; }
      default: { piece_direction = 0; } // defaults to UP
   }

   // render the moved piece in its new position
   SDL_Rect dest_rect = {
      ResourceManager_n::TILE_WIDTH,
      ResourceManager_n::TILE_WIDTH,
      move_position.first * ResourceManager_n::TILE_WIDTH,
      move_position.second * ResourceManager_n::TILE_WIDTH 
   };
   SDL_RenderCopyEx(gRenderer, sprite->mTexture, sprite->mClip, &dest_rect, piece_direction, NULL, SDL_FLIP_NONE);

   // clear the piece image from the moved piece's old position
   dest_rect.x = empty_position.first * ResourceManager_n::TILE_WIDTH;
   dest_rect.y = empty_position.second * ResourceManager_n::TILE_WIDTH;
   renderTileBaseSprite(empty_position, dest_rect);
}

void StandoffApp_c::renderTileBaseSprite(const std::pair<int, int>& tile_position, SDL_Rect& dest_rect)
{
   ResourceManager_n::Sprite_s* sprite = NULL;

   if (tile_position == Game_n::P1_SCORING_COORD)
   {
      sprite = &mResourceManager->getSprite(ResourceManager_n::P1_SCORING_TILE);
      SDL_RenderCopy(gRenderer, sprite->mTexture, sprite->mClip, &dest_rect);
      return;
   }
   
   if (tile_position == Game_n::P2_SCORING_COORD)
   {
      sprite = &mResourceManager->getSprite(ResourceManager_n::P2_SCORING_TILE);
      SDL_RenderCopy(gRenderer, sprite->mTexture, sprite->mClip, &dest_rect);
      return;
   }

   std::vector<std::pair<int, int>>::const_iterator p1_it;
   for (p1_it = Piece_n::P1_PAWN_DEPLOYMENT_ZONES.begin(); p1_it != Piece_n::P1_PAWN_DEPLOYMENT_ZONES.end(); ++p1_it)
   {
      if (p1_it->first == tile_position.first && p1_it->second == tile_position.second)
      {
         sprite = &mResourceManager->getSprite(ResourceManager_n::P1_MANHOLE_TILE);
         SDL_RenderCopy(gRenderer, sprite->mTexture, sprite->mClip, &dest_rect);
         return;
      }
   }

   std::vector<std::pair<int, int>>::const_iterator p2_it;
   for (p2_it = Piece_n::P2_PAWN_DEPLOYMENT_ZONES.begin(); p2_it != Piece_n::P2_PAWN_DEPLOYMENT_ZONES.end(); ++p2_it)
   {
      if (p2_it->first == tile_position.first && p2_it->second == tile_position.second)
      {
         sprite = &mResourceManager->getSprite(ResourceManager_n::P2_MANHOLE_TILE);
         SDL_RenderCopy(gRenderer, sprite->mTexture, sprite->mClip, &dest_rect);
         return;
      }
   }

   if (tile_position.first == Game_n::BOARD_COORD.first)
   {
      // upper left corner
      if (tile_position.second == Game_n::BOARD_COORD.second)
      {
         sprite = &mResourceManager->getSprite(ResourceManager_n::CORNER_TILE);
      }
      else // left edge
      {
         sprite = &mResourceManager->getSprite(ResourceManager_n::EDGE_TILE);
      }

      SDL_RenderCopy(gRenderer, sprite->mTexture, sprite->mClip, &dest_rect);
      return;
   }

   if (tile_position.first == Game_n::BOARD_COORD.first)
   {
      // upper right corner
      if (tile_position.second == Game_n::BOARD_COORD.second)
      {
         sprite = &mResourceManager->getSprite(ResourceManager_n::CORNER_TILE);
      }
      else // top edge
      {
         sprite = &mResourceManager->getSprite(ResourceManager_n::EDGE_TILE);
      }

      SDL_RenderCopyEx(gRenderer, sprite->mTexture, sprite->mClip, &dest_rect, 90, NULL, SDL_FLIP_NONE);
      return;
   }

   if (tile_position.first == Game_n::BOARD_COORD.first)
   {
      // lower right corner
      if (tile_position.second == Game_n::BOARD_COORD.second)
      {
         sprite = &mResourceManager->getSprite(ResourceManager_n::CORNER_TILE);
      }
      else // right edge
      {
         sprite = &mResourceManager->getSprite(ResourceManager_n::EDGE_TILE);
      }

      SDL_RenderCopyEx(gRenderer, sprite->mTexture, sprite->mClip, &dest_rect, 180, NULL, SDL_FLIP_NONE);
      return;
   }

   if (tile_position.first == Game_n::BOARD_COORD.first)
   {
      // lower left corner
      if (tile_position.second == Game_n::BOARD_COORD.second)
      {
         sprite = &mResourceManager->getSprite(ResourceManager_n::CORNER_TILE);
      }
      else // bottom edge
      {
         sprite = &mResourceManager->getSprite(ResourceManager_n::EDGE_TILE);
      }

      SDL_RenderCopyEx(gRenderer, sprite->mTexture, sprite->mClip, &dest_rect, 270, NULL, SDL_FLIP_NONE);
      return;
   }

   sprite = &mResourceManager->getSprite(ResourceManager_n::ImageType_e::BASE_TILE);
   SDL_RenderCopy(gRenderer, sprite->mTexture, sprite->mClip, &dest_rect);
}