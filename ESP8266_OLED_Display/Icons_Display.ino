#include <U8g2lib.h>
// library: https://github.com/olikraus/u8g2/wiki/fnticons 
U8G2_SSD1309_128X64_NONAME2_1_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 15, /* dc=*/ 4, /* reset=*/ 5);  

/*
******************************************************
INITIALIZE VARIABLES
******************************************************
*/
const uint16_t EMOJI_MIN = 0x0020;
const uint16_t EMOJI_MAX = 0x00FF;

uint16_t currentEmoji;
unsigned int pattern = 0;
unsigned long lastChange = 0;
unsigned long interval = 13000; 

/*
******************************************************
SCREEN STRUCTURES & ICON POSITIONS & TEXT POSITIONS
******************************************************
*/
const int SCREEN_W = 128; // Default Horizontal Pixels 
const int SCREEN_H = 64;  // Default Vertical Pixels
const uint8_t COLS = 6;   
const uint8_t ROWS = 3;

// Width and Height of each icon (in pixels)
const int glyphW = 16;
const int glyphH = 16;

// Gap between each icon
int gapX = 3;
int gapY = (SCREEN_H - glyphH * ROWS) / (ROWS + 1); 

// Total grid size
int gridW = COLS * glyphW + (COLS - 1) * gapX;
int gridH = ROWS * glyphH + (ROWS - 1) * gapY;

// Center the grid
int startX = (SCREEN_W - gridW) / 2;
int startY = (SCREEN_H - gridH) / 2 + glyphH;

/*
******************************************************
x (horizontal) position index: 
                  0  1  2  3  4  5
                  6  7  8  9  10 11
                  12 13 14 15 16 17
y (vertical) position index:s
                  0
                  1
                  2
******************************************************
*/
int emoji_x[18];
int emoji_y[3];

void get_emoji_positions(){
  /*  Get Arrays of Fixed Positions of Icons */
  for (int row = 0; row < ROWS; row++) {
    int y = startY + row * (glyphH + gapY); // y baseline is the bottom line of the glyph height
    emoji_y[row] = y;
    for (int col = 0; col < COLS; col++) {
      int x = startX + col * (glyphW + gapX);
      emoji_x[col + row * 6] = x;
    }
  }
} 

const char* name = "AvocadoLabs"; // Change to your name
int textStartX;
int textStartY;

void get_title_positions(){
   /* Indicate Position of Text */
  u8g2.setFont(u8g2_font_6x12_tf);

  int textW = u8g2.getStrWidth(name);
  int textH = u8g2.getAscent() - u8g2.getDescent();
  
  textStartX = (SCREEN_W - textW) / 2;
  textStartY = SCREEN_H / 2 + textH / 2;
}

/*
******************************************************
SET UP & DISPLAY
******************************************************
*/
void setup() {
  u8g2.begin();  
  u8g2.setFont(u8g2_font_unifont_t_emoticons); // change to your desired fonts 

  Serial.begin(9600);
  
  randomSeed(ESP.getCycleCount()); // good randomness
  currentEmoji = random(EMOJI_MIN, EMOJI_MAX);
  get_emoji_positions();
}

void loop() {  
  unsigned long now = millis();
  pattern++;    

  // Change icon after one cycle 
  if (lastChange == 0 || now - lastChange >= interval){
    pattern = 0; 
    lastChange = now; 
    currentEmoji = random(EMOJI_MIN, EMOJI_MAX);
  }
  
  // Display Patterns
  if (pattern == 0){
    for (int row = 0; row < ROWS; row++) {
      u8g2.firstPage();
      do {
        for (int col = 0; col < COLS; col++) {
          u8g2.drawGlyph(emoji_x[col + row * 6], emoji_y[row], currentEmoji);
        } 
      delay(50);
      } while (u8g2.nextPage());
      delay(500); 
    } 
  } else if (pattern == 1){
    int order_x[18] = {0, 6, 12, 1, 7, 13, 2, 8, 14, 3, 9, 15, 4, 10, 16, 5, 11, 17};
    int order_y[3] = {0, 1, 2};
    for (int i = 0; i < 6; i++) {
      u8g2.firstPage();
        do {
          for (int e = 0; e < 3; e++) {
            int index_x = order_x[i * 3 + e];
            int index_y = order_y[e];
            u8g2.drawGlyph(emoji_x[index_x], emoji_y[index_y], currentEmoji);  
          }delay(50);
        } while (u8g2.nextPage());
        delay(300);       
    }     
  } else if (pattern == 2){
    int order_x[18] = {0, 1, 2, 3, 4, 5, 11, 10, 9, 8, 7, 6, 12, 13, 14, 15, 16, 17};
    int order_y[3] = {0, 1, 2};
    for (int i = 0; i < 3; i++) {
      for (int e = 0; e < 6; e++) {
        int index_x = order_x[i * 6 + e];
        int index_y = order_y[i];
        u8g2.firstPage();
        do {
          u8g2.drawGlyph(emoji_x[index_x], emoji_y[index_y], currentEmoji);  
        }while (u8g2.nextPage());
        delay(100);
      }  
    } 
  } else if (pattern == 3){
    int order_x[18] = {11, 5, 4, 10, 16, 15, 9, 3, 2, 8, 14, 13, 7, 1, 12, 6, 0};
    int order_y[18] = {1, 0, 0,  1,  2,  2, 1, 0, 0, 1,  2,  2, 1, 0,  0, 1, 2};
    for (int i = 0; i < 17; i++) {
      int index_x = order_x[i];
      int index_y = order_y[i];
      u8g2.firstPage();
      do {
        u8g2.drawGlyph(emoji_x[index_x], emoji_y[index_y], currentEmoji);  
      }while (u8g2.nextPage());
      delay(100); 
    } 
  } else if (pattern == 4){
    int order_x[18] = {0, 6, 1, 2, 7, 12, 13, 8, 3, 4, 9, 14, 15, 10, 5, 11, 17};
    int order_y[18] = {0, 1, 0, 0, 1,  2,  2, 1, 0, 0, 1,  2,  2,  1, 0,  1,  2};
    
    for (int i = 0; i < 18; i++) {
      int index_x = order_x[i];
      int index_y = order_y[i];
      u8g2.firstPage();
      do {
        u8g2.drawGlyph(emoji_x[index_x], emoji_y[index_y], currentEmoji);  
      } while (u8g2.nextPage());
      delay(100);  
    }        
  } else if (pattern == 5){
    int order_x[14] = {0, 1, 2, 3, 4, 5, 6, 11, 12, 13, 14, 15, 16, 17};
    int order_y[14] = {0, 0, 0, 0, 0, 0, 1,  1,  2,  2,  2,  2,  2,  2};
    u8g2.firstPage();
    do {  
      get_title_positions();
      u8g2.setFont(u8g2_font_6x12_tf);
      u8g2.drawStr(textStartX, textStartY, name);
            
      u8g2.setFont(u8g2_font_unifont_t_emoticons);
      for (int i = 0; i < 14; i++) {
        int index_x = order_x[i];
        int index_y = order_y[i];
        u8g2.drawGlyph(emoji_x[index_x], emoji_y[index_y], currentEmoji);
      } 
    } while (u8g2.nextPage());
    delay(2000);
    yield();

    // // Keep track of the duration of 1 cycle
    // int in = now - lastChange;
    // Serial.println(in);
  }
}
