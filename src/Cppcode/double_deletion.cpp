int *x = nullptr; // okay
delete x;      // okay, nullptr can be deleted, no effect
x = new int;   // okay
delete x;      // okay
delete x;      // error, double deletion
