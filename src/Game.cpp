#include "Game.hpp"
#include "Piece.hpp"
#include <vector>
#include <iostream>
#include <cmath>

using namespace Game_n;

const int& Game_c::BOARD_SIDE_LENGTH = 9;
const std::pair<int,int>& Game_c::P1_RESERVE_COORD = std::make_pair(15, 9);
const std::pair<int,int>& Game_c::P2_RESERVE_COORD = std::make_pair(3, 1);
const std::pair<int, int>& Game_c::P1_SCORING_COORD = std::make_pair(13, 9);
const std::pair<int, int>& Game_c::P2_SCORING_COORD = BOARD_COORD;
const std::pair<int, int>& Game_c::BRIEFCASE_COORD = std::make_pair(9, 5);
const int Game_c::NUM_PAWNS = 4;
const int Game_c::NUM_GUNS = 6;
const int Game_c::NUM_SLINGERS = 2;

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// - Constructor and Destructor
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Game_c::Game_c() :
   mCurrentTurn(1),
   mCurrentPiece(nullptr),
   mBriefcasePosition(BRIEFCASE_COORD)
{
   initPieces2();
}

// Game_c::~Game_c()
// {
//    mPlayer1Pieces.clear();
//    mPlayer2Pieces.clear();
// }

Game_c::~Game_c()
{
   std::vector<PiecePtr>::iterator p1_it;
   for (p1_it = mPlayer1Pieces.begin(); p1_it != mPlayer1Pieces.end(); ++p1_it)
   {
      delete (*p1_it);
   }

   std::vector<PiecePtr>::iterator p2_it;
   for (p2_it = mPlayer2Pieces.begin(); p2_it != mPlayer2Pieces.end(); ++p2_it)
   {
      delete (*p2_it);
   }
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// - move
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Game_c::move(const std::pair<int, int>& screen_tile_coord)
{
   std::cout << "stc: { " << screen_tile_coord.first << " " << screen_tile_coord.second << " }" << std::endl;
   std::cout << "pos: { " << mCurrentPiece->getPosition().first << " " << mCurrentPiece->getPosition().second << " }" << std::endl;

   // if the mouse click event's "screen tile" coordinate is within the board's bounds...
   if (screen_tile_coord.first >= BOARD_COORD.first &&
       screen_tile_coord.first < BOARD_COORD.first + BOARD_SIDE_LENGTH &&
       screen_tile_coord.second >= BOARD_COORD.second &&
       screen_tile_coord.second < BOARD_COORD.second + BOARD_SIDE_LENGTH)
   {
      // if the move is within one tile of the current piece's starting position...
      if ((abs(screen_tile_coord.first - mCurrentPiece->getPosition().first) <= 1 &&
           abs(screen_tile_coord.second - mCurrentPiece->getPosition().second) <= 1) ||
           mCurrentPiece->getPlayState() == Piece_n::RESERVE) 
      {
         // the briefcase moves with any piece that starts a move from its position
         if (mCurrentPiece->getPosition() == BRIEFCASE_COORD) 
         { 
            mBriefcasePosition = screen_tile_coord; 
         }
   
         // a piece moved from reserve waits for a direction
         if (mCurrentPiece->getPlayState() == Piece_n::RESERVE) 
         {
            if (mCurrentPiece->isValidDeployment(screen_tile_coord))
            {
               mCurrentPiece->nextPlayState();
               if (mCurrentPiece->getPieceType() != Piece_n::PAWN)
               {
                  mCurrentMove.mCurrentAction = PRE_DEPLOY;
               }
               else // unless that piece is a pawn
               {
                  mCurrentMove.mCurrentAction = DEPLOY;
               }
            }
         }
         else // live pieces cannot be rotated after a move
         {
            mCurrentMove.mCurrentAction = MOVE;
         }

         std::cout << "DOGY" << std::endl;
   
         // store data about the move
         mCurrentMove.mMovedPiece = mCurrentPiece;
         mCurrentMove.mPrevPosition = mCurrentPiece->getPosition();
         mCurrentPiece->setPosition(screen_tile_coord);

         std::cout << "pos: { " << mCurrentPiece->getPosition().first << " " << mCurrentPiece->getPosition().second << " }" << std::endl;
      }
   }
   else // invalid move
   {
       mCurrentPiece = nullptr;
   }
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// - rotate
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Game_c::rotate(const Piece_n::Direction_e& rotate_direction)
{
   // a piece is fully deployed after both a move and a rotation
   if (mCurrentMove.mCurrentAction == PRE_DEPLOY) 
   { 
      // std::cout << static_cast<int>(mCurrentPiece->getPlayState()) << std::endl;
      // mCurrentPiece->nextPlayState();
      mCurrentMove.mCurrentAction = DEPLOY;
   }
   // only rotate live pieces
   if (mCurrentPiece->getPlayState() == Piece_n::LIVE)
   {
      // if this isn't a deployment it's just a standard rotation
      if (mCurrentMove.mCurrentAction != DEPLOY)
      {
         mCurrentMove.mCurrentAction = ROTATE;
      }
      // store data about the move
      mCurrentMove.mMovedPiece = mCurrentPiece;
      mCurrentMove.mPrevDirection = mCurrentPiece->getDirection();
      mCurrentPiece->setDirection(rotate_direction);
   }
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// - State Control: nextPlayer, revertMove
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Game_c::nextPlayer()
{
   // null stored pointers
   mCurrentMove.mMovedPiece = nullptr;
   mCurrentPiece = nullptr;

   // reset current action
   mCurrentMove.mCurrentAction = NONE;

   // increment the turn
   mCurrentTurn++;
}

void Game_c::revertMove()
{
   switch (mCurrentMove.mCurrentAction)
   {
      case (PRE_DEPLOY):
      {
         mCurrentMove.mMovedPiece->setPosition(mCurrentMove.mPrevPosition);
         mCurrentMove.mMovedPiece->setPlayState(Piece_n::RESERVE);
         mCurrentMove.mMovedPiece = nullptr;
         break;
      }
      case (DEPLOY) :
      {
         mCurrentMove.mMovedPiece->setPosition(mCurrentMove.mPrevPosition);
         mCurrentMove.mMovedPiece->setDirection(mCurrentMove.mPrevDirection);
         mCurrentMove.mMovedPiece->setPlayState(Piece_n::RESERVE);
         mCurrentMove.mMovedPiece = nullptr;
         break;
      }
      case (MOVE) :
      {
         mCurrentMove.mMovedPiece->setPosition(mCurrentMove.mPrevPosition);
         mCurrentMove.mMovedPiece = nullptr;
         break;
      }
      case (ROTATE) :
      {
         mCurrentMove.mMovedPiece->setDirection(mCurrentMove.mPrevDirection);
         mCurrentMove.mMovedPiece = nullptr;
         break;
      }
      default :
      {
         break;
      }
   }

   if (mCurrentPiece != nullptr)
   {
      mCurrentPiece = nullptr;
   }

   mCurrentMove.mCurrentAction = NONE;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// - Getters and Setters
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

PiecePtr Game_c::getCurrentPiece()
{
   return mCurrentPiece;
}

void Game_c::setCurrentPiece(PiecePtr piece_ptr)
{
   mCurrentPiece = piece_ptr;
}

std::pair<int, int>& Game_c::getBriefcasePosition()
{
   return mBriefcasePosition;
}

const std::vector<PiecePtr>& Game_c::getPlayer1Pieces()
{
   return mPlayer1Pieces;
}

const std::vector<PiecePtr>& Game_c::getPlayer2Pieces()
{
   return mPlayer2Pieces;
}

Move_s& Game_c::getCurrentMove()
{
   return mCurrentMove;
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// - initPieces
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// void Game_c::initPieces()
// {
//    // initialize player 1's starting reserve position (screen tile coordinates)
//    std::pair<int, int> reserve_position = P1_RESERVE_COORD;

//    std::vector<std::pair<int, int>> p1_gunslinger_deployment_zones =
//    {
//       std::make_pair(6, 9), std::make_pair(8, 9), std::make_pair(12, 9),
//       std::make_pair(13, 2), std::make_pair(13, 4), std::make_pair(13, 8)
//    };

//    // create the set of valid deployment positions for player 1
//    // std::vector<std::pair<int, int>> p1_pawn_deployment_zones = P1_GUNSLINGER_DEPLOYMENT_ZONES;
//    // std::vector<std::pair<int, int>>::iterator p1_it;
//    // p1_it = p1_pawn_deployment_zones.begin();
//    // p1_it = p1_pawn_deployment_zones.insert(
//    //    p1_it,
//    //    {
//    //       std::make_pair(BOARD_COORD.first + 1, BOARD_COORD.second + 4),
//    //       std::make_pair(BOARD_COORD.first + 2, BOARD_COORD.second + 3),
//    //       std::make_pair(BOARD_COORD.first + 3, BOARD_COORD.second + 2),
//    //       std::make_pair(BOARD_COORD.first + 4, BOARD_COORD.second + 1)
//    //    }
//    // );

//    // add pawns
//    for (int i = 0; i < NUM_PAWNS; ++i)
//    {
//       mPlayer1Pieces.push_back(
//          std::make_shared<Piece_n::Piece_c>(
//             Piece_n::PAWN,
//             p1_gunslinger_deployment_zones,
//             reserve_position,
//             Piece_n::PLAYER_ONE));
//       reserve_position.second--;
//    }

//    // place guns on a new row
//    reserve_position = P1_RESERVE_COORD;
//    reserve_position.first++;

//    // add guns
//    for (int i = 0; i < NUM_GUNS; ++i)
//    {
//       mPlayer1Pieces.push_back(
//          std::make_shared<Piece_n::Piece_c>(
//             Piece_n::GUN,
//             p1_gunslinger_deployment_zones,
//             reserve_position,
//             Piece_n::PLAYER_ONE));
//       reserve_position.second--;
//    }

//    // place slingers on a new row
//    reserve_position = P1_RESERVE_COORD;
//    reserve_position.first = reserve_position.first + 2;

//    // add slingers
//    for (int i = 0; i < NUM_SLINGERS; ++i)
//    {
//       mPlayer1Pieces.push_back(
//          std::make_shared<Piece_n::Piece_c>(
//             Piece_n::SLINGER,
//             p1_gunslinger_deployment_zones,
//             reserve_position,
//             Piece_n::PLAYER_ONE));
//       reserve_position.second--;
//    }

//    // initialize player 2's starting reserve position (screen tile coordinates)
//    reserve_position = P2_RESERVE_COORD;

//    std::vector<std::pair<int, int>> p2_gunslinger_deployment_zones =
//    {
//       std::make_pair(6, 1), std::make_pair(10, 1), std::make_pair(12, 1),
//       std::make_pair(5, 2), std::make_pair(5, 6), std::make_pair(5, 8)
//    };

//    // create the set of valid deployment positions for player 2
//    // std::vector<std::pair<int, int>> p2_pawn_deployment_zones = P2_GUNSLINGER_DEPLOYMENT_ZONES;
//    // std::vector<std::pair<int, int>>::iterator p2_it;
//    // p2_it = p1_pawn_deployment_zones.begin();
//    // p2_it = p1_pawn_deployment_zones.insert(
//    //    p2_it, 
//    //    {
//    //       std::make_pair(BOARD_COORD.first + 4, BOARD_COORD.second + 7),
//    //       std::make_pair(BOARD_COORD.first + 5, BOARD_COORD.second + 5),
//    //       std::make_pair(BOARD_COORD.first + 6, BOARD_COORD.second + 6),
//    //       std::make_pair(BOARD_COORD.first + 7, BOARD_COORD.second + 4)
//    //    }
//    // );

//    // add pawns
//    for (int i = 0; i < NUM_PAWNS; ++i)
//    {
//       mPlayer2Pieces.push_back(
//          std::make_shared<Piece_n::Piece_c>(
//             Piece_n::PAWN,
//             p2_gunslinger_deployment_zones,
//             reserve_position,
//             Piece_n::PLAYER_TWO));
//       reserve_position.second++;
//    }

//    // place guns on a new row
//    reserve_position = P2_RESERVE_COORD;
//    reserve_position.first--;

//    // add guns
//    for (int i = 0; i < NUM_GUNS; ++i)
//    {
//       mPlayer2Pieces.push_back(
//          std::make_shared<Piece_n::Piece_c>(
//             Piece_n::GUN,
//             p2_gunslinger_deployment_zones,
//             reserve_position,
//             Piece_n::PLAYER_TWO));
//       reserve_position.second++;
//    }

//    // place slingers on a new row
//    reserve_position = P2_RESERVE_COORD;
//    reserve_position.first = reserve_position.first - 2;

//    // add slingers
//    for (int i = 0; i < NUM_SLINGERS; ++i)
//    {
//       mPlayer2Pieces.push_back(
//          std::make_shared<Piece_n::Piece_c>(
//             Piece_n::SLINGER,
//             p2_gunslinger_deployment_zones,
//             reserve_position,
//             Piece_n::PLAYER_TWO));
//       reserve_position.second++;
//    }
// }

void Game_c::initPieces2()
{
   std::pair<int, int> reserve_position = P1_RESERVE_COORD;

   std::vector<std::pair<int, int>> p1_gunslinger_deployment_zones =
   {
      std::make_pair(6, 9), std::make_pair(8, 9), std::make_pair(12, 9),
      std::make_pair(13, 2), std::make_pair(13, 4), std::make_pair(13, 8)
   };

   for (int i = 0; i < NUM_PAWNS; ++i)
   {
      mPlayer1Pieces.push_back(
         new Piece_n::Piece_c(
            Piece_n::PAWN,
            p1_gunslinger_deployment_zones,
            reserve_position,
            Piece_n::PLAYER_ONE));
      reserve_position.second--;
   }

   // place guns on a new row
   reserve_position = P1_RESERVE_COORD;
   reserve_position.first++;

   // add guns
   for (int i = 0; i < NUM_GUNS; ++i)
   {
      mPlayer1Pieces.push_back(
         new Piece_n::Piece_c(
            Piece_n::GUN,
            p1_gunslinger_deployment_zones,
            reserve_position,
            Piece_n::PLAYER_ONE));
      reserve_position.second--;
   }

   // place slingers on a new row
   reserve_position = P1_RESERVE_COORD;
   reserve_position.first = reserve_position.first + 2;

   // add slingers
   for (int i = 0; i < NUM_SLINGERS; ++i)
   {
      mPlayer1Pieces.push_back(
         new Piece_n::Piece_c(
            Piece_n::SLINGER,
            p1_gunslinger_deployment_zones,
            reserve_position,
            Piece_n::PLAYER_ONE));
      reserve_position.second--;
   }

   // initialize player 2's starting reserve position (screen tile coordinates)
   reserve_position = P2_RESERVE_COORD;

   std::vector<std::pair<int, int>> p2_gunslinger_deployment_zones =
   {
      std::make_pair(6, 1), std::make_pair(10, 1), std::make_pair(12, 1),
      std::make_pair(5, 2), std::make_pair(5, 6), std::make_pair(5, 8)
   };

   // add pawns
   for (int i = 0; i < NUM_PAWNS; ++i)
   {
      mPlayer2Pieces.push_back(
         new Piece_n::Piece_c(
            Piece_n::PAWN,
            p2_gunslinger_deployment_zones,
            reserve_position,
            Piece_n::PLAYER_TWO));
      reserve_position.second++;
   }

   // place guns on a new row
   reserve_position = P2_RESERVE_COORD;
   reserve_position.first--;

   // add guns
   for (int i = 0; i < NUM_GUNS; ++i)
   {
      mPlayer2Pieces.push_back(
         new Piece_n::Piece_c(
            Piece_n::GUN,
            p2_gunslinger_deployment_zones,
            reserve_position,
            Piece_n::PLAYER_TWO));
      reserve_position.second++;
   }

   // place slingers on a new row
   reserve_position = P2_RESERVE_COORD;
   reserve_position.first = reserve_position.first - 2;

   // add slingers
   for (int i = 0; i < NUM_SLINGERS; ++i)
   {
      mPlayer2Pieces.push_back(
         new Piece_n::Piece_c(
            Piece_n::SLINGER,
            p2_gunslinger_deployment_zones,
            reserve_position,
            Piece_n::PLAYER_TWO));
      reserve_position.second++;
   }
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// - shootout
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
void Game_c::shootout()
{
   // stores the indices of all pieces hit over the course of the shootout
   std::vector<std::pair<int, int>> hit_piece_index;

   std::vector<PiecePtr>::iterator p1_it;
   for (p1_it = mPlayer1Pieces.begin(); p1_it != mPlayer1Pieces.end(); ++p1_it)
   {
      if ((*p1_it)->getPlayState() == Piece_n::LIVE && (*p1_it)->getPieceType() != Piece_n::PAWN)
      {
         Piece_n::Direction_e direction = (*p1_it)->getDirection();
         hit_piece_index.push_back(
            std::make_pair(
               1, detectHit(**p1_it, direction, mPlayer2Pieces)));

         if ((*p1_it)->getPieceType() == Piece_n::SLINGER)
         {
            Piece_n::Direction_e secondary_direction;
            switch(direction)
            {
               case Piece_n::UP : { secondary_direction = Piece_n::RIGHT; }
               case Piece_n::DOWN : { secondary_direction = Piece_n::LEFT; }
               case Piece_n::LEFT : { secondary_direction = Piece_n::UP; }
               case Piece_n::RIGHT : { secondary_direction = Piece_n::DOWN; }
            }
            hit_piece_index.push_back(
               std::make_pair(
                  1, detectHit(**p1_it, secondary_direction, mPlayer2Pieces)));
         }
      }
   }
   std::vector<PiecePtr>::iterator p2_it;
   for (p2_it = mPlayer2Pieces.begin(); p2_it != mPlayer2Pieces.end(); ++p2_it)
   {
      if ((*p2_it)->getPlayState() == Piece_n::LIVE && (*p2_it)->getPieceType() != Piece_n::PAWN)
      {
         Piece_n::Direction_e direction = (*p2_it)->getDirection();
         hit_piece_index.push_back(
            std::make_pair(
               2, detectHit(**p2_it, direction, mPlayer1Pieces)));

         if ((*p2_it)->getPieceType() == Piece_n::SLINGER)
         {
            Piece_n::Direction_e secondary_direction;
            switch(direction)
            {
               case Piece_n::UP : { secondary_direction = Piece_n::RIGHT; }
               case Piece_n::DOWN : { secondary_direction = Piece_n::LEFT; }
               case Piece_n::LEFT : { secondary_direction = Piece_n::UP; }
               case Piece_n::RIGHT : { secondary_direction = Piece_n::DOWN; }
            }
            hit_piece_index.push_back(
               std::make_pair(
                  2, detectHit(**p2_it, secondary_direction, mPlayer1Pieces)));
         }
      }
   }

   std::vector<std::pair<int, int> >::iterator hit_it;
   for (hit_it = hit_piece_index.begin(); hit_it != hit_piece_index.end(); ++hit_it)
   {
      if (hit_it->second != -1)
      {
         if (hit_it->first == 1) // P1's hits
         {
            mPlayer2Pieces.at(hit_it->second)->nextPlayState();
         }
         else // P2's hits
         {
            mPlayer1Pieces.at(hit_it->second)->nextPlayState();         
         }
      }
   }
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// - detectHit
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
int Game_c::detectHit(Piece_n::Piece_c& piece, 
                      Piece_n::Direction_e direction,
                      std::vector<PiecePtr>& pieces)
{
   // true once if we have already hit a piece (monodirectional)
   bool hit_piece_flag = false;

   // stores the hit piece closest to the shooter
   PiecePtr hit_piece;

   // stores the index of the closest piece hit by the bullet ray
   int hit_piece_index = -1;

   std::vector<PiecePtr>::iterator it;
   for (it = pieces.begin(); it != pieces.end(); ++it)
   { 
      switch (direction)
      {
         case Piece_n::UP :
         {
            int index = 0;
            std::vector<PiecePtr>::iterator it;
            for (it = pieces.begin(); it != pieces.end(); ++it)
            {
               if ((*it)->getPlayState() == Piece_n::LIVE)
               {
                  if ((*it)->getPosition().first == piece.getPosition().first &&
                      (*it)->getPosition().second < piece.getPosition().second)
                  {
                     if (hit_piece_flag &&
                        (*it)->getPosition().second > hit_piece->getPosition().second)
                     {
                        hit_piece_index = index;
                        hit_piece = *it;
                     }
                     else
                     {
                        hit_piece_index = index;
                        hit_piece = *it;
                        hit_piece_flag = true;
                     }
                  }
               }
               index++;
            }
            break;
         }
         case Piece_n::DOWN :
         {
            int index = 0;
            std::vector<PiecePtr>::iterator it;
            for (it = pieces.begin(); it != pieces.end(); ++it)
            {
               if ((*it)->getPlayState() == Piece_n::LIVE)
               {
                  if ((*it)->getPosition().first == piece.getPosition().first &&
                     (*it)->getPosition().second > piece.getPosition().second)
                  {
                     if (hit_piece_flag &&
                        (*it)->getPosition().second < hit_piece->getPosition().second)
                     {
                        hit_piece_index = index;
                        hit_piece = *it;
                     }
                     else
                     {
                        hit_piece_index = index;
                        hit_piece = *it;
                        hit_piece_flag = true;
                     }
                  }
               }
               index++;
            }
            break;
         }
         case Piece_n::LEFT :
         {
            int index = 0;
            std::vector<PiecePtr>::iterator it;
            for (it = pieces.begin(); it != pieces.end(); ++it)
            {
               if ((*it)->getPlayState() == Piece_n::LIVE)
               {
                  if ((*it)->getPosition().second == piece.getPosition().second &&
                      (*it)->getPosition().first < piece.getPosition().first)
                  {
                     if (hit_piece_flag &&
                        (*it)->getPosition().first > hit_piece->getPosition().first)
                     {
                        hit_piece_index = index;
                        hit_piece = *it;
                     }
                     else
                     {
                        hit_piece_index = index;
                        hit_piece = *it;
                        hit_piece_flag = true;
                     }
                  }
               }
               index++;
            }
            break;
         }
         case Piece_n::RIGHT :
         {
            int index = 0;
            std::vector<PiecePtr>::iterator it;
            for (it = pieces.begin(); it != pieces.end(); ++it)
            {
               if ((*it)->getPlayState() == Piece_n::LIVE)
               {
                  if ((*it)->getPosition().second == piece.getPosition().second &&
                      (*it)->getPosition().first > piece.getPosition().first)
                  {
                     if (hit_piece_flag &&
                        (*it)->getPosition().first < hit_piece->getPosition().first)
                     {
                        hit_piece_index = index;
                        hit_piece = *it;
                     }
                     else
                     {
                        hit_piece_index = index;
                        hit_piece = *it;
                        hit_piece_flag = true;
                     }
                  }
               }
               index++;
            }
            break;
         }
         default :
         {
            // in case a reserve piece is hit inadvertently 
            break;
         }
      }
   }

   return hit_piece_index;
}

bool Game_c::gameOver()
{
   if ((mBriefcasePosition.first == P1_SCORING_COORD.first &&
        mBriefcasePosition.second == P1_SCORING_COORD.second) ||
       (mBriefcasePosition.first == P1_SCORING_COORD.first &&
        mBriefcasePosition.second == P1_SCORING_COORD.second))
   {
      return true;
   }
   else
   {
      return false;
   }
}

void Game_c::shootout2()
{
   std::vector<PiecePtr> live_pieces;
   std::vector<PiecePtr> hit_pieces;

   std::vector<PiecePtr>::iterator p1_it;
   for (p1_it = mPlayer1Pieces.begin(); p1_it != mPlayer1Pieces.end(); ++p1_it)
   {
      if ((*p1_it)->getPlayState() == Piece_n::LIVE)
      {
         live_pieces.push_back(*p1_it);
      }
   }

   std::vector<PiecePtr>::iterator p2_it;
   for (p2_it = mPlayer2Pieces.begin(); p2_it != mPlayer2Pieces.end(); ++p2_it)
   {
      if ((*p2_it)->getPlayState() == Piece_n::LIVE)
      {
         live_pieces.push_back(*p2_it);
      }
   }

   std::vector<PiecePtr>::iterator it;
   for (it = live_pieces.begin(); it != live_pieces.end(); ++it)
   {
      if ((*it)->getPieceType() != Piece_n::PAWN)
      {
         Piece_n::Direction_e direction = (*it)->getDirection();
         detectHit2(**it, direction, live_pieces);

         if ((*it)->getPieceType() == Piece_n::SLINGER)
         {
            Piece_n::Direction_e secondary_direction;
            switch(direction)
            {
               case Piece_n::UP : { secondary_direction = Piece_n::RIGHT; }
               case Piece_n::DOWN : { secondary_direction = Piece_n::LEFT; }
               case Piece_n::LEFT : { secondary_direction = Piece_n::UP; }
               case Piece_n::RIGHT : { secondary_direction = Piece_n::DOWN; }
            }
            detectHit2(**it, secondary_direction, live_pieces);
         }
      }
   }
}

void Game_c::detectHit2(Piece_n::Piece_c& piece, 
                        Piece_n::Direction_e direction,
                        std::vector<PiecePtr>& pieces)
{
   std::cout << "detecting hit..." << std::endl;

   // true once if we have already hit a piece (monodirectional)
   bool hit_piece_flag = false;

   // stores the hit piece closest to the shooter
   PiecePtr hit_piece;

   std::vector<PiecePtr>::iterator it;
   for (it = pieces.begin(); it != pieces.end(); ++it)
   { 
      switch (direction)
      {
         case Piece_n::UP :
         {
            std::vector<PiecePtr>::iterator it;
            for (it = pieces.begin(); it != pieces.end(); ++it)
            {
               if ((*it)->getPlayState() == Piece_n::LIVE)
               {
                  if ((*it)->getPosition().first == piece.getPosition().first &&
                      (*it)->getPosition().second < piece.getPosition().second)
                  {
                     if (hit_piece_flag &&
                        (*it)->getPosition().second > hit_piece->getPosition().second)
                     {
                        hit_piece = *it;
                     }
                     else
                     {
                        hit_piece = *it;
                        hit_piece_flag = true;
                     }
                  }
               }
            }
            break;
         }
         case Piece_n::DOWN :
         {
            std::vector<PiecePtr>::iterator it;
            for (it = pieces.begin(); it != pieces.end(); ++it)
            {
               if ((*it)->getPlayState() == Piece_n::LIVE)
               {
                  if ((*it)->getPosition().first == piece.getPosition().first &&
                     (*it)->getPosition().second > piece.getPosition().second)
                  {
                     if (hit_piece_flag &&
                        (*it)->getPosition().second < hit_piece->getPosition().second)
                     {
                        hit_piece = *it;
                     }
                     else
                     {
                        hit_piece = *it;
                        hit_piece_flag = true;
                     }
                  }
               }
            }
            break;
         }
         case Piece_n::LEFT :
         {
            std::vector<PiecePtr>::iterator it;
            for (it = pieces.begin(); it != pieces.end(); ++it)
            {
               if ((*it)->getPlayState() == Piece_n::LIVE)
               {
                  if ((*it)->getPosition().second == piece.getPosition().second &&
                      (*it)->getPosition().first < piece.getPosition().first)
                  {
                     if (hit_piece_flag &&
                        (*it)->getPosition().first > hit_piece->getPosition().first)
                     {
                        hit_piece = *it;
                     }
                     else
                     {
                        hit_piece = *it;
                        hit_piece_flag = true;
                     }
                  }
               }
            }
            break;
         }
         case Piece_n::RIGHT :
         {
            std::vector<PiecePtr>::iterator it;
            for (it = pieces.begin(); it != pieces.end(); ++it)
            {
               if ((*it)->getPlayState() == Piece_n::LIVE)
               {
                  if ((*it)->getPosition().second == piece.getPosition().second &&
                      (*it)->getPosition().first > piece.getPosition().first)
                  {
                     if (hit_piece_flag &&
                        (*it)->getPosition().first < hit_piece->getPosition().first)
                     {
                        hit_piece = *it;
                     }
                     else
                     {
                        hit_piece = *it;
                        hit_piece_flag = true;
                     }
                  }
               }
            }
            break;
         }
         default :
         {
            // in case a reserve piece is hit inadvertently 
            break;
         }
      }
   }

   if (piece.getTeam() != hit_piece->getTeam())
   {
      hit_piece->nextPlayState();
   }
}