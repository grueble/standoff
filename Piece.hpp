#ifndef _PIECE_HPP_
#define _PIECE_HPP_

#include <pair>

#include "Player.hpp"

namespace Piece_n
{
   // the possible directions a piece could be facing
   enum Direction_e
   {
      UP = 0,
      RIGHT = 1,
      DOWN = 2,
      LEFT = 3
   };

   // the in-play status for any particular piece
   enum PlayState_e
   {
      RESERVE = 0,
      LIVE = 1,
      DEAD = 2
   };

   class Piece_c
   {
   public:
      // constructor
      Piece_c(Player_c& player_owner);

      // default destructor
      ~Piece_c();

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
      void setPosition(std::pair<int, int> const& new_position);

      // \Name: getDirection
      // \Description:
      // - returns the mDirection member field
      // \Argument:
      // - none
      // \Returns
      // - Direction_e, this Piece's direction
      const virtual Direction_e& getDirection() = 0;

      // May remove this, only guns and slinger have direction
      // \Name: setDirection
      // \Description:
      // - sets the mDirection member field
      // \Argument:
      // - Direction_e, the new direction to set
      // \Returns
      // - none 
      virtual void setDirection(const Direction_e& new_direction) = 0;

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
      // reference to the Player that owns this Piece
      Player_c &mPlayer;

      // this Piece's current board position, or null if it is not currently in play
      std::pair<int, int> mPosition;

      // stores a Piece's available deployment zones
      // -> these are initialized differently based on Piece type
      std::vector<std::pair<int, int>> mDeploymentZones;

      // this Piece's current in-play status
      PlayState_e mPlayState;
   };
}

#endif _PIECE_HPP_