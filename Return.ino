void ReturnMenu(char* menu_selector) {
  tft.fillScreen(BLACK);
  if (menu_selector == "menu") {
    menu(false);
  }
  else if (menu_selector == "menu1") {
    menu1(false);
  }
  else if (menu_selector == "menu2") {
    menu2();
  }
  else if (menu_selector == "menu3") {
    menu3();
  }
  menu_active = menu_selector;
}
