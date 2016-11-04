#ifndef _PLAYER_
#define _PLAYER_

#include <vector>

#include

namespace Player_n
{
   class Player_c
   {
   public:
      // default constructor
      Player_c(void);

      // default destructor
      ~Player_c(void);

      // \Name: GetReservePieces
      // \Description:
      // - returns the address of this player's reserve pieces
      // \Argument:
      // - none
      // \Returns
      // - std::vector<Piece>&, this player's reserve pieces
      std::vector<Piece>& GetReservePieces(void);

      // \Name: GetLivePieces
      // \Description:
      // - returns the address of this player's live pieces
      // \Argument:
      // - none
      // \Returns
      // - std::vector<Piece>&, this player's live pieces
      std::vector<Piece>& GetLivePieces(void);

      // \Name: GetDeadPieces
      // \Description:
      // - returns the address of this player's dead pieces
      // \Argument:
      // - none
      // \Returns
      // - std::vector<Piece>&, this player's dead pieces
      std::vector<Piece>& GetDeadPieces(void);

      // \Name: Shoot
      // \Description:
      // - initiates a shootout event
      // \Argument:
      // - none
      // \Returns
      // - none
      void Shoot(void);

   protected:
      // \Name: InitPieces
      // \Description:
      // - initializes mReservePieces; called prior to game start
      // \Argument:
      // - none
      // \Returns
      // - none
      void InitPieces(void);

      // piece vectors
      std::vector<Piece> mReservePieces;
      std::vector<Piece> mLivePieces;
      std::vector<Piece> mDeadPieces;
   };
}

#endif _PLAYER_