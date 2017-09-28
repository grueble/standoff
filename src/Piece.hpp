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
      Piece_c(const PieceType_e& piece_type,
              const std::vector<std::pair<int, int> > deployment_zones,
              const std::pair<int, int> reserve_position,
              const Team_e& team);

      // default destructor
      ~Piece_c();

      // returns the mPieceType member field
      const PieceType_e& getPieceType() const;

      // returns the mPosition member field
      const std::pair<int, int>& getPosition();

      // sets the mPosition member field 
      void setPosition(const std::pair<int, int>& new_position);

      // returns the mDirection member field
      const Direction_e& getDirection() const;

      // sets the mDirection member field
      void setDirection(const Direction_e& new_direction);

      // returns the mPlayerState member field
      const PlayState_e& getPlayState();

      // sets the mPlayState member field to the next legal play state
      void nextPlayState();

      // sets the mPlayState member field to a particular play state
      void setPlayState(const PlayState_e& play_state);

      // returns the mPieceType member field
      const Team_e& getTeam() const;

      // determines if the input deployment is valid
      bool isValidDeployment(const std::pair<int, int>& deploy_position);

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
      std::vector<std::pair<int, int> > mDeploymentZones;

      // this Piece's current in-play status
      PlayState_e mPlayState;

      // this Piece's allegiance
      Team_e mTeam;
   };
}

#endif
