/*
 * $Id$
 *
 * A simple Othello game written in Decaf.
 *
 * Written by Bret Taylor and Jim Norris in Fall 2001. A lot of the
 * code comes from the CS221 Othello project, so thanks to Prof.
 * Koller and all the TAs who worked on that.
 */

class Board
{
    int[][] squares;
    int[] pieces;
    int size;
    int current_side;

    int EMPTY;
    int WHITE;
    int RED;

    void CopyTo(Board copy)
    {
	int i;
	int j;

	copy.size = size;
	copy.current_side = current_side;
	copy.EMPTY = EMPTY;
	copy.WHITE = WHITE;
	copy.RED = RED;

	copy.squares = NewArray(size, int[]);
	for (i = 0; i < size; i = i + 1) {
	    copy.squares[i] = NewArray(size, int);
	    for (j = 0; j < size; j = j + 1) {
		copy.squares[i][j] = squares[i][j];
	    }
	}

	copy.pieces = NewArray(2, int);
	copy.pieces[WHITE] = pieces[WHITE];
	copy.pieces[RED] = pieces[RED];
    }

    Board Copy()
    {
	Board copy;
	copy = New(Board);
	CopyTo(copy);
	return copy;
    }

    int GetSize()
    {
	return size;
    }

    void Init(int size)
    {
	int i;
	int j;
	int half;

	WHITE = 0;
	RED = 1;
	EMPTY = 2;

	this.size = size;
	squares = NewArray(size, int[]);
	for (i = 0; i < size; i = i + 1) {
	    squares[i] = NewArray(size, int);
	    for (j = 0; j < size; j = j + 1) {
		squares[i][j] = EMPTY;
	    }
	}

	// Set initial pieces

	half = size / 2;
	squares[half - 1][half - 1] = WHITE;
	squares[half][half] = WHITE;
	squares[half][half - 1] = RED;
	squares[half - 1][half] = RED;

	// Set initial piece counts
	pieces = NewArray(2, int);
	pieces[WHITE] = 2;
	pieces[RED] = 2;

	// White starts
	current_side = WHITE;
    }

    int GetScore(int side)
    {
	return pieces[side];
    }

    int GetCurrentSide()
    {
	return current_side;
    }

    void SetCurrentSide(int side)
    {
	current_side = side;
    }

    string GetSideString(int side)
    {
	if (side == WHITE) return "[1mWhite[0m";
	return "[1m[31mRed[0m";
    }

    void ChangeSides()
    {
	if (current_side == WHITE) current_side = RED;
	else current_side = WHITE;
    }

    void SetPieceAt(int row, int col, int side)
    {
	squares[row][col] = side;
    }

    int GetPieceAt(int row, int col)
    {
	return squares[row][col];
    }

    bool SpaceIsBlank(int row, int col)
    {
	return (squares[row][col] == EMPTY);
    }

    bool HasPieceAt(int row, int col)
    {
	return !SpaceIsBlank(row, col);
    }

    void PrintBoard()
    {
	int i;
	int j;

	Print("  ");
	for (i = 0; i < size; i = i + 1)
	    Print(i);
	Print("\n +");
	for (i = 0; i < size; i = i + 1)
	    Print("-");
	Print("+\n");

	for (i = 0; i < size; i = i + 1) {
	    Print(i, "|");
	    for (j = 0; j < size; j = j + 1) {
		if (squares[i][j] == WHITE) {
		    Print("[1mW[0m");
		} else if (squares[i][j] == RED) {
		    Print("[1m[31mR[0m");
		} else {
		    Print(".");
		}
	    }
	    Print("|", i, "\n");
	}

	Print(" +");
	for (i = 0; i < size; i = i + 1)
	    Print("-");
	Print("+\n");
	Print("  ");
	for (i = 0; i < size; i = i + 1)
	    Print(i);	
	Print("\n");
    }

    bool IsOnBoard(int row, int col)
    {
	return (row >= 0 && row < size && col >= 0 && col < size);
    }

    bool IsDifferentSide(int side1, int side2)
    {
	return ((side1 == WHITE && side2 == RED) ||
		(side1 == RED && side2 == WHITE));
    }

    int GetOtherSide(int side)
    {
	if (side == WHITE) return RED;
	return WHITE;
    }

    bool DoOneDirection(int row, int col, int side, bool do_move,
			int dr, int dc)
    {
	int i;
	int j;
	int other_side;

	// Quick rejection if we are too close to the borders
	if ((dr == -1 && row < 2) || (dr == 1  && row >= size - 2))
	    return false;
	if ((dc == -1 && col < 2) || (dc == 1  && col >= size - 2))
	    return false;

	i = row + dr;
	j = col + dc;

	// The adjacent square must be the opposite color
	if (!IsDifferentSide(side, squares[i][j])) {
	    return false;
	}
  
	// Go over all the consecutive squares in the opponent's color
	i = i + dr;
	j = j + dc;
	while (IsOnBoard(i, j)) {
	    if (!IsDifferentSide(side, squares[i][j])) break;
	    i = i + dr;
	    j = j + dc;
	}

	// Return false if we reached the end of the board or an empty square
	if (!IsOnBoard(i, j))
	    return false;
	if (side != squares[i][j])
	    return false;

	// If we have reached here, then we have a legal move
	if (!do_move) return true;

	// Flip the other side pieces

	other_side = GetOtherSide(side);
	i = i - dr;
	j = j - dc;
	for ( ; i != row || j != col; ) {
	    squares[i][j] = side;
	    pieces[side] = pieces[side] + 1;
	    pieces[other_side] = pieces[other_side] - 1;

	    i = i - dr;
	    j = j - dc;
	}

	return true;  
    }

    bool DoAllDirections(int row, int col, int side, bool do_move)
    {
	int i;
	int j;
	bool legal;

	legal = false;
	if (squares[row][col] != EMPTY) return false;

	for (i = -1; i <= 1; i = i + 1) {
	    for (j = -1; j <= 1; j = j + 1) {
		if (j != 0 || i != 0) {
		    if (DoOneDirection(row, col, side, do_move, i, j))
			legal = true;

		    // If we found something legal it may be enough
		    if (legal && !do_move)
			return true;
		}
	    }
	}

	// Put the piece in its place
	if (legal && do_move) {
	    squares[row][col] = side;
	    pieces[side] = pieces[side] + 1;
	}

	return legal;
    }

    bool IsLegalMove(int row, int col, int side)
    {
	if (!IsOnBoard(row, col)) return false;
	if (squares[row][col] != EMPTY) return false;
	return DoAllDirections(row, col, side, false);
    }

    void DoMove(int row, int col, int side)
    {
	DoAllDirections(row, col, side, true);
    }

    bool CanMove(int side)
    {
	int i;
	int j;

	for (i = 0; i < size; i = i + 1) {
	    for (j = 0; j < size; j = j + 1) {
		if (IsLegalMove(i, j, side))
		    return true;
	    }
	}

	return false;
    }

    int NumSuccessors(int side)
    {
	int count;
	int i;
	int j;

	count = 0;

	for (i = 0; i < size; i = i + 1) {
	    for (j = 0; j < size; j = j + 1) {
		if (IsLegalMove(i, j, side))
		    count = count + 1;
	    }
	}

	return count;
    }
}

Board MakeNewBoard()
{
    Board board;
    board = New(Board);
    board.Init(10);

    return board;
}

void MakeComputerMove(Board board)
{
    int i;
    int j;
    int size;
    int side;
    int row;
    int col;
    int best;

    // We need a temporary board on which we can test our moves
    Board copy;
    copy = New(Board);

    size = board.GetSize();
    side = board.GetCurrentSide();
    best = 0;
    row = -1;
    col = -1;

    for (i = 0; i < size; i = i + 1) {
	for (j = 0; j < size; j = j + 1) {
	    if (board.IsLegalMove(i, j, side)) {
                board.CopyTo(copy);
		copy.DoMove(i, j, side);
		if (copy.GetScore(side) > best) {
		    row = i;
		    col = j;
		    best = copy.GetScore(side);
		}
	    }
	}
    }

    if (best > 0) {
        Print(board.GetSideString(side), " chooses to place its piece at (",
	      row, ", ", col, ").\n\n");
	board.DoMove(row, col, side);
    }
}

void PrintScore(Board board)
{
    int player1;
    int player2;
    int score1;
    int score2;
    string name1;
    string name2;

    player1 = board.GetCurrentSide();
    player2 = board.GetOtherSide(player1);
    score1 = board.GetScore(player1);
    score2 = board.GetScore(player2);
    name1 = board.GetSideString(player1);
    name2 = board.GetSideString(player2);

    if (score1 > score2) {
	Print(name1, " wins! The final score is ", name1, " ", score1,
	      ", ", name2, " ", score2, ".\n");
    } else if (score2 > score1) {
	Print(name2, " wins! The final score is ", name2, " ", score2,
	      ", ", name1, " ", score1, ".\n");
    } else {
	Print("A tie game! The final score is ", name1, " ", score1,
	      ", ", name2, " ", score2, ".\n");
    }
}

void main()
{
    bool computer;
    Board board;
    computer = false;

    Print("Welcome to [1mOthello[0m by Bret Taylor and Jim Norris!\n\n");

    while (true) {
	int players;
    	Print("How many human players will there be (1 or 2)? ");
	players = ReadInteger();
	if (players == 1) {
	    computer = true;
	    break;
	} else if (players == 2) break;
    }


    board = MakeNewBoard();

    Print("\n");
    while (true) {
	int row;
	int col;
	int side;
	int num_moves;
	string side_name;
	bool stop;

	side = board.GetCurrentSide();
	side_name = board.GetSideString(side);
	num_moves = board.NumSuccessors(side);
	stop = false;

	if (num_moves > 0) {
	    board.PrintBoard();
	    Print("\nIt is ", side_name, "'s turn. ");
	    Print("Enter a legal move or (-1, -1) to stop the game early.\n");

	    while (true) {
		Print("Enter a row: ");
		row = ReadInteger();
		Print("Enter a column: ");
		col = ReadInteger();

		if (row == -1 && col == -1) {
		    Print("\n");
		    stop = true;
		    break;
		}

		if (board.IsLegalMove(row, col, side))
		    break;
		Print("(", row, ", ", col, ") is not a legal move for ",
		      side_name, ".\n");
	    }

	    if (stop) break;

	    board.DoMove(row, col, side);
	    board.ChangeSides();
	} else {
	    board.ChangeSides();

	    side = board.GetCurrentSide();
	    num_moves = board.NumSuccessors(side);

	    // Stop if neither player can move
	    if (num_moves == 0) break;
	}

	Print("\n");

	if (computer) {
	     MakeComputerMove(board);
	     board.ChangeSides();
	}
    }

    board.PrintBoard();
    Print("\nThe game is over!\n");
    PrintScore(board);

    Print("\nWritten by Bret Taylor and Jim Norris in Fall 2001.\n");
}
