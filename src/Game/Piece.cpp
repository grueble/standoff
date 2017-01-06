#include <cmath>

#include "Piece.hpp"

using namespace Piece_n;

Piece_c::Piece_c(const PieceType_e& piece_type) :
   mPieceType(piece_type) 
   mPosition(std::make_pair(-1, -1)),
   mDirection(Direction_e::NONE),
   // mDeploymentZones(deployment_zones),
   mPlayState(PlayState_e::RESERVE),
   mTeam(team)
{
}

Piece_c::~Piece_c()
{
}

const PieceType_e& Piece_c::getPieceType()
{
   return mPieceType;
}

const std::pair<int, int>& Piece_c::getPosition()
{
   return mPosition;
}

void Piece_c::setPosition(const int& new_position_x, const int& new_position_y) 
{
   switch (mPlayState)
   {
      case RESERVE :
      {
         if (this->isValidDeployment(new_position))
         {
            mPosition = new_position;
         }
         else 
         {
            // invalid deployment
         }  
         break;
      }
      case LIVE :
      {
         if (this->isValidMove(new_position))
         {
            mPosition = new_position;
         }
         else
         {
            // invalid move
         }  
         break;
      }

      // no case for PlayState_e::DEAD; cannot set the position of a dead piece

      default :
      {
         // unrecognized PlayState_e
         break;
      }
   }
}

const Direction_e& Piece_c::getDirection()
{
   return mDirection;
}

void Piece_c::setDirection(const Direction_e& new_direction)
{
   if (mDirection != new_direction)
   {
      mDirection = new_direction;
   }
}

const PlayState_e& Piece_c::getPlayState()
{
   return mPlayState;
}

void Piece_c::nextPlayState()
{
   switch (mPlayState)
   {
      case RESERVE :
      {
         mPlayState = LIVE;
         break;
      }
      case LIVE :
      {
         mPlayState = DEAD;
         break;
      }

      // no case for PlayState_e::DEAD; there is no legal next play state

      default :
      {
         // unrecognized PlayState_e
         break;
      }
   }
}

bool Piece_c::isValidDeployment(const int& deploy_x, const int& deploy_y)
{
   std::vector<std::pair<int, int>>::iterator it;
   for (it = mDeploymentZones.begin(); it != mDeploymentZones.end(); ++it)
   {
      if (it->x == deploy_x && it->y == deploy_y)
      {
         return true;
      }
   }
   
   return false;
}

bool Piece_c::isValidMove(const int& move_x, const int& move_y)
{
   if (abs(mPosition.first - move_x <= 1))
   {
      if (abs(mPosition.second - move_y <= 1))
      {
         return true;
      }
   }

   return false;
}