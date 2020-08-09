//******************************************************************************
//
//    THE SOFTWARE INCLUDED IN THIS FILE IS FOR GUIDANCE ONLY.
//    AUTHOR SHALL NOT BE HELD LIABLE FOR ANY DIRECT, INDIRECT
//    OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
//    FROM USE OF THIS SOFTWARE.
//
//    Library from "STM32. Aplikacje i ćwiczenia w języku C z biblioteką HAL" Marek Galewski
//******************************************************************************

#ifndef ST7735S_SPI_LIB_H_
#define ST7735S_SPI_LIB_H_

#define ST7735_SPI  SPI1
#define ST7735_SPI_GPIO GPIOB     //ST7735 GPIO port for SPI
                                
#define ST7735_SS   GPIO_PIN_4    //SPI SS   (CS)
#define ST7735_SCK  GPIO_PIN_3    //SPI SCK  (SCL)
#define ST7735_MOSI GPIO_PIN_5    //SPI MOSI (DIN)
#define ST7735_DC   GPIO_PIN_6    //         (DC)
#define ST7735_RES  GPIO_PIN_7    //         (RST)
#define ST7735_BL   GPIO_PIN_8    //         (BL)


#define ST7735_CMD                            0
#define ST7735_DATA                           1

#define ST7735_OLED_WIDTH                      128
#define ST7735_OLED_HEIGHT                     160
#define ST7735_OLED_XOFFSET                    2
#define ST7735_OLED_YOFFSET                    1


//Command codes
#define ST7735_NOP                             0x00

//Set, write commands
#define ST7735_SWRESET                         0x01
#define ST7735_SLPIN                           0x10
#define ST7735_SLOUT                           0x11
#define ST7735_PTLON                           0x12
#define ST7735_NORON                           0x13
#define ST7735_INVOFF                          0x20
#define ST7735_INVON                           0x21
#define ST7735_GAMSET                          0x26
#define ST7735_DSIPOFF                         0x28
#define ST7735_DSIPON                          0x29
#define ST7735_CASET                           0x2A
#define ST7735_RASET                           0x2B
#define ST7735_RAMWR                           0x2C
#define ST7735_RGBSET                          0x2D
#define ST7735_PTLAR                           0x30
#define ST7735_SCRLAR                          0x33
#define ST7735_TEOFF                           0x34
#define ST7735_TEON                            0x35
#define ST7735_MADCTL                          0x36
#define ST7735_VSCSAD                          0x37
#define ST7735_IDMOFF                          0x38
#define ST7735_IDMON                           0x39
#define ST7735_COLMOD                          0x3A
#define ST7735_FRMCTR1                         0xB1
#define ST7735_FRMCTR2                         0xB2
#define ST7735_FRMCTR3                         0xB3
#define ST7735_INVCTR                          0xB4
#define ST7735_PWCTR1                          0xC0
#define ST7735_PWCTR2                          0xC1
#define ST7735_PWCTR3                          0xC2
#define ST7735_PWCTR4                          0xC3
#define ST7735_PWCTR5                          0xC4
#define ST7735_VMCTR1                          0xC5
#define ST7735_VMOFCTR                         0xC7
#define ST7735_WRID2                           0xD1
#define ST7735_WRID3                           0xD2
#define ST7735_NVCTR1                          0xD9
#define ST7735_NVCTR2                          0xDE
#define ST7735_NVCTR3                          0xDF
#define ST7735_GAMCTRP1                        0xE0
#define ST7735_GAMCTRN1                        0xE1
#define ST7735_GCV                             0xFC

//Read commands
#define ST7735_RDDID                           0x04
#define ST7735_RDDST                           0x09
#define ST7735_RDDPM                           0x0A
#define ST7735_RDDMADCTL                       0x0B
#define ST7735_RDDCOMOD                        0x0C
#define ST7735_RDDIM                           0x0D
#define ST7735_RDDSM                           0x0E
#define ST7735_RDDSDR                          0x0F
#define ST7735_RAMRD                           0x2E
#define ST7735_RDID1                           0xDA
#define ST7735_RDID2                           0xDB
#define ST7735_RDID3                           0xDC



#define RGB565(R,G,B)  (((R >> 3) << 11) | ((G >> 2) << 5) | (B >> 3))   //color conversion from standard 888RGB to 12 bit 565RGB
#define RGB(R,G,B)  ((R << 16) | ((G  << 8) | B))                       //color conversion from standard 888RGB to 32 bit long int
#define RGB2RGB565(color)  ((((color & 0x00FF0000) >> (16+3)) << 11)\
		| (((color  & 0x0000FF00) >> (8+2)) << 5) | ((color & 0x000000FF) >> 3))
		//color conversion from standard 888RGB to 32 bit long int

typedef enum{
	L2R_U2D  = 0,	//Memory to display scan left to right, up to down
	L2R_D2U  ,
	R2L_U2D  ,
	R2L_D2U  ,

	U2D_L2R  ,
	U2D_R2L  ,
	D2U_L2R  ,
	D2U_R2L  ,
} ST7735_SCAN_DIR;

typedef struct {
  int XOffset;
  int YOffset;
  int Width;
  int Height;
  char ColorMode;
  char InversionMode;
  char ScanDir;
} tST7735settings;

tST7735settings ST7735settings;

#define ST7735_COLOR_RGB565 5
#define ST7735_COLOR_RGB666 6
#define ST7735_COLOR_RGB888 6


void ST7735_SPI_HwConfig();
void ST7735_SPI_Init();
void ST7735_SPI_WriteByte(unsigned char v, unsigned char DC);

void ST7735_ClearScreen(unsigned long int color);
void ST7735_SetWindow(unsigned short int x0, unsigned short int y0, unsigned short int x1, unsigned short int y1);
void ST7735_DrawPixel(unsigned short int x, unsigned short int y, unsigned long int color);
void ST7735_DrawLine(unsigned short int x0, unsigned short int y0, unsigned short int x1, unsigned short int y1, unsigned long int lineColor);
void ST7735_FillRect(unsigned short int x0, unsigned short int y0, unsigned short int x1, unsigned short int y1, unsigned long int fillColor);
void ST7735_DrawRect(unsigned short int x0, unsigned short int y0, unsigned short int x1, unsigned short int y1, unsigned long int lineColor);
void ST7735_DrawCircle(unsigned short int x0, unsigned short int y0, unsigned short int radius, unsigned long int color);
void ST7735_FillCircle(unsigned short int x0, unsigned short int y0, unsigned short int radius, unsigned long int color);
void ST7735_WriteTextXY(const char *text, unsigned short int xs, unsigned short int ys, unsigned long int fontColor, unsigned long int bkgColor, const unsigned char * FontData);
void ST7735_Write565Bitmap(const unsigned char *buffer, unsigned short int xPos, unsigned short int yPos, unsigned short int xSize, unsigned short int ySize);
void ST7735_Write666Bitmap(const unsigned char *buffer, unsigned short int xPos, unsigned short int yPos, unsigned short int xSize, unsigned short int ySize);
void ST7735_Rotate(unsigned char RotMode);

#endif /* ST7735S_SPI_LIB_H_ */
