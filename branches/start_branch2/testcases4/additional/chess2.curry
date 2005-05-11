// amin-gopals, CS143 Winter 2000-01
// modified by rivoire-malhotra, CS143 Fall 2001

// Classes
class Piece {
  bool isWhite;
  bool moved;
  
  // Every subclass will have its own version of this function
  // so we'll just return false here
  bool CanMove(int x1, int y1, int x2, int y2, bool dst_occ) 
	{ return false; }

  bool IsWhite()                  { return isWhite;       }
  void SetColor(bool white)       { isWhite = white;      }

  string GetSymbol()
  {
    if (isWhite) return " X";
    else         return "*X";
  }
  
  bool IsKing()                   { return false;          }
  bool IsPawn()			  { return false;	   }
  
  bool IsLongRangeDiag()	  { return false;	   }
  bool IsLongRangeStraight()      { return false;          }

  bool SetMoved(bool omoved)      { moved = omoved;        }
  bool HasMoved()                 { return moved;          }
}

class Pawn extends Piece {
  bool IsPawn () { return true; }

  bool CanMove(int x1, int y1, int x2, int y2, bool dst_occ)
  {
    if (x1 == x2) // if destination is same column
    {
	if (dst_occ)
	{
	    Print ("*** invalid move: Pawns can't capture vertically");
            return false;
	}
	if (isWhite)
	{ 
	  if (y2 == y1 + 1)  return true;  // pawn can move one square ahead
	  if ((y2 == 4) && (y1 == 2)) return true; 
					  // or 2 on its first move
        }
        else
	{
	  if (y2 == y1 - 1) return true;
	  if ((y2 == 5) && (y1 == 7)) return true;
	}
	Print ("*** invalid move: Pawn can move one square ahead ");
	Print ("or 2 on its first move");
	return false;
    }

    if (x1 != x2) // if destination is different column
    {
	if (!dst_occ) 	  
	{
	   // can't print an error message here, since
	   // en passant might be legal - we'll check later
  	   return false;
	}

	if ((x1 != x2 + 1) && (x1 != x2 - 1)) // on an adj. column
	{
	   Print ("*** invalid move: Captured piece must be on ");
	   Print ("an adjacent column");
	   return false;
	}

	if (isWhite)
        {
	   if (y2 != y1 + 1)
           { 
             Print("*** invalid move: captured piece must be on next rank"); 
	     return false;
	   }
        }
	else
	   if (y2 != y1 - 1)
           { 
             Print("*** invalid move: captured piece must be on next rank"); 
	     return false;
	   }
	return true;
    }

  }
  
  string GetSymbol()
  {
    if (isWhite) return " P";
    else         return "*P";
  }
}

class Bishop extends Piece {

  bool IsLongRangeDiag() { return true; }
  bool CanMove(int x1, int y1, int x2, int y2, bool dst_occ)
  {
     int diff_x;
     int diff_y;

     diff_x = x2 - x1;
     diff_y = y2 - y1;

	// must be on same diagonal
     if ((diff_x != diff_y) && (diff_x != -1 * diff_y))
     {	
	Print ("*** invalid move: Bishops move along diagonals only");
 	return false;     
     }
     return true;
  }

  string GetSymbol()
  {
    if (isWhite) return " B";
    else         return "*B";
  }
}

class Knight extends Piece {
  bool CanMove(int x1, int y1, int x2, int y2, bool dst_occ)
  {
    if ((x2 == x1 + 2) || (x2 == x1 - 2))
	if ((y2 == y1 + 1) || (y2 == y1 - 1))
	   return true;
 
    if ((y2 == y1 + 2) || (y2 == y1 - 2))
	if ((x2 == x1 + 1) || (x2 == x1 - 1))
	   return true;

    Print ("*** invalid move: Knights move in an L-shaped pattern");
    return false;
  }

  string GetSymbol()
  {
    if (isWhite) return " N";
    else         return "*N";
  }
}

class Rook extends Piece {
  bool IsLongRangeStraight () { return true; }

  bool CanMove(int x1, int y1, int x2, int y2, bool dst_occ)
  {
    if ((x1 == x2) || (y1 == y2)) return true;
    Print ("*** invalid move: Rooks move horizontally or vertically");
    return false;
  }
  
  string GetSymbol()
  {
    if (isWhite) return " R";
    else         return "*R";
  }
}

class Queen extends Piece {

  bool isLongRangeStraight() { return true; }
  bool isLongRangeDiag()     { return true; }
 
  bool CanMove(int x1, int y1, int x2, int y2, bool dst_occ)
  {
    if ((x1 == x2) || (y1 == y2))
	return true;

    if ((x2 - x1 == y2 - y1) || (x1 - x2 == y2 - y1)) return true;
 
    Print ("*** invalid move: Queens can move horizontally, ");
    Print ("vertically, or diagonally");
    return false;
  }
  
  string GetSymbol()
  {
    if (isWhite) return " Q";
    else         return "*Q";
  }
}

class King extends Piece {
  bool CanMove(int x1, int y1, int x2, int y2, bool dst_occ)
  {
    if ((x1 + 1 >= x2) && (x1 - 1 <= x2))
	if ((y1 + 1 >= y2) && (y1 - 1 <= y2))
	   return true;
    // can't print helpful error message here, since castling might
    // still be legal
    return false;
  }
  
  bool IsKing()                 { return true; }
  
  string GetSymbol()
  {
    if (isWhite) return " K";
    else         return "*K";
  }
}

class Square {
  bool hasPiece;
  Piece piece;

  int x; int y;
  
  string symbol;
  
  bool HasAPiece() { return hasPiece; }
  
  void SetPiece(Piece newPiece) {
    if (newPiece == null) {
      hasPiece = false;
    } else {
      piece = newPiece;
      hasPiece = true;
    }
  }
  
  Piece GetPiece() {
    if (hasPiece)
      return piece;
    else
      return null;
  }
  
  string GetSymbol() {
    if (hasPiece)
      return piece.GetSymbol();
    else
      return ("  ");
  }
  
  void SetPos(int posX, int posY) {
    x = posX;
    y = posY;
  }
  
  int GetX() { return x;  }
  int GetY() { return y;  }
}

class Board {
  Square[][] board;
  int width;
  int height;
  bool[] whiteCanEP;
  bool[] blackCanEP;
  string[] columnLabels;

  void InitPieces(int pieceRow, int pawnRow, bool white) {
    int i;
    Piece piece;
    Square square;
    
    for (i = 0; i < width; i = i + 1) {
      piece = New(Pawn);
      piece.SetColor(white);
      square = board[pawnRow][i];
      piece.SetMoved(false);
      square.SetPiece(piece);
    }
    
    piece = New(King);
    piece.SetColor(white);
    piece.SetMoved(false);
    board[pieceRow][4].SetPiece(piece);
    
    piece = New(Queen);
    piece.SetColor(white);
    piece.SetMoved(false);
    board[pieceRow][3].SetPiece(piece);
    
    piece = New(Bishop);
    piece.SetColor(white);
    piece.SetMoved(false);
    board[pieceRow][2].SetPiece(piece);
    
    piece = New(Bishop);
    piece.SetColor(white);
    piece.SetMoved(false);
    board[pieceRow][5].SetPiece(piece);
    
    piece = New(Knight);
    piece.SetColor(white);
    piece.SetMoved(false);
    board[pieceRow][1].SetPiece(piece);
    
    piece = New(Knight);
    piece.SetColor(white);
    piece.SetMoved(false);
    board[pieceRow][6].SetPiece(piece);
    
    piece = New(Rook);
    piece.SetColor(white);
    piece.SetMoved(false);
    board[pieceRow][0].SetPiece(piece);
    
    piece = New(Rook);
    piece.SetColor(white);
    piece.SetMoved(false);
    board[pieceRow][7].SetPiece(piece);
  }

  void InitBoard() {
    int i;
    int j;
  
  	width = 8;
  	height = 8;

  	board = NewArray(height, Square[]);
  	for (i = 0; i < height; i = i + 1) {
  	  board[i] = NewArray(width, Square);
  	}
  	
  	for (i = 0; i < width; i = i + 1) {
  	  for (j = 0; j < height; j = j + 1) {
  	    board[i][j] = New(Square);
  	    board[i][j].SetPos(i, j);
  	    board[i][j].SetPiece(null);
  	  }
  	}

	whiteCanEP = NewArray(8, bool);
	blackCanEP = NewArray(8, bool);
	for (i = 0; i < 8; i = i + 1) {
	   whiteCanEP[i] = false;
	   blackCanEP[i] = false;
	}
  	
  	InitPieces(0, 1, true);
  	InitPieces(7, 6, false);
  }
  
  void PrintBetweenLines() {
    int i;
    
    Print("        |");
    for (i = 0; i < width - 1; i = i + 1) {
      Print("---+");
    }
    Print("---|\n");
  }
  
  void PrintTop() {
    int j;
    
    Print("        -");
    for (j = 0; j < width; j = j + 1) {
      Print("----");
    }
    Print("\n");
  }

  void PrintBottomLabels() {
  	int i;
	columnLabels = NewArray(8, string);
	columnLabels[0] = "a";
	columnLabels[1] = "b";
	columnLabels[2] = "c";
	columnLabels[3] = "d";
	columnLabels[4] = "e";
	columnLabels[5] = "f";
	columnLabels[6] = "g";
	columnLabels[7] = "h";
  	
  	Print("        ");
  	for (i = 0; i < width; i = i + 1) {
  	  Print("  ", columnLabels[i], " ");
  	}
  	Print("\n");
  }
  
  void DrawBoard() {
  	int i;
  	int j;
      Square square;
  
	PrintTop();
	for (i = height - 1; i >= 0; i = i - 1) {
	  Print("     ", i + 1, "  |");
	  for (j = 0; j < width; j = j + 1) {
	  	square = board[i][j];
	    Print(" ", square.GetSymbol(), "|");
	  }
	  Print("\n");
	  if (i > 0) {
	    PrintBetweenLines();
	  }
	}
	PrintTop();
	PrintBottomLabels();
  }
  
  bool CheckInterveningSquares (int x1, int x2, int y1, int y2, bool diag)
  {
    int i;
    int diff_x;
    int diff_y;
    int inc_x;
    int inc_y;

    diff_x = x2 - x1;
    diff_y = y2 - y1;
    
    if (diff_x > 0) inc_x = 1;
    else inc_x = -1;

    if (diff_y > 0) inc_y = 1;
    else inc_y = -1;

    if ((diag) && (diff_x * inc_x == diff_y * inc_y))
    { 
      for (i = 1; i < inc_x * diff_x; i = i + 1)
	 if (board[y1 + inc_y * i - 1][x1 + inc_x * i - 1].HasAPiece())
	 {
           Print ("*** invalid move: This piece can't jump over other pieces");
	   return true;
  	 }
      return false;
    }

    if ((diff_x != 0) && (diff_y == 0))
    	for (i = 1; i < inc_x * diff_x; i = i + 1)        
    	{
      	   if (board[y1 - 1][x1 + inc_x * i - 1].HasAPiece())
	   {
    	    Print ("*** invalid move: This piece can't jump over other pieces");
	    return true;
	   }
        }

    if ((diff_y != 0) && (diff_x == 0))
	for (i = 1; i < inc_y * diff_y; i = i + 1)
	{
	   if (board[y1 + inc_y * i - 1][x1 -1].HasAPiece())
	   {
	     Print ("*** invalid move: This piece can't jump over other pieces");
	     return true;
	   }
        }
    return false;
  }

  int CanCastle (bool white, int x1, int y1, int x2, int y2)
  {
      if (y1 != y2) return 0;
      if (x2 == 7) // Kingside
      {
	 // The rook should be in its original position
	 // and not have moved
	 if ( !(board[y2-1][7].HasAPiece()) || 
	       (board[y2-1][7].GetPiece().HasMoved()) )
	 {
	    Print("*** invalid move: To castle kingside, the rook must ");
	    Print("be in its original position and must not have moved");
	    return 0;
	 }

	 // The squares between king and rook should be empty.
	 if ( (board[y2-1][5].HasAPiece()) ||
	      (board[y2-1][6].HasAPiece()) ) 
         {
            Print("*** invalid move: To castle kingside, the squares "); 
            Print("between king and rook must be empty");
            return 0;
         }

	 return 1;
      }

     if (x2 == 3) // Queenside
     {
         if ( !(board[y2-1][0].HasAPiece()) ||
               (board[y2-1][0].GetPiece().HasMoved()) )
         {
            Print("*** invalid move: To castle queenside, the rook must ");
            Print("be in its original position and must not have moved");
            return 0;
         }

	 if ( (board[y2-1][1].HasAPiece()) ||
	      (board[y2-1][2].HasAPiece()) ||
	      (board[y2-1][3].HasAPiece()) )
         {
            Print("*** invalid move: To castle queenside, the squares ");
            Print("between king and rook must be empty");
            return 0;
         }
         return 2;
     }
     return 0;
  }

  Piece Promote (bool white)
  {
    bool promoted;
    string newPieceName;
    Piece newPiece;
   promoted = false;

    while (!promoted)
    {
	Print ("Congratulations, you've promoted a pawn!\n");
	Print ("Select piece to promote to (Q/R/B/N):\n");
	newPieceName = ReadLine();
	if (newPieceName == "Q")
	{
	   newPiece = New (Queen);
	   promoted = true;
	}
	if (newPieceName == "R")
	{
	   newPiece = New (Rook);
	   promoted = true;
	}
	if (newPieceName == "B")
	{
	   newPiece = New (Bishop);
	   promoted = true;
	}
	if (newPieceName == "N")
	{
	   newPiece = New (Knight);
	   promoted = true;
	}
    }
      
    newPiece.SetColor(white);
    newPiece.SetMoved(true);
    return newPiece;
  }

  int Move(int x1, int y1, int x2, int y2, bool whiteTurn) {
    bool can_move_piece;
    Square square1;
    Square square2;
    Piece piece;
    Piece dst_piece;
    bool dst_occ;
    int i;

    square1 = board[y1 - 1][x1 - 1];
    square2 = board[y2 - 1][x2 - 1];
   
    if (!square1.HasAPiece()) {
      Print("*** invalid move: no piece on starting square\n");
      return -1;
    }
    
    if (x1 == x2 && y1 == y2) {
      Print("*** invalid move: cannot move piece on to itself\n");
      return -1;
    }
    
    piece = square1.GetPiece();
    if (square2.HasAPiece())
    {
	dst_piece = square2.GetPiece();
        dst_occ   = true;
    }
    else
    { 
	dst_occ   = false;
	dst_piece = null;
    }

    if (piece.IsWhite() != whiteTurn) {
      Print("*** invalid move: cannot move opponents pieces\n");
      return -1;
    }
   
   // The CanMove() function checks to see if most moves are possible.
   // Further checks will be necessary though.
    can_move_piece = piece.CanMove(x1, y1, x2, y2, dst_occ);

    if (can_move_piece) {
	int i;

	// We have to check the squares between the piece's starting
	// square and its destination.
	if (piece.IsLongRangeDiag())
	   if (CheckInterveningSquares(x1, x2, y1, y2, true)) return -1;
	if (piece.IsLongRangeStraight())
	   if (CheckInterveningSquares(x1, x2, y1, y2, false)) return -1;

	if (dst_occ)
	   if (dst_piece.IsWhite() == piece.IsWhite())
	   {
	      Print("*** invalid move: cannot capture own pieces\n");
	      return -1;
	   }
     }

    // CanMove() doesn't recognize if en passant captures are legal,
    // so we have to check ourselves.
    if (!can_move_piece && piece.IsPawn())
    {
	if (piece.IsWhite())
	{
	    if ( !dst_occ && (y2 == y1 + 1) && 
		((x2 - x1 == 1) || (x1 - x2 == 1)) )
		if (whiteCanEP[x2-1])
		{
	           board[y2-2][x2-1].SetPiece(null);
	           can_move_piece = true;
	       }
	}
	else
	{
            if ( !dst_occ && (y2 == y1 - 1) && 
                ((x2 - x1 == 1) || (x1 - x2 == 1)) )
                if (blackCanEP[x2-1])
                {
                  board[y2][x2-1].SetPiece(null);
                  can_move_piece = true;
                }
	}
    }

	// We also need to check to see if castling is legal.    
    if (!can_move_piece && piece.IsKing() && !piece.HasMoved())
    {
	int i;
	Piece rookToMove;

	i = CanCastle (piece.IsWhite(), x1, y1, x2, y2);
	if (i == 1) // Kingside
	{
	    // move rook from h-file to f-file
	    rookToMove = board[y2-1][7].GetPiece();
	    board[y2-1][5].SetPiece(rookToMove);
	    rookToMove.SetMoved(true);
	    board[y2-1][7].SetPiece(null);
	    can_move_piece = true;
	}
	if (i == 2) // Queenside
	{
	    // move rook from a-file to d-file
            rookToMove = board[y2-1][0].GetPiece();
            board[y2-1][3].SetPiece(rookToMove);
	    rookToMove.SetMoved(true);
            board[y2-1][0].SetPiece(null);
	    can_move_piece = true;
	}
    }

    if (can_move_piece == false) return -1;

// We reset the EP arrays, since en passant is only legal on the
// move immediately following a 2-square pawn advance.
    for (i = 0; i < 8; i = i + 1) {
        whiteCanEP[i] = false;
        blackCanEP[i] = false;
    }

// Now that we know the move is legal, we see if it's a 
// pawn promotion.  We also see if en passant is possible next
// move and set the corresponding element of the EP array.
    if (piece.IsPawn())
    {
       if (piece.IsWhite() && (y2 == 8))
	   piece = Promote (true);
       if (!piece.IsWhite() && (y2 == 1))
	   piece = Promote (false);
       if (piece.IsWhite() && (y1 == 2) && (y2 == 4))
	   blackCanEP[x2-1] = true;
       if (!piece.IsWhite() && (y1 == 7) && (y2 == 5))
	   whiteCanEP[x2-1] = true;		
     }

// At last, we move the piece to its destination square.
     square2.SetPiece(piece);
     piece.SetMoved(true);
     square1.SetPiece(null);
     if (dst_piece != null)
	if (dst_piece.IsKing()) return 1;
     return 0;
  }

}

bool MakeMove(Board board, bool whiteTurn) {
  int x1;
  int y1;
  int x2;
  int y2;
  int gameOver;
  
  string move;

  gameOver = 0;
  x1 = 0;
  y1 = 0;
  x2 = 0; 
  y2 = 0;
  Print("\n*************************************************\n");
  board.DrawBoard();
  if (whiteTurn) {
    Print("It is white's move.\n");
  }
  else {
    Print("It is black's move.\n");
  }
  
  while (x1 < 1 || x1 > 8) {
    string column;
	x1 = 0;

    Print("  Enter initial file (a-h or 'q' to quit): ");
    column = ReadLine();
    if (column == "a") x1 = 1;
    if (column == "b") x1 = 2;
    if (column == "c") x1 = 3;
    if (column == "d") x1 = 4;
    if (column == "e") x1 = 5;
    if (column == "f") x1 = 6;
    if (column == "g") x1 = 7;
    if (column == "h") x1 = 8;
    if (column == "q") return false;
    if (x1 == 0)
       Print("  *** That is not a valid column.\n");
  }
  while (y1 < 1 || y1 > 8) {
    Print("  Enter initial rank (1-8): ");
    y1 = ReadInteger();
    if (y1 < 1 || y1 > 8) {
      Print("  *** That is not a valid row.\n");
    }
  }
  
  while (x2 < 1 || x2 > 8) {
    string column;
    x2 = 0;
	Print("  Enter final file (a-h): ");
    column = ReadLine();
    if (column == "a") x2 = 1;
    if (column == "b") x2 = 2;
    if (column == "c") x2 = 3;
    if (column == "d") x2 = 4;
    if (column == "e") x2 = 5;
    if (column == "f") x2 = 6;
    if (column == "g") x2 = 7;
    if (column == "h") x2 = 8;
    if (x2 == 0)
       Print("  *** That is not a valid column.\n");
  }
  while (y2 < 1 || y2 > 8) {
    Print("  Enter final rank (1-8): ");
    y2 = ReadInteger();
    if (y2 < 1 || y2 > 8) {
      Print("  *** That is not a valid row.\n");
    }
  }

  Print("\n");

  // Board::Move returns -1 if illegal, 0 if legal, and 1 if game over
  gameOver = board.Move(x1, y1, x2, y2, whiteTurn);
  
  if (gameOver == 0) {
    return true;
  } else if (gameOver == 1) {
    return false;
  } else {
    Print("\n*** ILLEGAL MOVE.  TRY AGAIN. ***\n");
    return (MakeMove(board, whiteTurn));
  }
}


void main() {
  bool keepPlaying;
  Board board;
  bool whiteTurn;

  Print("Welcome to CheezyChess 2!!\n");
  Print("\nCaveats: We don't recognize check, checkmate, stalemate ");
  Print("or draws by insufficient material.\n");
  Print("To castle, enter the king's starting square and destination. ");
  Print("The rook will move automatically if castling is legal.");  
  Print("\n\nPress ENTER to continue.\n");
  ReadLine();
  board = New(Board);
  board.InitBoard();

  keepPlaying = true;
  whiteTurn = true;
  while (keepPlaying) {
    keepPlaying = MakeMove(board, whiteTurn);
    whiteTurn = !whiteTurn;
  }

  Print("Thank you for playing...come again soon.\n");
  Print("\nCheezyChess 2 Copyright (c) 2001 by Andrew Min and Gopal Santhanam.\n");
  Print("modified by Anisha Malhotra and Suzanne Rivoire\n");
}
