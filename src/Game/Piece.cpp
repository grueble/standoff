#include <cmath>

#include "Piece.hpp"

using namespace Piece_n;

Piece_c::Piece_c(const PieceType_e& piece_type, const std::pair<int, int> reserve_position) :
   mPieceType(piece_type),
   mPosition(reserve_position),
   mDirection(Direction_e::NONE),
   mPlayState(PlayState_e::RESERVE),
   mTeam(team)
{
   if (piece_type == PAWN)
   {
      mDeploymentZones = PAWN_DEPLOYMENT_ZONES;  
   }
   else
   {
      mDeploymentZones = GUNSLINGER_DEPLOYMENT_ZONES;
   }
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

void Piece_c::setPosition(const std::pair<int, int>& new_position) 
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
         // maybe just ~Piece_c()
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

bool Piece_c::isValidDeployment(const std::pair<int, int>& deploy_position)
{
   std::vector<std::pair<int, int>>::iterator it;
   for (it = mDeploymentZones.begin(); it != mDeploymentZones.end(); ++it)
   {
      if (*it = deploy_position)
      {
         return true;
      }
   }
   
   return false;
}