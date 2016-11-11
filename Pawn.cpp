#include "Pawn.hpp"

using namespace Piece_n;

Pawn_c::Pawn_c(Player_c& player_owner) :
   Piece_c(player_owner)
{
   mDeploymentZones = DEPLOYMENT_ZONES;
}

Pawn_c::~Pawn_c()
{
}

void Pawn_c::setDirection(const Direction_e& new_direction)
{
   // should never be called, fire an error
   return;
}