/* Lonnie Barnes Game of Craps, Winter 2000-01 */

/*The random module and the getyesorno method was borrowed from cs143
blackjack written by Peter Mork and in part by jdz*/

bool GetYesOrNo(string prompt)
{
   string answer;
   Print(prompt, " (y/n) ");
   answer = ReadLine();
   return (answer == "y" || answer == "Y");
}

class rndModule {

  int seed;

  void Init(int seedVal) {
    seed = seedVal;
  }

  int Random() {
    seed = (15625 * (seed % 10000) + 22221) % 65536;
    return seed;
  }

  int RndInt(int max) {
    return (Random() % max);
  }

}

rndModule gRnd;

class Dice
{
        int total;
        int numDie;
        int[] die;

        void Init(int num)
        {
                die = NewArray(num, int);
                numDie = num;
                total = 0;
        }

        void Roll()
        {
                int i;
                int newNum;
			total = 0;


                for (i = 0; i < numDie; i = i + 1)
                {
                        newNum = (gRnd.RndInt(6) + 1);
                        die[i] = newNum;
                        total = total + newNum;
                }
        }

        int GetDieAt(int which) { return die[which];}
        int GetTotal() { return total;}
}



class Player
{
        int money;
        int bet;
        bool shooter;
        bool betForShooter;
        string name;

        void Init(int num)
        {
                money = 1000;
                Print("What is your name, player #", num, "? ");
                name = ReadLine();
                bet = 0;
                shooter = false;
        }

        string GetName()
        {
                return name;
        }

        int GetMoney()
        {
                return money;
        }

        bool HasMoney()
        {
                return (money > 0);
        }

        void SetShooter(bool shooter)
        {
                this.shooter = shooter;
        }

        bool IsShooter()
        {
                return shooter;
        }

        bool BetForShooter()
        {
                return betForShooter;
        }

        int GetBet()
        {
                return bet;
        }

        void PlaceBet()
        {
                if (!shooter)
                {
                        Print("\n", name);
                        betForShooter = GetYesOrNo(", will you bet with the shooter? ");
                        while(bet <= 0 || bet > money)
                        {
                                Print("\n", name, ", you have: $", money, "\nHow much would you like to bet? ");
                                bet = ReadInteger();
                        }
                }
                else
                {
                        while(bet <= 0 || bet > money)
                        {
                                Print("\n", name, ", you're shooting! You have: $", money);
                                Print("\nHow much do you want to bet? ");
                                bet = ReadInteger();
                        }
                }
        }

        void Win()
        {
                money = money + bet;
                bet = 0;
        }

        void Lose()
        {
                money = money - bet;
                bet = 0;
        }
}

void EndCredits()
{
        Print("\nHow are you gentleman!!!\nDecaf Craps made your time by Lonnie Barnes.\nCopyright 2001 Oldman60 Productions.");
        Print("\n\nFeel to free modify this code as you like \n(adding the rest of rules for great justice)");
        Print("\n\nOh, by the way, this is an 'All your base are belongs to us' production!!!\n");
        Print("You have new mail.\n");
        ReadLine();
}

class Table
{
        Player[] players;
        Dice dice;
        Player thrower;
        int startingPlayer;
        int lastRoll;
        int point;

        void SetupGame()
        {
                Print("\n------Welcome To Craps!------\n\n");
                gRnd = New(rndModule);
                Print("Please Enter a Random Seed: ");
                gRnd.Init(ReadInteger());
                dice = New(Dice);
                dice.Init(2);
                point = 0;
                lastRoll = 0;
                startingPlayer = -1;
        }

        void GetNewThrower()
        {
                int next;
                int i;
                if (players.length() == 1)
                        next = 0;
                else
                {
                        next = gRnd.RndInt(players.length());
                        /* Pick the next player - make sure its not the same one */
                        while (next == startingPlayer)
                        {
                                next = gRnd.RndInt(players.length());
                        }
                }
                startingPlayer = next;
                thrower = players[startingPlayer];
                if (!thrower.HasMoney())
                {
                        Print("\n", thrower.GetName(), " has no more money!");
                        GetNewThrower();
                }
                else
                {
                        Print("\n", thrower.GetName(), " will be the next shooter.\n");
                        for (i = 0; i < players.length(); i = i + 1)
                        {
                                players[i].SetShooter(false);
                        }
                        thrower.SetShooter(true);
                }
        }

        void SetupPlayers() {
                int i;
                int numPlayers;
                int startingPlayer;

                numPlayers = 0;
                while(numPlayers <= 0)
                {
                        Print("How many players? ");
                        numPlayers = ReadInteger();
                }
                players = NewArray(numPlayers, Player);
                for (i = 0; i < players.length(); i = i + 1) {
                        players[i] = New(Player);
                        players[i].Init(i+1);
                }
                GetNewThrower();
        }

        void TakeBets() {
                int i;
                Print("\nFirst, take bets before the roll.\n");
                for (i = 0; i < players.length(); i = i + 1)
                        if (players[i].HasMoney()) players[i].PlaceBet();
                }

        int EstablishPoint() {
                int roll;
                Print(thrower.GetName(), ", roll to set the point! ");
                dice.Roll();
                roll = dice.GetTotal();
                Print("\n\nYou rolled a ", dice.GetDieAt(0), " and a ", dice.GetDieAt(1),
" for a total of ", roll);
                return roll;
        }

        void ShooterLoses()
        {
                int i;
                for (i = 0; i < players.length(); i = i + 1)
                {
                        if (players[i].HasMoney())
                        {
                                if (players[i].BetForShooter() && !players[i].IsShooter())
                                {
                                        Print("\n",players[i].GetName(), ", you bet for the shooter and (s)he lost. You lose: $", players[i].GetBet());
                                        players[i].Lose();
                                }
                                if (!players[i].BetForShooter() && !players[i].IsShooter())
                                {
                                        Print("\n",players[i].GetName(), ", you bet against the shooter and (s)he lost. You win: $", players[i].GetBet() * 2);
                                        players[i].Win();
                                }
                        }
                }
                Print("\n", thrower.GetName(), ", you lost. You lose: $",
thrower.GetBet());
                thrower.Lose();
        }

        void ShooterWins()
        {
                int i;
                for (i = 0; i < players.length(); i = i + 1)
                {
                        if (players[i].HasMoney())
                        {
                                if (players[i].BetForShooter() && !players[i].IsShooter())
                                {
                                        Print("\n",players[i].GetName(), ", you bet for the shooter and (s)he won. You win: $", players[i].GetBet() * 2);
                                        players[i].Win();
                                }
                                if (!players[i].BetForShooter() && !players[i].IsShooter())
                                {
                                        Print("\n",players[i].GetName(), ", you bet against the shooter and (s)he won. You lost: $", players[i].GetBet());
                                        players[i].Lose();
                                }
                        }
                }
                        Print("\n", thrower.GetName(), ", you won! You win: $", thrower.GetBet()
* 2);
                        thrower.Win();

        }

        void PrintAllMoney()
        {
                int i;
                for (i = 0; i < players.length(); i = i + 1)
                {
                        Print("\n", players[i].GetName(), " has $", players[i].GetMoney());
                }
        }

        bool PlayOneRound() {
                TakeBets();
                point = EstablishPoint();
                if (point == 2 || point == 3 || point == 12)
                {
                        Print("\nDOH! Shooter crapped out!\n");
                        ShooterLoses();
                        PrintAllMoney();
                        return false;
                }
                else if (point == 7 || point == 11)
                {
                        Print("\nNice! Natural Win!\n");
                        ShooterWins();
                        PrintAllMoney();
                        return true;
                }
                else
                {
                        Print("\nThe Point is now ", point, ". You want to roll this number again before a 7.");
                        while (lastRoll != point && lastRoll != 7)
                        {
                                dice.Roll();
                                Print("\nYou rolled a ", dice.GetDieAt(0), " and a ", dice.GetDieAt(1),
" for a total of ", dice.GetTotal());
                                lastRoll = dice.GetTotal();
                        }
                        if (lastRoll == 7)
                        {
                                ShooterLoses();
                                PrintAllMoney();
                                return false;
                        }
                        else if (point == lastRoll)
                        {
                                ShooterWins();
                                PrintAllMoney();
                                return true;
                        }
                }
                lastRoll = 0; //don't forget to reset the last roll!
        }

        bool IsGameOver()
        {
                int i;
                for (i = 0; i < players.length(); i = i + 1)
                {
                        if (players[i].HasMoney())
                                return false;
                }
                return true;
        }
}

void main() {
        bool keepPlaying;
        bool shooterWon;
        Table table;
        keepPlaying = true;
        table = New(Table);
        table.SetupGame();
        table.SetupPlayers();
        while (keepPlaying) {
                shooterWon = table.PlayOneRound();
                if(!table.IsGameOver())
                {
                        if (!shooterWon)
                                table.GetNewThrower();
                        keepPlaying = GetYesOrNo("\nDo you want to keep playing?");
                }
                else
                {
                        Print("\nNobody has any more money!!! Game over.");
                        keepPlaying = false;
                }
        }
        EndCredits();
}



