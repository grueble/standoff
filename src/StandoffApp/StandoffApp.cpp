#include "StandoffApp.hpp"
#include "iostream"

using namespace StandoffApp_n;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// - Constructor and Destructor
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
StandoffApp_c::StandoffApp_c(ResourceManager_n::ResourceManager_c& resource_manager, SDL_Renderer* renderer) :
   mResourceManager(resource_manager),
   gRenderer(renderer),
   mCurrentGame()
{

}

StandoffApp_c::~StandoffApp_c()
{

}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// - run
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int StandoffApp_c::run()
{
   // main loop flag
   bool quit = false;

   // event handler
   SDL_Event e;

   // main loop
   while (!quit)
   {
      // event handling
      while (SDL_PollEvent(&e) != 0)
      {
         draw();

         switch (e.type)
         {
            // user requests quit
            case SDL_QUIT:
            {
               quit = true;
               break;
            }
            // user presses a mouse button
            case SDL_MOUSEBUTTONDOWN:
            {
               if (e.button.button == SDL_BUTTON_LEFT)
               {
                  this->handleLmbDown(e);
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

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// - Input Handling Functions
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void StandoffApp_c::handleLmbDown(const SDL_Event& e)
{
   if (mCurrentGame.getCurrentMove().mCurrentAction == Game_n::NONE) // !mCurrentGame.moved() && !mCurrentGame.deployed())
   {
      /*
       * transform the mouse click event's coordinate in pixels into a
       * "screen tile" coordinate - for reference, the board's upper left
       * hand corner is at (1, 4) in the "screen tile" coordinate system
       */
      std::pair<int, int> screen_tile_coord =
         std::make_pair(e.button.x / ResourceManager_n::TILE_WIDTH,
            e.button.y / ResourceManager_n::TILE_WIDTH);

      /*
       * loop through the current player's available pieces (either in reserve
       * or in play) to determine if the mouse click event selects a piece
       */
      bool piece_hit_flag = false;
      const std::vector<Game_n::PiecePtr>& pieces = 
         (mCurrentGame.mCurrentTurn % 2) ? mCurrentGame.getPlayer1Pieces() : mCurrentGame.getPlayer2Pieces();
      std::vector<Game_n::PiecePtr>::const_iterator it;
      for (it = pieces.begin(); it != pieces.end(); ++it)
      {
         if ((*it)->getPosition() == screen_tile_coord && (*it)->getPlayState() != Piece_n::DEAD)
         {
            mCurrentGame.setCurrentPiece(*it);
            piece_hit_flag = true;
         }
      }

      /*
       * if we did not hit a piece with this click AND a piece is already
       * selected, attempt to move the current piece to the click position
       */
      if (!piece_hit_flag && mCurrentGame.getCurrentPiece() != nullptr && 
          mCurrentGame.getCurrentPiece()->getPlayState() == Piece_n::RESERVE)
      {
         mCurrentGame.move(screen_tile_coord);
      }
   }
}

void StandoffApp_c::handleKeyDown(const SDL_Event& e)
{
   std::cout << "Key Input" << std::endl;

   switch(e.key.keysym.sym)
   {
      case SDLK_RETURN :
      {
         // if the current player has moved and we aren't waiting for a deployment to complete...
         if (mCurrentGame.getCurrentMove().mCurrentAction != Game_n::NONE && 
             mCurrentGame.getCurrentMove().mCurrentAction != Game_n::PRE_DEPLOY)
         {
            // and neither player has just won the game...
            if (!mCurrentGame.gameOver())
            {
               std::cout << "PASSING TURN w/" << mCurrentGame.getCurrentMove().mCurrentAction << std::endl;

               // resolve a shootout if one has been called
               if (mCurrentGame.getCurrentMove().mCurrentAction == Game_n::SHOOTOUT)
               {
                  std::cout << "SHOOT" << std::endl;
                  mCurrentGame.shootout();
               }

               // pass the turn to the next player
               mCurrentGame.nextPlayer();
            }
            else // o/w end the game
            {

            }
         }
         break;
      }
      case SDLK_BACKSPACE :
      {
         // if the current player has moved...
         if (mCurrentGame.getCurrentMove().mCurrentAction != Game_n::NONE)
         {
            mCurrentGame.revertMove();
         }
         break;
      }
      case SDLK_SPACE :
      {
         // if the current player hasn't performed any actions this turn
         if (mCurrentGame.getCurrentMove().mCurrentAction == Game_n::NONE)
         {
            std::cout << "SHOOTOUT FLAG SET" << std::endl;
            mCurrentGame.getCurrentMove().mCurrentAction = Game_n::SHOOTOUT;
         }
         break;
      }
      case SDLK_UP :
      {
         if (mCurrentGame.getCurrentPiece() != nullptr &&
             (mCurrentGame.getCurrentMove().mCurrentAction == Game_n::NONE ||
              mCurrentGame.getCurrentMove().mCurrentAction == Game_n::PRE_DEPLOY))
         { 
            mCurrentGame.rotate(Piece_n::UP);
         }
         break;
      } 
      case SDLK_DOWN :
      {
         if (mCurrentGame.getCurrentPiece() != nullptr &&
             (mCurrentGame.getCurrentMove().mCurrentAction == Game_n::NONE ||
              mCurrentGame.getCurrentMove().mCurrentAction == Game_n::PRE_DEPLOY))
         { 
            mCurrentGame.rotate(Piece_n::DOWN);
         }
         break;
      }
      case SDLK_LEFT :
      {
         if (mCurrentGame.getCurrentPiece() != nullptr &&
             (mCurrentGame.getCurrentMove().mCurrentAction == Game_n::NONE ||
              mCurrentGame.getCurrentMove().mCurrentAction == Game_n::PRE_DEPLOY))
         { 
            mCurrentGame.rotate(Piece_n::LEFT);
         }
         break;
      } 
      case SDLK_RIGHT :
      {
         if (mCurrentGame.getCurrentPiece() != nullptr &&
             (mCurrentGame.getCurrentMove().mCurrentAction == Game_n::NONE ||
              mCurrentGame.getCurrentMove().mCurrentAction == Game_n::PRE_DEPLOY))
         { 
            mCurrentGame.rotate(Piece_n::RIGHT);
         }
         break;
      }  
   }
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// - draw
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void StandoffApp_c::draw()
{
   // clear the screen background to black
   SDL_RenderClear(gRenderer);

   // render the board as the base layer
   mResourceManager.renderSpriteAt(ResourceManager_n::BOARD, std::pair<int,int>(Game_n::BOARD_COORD));

   // render all of player 1's pieces in reserve
   const std::vector<Game_n::PiecePtr>& p1_pieces = mCurrentGame.getPlayer1Pieces();
   std::vector<Game_n::PiecePtr>::const_iterator p1_it;
   for (p1_it = p1_pieces.begin(); p1_it != p1_pieces.end(); ++p1_it)
   {
      if ((*p1_it)->getPlayState() != Piece_n::DEAD)
      {
         double piece_direction;
         switch ((*p1_it)->getDirection())
         {
            case Piece_n::DOWN : { piece_direction = 180; break; }
            case Piece_n::LEFT : { piece_direction = 270; break; }
            case Piece_n::RIGHT : { piece_direction = 90; break; }
            default: { piece_direction = 0; break; } // defaults to UP
         }
         switch ((*p1_it)->getPieceType())
         {
            case Piece_n::PAWN:
            {
               mResourceManager.renderSpriteAt(ResourceManager_n::P1_PAWN, (*p1_it)->getPosition(), piece_direction); break;
            }
            case Piece_n::GUN:
            {
               mResourceManager.renderSpriteAt(ResourceManager_n::P1_GUN, (*p1_it)->getPosition(), piece_direction); break;
            }
            case Piece_n::SLINGER:
            {
               mResourceManager.renderSpriteAt(ResourceManager_n::P1_SLINGER, (*p1_it)->getPosition(), piece_direction); break;
            }
         }
      }
   }

   // render all of player 2's pieces in reserve
   const std::vector<Game_n::PiecePtr>& p2_pieces = mCurrentGame.getPlayer2Pieces();
   std::vector<Game_n::PiecePtr>::const_iterator p2_it;
   for (p2_it = p2_pieces.begin(); p2_it != p2_pieces.end(); ++p2_it)
   {
      double piece_direction;
      switch ((*p2_it)->getDirection())
      {
         case Piece_n::DOWN: { piece_direction = 180; break; }
         case Piece_n::LEFT: { piece_direction = 270; break; }
         case Piece_n::RIGHT: { piece_direction = 90; break; }
         default: { piece_direction = 0; break; } // defaults to UP
      }
      switch ((*p2_it)->getPieceType())
      {
         case Piece_n::PAWN:
         {
            mResourceManager.renderSpriteAt(ResourceManager_n::P2_PAWN, (*p2_it)->getPosition(), piece_direction); break;
         }
         case Piece_n::GUN:
         {
            mResourceManager.renderSpriteAt(ResourceManager_n::P2_GUN, (*p2_it)->getPosition(), piece_direction); break;
         }
         case Piece_n::SLINGER:
         {
            mResourceManager.renderSpriteAt(ResourceManager_n::P2_SLINGER, (*p2_it)->getPosition(), piece_direction); break;
         }
      }
   }

   // render the cursor at the current piece's position
   if (mCurrentGame.getCurrentPiece() != nullptr)
   {
      mResourceManager.renderSpriteAt(ResourceManager_n::CURSOR, mCurrentGame.getCurrentPiece()->getPosition());
   }

   // render the briefcase at it's current location
   mResourceManager.renderSpriteAt(ResourceManager_n::BRIEFCASE, mCurrentGame.getBriefcasePosition());
  
   // update screen
   SDL_RenderPresent(gRenderer);
}