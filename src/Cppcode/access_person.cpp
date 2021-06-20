Person peter; 
Person *john = new Person; 

peter.name_set("Peter Smith"); // okay, public
std::cout << peter.name;       // error, private
john->age_set(35);             // okay, public
std::cout << john->age;        // error, private
delete john;
