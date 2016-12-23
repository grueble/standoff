#ifndef _PLAYER_HPP_
#define _PLAYER_HPP_

#include <vector>

#include "Piece.hpp"

namespace Player_n
{
   // deployment zone vectors used in piece subtype initialization
   static const std::vector<std::pair<int, int>> PAWN_DEPLOYMENT_ZONES = {};
   static const std::vector<std::pair<int, int>> GUN_DEPLOYMENT_ZONES = {};
   static const std::vector<std::pair<int, int>> SLINGER_DEPLOYMENT_ZONES = {};

   class Player_c
   {
   public:
      // constructor
      Player_c(); // const int& user_id);

      // default destructor
      ~Player_c();

      // \Name: getPieces
      // \Description:
      // - returns a vector of this player's pieces
      // \Argument:
      // - none
      // \Returns
      // - std::vector<Piece_c>&, the address of this player's reserve pieces
      std::vector<Piece_n::Piece_c>& getPieces();

      // \Name: getReservePieces
      // \Description:
      // - returns a vector of this player's reserve pieces
      // \Argument:
      // - none
      // \Returns
      // - std::vector<Piece_c>&, the address of this player's reserve pieces
      std::vector<Piece_n::Piece_c>& getReservePieces();

      // \Name: getLivePieces
      // \Description:
      // - returns a vector of this player's live pieces
      // \Argument:
      // - none
      // \Returns
      // - std::vector<Piece_c>&, the address of this player's live pieces
      std::vector<Piece_n::Piece_c>& getLivePieces();

      // \Name: getDeadPieces
      // \Description:
      // - returns a vector of this player's dead pieces
      // \Argument:
      // - none
      // \Returns
      // - std::vector<Piece_c>&, the address of this player's dead pieces
      std::vector<Piece_n::Piece_c>& getDeadPieces();

      // \Name: deploy
      // \Description:
      // - deploys a Piece from this Player's reserves
      // \Argument:
      // - none
      // \Returns
      // - bool, true if the deployment is successful, false o/w
      // bool deploy(Piece_n::Piece_c& piece, const std::pair<int, int>& deploy_position);

      // \Name: move
      // \Description:
      // - moves one of this Player's live pieces to a new position
      // \Argument:
      // - Piece_c&, the piece to move
      // - const int&, the piece's new x coordinate
      // - const int&, the piece's new y coordinate
      // \Returns
      // - bool, true if the move is successful, false o/w
      bool move(Piece_n::Piece_c& piece, const int& x, const int& y); // std::pair<int, int>& move_position);

      // \Name: rotate
      // \Description:
      // - initiates a shootout event
      // \Argument:
      // - Piece_c&, the piece to rotate
      // - Direction
      // \Returns
      // - bool, true if the rotation is successful, false o/w
      bool rotate(Piece_n::Piece_c& piece, const Piece_n::Direction_e& rotate_direction);

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
      // - initializes mPieces; called prior to game start
      // \Argument:
      // - none
      // \Returns
      // - none
      void initPieces();

      // \Name: destroyPieces
      // \Description:
      // - destroys the contensts of mPieces; called by the destructor
      // \Argument:
      // - none
      // \Returns
      // - none
      void destroyPieces();

      // piece vector
      std::vector<Piece_n::Piece_c> mPieces;

      // ID of the user that controls this Player
      const int mUserId;
   };
}

#endif _PLAYER_