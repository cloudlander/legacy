class Car {
  int wheels;
  void Drive() { }
}

class Honda extends Car {
  int trunk;
   bool Park() { return trunk == wheels;}
}

class Boat {}

void main() {
  Honda red;
  Boat blue;

  red = New(Honda);
  red.wheels = 2;
  if (red.Park()) 
      red.horn = 1.0;
  red.Honk();
  red = (blue < red && blue == null);
}



