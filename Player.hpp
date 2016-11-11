#ifndef _PLAYER_HPP_
#define _PLAYER_HPP_

#include <vector>

#include "Game.hpp"
#include "Piece.hpp"

namespace Player_n
{
   class Player_c
   {
   public:
      // default constructor
      Player_c();

      // default destructor
      ~Player_c();

      // \Name: getReservePieces
      // \Description:
      // - returns a vector of this player's reserve pieces
      // \Argument:
      // - none
      // \Returns
      // - std::vector<Piece>&, the address of this player's reserve pieces
      std::vector<Piece>& getReservePieces();

      // \Name: getLivePieces
      // \Description:
      // - returns a vector of this player's live pieces
      // \Argument:
      // - none
      // \Returns
      // - std::vector<Piece>&, the address of this player's live pieces
      std::vector<Piece>& getLivePieces();

      // \Name: getDeadPieces
      // \Description:
      // - returns a vector of this player's dead pieces
      // \Argument:
      // - none
      // \Returns
      // - std::vector<Piece>&, the address of this player's dead pieces
      std::vector<Piece>& getDeadPieces();

      // \Name: deploy
      // \Description:
      // - deploys a Piece from this Player's reserves
      // \Argument:
      // - none
      // \Returns
      // - bool, true if the deployment is successful, false o/w
      bool deploy(Piece_c& reserve_piece, const std::pair<int, int>& deploy_position);

      // \Name: move
      // \Description:
      // - moves one of this Player's live pieces to a new position
      // \Argument:
      // - none
      // \Returns
      // - bool, true if the move is successful, false o/w
      bool move(Piece_c& live_piece, const std::pair<int, int>& move_position);

      // \Name: rotate
      // \Description:
      // - initiates a shootout event
      // \Argument:
      // - none
      // \Returns
      // - bool, true if the rotation is successful, false o/w
      bool rotate(Piece_c& live_piece, const Piece_n::Direction_e& rotate_direction);

      // \Name: initShootout
      // \Description:
      // - initiates a shootout event
      // \Argument:
      // - none
      // \Returns
      // - none
      void initShootout();

   protected:
      // \Name: initPieces
      // \Description:
      // - initializes mReservePieces; called prior to game start
      // \Argument:
      // - none
      // \Returns
      // - none
      void initPieces();

      // piece vectors
      std::vector<Piece_c> mPieces;

      // ID of the user that controls this Player
      const int userId;

      // game object that owns this Player
      Game_c& mGame;
   };
}

#endif _PLAYER_