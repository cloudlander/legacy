// Battleship.curry
// Pieter Abbeel and Li Tsun Moore, Fall 2001-02

bool GetYesOrNo(string prompt)
{
	string answer;
	while(true) {
		Print(prompt, " (y/n) ");
		answer = ReadLine();
		if (answer == "y" || answer == "Y")
			return true;
		if (answer == "n" || answer == "N")
			return false;
		Print("Please answer y/n.");
	}
}

int GetIntegerNice(string prompt, int low, int up)
{
	int i;
	while(true) {
		Print(prompt, "(",low,"-",up,")  ");
		i = ReadInteger();
		if (i >= low && i <= up)
			return i;
		Print("The number must be in the range ",low,"-",up,"\n");
	}
}

class Ship {
	int length;
	int orig_row;
	int orig_col;
	bool vert;
	int damage;

	void Init(int length_, int orig_row_, int orig_col_, bool vert_)
	{
		length = length_;
		orig_row = orig_row_;
		orig_col = orig_col_;
		vert = vert_;
		damage=0;
	}

	bool IsHit(int hit_row, int hit_col) {
		if (vert) {
			if (hit_row >= orig_row && hit_row < orig_row + length && hit_col==orig_col) 
				return true;
			
			
		} else { //horizontal
			if (hit_col >= orig_col && hit_col < orig_col + length && hit_row==orig_row) 
				return true;
		}
		   
		
		return false;
	}
	
	bool Overlaps(Ship s2)
	{
		int i;
		if(vert){
			for(i=0; i<length; i=i+1){
				if(s2.IsHit(orig_row+i, orig_col)){
					return true;
				}
			}
		} else { //hor
			for (i=0; i<length; i=i+1){
				if(s2.IsHit(orig_row, orig_col+i)){
					return true;
				}
			}
		}
		return false;
	}
	
	
	int GetOrigRow() { return orig_row; }
	int GetOrigCol() { return orig_col; }
	int GetLength() { return length; }
	bool GetVert() { return vert;}
	bool IsSunk() { return damage==length;}
	void IncDamage() { damage = damage+1;}
}


class Board {
	string[][] grid;
	Ship[] ships;
	int num_missiles_left;
	int num_missiles_used;
	int num_hits;
	int num_targets;
	int num_ships;
	int num_ships_left;
	int seed;
	int a; int b; int c;
	int board_size;

	int GetNextRand()// generates random numbers [1-c]
	{
		seed = (seed * a)%b;
		return (seed%c);
	}

	void InitRand(int sd)
	{
		seed=sd;
		a = 5957;
		b = 7919;
		c = 619;
	}

	bool Overlaps(bool[] needPlacement)
	{
		int i;
		int j;
		bool res;
		res = false;

		for(i=0; i<ships.length(); i=i+1){
			needPlacement[i]=false;
		}

		for(i=0; i<ships.length(); i=i+1){
			for(j=i+1; j<ships.length(); j=j+1){
				if(ships[i].Overlaps(ships[j])){
					needPlacement[i]=true;
					res = true;
				}
			}
		}
		return res;
	}
	

	void Init(int bs, int num_ships_, int num_missiles)
	{

		
		int i;
		int j;
		bool[] needPlacement;
		int minShipSize; int shipSize;

		board_size = bs; num_ships=num_ships_;		
		grid = NewArray(bs, string[]);
		for (i = 0; i < bs; i = i + 1) {
			grid[i] = NewArray(bs, string);
			for (j = 0; j < bs; j = j+1) {
				grid[i][j] = ".";
			}
		}
		
	    num_ships_left=num_ships;
		ships = NewArray(num_ships, Ship);


		needPlacement = NewArray(num_ships, bool);
		for(i=0; i<num_ships; i=i+1){
			needPlacement[i]=true;
			ships[i]=New(Ship);
		}
		
		minShipSize=2;

		while(true){
			for(i=0; i<num_ships; i=i+1){
				if(needPlacement[i]){
					bool vert;
					vert = GetNextRand()%2==0;
					shipSize=i+minShipSize;
					if(vert)
						ships[i].Init(shipSize, GetNextRand()%(bs+1-shipSize), GetNextRand()%bs, vert);
					else // hor
						ships[i].Init(shipSize, GetNextRand()%bs, GetNextRand()%(bs+1-shipSize), vert);
				}
			}
			if(!Overlaps(needPlacement))  // not implemented yet ...
				break;
		}
		
		num_missiles_left = num_missiles; num_missiles_used = 0; num_hits= 0;
		
		num_targets=0;
	    for (i=0; i<ships.length(); i=i+1)
			num_targets=num_targets+ships[i].GetLength();
		
	}
	
	void DisplayLegend() 
	{
		Print(".: unexplored, o: miss, *: hit, X: unfound target (only displayed after game ends)\n");
	}
	
	void Display()
	{
		int i;
		int j;
		string s;
		
		Print(" ");
		for(i=0; i<board_size; i=i+1)
			Print(" ",i);
		Print("\n");

		for (i = 0; i < board_size; i = i+1) {
			Print(i);
			for (j = 0; j < board_size; j = j+1) {
				Print(" ", grid[i][j]);
			}
			Print("\n");
		}
		DisplayLegend();
		Print("\n");
		Print("Hits: ", num_hits, "  missiles fired: ", num_missiles_used, "  missiles left: ", num_missiles_left, "\n");
		Print("Target squares left: ", num_targets-num_hits, "   ", "Ships left: ", num_ships_left, " (out of ",num_ships,")\n");
	}


	int IsHit(int row, int col)
	{
		int i;
		int l;
		l= ships.length();
		for(i=0; i<l; i=i+1)
			if(ships[i].IsHit(row,col))
				return i;
		return -1;
	}

	bool OneTurn()
	{
		int row;
		int col;
		int ship_hit;

		Print("(-1 to quit)\n");
		row = GetIntegerNice("row: ", -1, board_size);
		if(row==-1) return false;
		col = GetIntegerNice("col: ", -1, board_size);
		if(col==-1) return false;

		ship_hit = IsHit(row,col);
		if(ship_hit!=-1){ // a hit
			if(grid[row][col]!="*") {// a new hit
				num_hits=num_hits+1;
				ships[ship_hit].IncDamage();
				grid[row][col] = "*";
				if(ships[ship_hit].IsSunk()){
					num_ships_left=num_ships_left-1;
					Print("\n\n***** you sunk a ship of length ", ships[ship_hit].GetLength(),"\n");
					Print("***** ", num_ships_left, " ships to go\n\n");
				}
			}
		} else {
			grid[row][col] = "o";
		}
		num_missiles_left = num_missiles_left-1;
		num_missiles_used = num_missiles_used+1;

		Display();
		return true;
		
	}


	void FillOutSolution(){
		int i;
		int num_ships;
		num_ships = ships.length();

		for(i=0; i<num_ships; i=i+1){
			if(!ships[i].IsSunk()){
				int j;
				int l;
				int orig_row;
				int orig_col;
				bool vert;
				l = ships[i].GetLength();
				orig_row = ships[i].GetOrigRow();
				orig_col = ships[i].GetOrigCol();
				vert = ships[i].GetVert();
				if (vert){
					for(j=0; j<l; j=j+1){
						if(grid[orig_row+j][orig_col]!="*")
							grid[orig_row+j][orig_col]="X";
					}
				} else { // hor
					for(j=0; j<l; j=j+1){
						if(grid[orig_row][orig_col+j]!="*")
							grid[orig_row][orig_col+j]="X";
					}
				}			
				
			}
		}
	}
	
	bool GameOver() 
	{
		return (num_hits==num_targets || num_missiles_left==0);
	}

	bool AllShipsDestroyed()
	{ return (num_ships_left==0);}

}


void main()
{
	Board board;
	int level;
	int seed;
	int board_size;
	seed = GetIntegerNice("give a random seed please: ", 1, 1000);
	board = New(Board);
	board.InitRand(seed);
	
	while (GetYesOrNo("Do you want to play another game?")) {
	    board_size = GetIntegerNice("What board size you want to play?",6,10);
		level = GetIntegerNice("What level you want to play?", 0, 9) ;
		
		board.Init(board_size, 4, board_size*board_size/10*(10-level) );//boardsize,numships,level,rseed
		board.Display();
		while(!board.GameOver())
			if(!board.OneTurn()) // play is in one turn
				break;
		board.FillOutSolution();
		board.Display();
		if(board.AllShipsDestroyed()){
			Print("************************************************************\n\n");
			Print("CONGRATULATIONS, you sunk all ships!!! excellent game play!\n\n");
			Print("************************************************************\n\n");
		}
	}
}
