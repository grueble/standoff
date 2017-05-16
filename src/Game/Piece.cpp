#include "Piece.hpp"
#include <cmath>
#include <iostream>

using namespace Piece_n;

Piece_c::Piece_c(const PieceType_e& piece_type, 
                 const std::vector<std::pair<int,int>> deployment_zones,
                 const std::pair<int, int> reserve_position,
                 const Team_e& team) :
   mPieceType(piece_type),
   mPosition(reserve_position),
   mDirection(Direction_e::NONE),
   mDeploymentZones(deployment_zones),
   mPlayState(PlayState_e::RESERVE),
   mTeam(team)
{
   // printf("mD=%d", deployment_zones.size());
   // mDeploymentZones = deployment_zones;
}

Piece_c::~Piece_c()
{
}

const PieceType_e& Piece_c::getPieceType() const
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
         // move validity is checked by the caller
         mPosition = new_position;
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

const Direction_e& Piece_c::getDirection() const
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

void Piece_c::setPlayState(const PlayState_e& play_state)
{
   mPlayState = play_state;
}

const Team_e& Piece_c::getTeam() const
{
   return mTeam;
}

bool Piece_c::isValidDeployment(const std::pair<int, int>& deploy_position)
{
   // printf("TRY: { %d, %d } w/ l=%d\n", deploy_position.first, deploy_position.second, mDeploymentZones.size());
   std::vector<std::pair<int, int>>::iterator it;
   for (it = mDeploymentZones.begin(); it != mDeploymentZones.end(); ++it)
   {
      // printf("{ %d, %d }", it->first, it->second);
      if (deploy_position == *it)
      {
         // printf("\n");
         return true;
      }
   }
   
   return false;
}
