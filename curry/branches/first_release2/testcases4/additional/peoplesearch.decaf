// ayeah-geraldoh CS143 Winter 2000-01
void PrintLine(){
	Print("------------------------------------------------------------------------\n");
}

class Person{
	string firstName;
	string lastName;
	string phoneNumber;
	string address;
	
	void InitPerson(string f, string l, string p, string a){
		firstName = f;
		lastName = l;
		phoneNumber = p;
		address = a;
	}
	
	void SetFirstName(string f){
		firstName = f;
	}
	
	string GetFirstName(){
		return firstName;
	}
	
	void SetLastName(string l){
		lastName = l;
	}
	
	string GetLastName(){
		return lastName;
	}
	
	void SetPhoneNumber(string p){
		phoneNumber = p;
	}
	
	string GetPhoneNumber(){
		return phoneNumber;
	}
	
	void SetAddress(string a){
		address = a;
	}
	
	string GetAddress(){
		return address;
	}

	bool IsNamed(string name){
		return(name == firstName || name == lastName);
	}
	
	void PrintInfo(){
		Print("First Name: ", firstName, "\n");
		Print("Last Name: ", lastName, "\n");
		Print("Phone Number: ", phoneNumber, "\n");
		Print("Address: ", address, "\n");
	}
}
class Database{
	int currentSize;
	int maxSize;
	Person[] people;

	void InitDatabase(int size){
		currentSize = 0;
		maxSize = size;
		people = NewArray(size, Person);
	}

	void Search(){
		int i; string name; bool found;
		PrintLine();
		Print("Enter the name of the person you would like to find: ");
		name = ReadLine();
		found = false;
		for(i = 0; i < currentSize; i = i + 1){
			if(people[i].IsNamed(name)){
				if(!found){
					found = true;
					Print("\nListing people with name '", name, "'...\n");
					PrintLine();
				}
				people[i].PrintInfo();
				Print("\n");
				//Print("hit enter to continue searching...\n");
				//ReadLine();
			}
		}
		if(!found){
			Print("\n", name, " not found!\n");
		}
		else{
			//Print("End of list\n");
		}	
		PrintLine();
	}
	
	int PersonExists(string f, string l){
		int i;
		for(i = 0; i < currentSize; i = i + 1){
			if(people[i].GetFirstName() == f && people[i].GetLastName() == l){
				return i;
			}
		}
		return -1;
	}
	
	void Edit(){
		string f; string l; string p; string a; int index;

		PrintLine();
		Print("Editting person...\n\n");
		Print("Enter first name: ");
		f = ReadLine();
		Print("Enter last name: ");
		l = ReadLine();
		index = PersonExists(f, l);
		if(index == -1){
			Print("\n", l, ", ", f, " not found!\n");
			PrintLine();
			return;
		}
		Print("\n", l, ", ", f, " found...\n\n");
		Print("Old first name: ", people[index].GetFirstName(), "\n");
		Print("Enter new first name (or nothing to leave unchanged): ");
		f = ReadLine();
		if(f != "") people[index].SetFirstName(f);
		Print("Old last name: ", people[index].GetLastName(), "\n");
		Print("Enter new first name (or nothing to leave unchanged): ");
		l = ReadLine();
		if(l != "") people[index].SetLastName(l);
		Print("Old phone number: ", people[index].GetPhoneNumber(), "\n");
		Print("Enter new first name (or nothing to leave unchanged): ");
		p = ReadLine();
		if(p != "") people[index].SetPhoneNumber(p);
		Print("Old first name: ", people[index].GetAddress(), "\n");
		Print("Enter new address (or nothing to leave unchanged): ");
		a = ReadLine();
		if(a != "") people[index].SetAddress(a);
		Print("\nChanges successfully saved!\n");
		PrintLine();
	}

	void Add(){
		string f; string l; string  p; string a;

		PrintLine();
		Print("Adding New Person...\n\n");
		Print("Enter first name: ");
		f = ReadLine();
		Print("Enter last name: ");
		l = ReadLine();
		if(PersonExists(f, l)>=0){
			Print("\n", l, ", ", f, " already exists in the db!\n");
			PrintLine();
			return;
		}
		Print("Enter phone number: ");
		p = ReadLine();
		Print("Enter address: ");
		a = ReadLine();
		
		if(currentSize == maxSize){
			int i; int cur;
			Person[] newPeople;

			cur = maxSize;
			maxSize = maxSize * 2;
			newPeople = NewArray(maxSize, Person);
			for(i = 0; i < cur; i = i + 1){
				newPeople[i] = people[i];
			}
			people = newPeople;
		}
		
		people[currentSize] = New(Person);
		people[currentSize].InitPerson(f, l, p, a);
		currentSize = currentSize + 1;
		Print("\n", l, ", ", f, " successfully added!\n");
		PrintLine();
	}

	void Delete(){
		string f; string l; int index;
		PrintLine();
		Print("Deleting person...\n\n");
		Print("Enter first name: ");
		f = ReadLine();
		Print("Enter last name: ");
		l = ReadLine();
		index = PersonExists(f, l);
		if(index != -1){
			people[index] = people[currentSize - 1];
			currentSize = currentSize - 1;
			Print("\n", l, ", ", f, " successfully deleted!\n");
			PrintLine();
		}
		else{
			Print("\n", l, ", ", f, " not found!\n");
			PrintLine();
		}
		
	}
}

void PrintHelp(){
	PrintLine();
	Print("List of Commands...\n\n");
	Print("add - lets you add a person\n");
	Print("delete - lets you delete a person\n");
	Print("search - lets you search for a specific person\n");
	Print("edit - lets you edit the attributes of a specific person\n");
	PrintLine();
}

void main(){
 	Database db;
	string input;

	db = New(Database);
	db.InitDatabase(10);
	PrintLine();
	Print("Welcome to PeopleSearch!\n");
	PrintLine();
	Print("\n");
	input = "";
	while(input != "quit"){
		//PrintLine();
		Print("Please enter your command(type help for a list of commands): ");
		input = ReadLine();
		if(input == "help"){
			PrintHelp();
		}
		if(input == "search"){
			db.Search();
		}
		if(input == "add"){
			db.Add();
		}
		if(input == "delete"){
			db.Delete();
		}
		if(input == "edit"){
			db.Edit();
		}
	}
}