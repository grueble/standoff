#ifndef _GAME_HPP_
#define _GAME_HPP_

#include <map>
#include <utility>

#include "Piece.hpp"
#include "ResourceManager/ResourceManager.hpp"

namespace Game_n
{
   // the width of (both) the border tiles (and the margin)
   static const int TILE_WIDTH = 60; 

   // the board's upper left hand corner in "screen tile" coordinates
   // -> the window's upper left hand corner is at (0, 0)
   static const std::pair<int,int> BOARD_COORD = std::make_pair(1, 4); 

   // starting "screen tile" coordinates for reserve piece positions
   static const std::pair<int,int> PLAYER_1_RESERVE_COORD = std::make_pair(9, 14);
   static const std::pair<int,int> PLAYER_2_RESERVE_COORD = std::make_pair(1, 3);

   // the board's side length
   static const int BOARD_SIDE_LENGTH = 9;

   // stores all of the data about a player
   struct Player_s
   {
      int mUserId;
      std::vector<Piece_n::Piece_c> mPieces;
   };

   class Game_c
   {
   public:
      // constructor
      Game_c(const ResourceManager_n::ResourceManager_c& resource_manager);

      // default destructor
      ~Game_c();

      // \Name: move
      // \Description:
      // - moves a piece
      // \Argument:
      // - std::pair<int, int>&, the click position in screen tile coordinates
      // \Returns
      // - none
      void move(const std::pair<int, int>& screen_tile_coord);

      // \Name: rotate
      // \Description:
      // - initiates a shootout event
      // \Argument:
      // - Piece_c&, the piece to rotate
      // - Direction
      // \Returns
      // - bool, true if the rotation is successful, false o/w
      bool rotate(const Piece_n::Direction_e& rotate_direction);

      // \Name: shootout
      // \Description:
      // - initiates a shootout event for both players
      // \Argument:
      // - none
      // \Returns
      // - none
      void shootout();

      // \Name: getCurrentPlayer
      // \Description:
      // - returns the player whose turn it is
      // \Argument:
      // - none
      // \Returns
      // - Player_s&, the player whose turn it is
      Player_s& getCurrentPlayer();

      // \Name: nextPlayer
      // \Description:
      // - passes the turn to the player whose turn it is not
      // \Argument:
      // - none
      // \Returns
      // - none
      void nextPlayer();

      // \Name: getCurrentPiece
      // \Description:
      // - returns the currently seleced piece
      // \Argument:
      // - none
      // \Returns
      // - Piece_c&, the currently selected piece
      Piece_c& getCurrentPiece();

      // \Name: emptyCurrentPiece
      // \Description:
      // - sets the currently seleced piece
      // \Argument:
      // - Piece_c&, the piece to set as current
      // \Returns
      // - none
      void emptyCurrentPiece();

      // \Name: getShootoutFlag
      // \Description:
      // - returns the shootout flag
      // \Argument:
      // - none
      // \Returns
      // - bool&, the shootout flag
      const bool& getShootoutFlag();

      // \Name: setShootoutFlag
      // \Description:
      // - sets the shootout flag to true
      // \Argument:
      // - none
      // \Returns
      // - none
      void setShootoutFlag();

      // \Name: revertMove
      // \Description:
      // - reverts a moved piece to its previous position
      // \Argument:
      // - none
      // \Returns
      // - none
      void revertMove();

   protected:
      // \Name: initPieces
      // \Description:
      // - initializes a player's pieces
      // \Argument:
      // - Player_s&, the player to initialize pieces for 
      // - bool, true if player is mPlayer1, false o/w
      // \Returns
      // - none
      void initPieces(Player_s& player, bool player1);

      // \Name: destroyPieces
      // \Description:
      // - frees a player's pieces
      // \Argument:
      // - Player_s& the player to destroy pieces for
      // \Returns
      // - none
      void destroyPieces(Player_s& player);

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
      Player_s mPlayer1;
      Player_s mPlayer2;

      // the current player
      Player_s mCurrentPlayer;

      // pointer to the current piece, if any
      Piece_n::Piece_c* mCurrentPiece;

      // pointer to the piece staged for moving, if any
      Piece_n::Piece_c* mMovedPiece;

      // stores the previous state of the piece staged for moving in case a revert is made
      Piece_n::Piece_c mPreMovePiece;

      // true if a shootout has been called, false o/w
      bool mShootoutFlag;

      // true if a move is currently staged, false o/w
      bool mMoveFlag;
   };
}

#endif _GAME_HPP_