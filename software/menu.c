/* fisier: menu.c
 * 13/05/2015 vadim@onsemi
 * prototip meniu
*/

#include <avr/io.h>
#include "menu.h"

void draw_content(uint8_t crt_menu){
  lcd_gotoxy(1,0); lcd_puts(menu_header[crt_menu]);
  lcd_gotoxy(0,1); lcd_puts(menu_content[crt_menu]);
}


void menu(void){
  if(button_activity = 1){//todo proccesing and display
    if(button = 3){
      submenu_active = submenu_active ^ 1;
    }
    if(button = 2 && crt_menu <2)       crt_menu +=1;
    if(button = 1 && crt_menu >0)       crt_menu -=1;
    draw_header(crt_menu);
  }
  button_activity=0;
}
