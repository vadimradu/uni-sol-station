/* fisier: variables.h
 * 19/07/2014 vadim@anti
 *variabile si definitii pentru program
*/

#define LINE_SIZE	32				//line buffer size
#define NOP() __asm__ __volatile__ ("nop")

#define MAX_TEMP_CONSTANT 450

// constante PID stocate in eeprom
//nu treb volatile?
uint8_t EEMEM EEMEM_KP;
uint8_t EEMEM EEMEM_KI;
uint8_t EEMEM EEMEM_KD;
uint8_t EEMEM EEMEM_KT;


//comportament
volatile uint8_t log_enable;
volatile int16_t temperature = 0;
//din nou oare volatile?cel putin la ultima?
int16_t tmp;
uint8_t button=0;
uint8_t button_activity=0;
uint8_t submenu_active =0;
uint8_t crt_menu = 0;

const char menu_header[3][14] PROGMEM = {	"Oreview", "Amb. temp.", "Other"};
const char menu_content[3][16] PROGMEM = {"Set:    Crt:    ","    'C","c Radu Vadim"};

const char *OK = PSTR("OK\n>");
const char *PARAM_RANGE_MSG = PSTR("Out of range 0-%d\n>");
const char *STATUS = PSTR("SolSt\n\tKP %d\n\tKI %d\n\tKD %d\n\tKT %d\n");
char line[LINE_SIZE]; //line buffer
