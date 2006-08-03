class Squash extends Vegetable {
   void Grow(Seeds []seeds, int [][]water)
   {
      Print("But I don't like squash\n");
      Print(10 * 5);
   }
}

class Vegetable {
    int weight;
    int color;


    void Eat(Vegetable veg)
    {
       Seeds[] s;
       int [][]w;
       color = 5 % 2;
       Print("Yum! ", color, "\n");
       veg.Grow(s, w);
       return;
    }

   void Grow(Seeds []seeds, int [][]water)
   {
      Print("Grow, little vegetables, grow!\n");
      Eat(this);
   }
}


void Grow(int a) {
   Print("mmm... veggies!\n");
}

class Seeds {
   int size;
}


void main()
{
   Vegetable []veggies;
   veggies = NewArray(2, Vegetable);
   veggies[0] = New(Squash);
   veggies[1] = New(Vegetable);
   Grow(10);
   veggies[1].Eat(veggies[0]);
}
