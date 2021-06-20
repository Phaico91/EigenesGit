int &func() {
  int x = 0;
  int &rx = x;
  return rx;    // forbidden
}
