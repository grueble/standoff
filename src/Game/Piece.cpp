#include <cmath>

#include "Piece.hpp"

using namespace Piece_n;

Piece_c::Piece_c(const PieceType_e& piece_type, 
                 const std::vector<std::pair<int, int>>& deployment_zones) :
   //mPieceType(piece_type),
   mPosition(std::make_pair(-1, -1)),
   mDirection(Direction_e::NONE),
   mDeploymentZones(deployment_zones),
   mPlayState(PlayState_e::RESERVE)
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

void Piece_c::setPosition(const std::pair<int, int>& new_position)
{
   if (mPosition.first != -1 || mPosition.second != -1)
   {
      if (this->isValidDeployment(new_position))
      {
         mPosition = new_position;
      }
      else 
      {
         // invalid deployment
      }
   }
   else 
   {
      if (this->isValidMove(new_position))
      {
         mPosition = new_position;
      }
      else
      {
         // invalid move
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
   switch (this->getPlayState())
   {
      case PlayState_e::RESERVE:
      {
         mPlayState = PlayState_e::LIVE;
         break;
      }
      case PlayState_e::LIVE:
      {
         mPlayState = PlayState_e::DEAD;
         break;
      }

      // no case for PlayState_e::DEAD; there is no next legal play state

      default:
      {
         break;
      }
   }
}

bool Piece_c::isValidDeployment(const std::pair<int, int>& deploy_position)
{
   std::vector<std::pair<int, int>>::iterator it;
   for (it = mDeploymentZones.begin(); it != mDeploymentZones.end(); ++it)
   {
      if (*it == deploy_position)
      {
         return true;
      }
   }
   
   return false;
}

bool Piece_c::isValidMove(const std::pair<int, int>& move_position)
{
   if (abs(mPosition.first - move_position.first <= 1))
   {
      if (abs(mPosition.second - move_position.second <= 1))
      {
         return true;
      }
   }

   return false;
}