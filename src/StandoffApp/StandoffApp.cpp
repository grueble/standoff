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

   mResourceManager = ResourceManager_n::ResourceManager_c(gRenderer);

   return mResourceManager.loadTextures();
}

int StandoffApp_c::run()
{
   // main loop flag
   bool quit = false;

   // event handler
   SDL_Event e;

   // initialize the current game
   mCurrentGame = Game_n::Game_c(mResourceManager)

   // set the render target (static)
   SDL_SetRenderTarget(gRenderer, gTexture);

   // render initial board state
   draw(DrawType_e::START);

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
                  this->handleLmbDown(e, mCurrentGame);
               }
               else
               {
                  // unhandled mouse button
               }
            }
            // user presses a key
            case SDL_KEYDOWN :
            {
               this->handleKeyDown(e, mCurrentGame);
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
      std::make_pair( e.x / Game_n::TILE_WIDTH , e.y / Game_n::TILE_WIDTH ); // check that integer div

   /*
    * loop through the current player's available pieces (either in reserve 
    * or in play) to determine if the mouse click event selects a piece
    */
   std::vector<Piece_n::Piece_c>& pieces = mCurrentGame.getCurrentPlayer().mPieces; 
   std::vector<Piece_n::Piece_c>::iterator it;
   for (it = pieces.begin(); it != pieces.end(); ++it)
   {
      if (mCurrentGame.getCurrentPiece().getPosition() == screen_tile_coord &&
          it->getCurrentPiece().getPlayState() != Piece_n::DEAD)
      {
         mCurrentGame.setCurrentPiece(it);
         break;
      }
   }

   // if we didn't hit a piece with this mouse click event AND a piece is currently selected...
   if (it = pieces.end() && mCurrentGame.getCurrentPiece() != NULL)
   {
      mCurrentGame.move(screen_tile_coord);
      draw(DrawType_e::MOVE);
   }

   // if mDeploymentFlag = true, SDL_WaitEvent e for an arrow key, then call handleKeyDown(e)
}

void StandoffApp_c::handleKeyDown(const SDL_Event& e)
{
   switch(e.keysym.scancode)
   {
      case SDLK_RETURN :
      {
         if (!mCurrentGame.gameOver())
         {
            if (mCurrentGame.getShootoutFlag())
            {
               mCurrentGame.shootout();
               draw(DrawType_e::SHOOTOUT);
            }

            mCurrentGame.setCurrentPiece(NULL);;
            mCurrentGame.revertMove();
            mCurrentGame.nextPlayer();
         }
         else
         {
            // end the game
         }
      }
      case SDLK_BACKSPACE :
      {
         draw(DrawType_e::UNDO_MOVE);

         mCurrentGame.setCurrentPiece(NULL);;
         mCurrentGame.revertMove();
      }
      case SDLK_SPACE :
      {
         mCurrentGame.setShootoutFlag();
      }
      case SDLK_UP :
      {
         mCurrentGame.rotate(Piece_n::UP);
         draw(DrawType_e::MOVE);
      } 
      case SDLK_DOWN :
      {
         mCurrentGame.rotate(Piece_n::DOWN);
         draw(DrawType_e::MOVE);  
      }
      case SDLK_LEFT :
      {
         mCurrentGame.rotate(Piece_n::LEFT);
         draw(DrawType_e::MOVE);
      } 
      case SDLK_RIGHT :
      {
         mCurrentGame.rotate(Piece_n::RIGHT);
         draw(DrawType_e::MOVE);
      }  
   }
}

void StandoffApp_c::draw(DrawType_e& draw_action)
{
   switch (draw_action)
   {
      case START :
      {
         // create the base screen + board texture
         gTexture = SDL_CreateTexture(gRenderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_TARGET, SCREEN_WIDTH, SCREEN_HEIGHT);
         SDL_Texture* board_texture = getImage(ResourceManager_n::BOARD);
         SDL_RenderCopy(gRenderer, board_texture, NULL, BOARD_DEST&);

         // pre-fetch the piece images
         SDL_Texture* p1_pawn_texture = getImage(ResourceManager_n::PINK_PAWN);
         SDL_Texture* p1_gun_texture = getImage(ResourceManager_n::PINK_GUN);
         SDL_Texture* p1_slinger_texture = getImage(ResourceManager_n::PINK_SLINGER);
         SDL_Texture* p2_pawn_texture = getImage(ResourceManager_n::GREEN_PAWN);
         SDL_Texture* p2_gun_texture = getImage(ResourceManager_n::GREEN_GUN);
         SDL_Texture* p2_slinger_texture = getImage(ResourceManager_n::GREEN_SLINGER);

         std::pair<int, int> piece_position;
         SDL_Rect = piece_dest = SDL_Rect(0, 0, Game_n::TILE_WIDTH, Game_n::TILE_WIDTH);

         // render all of player 1's pieces in reserve
         std::std::vector<Piece_n::Piece_c> pieces = mCurrentGame.getPlayer1Pieces();
         std::vector<Piece_n::Piece_c>::const_iterator p1_it;
         for (p1_it = pieces.begin(); p1_it != pieces.end(); ++p1_it)
         {
            piece_position = p1_it->getPosition();

            piece_dest.x = piece_position.first * Game_n::TILE_WIDTH;
            piece_dest.y = piece_position.second * Game_n::TILE_WIDTH;

            switch (p1_it->getPieceType())
            {
               case Piece_n::PAWN :
               {
                  SDL_RenderCopyEx(gRenderer, p1_pawn_texture, NULL, &piece_dest);
               }
               case Piece_n::GUN :
               {
                  SDL_RenderCopyEx(gRenderer, p1_gun_texture, NULL, &piece_dest);
               }
               case Piece_n::SLINGER :
               {
                  SDL_RenderCopyEx(gRenderer, p1_slinger_texture, NULL, &piece_dest);
               }
            }
         }

         // render all of player 2's pieces in reserve
         pieces = mCurrentGame..getPlayer2Pieces();
         std::vector<Piece_n::Piece_c>::const_iterator p2_it;
         for (p2_it = pieces.begin(); p2_it != pieces.end(); ++p2_it)
         {
            piece_position = p2_it->getPosition();

            piece_dest.x = piece_position.first * Game_n::TILE_WIDTH;
            piece_dest.y = piece_position.second * Game_n::TILE_WIDTH;

            switch (p2_it->getPieceType())
            {
               case Piece_n::PAWN :
               {
                  SDL_RenderCopy(gRenderer, p2_pawn_texture, NULL, &piece_dest);
               }
               case Piece_n::GUN :
               {
                  SDL_RenderCopy(gRenderer, p2_gun_texture, NULL, &piece_dest);
               }
               case Piece_n::SLINGER :
               {
                  SDL_RenderCopy(gRenderer, p2_slinger_texture, NULL, &piece_dest);
               }
            }
         }

         SDL_DestroyTexture(pawn_texture);
         SDL_DestroyTexture(gun_texture);
         SDL_DestroyTexture(slinger_texture);
      }
      case MOVE :
      {
         drawMove(mCurrentGame.getMovedPiece().getPosition(), 
                  mCurrentGame.getPreMovePieceState().getPosition());
      }
      case UNDO_MOVE :
      {
         drawMove(mCurrentGame.getPreMovePieceState().getPosition(), 
                  mCurrentGame.getMovedPiece().getPosition());
      }
      case SHOOTOUT :
      {
         SDL_Texture* tile_texture = NULL;
         std::pair<int, int> piece_position;
         SDL_Rect = dest_rect = SDL_Rect(0, 0, Game_n::TILE_WIDTH, Game_n::TILE_WIDTH);

         std::std::vector<Piece_n::Piece_c> pieces = mCurrentGame.getPlayer1Pieces();
         std::vector<Piece_n::Piece_c>::const_iterator p1_it;
         for (p1_it = pieces.begin(); p1_it != pieces.end();)
         {
            if (p1_it->getPlayState() == Piece_n::DEAD)
            {
               piece_position = p1_it->getPosition();
               tile_texture = getTileBaseTexture(piece_position);
               piece_rect.x = piece_position.first * Game_n::TILE_WIDTH;
               piece_rect.y = piece_position.second * Game_n::TILE_WIDTH;
               SDL_RenderCopy(gRenderer, tile_texture, NULL, &dest_rect);
               pieces.erase(p1_it);
            }
            else
            {
               ++p1_it;
            }
         }
         pieces = mCurrentGame..getPlayer2Pieces();
         std::vector<Piece_n::Piece_c>::const_iterator p2_it;
         for (p2_it = pieces.begin(); p2_it != pieces.end();)
         {
            if (p2_it->getPlayState() == Piece_n::DEAD)
            {
               piece_position = p2_it->getPosition();
               tile_texture = getTileBaseTexture(piece_position);
               piece_rect.x = piece_position.first * Game_n::TILE_WIDTH;
               piece_rect.y = piece_position.second * Game_n::TILE_WIDTH;
               SDL_RenderCopy(gRenderer, tile_texture, NULL, &dest_rect);
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
   Piece_n::Piece_c moved_piece = mCurrentGame.getMovedPiece();

   SDL_Texture* piece_texture = NULL;
   switch (moved_piece.getPieceType())
   {
      case Piece_n::PAWN :
      {
         if (moved_piece.getTeam() == Piece_n::PINK)
         {
            piece_texture = getImage(ResourceManager_n::PINK_PAWN);
         }
         else // Piece_n::GREEN
         {
            piece_texture = getImage(ResourceManager_n::GREEN_PAWN);
         }
      }
      case Piece_n::GUN :
      {
         if (moved_piece.getTeam() == Piece_n::PINK)
         {
            piece_texture = getImage(ResourceManager_n::PINK_GUN);
         }
         else // Piece_n::GREEN
         {
            piece_texture = getImage(ResourceManager_n::GREEN_GUN);
         }
      }
      case Piece_n::SLINGER :
      {
        if (moved_piece.getTeam() == Piece_n::PINK)
         {
            piece_texture = getImage(ResourceManager_n::PINK_SLINGER);
         }
         else // Piece_n::GREEN
         {
            piece_texture = getImage(ResourceManager_n::GREEN_SLINGER);
         }
      }
   }

   double piece_direction = 0;
   switch (moved_piece.getDirection())
   {
      // no case for Piece_n::UP bc piece_direction is intialized to 0 degrees

      case Piece_n::DOWN :
      {
         piece_direction = 180;
      }
      case Piece_n::LEFT :
      {
         piece_direction = 270;
      }
      case Piece_n::RIGHT :
      {
         piece_direction = 90;
      }
   }

   // render the moved piece in its new position
   SDL_Rect dest_rect = SDL_Rect(move_position.first * Game_n::TILE_WIDTH,
                                 move_position.second * Game_n::TILE_WIDTH,
                                 Game_n::TILE_WIDTH, Game_n::TILE_WIDTH);

   SDL_RenderCopyEx(gRenderer, piece_texture, NULL, &dest_rect, piece_direction, NULL, SDL_FLIP_NONE);

   // clear the piece image from the moved piece's old position
   SDL_Texture* tile_texture = getTileBaseTexture(empty_position);
   dest_rect.x = pre_move_position.first * Game_n::TILE_WIDTH;
   dest_rect.y = pre_move_position.second * Game_n::TILE_WIDTH;
   SDL_RenderCopy(gRenderer, tile_texture, NULL, &dest_rect);
}

SDL_Texture* StandoffApp_c::getImage(ResourceManager_n::ImageType_e image_type)
{
   return mResourceManager->getTexture(image_type);
}

SDL_Texture* StandoffApp_c::getTileBaseTexture(const std::pair<int, int>& tile_position)
{

}