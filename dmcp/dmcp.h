/*

  Copyright (c) 2018 SwissMicros GmbH

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions
  are met:

  1. Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.

  2. Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in
     the documentation and/or other materials provided with the
     distribution.

  3. Neither the name of the copyright holder nor the names of its
     contributors may be used to endorse or promote products derived
     from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
  PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS
  BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,
  OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
  OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
  IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
  POSSIBILITY OF SUCH DAMAGE.

*/
#ifndef __SYS_DMCP_H__
#define __SYS_DMCP_H__

#include <stdint.h>

#include "ff_ifc.h"


// ----------------------------------


// Configuration
#define LCD_INVERT_XAXIS
#define LCD_INVERT_DATA
// -------


#define BLT_OR    0
#define BLT_ANDN  1
#define BLT_XOR   2 // 3

#define BLT_NONE  0
#define BLT_SET   1


#ifdef LCD_INVERT_DATA
# define LCD_EMPTY_VALUE 0xFF
# define LCD_SET_VALUE      0
#else
# define LCD_EMPTY_VALUE    0
# define LCD_SET_VALUE   0xFF
#endif



#define BLT_OR    0
#define BLT_ANDN  1
#define BLT_XOR   2 // 3

#define BLT_NONE  0
#define BLT_SET   1


// HW interface
void LCD_clear();
void LCD_power_on();
void LCD_power_off(int clear);
void LCD_write_line(uint8_t * buf);


void bitblt24(uint32_t x, uint32_t dx, uint32_t y, uint32_t val, int invert, int fill);

// Returns pointer to line buffer (doesn't depend on LCD_INVERT_XAXIS)
uint8_t * lcd_line_addr(int y);

// Drawing Prototypes
void lcd_clear_buf();
void lcd_refresh();
void lcd_forced_refresh();
void lcd_refresh_lines(int ln, int cnt);


void lcd_fill_rect(uint32_t x, uint32_t y, uint32_t dx, uint32_t dy, int val);

// Place image into LCD buffer
void lcd_draw_img(const char* img, uint32_t xo, uint32_t yo, uint32_t x, uint32_t y);
void lcd_draw_img_direct(const char* img, uint32_t xo, uint32_t yo, uint32_t x, uint32_t y);
void lcd_draw_img_part(const char* img, uint32_t xo, uint32_t yo, uint32_t x, uint32_t y, uint32_t dx);

#define LCD_X 400
#define LCD_Y 240
#define LCD_LINE_SIZE      50
#define LCD_LINE_BUF_SIZE  LCD_LINE_SIZE+4

void lcd_fillLine(int ln, uint8_t val);
void lcd_fillLines(int ln, uint8_t val, int cnt);


void lcd_set_buf_cleared(int val);
int lcd_get_buf_cleared();


// ----------------------------------


// Font structure
typedef struct {
  const char * name;
  uint8_t width;
  uint8_t height;
  uint8_t baseline;
  uint8_t first_char;
  uint8_t char_cnt;
  uint8_t scale_x;
  uint8_t scale_y;
  uint8_t const * data;
  uint16_t const * offs;
} line_font_t;


#define NR2T(x) (-(x)-1)   // x<0
#define T2NR(x) (-(x)-1)   // x>=0

// Font display state
typedef struct {
  line_font_t const * f; // Current font
  int16_t x, y;      // Current x,y position
  int16_t ln_offs;   // Line offeset (when displaying by line numbers)
  int16_t y_top_grd; // Don'w overwrite anything above this line
  int8_t  ya;     // Lines to fill above the font
  int8_t  yb;     // Lines to fill below the font
  int8_t  xspc;   // Space between chars
  int8_t  xoffs;  // X offst for first char on line

  uint8_t fixed;  // Draw in fixed width
  uint8_t inv;    // Draw inverted
  uint8_t bgfill; // Fill background while drawing
  uint8_t lnfill; // Fill whole lines before writing line
  uint8_t newln;  // New line after writing line
  const uint8_t *post_offs; // X-advance character width minus this value (if not-null)
} disp_stat_t;

void lcd_writeNl(disp_stat_t * ds);
void lcd_prevLn(disp_stat_t * ds);
void lcd_writeClr(disp_stat_t * ds);
void lcd_setLine(disp_stat_t * ds, int ln_nr);
void lcd_setXY(disp_stat_t * ds, int x, int y);

int lcd_lineHeight(disp_stat_t * ds);
int lcd_baseHeight(disp_stat_t * ds);
int lcd_fontWidth(disp_stat_t * ds);

// Font display functions
void lcd_writeText(disp_stat_t * ds, const char* text);

// Width calculation functions
int lcd_textWidth(disp_stat_t * ds, const char* text);
int lcd_charWidth(disp_stat_t * ds, int c);

// Get just text which fits in expected_width
int lcd_textToWidth(disp_stat_t * ds, const char* text, int expected_width, int * plen);

// Just advance ds->x don't print anythig
void lcd_writeTextWidth(disp_stat_t * ds, const char* text);

// Get text which fits in expected width without breaking words
// - word could be broken in middle only when is placed single long word on line 
int lcd_textForWidth(disp_stat_t * ds, const char* text, int expected_width, int * plen);


// Font switching
int lcd_nextFontNr(int nr);
int lcd_prevFontNr(int nr);
void lcd_switchFont(disp_stat_t * ds, int nr);
int lcd_toggleFontT(int nr);


// ----------------------------------


// Display screens for calc
#define DISP_CALC                0
#define DISP_SYS_MENU            2
#define DISP_BOOTLOADER          4
#define DISP_UNIMPLEMENTED       5
#define DISP_USB_WRITE           6
#define DISP_MSC_CONNECT_USB     7
#define DISP_ABOUT               8
#define DISP_FAT_FORMAT          9
#define DISP_FAULT              11
#define DISP_QSPI_BAD_CRC       12
#define DISP_QSPI_CHECK         13
#define DISP_MARK_REGION        15
#define DISP_DISK_TEST          16
#define DISP_DSKTST_CONNECT_USB 17
#define DISP_QSPI_CONNECT_USB   18
#define DISP_OFF_IMAGE_ERR      19
#define DISP_HELP               21
#define DISP_BOOTLDR_CON_USB    22
#define DISP_PROD_DIAG          23
#define DISP_POWER_CHECK        24
#define DISP_FLASH_CONNECT_USB  26
// ----


// Display predefined screen by number
int lcd_for_calc(int what);


// == Menu keys

#define LCD_HEADER_LINES 24
#define LCD_ANN_LINES    16
#define LCD_MENU_LINES   32

#define MENU_KEY_LABEL_LEN    12
#define MENU_KEY_COUNT         6

void lcd_draw_menu_bg();
void lcd_draw_menu_key(int n, const char *s, int highlight);
void lcd_draw_menu_keys(const char *keys[]);

void lcd_print(disp_stat_t * ds, const char* fmt, ...);

#define lcd_printAt(ds, ln, ...)  do { lcd_setLine(ds, ln); lcd_print(ds, __VA_ARGS__); } while(0)
#define lcd_printR(ds, ...)       do { ds->inv=1; lcd_print(ds, __VA_ARGS__); ds->inv=0; } while(0)
#define lcd_printRAt(ds, ln, ...) do { lcd_setLine(ds, ln); ds->inv=1; lcd_print(ds, __VA_ARGS__); ds->inv=0; } while(0)

#define lcd_puts lcd_writeText
#define lcd_putsAt(ds, ln, str)   do { lcd_setLine(ds, ln); lcd_puts(ds,str); } while(0)
#define lcd_putsR(ds, str)        do { ds->inv=1; lcd_puts(ds,str); ds->inv=0; } while(0)
#define lcd_putsRAt(ds, ln, str)  do { lcd_setLine(ds, ln); ds->inv=1; lcd_puts(ds,str); ds->inv=0; } while(0)


// ----------------------------------


typedef struct {
  uint16_t year;
  uint8_t  month;
  uint8_t  day;
} dt_t;

typedef struct {
  uint8_t hour;
  uint8_t min;
  uint8_t sec;
  uint8_t csec;
  uint8_t dow;
} tm_t;

const char* get_wday_shortcut(int day); // 0 = Monday
const char* get_month_shortcut(int month); // 1 = Jan

// DOW is julian_day % 7 ... where 0 = Mon
int julian_day(dt_t *d);
void julian_to_date(int julian_day, dt_t *d);


// ----------------------------------

// System data block

typedef int get_flag_fn_t();
typedef void set_flag_fn_t(int val);

typedef int run_menu_item_fn_t(uint8_t line_id);
typedef const char * menu_line_str_fn_t(uint8_t line_id, char * s, const int slen);

typedef void void_fn_t();


typedef struct {
  volatile uint32_t calc_state;
  FIL * ppgm_fp;
  const char * key_to_alpha_table;

  run_menu_item_fn_t * run_menu_item_app;
  menu_line_str_fn_t * menu_line_str_app;

  void_fn_t * after_fat_format;

  get_flag_fn_t * is_flag_dmy;
  set_flag_fn_t * set_flag_dmy;
  get_flag_fn_t * is_flag_clk24;
  set_flag_fn_t * set_flag_clk24;
  get_flag_fn_t * is_beep_mute;
  set_flag_fn_t * set_beep_mute;

  disp_stat_t * pds_t20;
  disp_stat_t * pds_t24;
  disp_stat_t * pds_fReg;

} sys_sdb_t;


#define calc_state      (sdb.calc_state)
#define ppgm_fp         (sdb.ppgm_fp)

#define key_to_alpha_table (sdb.key_to_alpha_table)

#define run_menu_item_app  (sdb.run_menu_item_app)
#define menu_line_str_app  (sdb.menu_line_str_app)

#define after_fat_format  (sdb.after_fat_format)

#define is_flag_dmy     (sdb.is_flag_dmy)
#define set_flag_dmy    (sdb.set_flag_dmy)
#define is_flag_clk24   (sdb.is_flag_clk24)
#define set_flag_clk24  (sdb.set_flag_clk24)
#define is_beep_mute    (sdb.is_beep_mute)
#define set_beep_mute   (sdb.set_beep_mute)


#define t20             (sdb.pds_t20)
#define t24             (sdb.pds_t24)
#define fReg            (sdb.pds_fReg)

#define sdb (*((sys_sdb_t*)0x10002000))


// ----------------------------------

#define PLATFORM_VERSION "3.7"

// System interface version
#define PLATFORM_IFC_CNR   3
#define PLATFORM_IFC_VER   7

// STATIC_ASSERT ...
#define ASSERT_CONCAT_(a, b) a##b
#define ASSERT_CONCAT(a, b) ASSERT_CONCAT_(a, b)
#define STATIC_ASSERT(e,m) ;enum { ASSERT_CONCAT(assert_line_, __LINE__) = 1/(int)(!!(e)) }
//#define STATIC_ASSERT(expr, msg) typedef char ASSERT_CONCAT(static_assert_check_, __LINE__) [(expr) ? (+1) : (-1)]

#define _STRINGIFY(x) #x
#define STR(x) _STRINGIFY(x)

#define BIT(n) (1<<(n))

// ==== HW ID
uint8_t get_hw_id();

// ==== RTC
void rtc_read(tm_t * tm, dt_t *dt);
void rtc_write(tm_t * tm, dt_t *dt);
uint8_t rtc_read_century();
void rtc_write_century(uint8_t cent);
uint8_t rtc_read_min();
uint8_t rtc_read_sec();
void rtc_wakeup_delay();

// ==== VBAT
uint32_t read_power_voltage();
int get_lowbat_state();
int get_vbat();

// ==== Buzzer
// Freq in mHz
void start_buzzer_freq(uint32_t freq);
void stop_buzzer();

void beep_volume_up();
void beep_volume_down();
int get_beep_volume();


// ==== REGIONS
#define MARK_42_KEYP            0xd3770101
#define MARK_42_KEYR            0xd3770102
#define MARK_42_KEY0            0xd3770103
#define MARK_42_PGM_LOAD        0xd3770104
#define MARK_42_PGM_SAVE        0xd3770105
#define MARK_42_STAT_LOAD       0xd3770106
#define MARK_42_STAT_SAVE       0xd3770107

uint32_t mark_region(uint32_t id);
void no_region();


// ==== RESET values
#define NO_SPLASH_MAGIC         0xEACE7362
#define ALLOC_FAIL_MAGIC        0x363EACE7

void set_reset_magic(uint32_t value);



// === RESET STATE FILE
int is_reset_state_file();
char * get_reset_state_file();
void set_reset_state_file(const char * str);


// ==== USB functions
int usb_powered();


// Aux buf
#define AUX_BUF_SIZE (5*512)

#define AUX_BUF_PGM_LIST_SIZE (3*512)
#define AUX_BUF_SELS_SIZE       (512)

char * aux_buf_ptr();
void * write_buf_ptr();



// Program info structure
#define PROG_INFO_MAGIC 0xd377C0DE

void program_main();

typedef struct {
	uint32_t pgm_magic;
	uint32_t pgm_size;
	void * pgm_entry;
	uint32_t ifc_cnr;
	uint32_t ifc_ver;
	uint32_t qspi_size;
	uint32_t qspi_crc;
	char pgm_name[16];
	char pgm_ver[16];
} __packed prog_info_t;



// ----------------------------------


// Printer 
#define PRINT_GRA_LN  1
#define PRINT_TXT_LN  0

#define MAX_82240_WIDTH 166
#define DFLT_82240_LINE_DUR 1800

void print_byte(uint8_t b);

// Printer delay in ms
uint printer_get_delay();
void printer_set_delay(uint val);


void printer_advance_buf(int what);
int printer_busy_for(int what);


// ----------------------------------


// --------------------------------
//  Menu pages
// --------------------------------

typedef void void_fn_t();

typedef struct {
  const char  * name;
  const uint8_t * items;
  const char  ** msg;
  void_fn_t * post_disp;
} smenu_t;

extern const smenu_t     MID_SYS_WARN; // System menu entry warning
extern const smenu_t       MID_SYSTEM; // System menu
extern const smenu_t   MID_FAT_FORMAT; // FAT format menu
extern const smenu_t MID_DSKTST_ENTER; // Disk test menu
extern const smenu_t    MID_PROD_DIAG; // Production diagnostic screen
extern const smenu_t   MID_PROD_DIAG2; // Production diagnostic screen - selftest version in main menu
extern const smenu_t         MID_DMCP; // Top level system menu
extern const smenu_t   MID_BASE_SETUP; // System setup menu


// --------------------------------
//  Menu items
//    app range   0-127
//    sys range 128-255
// --------------------------------

#define MI_SYSTEM          192
#define MI_BOOTLOADER      193
#define MI_QSPI_LOADER     194
#define MI_DIAG            195
#define MI_MSC             196
#define MI_ABOUT           197
#define MI_BASE_SETUP      198
#define MI_BEEP_MUTE       199
#define MI_SYSTEM_ENTER    200
#define MI_RELOAD_RESET    201
#define MI_SET_TIME        202
#define MI_SET_DATE        203
#define MI_FF_ENTER        204
#define MI_FAT_FORMAT      205
#define MI_DISK_TEST       206
#define MI_DSKTST_ENTER    207
#define MI_DISK_INFO       208
#define MI_LOAD_QSPI       209
#define MI_SLOW_AUTOREP    210

#define MI_EXIT            211

#define MI_KBD_TEST        212
#define MI_LCD_TEST        213
#define MI_IR_TEST         214
#define MI_BEEP_TEST       215
#define MI_DMCP_MENU       216

#define MI_SELF_TEST       217

#define MI_RAMFLASH        218

#define MI_PGM_INFO        219
#define MI_PGM_RUN         220
#define MI_PGM_LOAD        221

#define MI_RUN_DMCP        222
// --------------------------------



#define MRET_LEAVELIMIT  512



// --------------------------------

#define MENU_MAX_LEVEL     8

#define MENU_FONT        t24
#define MENU_LCD_LINES     8

#define MENU_RESET         0
#define MENU_ADD           1

#define MRET_UNIMPL       -1
#define MRET_EXIT         -2

// === Date/Time

#define PRINT_DT_TM_SZ 20


void rtc_check_unset();
void run_set_time();
void run_set_date();


// === Base dialogs
void disp_disk_info(const char * hdr);
int power_check_screen();


// === Base menu functions ===
int handle_menu(const smenu_t * menu_id, int action, int cur_line);

// === Menu formatting support
const char * rb_str(int val);
const char * sel_str(int val);
char * opt_str(char * s, char const *txt, int val);
char * date_str(char * s, const char * txt);
char * time_str(char * s, const char * txt);

// === File selection ===

#define MAX_PGM_FN_LEN  24

typedef int (*file_sel_fn_t)(const char * fpath, const char * fname, void * data);

int file_selection_screen(const char * title, const char * base_dir, const char * ext, file_sel_fn_t sel_fn,
                          int disp_new, int overwrite_check, void * data);


// ----------------------------------


#define MAX_LCD_LINE_LEN 40

#define MAX_KEY_NR       37
#define MAX_FNKEY_NR     43


// -------------
//  Key codes
// -------------

#define KEY_SIGMA  1
#define KEY_INV    2
#define KEY_SQRT   3
#define KEY_LOG    4
#define KEY_LN     5
#define KEY_XEQ    6
#define KEY_STO    7
#define KEY_RCL    8
#define KEY_RDN    9
#define KEY_SIN   10
#define KEY_COS   11
#define KEY_TAN   12
#define KEY_ENTER 13
#define KEY_SWAP  14
#define KEY_CHS   15
#define KEY_E     16
#define KEY_BSP   17
#define KEY_UP    18
#define KEY_7     19
#define KEY_8     20
#define KEY_9     21
#define KEY_DIV   22
#define KEY_DOWN  23
#define KEY_4     24
#define KEY_5     25
#define KEY_6     26
#define KEY_MUL   27
#define KEY_SHIFT 28
#define KEY_1     29
#define KEY_2     30
#define KEY_3     31
#define KEY_SUB   32
#define KEY_EXIT  33
#define KEY_0     34
#define KEY_DOT   35
#define KEY_RUN   36
#define KEY_ADD   37

#define KEY_F1    38
#define KEY_F2    39
#define KEY_F3    40
#define KEY_F4    41
#define KEY_F5    42
#define KEY_F6    43

#define KEY_SCREENSHOT 44
#define KEY_SH_UP      45
#define KEY_SH_DOWN    46

#define KEY_DOUBLE_RELEASE 99

#define KEY_PAGEUP     KEY_DIV
#define KEY_PAGEDOWN   KEY_MUL

// -----------------------
//  Bit masks operations
// -----------------------
#define VAL(x,val)      ((x) & (val))
#define CLR(x,val)       val &= ~(x)
#define SET(x,val)       val |=  (x)
#define MSK(x,val)     (~(x) & (val))
#define SETMSK(x,m,val)  val = (MSK(m,val)|(x))
//#define SETBY(c,x,val)   (c) ? SET(x,val) : CLR(x,val)
#define SETBY(c,x,val)   if (c) { SET(x,val); } else { CLR(x,val); }

#define     ST(x)       VAL(x,calc_state)
#define VAL_ST(x)       VAL(x,calc_state)
#define CLR_ST(x)       CLR(x,calc_state)
#define SET_ST(x)       SET(x,calc_state)
#define SETMSK_ST(x,m)  SETMSK(x,m,calc_state) 
#define SETBY_ST(c,x)   SETBY(c,x,calc_state)



#define STAT_CLEAN_RESET       BIT(0)
#define STAT_RUNNING           BIT(1)
#define STAT_SUSPENDED         BIT(2)
#define STAT_KEYUP_WAIT        BIT(3)
#define STAT_OFF               BIT(4)
#define STAT_SOFT_OFF          BIT(5)
#define STAT_MENU              BIT(6)
#define STAT_BEEP_MUTE         BIT(7)
#define STAT_SLOW_AUTOREP      BIT(8)
#define STAT_PGM_END           BIT(9)
#define STAT_CLK_WKUP_ENABLE   BIT(10)
#define STAT_CLK_WKUP_SECONDS  BIT(11) // 0 - wakeup runner each minute, 1 - each second
#define STAT_CLK_WKUP_FLAG     BIT(12)
#define STAT_DMY               BIT(13)
#define STAT_CLK24             BIT(14)
#define STAT_POWER_CHANGE      BIT(15)


#define STAT_HW_BEEP           BIT(28)
#define STAT_HW_USB            BIT(29)
#define STAT_HW_IR             BIT(30)

#define STAT_HW                (STAT_HW_BEEP | STAT_HW_USB | STAT_HW_IR)


// == File Item list

#define pgm_fn_len 31

typedef struct {
  char fn[pgm_fn_len+1]; // Part of filename that fits on screen
  char f8[16];           // 8.3 filename
} file_item_t;

// Enumerates files in directory
// Parameter fis is filled with file names - fis could be NULL to just get the number of files
// Returns >=0 number of the files in directory
//          <0 fail
int read_file_items(const char * dir_name, const char * filt, file_item_t * fis);

void sort_file_items(file_item_t *fis, int fcnt);



// Screenshots
#define SCR_DIR        "/SCREENS"

// Power OFF images
#define OFFIMG_DIR     "/OFFIMG"

// Help
#define HELP_INDEX "/HELP/index.htm"


// Screenshot
int create_screenshot(int report_error);


// ---------------------------
//  Key buffer functions
// ---------------------------
int key_empty();
int key_push(int k1);
int key_tail();
int key_pop();
int key_pop_last();
void key_pop_all();


// Key functions
int key_to_nr(int key);
void wait_for_key_press();
int runner_get_key(int *repeat);
int runner_get_key_delay(int *repeat, uint timeout, uint rep0, uint rep1, uint rep1tout);
void wait_for_key_release(int tout);



// ---------------------------
//  Runner get key
// ---------------------------

int runner_key_tout_value(const int first);
void runner_key_tout_init(const int slow);


// Autorepeat
int toggle_slow_autorepeat();
int is_slow_autorepeat();

// Auto off
void reset_auto_off();
int is_auto_off();
int is_menu_auto_off();
int sys_auto_off_cnt();

// Time/date
void print_dmy_date(char * s, int const sz, dt_t *dt, const char * append, int shortmon, char sep_arg);
void print_clk24_time(char * t, int const sz, tm_t *tm, int disp_sec, int disp_dow);


// Check and create dir
// returns 0 on success
int check_create_dir(const char * dir);

// Set disk label
void set_fat_label(const char * label);

int file_exists(const char * fn);

int sys_disk_ok();
int sys_disk_write_enable(int val);
void sys_disk_check_valid();
int sys_is_disk_write_enable();

void sys_clear_write_buf_used();
int sys_write_buf_used();


// System timers
void sys_timer_disable(int timer_ix);
void sys_timer_start(int timer_ix, uint32_t ms_value);
int sys_timer_active(int timer_ix);
int sys_timer_timeout(int timer_ix);

// Millisecond delay
void sys_delay(uint32_t ms_delay); 

// Current systick count
uint32_t sys_tick_count();
uint32_t sys_current_ms();

// Critical sections
void sys_critical_start();
void sys_critical_end();

// Sleep
void sys_sleep();

// Free memory
int sys_free_mem();

// System
void sys_reset();

// Key
int sys_last_key();


// ----------------------------------

void run_help();

// ----------------------------------

// Off images
void draw_power_off_image(int allow_errors);
void reset_off_image_cycle();

// ----------------------------------


#include "lft_ifc.h"

#endif
