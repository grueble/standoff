#include "Player.hpp"

static const int NUM_PAWNS = 4;
static const int NUM_GUNS = 6;
static const int NUM_SLINGERS = 2;

using namespace Player_n;

// Constructor
Player_c::Player_c(void)
{
   init_pieces();
}

// Destructor
Player_c::~Player_c(void)
{
}

// GetReservePieces
std::vector<Piece>& getReservePieces(void)
{
   return mReservePieces;
}

// GetLivePieces
std::vector<Piece>& getLivePieces(void)
{
   return mLivePieces;
}

// GetDeadPieces
std::vector<Piece>& getDeadPieces(void)
{
   return mDeadPieces;
}

void shoot(void)
{

}

void initPieces(void)
{
   // add pawns
   for (int i = 0; i < NUM_PAWNS; ++i)
   {
      Pawn new_pawn = new Pawn();
      mReservePieces.push_back(new_pawn);
   }

   // add guns
   for (int i = 0; i < NUM_GUNS; ++i)
   {
      Gun new_gun = new Gun(); 
      mReservePieces.push_back(new_gun);
   }

   // add slingers
   for (int i = 0; i < NUM_SLINGERS; ++i)
   {
      Slinger new_slinger = new Slinger();
      mReservePieces.push_back(new_slinger);
   }
}
