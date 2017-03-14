#ifndef _PIECE_HPP_
#define _PIECE_HPP_

#include <vector>
#include <utility>

namespace Piece_n
{
   // the possible types of pieces
   enum PieceType_e
   {
      PAWN = 0,
      GUN = 1,
      SLINGER = 2,
      EMPTY = 3
   };

   // the possible directions a piece could be facing
   enum Direction_e
   {
      UP = 0,
      DOWN = 1,
      LEFT = 2,
      RIGHT = 3,
      NONE = 4
   };

   // the in-play status for any particular piece
   enum PlayState_e
   {
      RESERVE = 0,
      LIVE = 1,
      DEAD = 2,
   };

   // the allegiance of a particular piece
   enum Team_e
   {
      PLAYER_ONE = 0,
      PLAYER_TWO = 1
   };

   class Piece_c
   {
   public: 
      // constructor
      // \Parameters:
      // - Piecetype_e& the type of Piece to create
      // - std::vector<std::pair<int, int>>& a list of valid deployments for this Piece
      Piece_c(const PieceType_e& piece_type,
              const std::vector<std::pair<int, int>>& deployment_zones,
              const std::pair<int, int> reserve_position,
              const Team_e& team);

      // default destructor
      ~Piece_c();

      // \Name: getPieceType
      // \Description:
      // - returns the mPieceType member field
      // \Argument:
      // - none
      // \Returns
      // - PieceType_& this Piece's type
      const PieceType_e& getPieceType() const;

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
      // - int&, the new position's x coordinate
      // - int&, the new position's y coordinate
      // \Returns
      // - bool, true on a success, false o/w
      void setPosition(const std::pair<int, int>& new_position);

      // \Name: getDirection
      // \Description:
      // - returns the mDirection member field
      // \Argument:
      // - none
      // \Returns
      // - Direction_e, this Piece's direction
      const Direction_e& getDirection() const;

      // \Name: setDirection
      // \Description:
      // - sets the mDirection member field
      // \Argument:
      // - Direction_e, the new direction to set
      // \Returns
      // - bool, true on a success, false o/w 
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

      void setPlayState(const PlayState_e& play_state);

      // \Name: getTeam
      // \Description:
      // - returns the mPieceType member field
      // \Argument:
      // - none
      // \Returns
      // - PieceType_& this Piece's type
      const Team_e& getTeam() const;

      // \Name: isValidDeployment
      // \Description:
      // - determines if the input deployment is valid
      // \Argument:
      // - int&, the proposed deployment position's x coordinate
      // - int&, the proposed deployment position's y coordinate
      // \Returns
      // - bool, true if the deployment is valid; false o/w
      bool isValidDeployment(const std::pair<int, int>& deploy_position);

      // \Name: isValidMove
      // \Description:
      // - determines if the input move is valid
      // \Argument:
      // - int&, the proposed move position's x coordinate
      // - int&, the proposed move position's y coordinate
      // \Returns
      // - bool, true if the move is valid; false o/w
      // bool isValidMove(const std::pair<int, int>& move_position);

   private:
      // this Piece's type
      PieceType_e mPieceType;

      // this Piece's current position on screen 
      // -> NULL on initialization
      // -> position is based on abstract "screen tile" coordinates
      // -> set by Game_c according to owning Player_c's position relative to the board
      std::pair<int, int> mPosition;

      // the direction this Piece is currently facing
      // -> always NONE for PieceType_e::PAWN
      // -> represents 'primary' direction for Piece_Type::SLINGER
      Direction_e mDirection;

      // stores a Piece's available deployment zones
      // -> these are initialized differently based on PieceType
      // -> reference to one of three shared deployment zone vectors in Game_c
      std::vector<std::pair<int, int>> mDeploymentZones;

      // this Piece's current in-play status
      PlayState_e mPlayState;

      // this Piece's allegiance
      Team_e mTeam;
   };
}

#endif _PIECE_HPP_