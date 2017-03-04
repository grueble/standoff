#include "StandoffApp.hpp"
#include "iostream"

using namespace StandoffApp_n;

StandoffApp_c::StandoffApp_c(ResourceManager_n::ResourceManager_c& resource_manager) :
   mResourceManager(resource_manager)
{

}

StandoffApp_c::~StandoffApp_c()
{

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

void StandoffApp_c::handleLmbDown(const SDL_Event& e)
{
   /*
    * transform the mouse click event's coordinate in pixels into a
    * "screen tile" coordinate - for reference, the board's upper left
    * hand corner is at (1, 4) in the "screen tile" coordinate system
    */
   std::pair<int, int> screen_tile_coord =
      std::make_pair(e.button.x / ResourceManager_n::TILE_WIDTH, 
                     e.button.y / ResourceManager_n::TILE_WIDTH); // check that integer div

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

   switch (draw_action)
   {
      case START :
      {
         //// create the base screen + board texture
         //gTexture = SDL_CreateTexture(
         //   gRenderer,
         //   SDL_PIXELFORMAT_ARGB8888,
         //   SDL_TEXTUREACCESS_TARGET,
         //   ResourceManager_n::SCREEN_WIDTH,
         //   ResourceManager_n::SCREEN_HEIGHT
         //);

         // render the board as the base layer
         mResourceManager.renderSpriteAt(ResourceManager_n::BOARD, std::pair<int,int>(Game_n::BOARD_COORD));

         std::pair<int, int> piece_position;

         // render all of player 1's pieces in reserve
         std::vector<Piece_n::Piece_c> pieces = mCurrentGame->getPlayer1Pieces();
         std::vector<Piece_n::Piece_c>::iterator p1_it;
         for (p1_it = pieces.begin(); p1_it != pieces.end(); ++p1_it)
         {
            piece_position = p1_it->getPosition();

            switch (p1_it->getPieceType())
            {
               case Piece_n::PAWN :
               {
                  mResourceManager.renderSpriteAt(ResourceManager_n::P1_PAWN, piece_position);
               }
               case Piece_n::GUN :
               {
                  mResourceManager.renderSpriteAt(ResourceManager_n::P1_GUN, piece_position);
               }
               case Piece_n::SLINGER :
               {
                  mResourceManager.renderSpriteAt(ResourceManager_n::P1_SLINGER, piece_position);
               }
            }
         }

         // render all of player 2's pieces in reserve
         pieces = mCurrentGame->getPlayer2Pieces();
         std::vector<Piece_n::Piece_c>::iterator p2_it;
         for (p2_it = pieces.begin(); p2_it != pieces.end(); ++p2_it)
         {
            piece_position = p2_it->getPosition();

            switch (p2_it->getPieceType())
            {
               case Piece_n::PAWN :
               {
                  mResourceManager.renderSpriteAt(ResourceManager_n::P2_PAWN, piece_position);
               }
               case Piece_n::GUN :
               {
                  mResourceManager.renderSpriteAt(ResourceManager_n::P2_GUN, piece_position);
               }
               case Piece_n::SLINGER :
               {
                  mResourceManager.renderSpriteAt(ResourceManager_n::P2_SLINGER, piece_position);
               }
            }
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

         // render over all of p1's dead pieces
         std::vector<Piece_n::Piece_c> pieces = mCurrentGame->getPlayer1Pieces();
         std::vector<Piece_n::Piece_c>::iterator p1_it;
         for (p1_it = pieces.begin(); p1_it != pieces.end();)
         {
            if (p1_it->getPlayState() == Piece_n::DEAD)
            {
               piece_position = p1_it->getPosition();

               // get the base tile texture for the dead piece's position
               renderTileBaseSprite(piece_position);
               pieces.erase(p1_it);
            }
            else
            {
               ++p1_it;
            }
         }

         // render over all of p2's dead pieces
         pieces = mCurrentGame->getPlayer2Pieces();
         std::vector<Piece_n::Piece_c>::iterator p2_it;
         for (p2_it = pieces.begin(); p2_it != pieces.end();)
         {
            if (p2_it->getPlayState() == Piece_n::DEAD)
            {
               piece_position = p2_it->getPosition();

               // get the base tile texture for the dead piece's position
               renderTileBaseSprite(piece_position);
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

   ResourceManager_n::Sprite_e sprite;
   switch (moved_piece.getPieceType())
   {
      case Piece_n::PAWN :
      {
         moved_piece.getTeam() == Piece_n::PLAYER_ONE ? sprite = ResourceManager_n::P1_PAWN : sprite = ResourceManager_n::P2_PAWN;
      }
      case Piece_n::GUN :
      {
         moved_piece.getTeam() == Piece_n::PLAYER_ONE ? sprite = ResourceManager_n::P1_GUN : sprite = ResourceManager_n::P2_GUN;
      }
      case Piece_n::SLINGER :
      {
         moved_piece.getTeam() == Piece_n::PLAYER_ONE ? sprite = ResourceManager_n::P1_SLINGER : sprite = ResourceManager_n::P2_SLINGER;
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

   mResourceManager.renderSpriteAt(sprite, moved_piece.getPosition(), piece_direction);

   // clear the piece image from the moved piece's old position
   renderTileBaseSprite(empty_position);
}

void StandoffApp_c::renderTileBaseSprite(const std::pair<int, int>& tile_position)
{
   ResourceManager_n::Sprite_e sprite = ResourceManager_n::BASE_TILE;
   double tile_rotation = 0;
   if (tile_position.first == Game_n::BOARD_COORD.first)
   {
      // upper left corner
      if (tile_position == Game_n::P2_SCORING_COORD)
      {
         sprite = ResourceManager_n::P2_SCORING_TILE;
      }
      else // left edge
      {
         tile_rotation = 270;
         sprite = ResourceManager_n::EDGE_TILE;
      }
   }
   else if (tile_position.second == Game_n::BOARD_COORD.second)
   {
      // upper right corner
      if (tile_position.second == Game_n::BOARD_COORD.second)
      {
         sprite = ResourceManager_n::CORNER_TILE;
      }
      else // top edge
      {
         sprite = ResourceManager_n::EDGE_TILE;
      }
   }
   else if (tile_position.first == Game_n::BOARD_COORD.first + (Game_n::BOARD_SIDE_LENGTH - 1))
   {
      // lower right corner
      if (tile_position == Game_n::P1_SCORING_COORD)
      {
         sprite = ResourceManager_n::P1_SCORING_TILE;
      }
      else // right edge
      {
         tile_rotation = 90;
         sprite = ResourceManager_n::EDGE_TILE;
      }
   }
   else if (tile_position.second == Game_n::BOARD_COORD.second + (Game_n::BOARD_SIDE_LENGTH - 1))
   {
      // lower left corner
      if (tile_position.second == Game_n::BOARD_COORD.second)
      {
         tile_rotation = 180;
         sprite = ResourceManager_n::CORNER_TILE;
      }
      else // bottom edge
      {
         tile_rotation = 180;
         sprite = ResourceManager_n::EDGE_TILE;
      }
   }
   else 
   {
      std::vector<std::pair<int, int>>::const_iterator p1_it;
      for (p1_it = Piece_n::P1_PAWN_DEPLOYMENT_ZONES.begin(); p1_it != Piece_n::P1_PAWN_DEPLOYMENT_ZONES.end(); ++p1_it)
      {
         if (*p1_it == tile_position)
         {
            sprite = ResourceManager_n::P1_MANHOLE_TILE;
         }
      }

      std::vector<std::pair<int, int>>::const_iterator p2_it;
      for (p2_it = Piece_n::P2_PAWN_DEPLOYMENT_ZONES.begin(); p2_it != Piece_n::P2_PAWN_DEPLOYMENT_ZONES.end(); ++p2_it)
      {
         if (*p1_it == tile_position)
         {
            sprite = ResourceManager_n::P2_MANHOLE_TILE;
         }
      }
   }

   mResourceManager.renderSpriteAt(sprite, tile_position, tile_rotation);
}