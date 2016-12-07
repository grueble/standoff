#ifndef _PIECE_HPP_
#define _PIECE_HPP_

#include <vector>
#include <utility>

// #include "Player.hpp"

namespace Piece_n
{
   // the possible types of pieces
   enum PieceType_e
   {
      PAWN = 0,
      GUN = 1,
      SLINGER = 0
   };

   // the possible directions a piece could be facing
   enum Direction_e
   {
      UP = 0,
      RIGHT = 1,
      DOWN = 2,
      LEFT = 3,
      NONE = 4
   };

   // the in-play status for any particular piece
   enum PlayState_e
   {
      RESERVE = 0,
      LIVE = 1,
      DEAD = 2,
   };

   class Piece_c
   {
   public:
      // constructor
      // \Parameters:
      // - Piecetype_e& the type of Piece to create
      // - std::vector<std::pair<int, int>>& a list of valid deployments for this Piece
      Piece_c(const PieceType_e& piece_type, 
              const std::vector<std::pair<int, int>>& deployment_zones); 

      // default destructor
      ~Piece_c();

      // \Name: getPiece_type
      // \Description:
      // - returns the mPieceType member field
      // \Argument:
      // - none
      // \Returns
      // - PieceType_& this Piece's type
      const PieceType_e& getPieceType();

      // \Name: getPosition
      // \Description:
      // - returns the mPosition member field
      // \Argument:
      // - none
      // \Returns
      // - std::pair<int, int>&, this Piece's position
      const std::pair<int, int>& getPosition();

      // \Name: setPosition
      // \Description:
      // - sets the mPosition member field 
      // \Argument:
      // - std::pair<int, int>, the new position to set
      // \Returns
      // - none
      void setPosition(const std::pair<int, int>& new_position);

      // \Name: getDirection
      // \Description:
      // - returns the mDirection member field
      // \Argument:
      // - none
      // \Returns
      // - Direction_e, this Piece's direction
      const Direction_e& getDirection();

      // \Name: setDirection
      // \Description:
      // - sets the mDirection member field
      // \Argument:
      // - Direction_e, the new direction to set
      // \Returns
      // - none 
      void setDirection(const Direction_e& new_direction);

      // \Name: getPlayState
      // \Description:
      // - returns the mPlayerState member field
      // \Argument:
      // - none
      // \Returns
      // - PlayState_e, this Piece's play state
      const PlayState_e& getPlayState();

      // \Name: nextPlayState
      // \Description:
      // - sets the mPlayState member field to the next legal play state
      // \Argument:
      // - none
      // \Returns
      // - none
      void nextPlayState();

      // \Name: isValidDeployment
      // \Description:
      // - determines if the input deployment is valid
      // \Argument:
      // - std::pair<int, int>, the deployment position to check
      // \Returns
      // - bool, true if the deployment is valid; false o/w
      bool isValidDeployment(const std::pair<int, int>& deploy_position);

      // \Name: isValidMove
      // \Description:
      // - returns the address of this player's reserve pieces
      // \Argument:
      // - std::pair<int, int>, the move position to check
      // \Returns
      // - bool, true if the move is valid; false o/w
      bool isValidMove(const std::pair<int, int>& move_position);

   protected:
      // this Piece's type
      PieceType_e mPieceType;

      // this Piece's current board position, or null if it is not currently in play
      std::pair<int, int> mPosition;

      // the direction this Piece is currently facing
      // -> always NONE for PieceType_e::PAWN
      // -> represents 'primary' direction for Piece_Type::SLINGER
      Direction_e mDirection;

      // stores a Piece's available deployment zones
      // -> these are initialized differently based on PieceType
      std::vector<std::pair<int, int>> mDeploymentZones;

      // this Piece's current in-play status
      PlayState_e mPlayState;
   };
}

#endif _PIECE_HPP_