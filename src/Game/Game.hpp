#ifndef _GAME_HPP_
#define _GAME_HPP_

#include <map>
#include <utility>

#include "Piece.hpp"
#include "ResourceManager/ResourceManager.hpp"

namespace Game_n
{
   // stores all of the data about a player
   struct Player_s
   {
      int mUserId;
      std::vector<Piece_n::Piece_c> mPieces;
   };

   /*
    * whenever a move is made, data about the old and new position 
    * and/or direction of the moved piece is stored in the case that
    * a revert is needed. also stores a flag indicating whether the
    * move was a deployment or not
    */
   struct Move_s
   {
      std::pair<int, int> mOldPosition;
      std::pair<int, int> mNewPosition;
      Piece_n::Direction_e mOldDirection;
      Piece_n::Direction_e mNewDirection;
      bool isDeployment;
   };

   // stores all the data related to a particular game's current state
   struct GameState_s
   {
      GameState_s() :
         mCurrentPlayer(mPlayer1),
         mCurrentPiece(NULL),
         mCurrentMove(NULL),
         mShootoutFlag(false)
      {

      }

      Player_s mCurrentPlayer;
      Piece_n::Piece_c* mCurrentPiece;
      Move_s* mCurrentMove;
      bool mShootoutFlag;
   };

   class Game_c
   {
   public:
      // constructor
      Game_c(const ResourceManager_n::ResourceManager_c& resource_manager);

      // default destructor
      ~Game_c();

      // \Name: start
      // \Description:
      // - initializes the board and game state
      // \Argument:
      // - none
      // \Returns
      // - none
      // void start();

      // \Name: move
      // \Description:
      // - moves a piece
      // \Argument:
      // - Piece_c&, the piece to move
      // - int, the x coordinate of the move position
      // - int, the y coordinate of the move position
      // \Returns
      // - none
      // void move(Piece_n::Piece_c& piece, int move_x, int move_y);

      // \Name: rotate
      // \Description:
      // - initiates a shootout event
      // \Argument:
      // - Piece_c&, the piece to rotate
      // - Direction
      // \Returns
      // - bool, true if the rotation is successful, false o/w
      // bool rotate(Piece_n::Piece_c& piece, const Piece_n::Direction_e& rotate_direction);

      // \Name: shootout
      // \Description:
      // - initiates a shootout event for both players
      // \Argument:
      // - none
      // \Returns
      // - none
      void shootout();

      // \Name: currentPlayer
      // \Description:
      // - returns the player whose turn it is
      // \Argument:
      // - none
      // \Returns
      // - Player_s&, the player whose turn it is
      Player_s& currentPlayer();

      // \Name: nextPlayer
      // \Description:
      // - passes the turn to the player whose turn it is not
      // \Argument:
      // - none
      // \Returns
      // - none
      void nextPlayer();

      // \Name: currentPiece
      // \Description:
      // - returns the currently seleced piece
      // \Argument:
      // - none
      // \Returns
      // - Piece_c&, the currently selected piece
      Piece_c& currentPiece();

      // \Name: setMove
      // \Description:
      // - save of the data about a possible move
      // \Argument:
      // - none
      // \Returns
      // - none
      void setMove();

      // \Name: revertMove
      // \Description:
      // - reverts a moved piece to its previous position
      // \Argument:
      // - none
      // \Returns
      // - none
      void revertMove();

   protected:
      // \Name: drawPieces
      // \Description:
      // - draws the Piece_c objects in their proper places
      // \Argument:
      // - none
      // \Returns
      // - none     
      void drawPieces(); 

      // \Name: detectHit
      // \Description:
      // - determines if a piece hits during a shootout
      // \Argument:
      // - none
      // \Returns
      // - none     
      void detectHit(); 

      // \Name: gameOver
      // \Description:
      // - checks for a win state from either player
      // \Argument:
      // - none
      // \Returns
      // - bool, true if the current player is the winner
      bool gameOver();

      // stores the app's ResourceManager_c
      ResourceManager_n::ResourceManager_c mResourceManager;

      // stores references to the two Player_s objects initialized on construction
      Player_s& mPlayer1;
      Player_s& mPlayer2;

      // the current state of the game
      GameState_s mGameState;
   };
}

#endif _GAME_HPP_