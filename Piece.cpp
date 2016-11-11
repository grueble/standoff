 #include <math>

#include "Piece.hpp"

using namespace Piece_n;

// constructor
Piece_c::Piece_c(Player_c& player_owner) :
   mPlayer(player_owner),
   mPlayState(PlayState_e::RESERVE)
{
}

// default destructor
Piece_c::~Piece_c()
{
}

std::pair<int, int> Piece_c::getPosition()
{
   if (mPosition)
   {
      return mPosition;
   }
   else 
   {
      // fire and error
   }
}

void Piece_c::setPosition(const std::pair<int, int> new_position)
{
   if (!mPosition)
   {
      if (this.isValidDeployment(new_position))
      {
         mPosition = new_position;
      }
      else 
      {
         // In an app implementation, this will be a warning UI pop-up
         std::cout << "Invalid Deployment" << std::endl;
      }
   }
   else 
   {
      if (this.isValidMove(new_position))
      {
         mPosition = new_position;
      }
      else
      {
         // In an app implementation, this will be a warning UI pop-up
         std::cout << "Invalid Move" << std::endl;
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