void draw(){
  u8g.setRGB(255, 255, 255); 
  drawCentered("^ Lue kortti ^", 11 , u8g_font_7x14);
  
  u8g.setFont(u8g_font_5x8);
  String drawString = "";
  int rowPixels = 22;
  for (int i = 0 ; i < drinkLog.length() ; i++) {
    char c = drinkLog.charAt(i);
    if (c == '#') {
      u8g.setPrintPos(0, rowPixels);
      u8g.print(drawString);
      drawString = "";
      rowPixels += 10;
    }
    else {
      drawString += c;
    }
  }
}

void drawCardInfo(){
  drawCentered("Luettiin:", 11, u8g_font_7x14);
  drawCentered(cardUID, 40, u8g_font_7x14);
}

void drawLog(String text) {
  u8g.firstPage();  
  do {
    u8g.setPrintPos(0,10);
    u8g.print(text);  
  } 
  while(u8g.nextPage());
}

void drawSuccess(String text) {
  u8g.firstPage();  
  do {
    drawCentered(text, 11, u8g_font_7x14);
    u8g.drawXBM( 45, 16, java_width, java_height, java_bits);
  } 
  while(u8g.nextPage());
}

int widthOfText;
char widthCharHelper[64];
void drawCentered(String text, int ypos, const u8g_fntpgm_uint8_t *font)
{
  text.toCharArray(widthCharHelper,128);
  u8g.setFont(font);
  u8g.setPrintPos(64 - (u8g.getStrWidth(widthCharHelper)/2), ypos);
  u8g.print(text);
}
