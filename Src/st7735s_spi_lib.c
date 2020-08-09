//******************************************************************************
//
//    THE SOFTWARE INCLUDED IN THIS FILE IS FOR GUIDANCE ONLY.
//    AUTHOR SHALL NOT BE HELD LIABLE FOR ANY DIRECT, INDIRECT
//    OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
//    FROM USE OF THIS SOFTWARE.
//
//    Library from "STM32. Aplikacje i ćwiczenia w języku C z biblioteką HAL" Marek Galewski
//******************************************************************************

#define HAL_SPI_MODULE_ENABLED
#define HAL_GPIO_MODULE_ENABLED
#include "st7735s_spi_lib.h"
#include "stm32f1xx_hal.h"


SPI_HandleTypeDef hspi_ST7735;

//ST7735 hardware initialization for SPI operation
void ST7735_SPI_HwConfig()
{

  GPIO_InitTypeDef  GPIO_InitStruct;

  //clocks activation
  if (ST7735_SPI == SPI1) __HAL_RCC_SPI1_CLK_ENABLE(); else
  if (ST7735_SPI == SPI2) __HAL_RCC_SPI2_CLK_ENABLE();

  if (ST7735_SPI_GPIO == GPIOA) __HAL_RCC_GPIOA_CLK_ENABLE(); else
  if (ST7735_SPI_GPIO == GPIOB) __HAL_RCC_GPIOB_CLK_ENABLE(); else
  if (ST7735_SPI_GPIO == GPIOC) __HAL_RCC_GPIOC_CLK_ENABLE(); else
  if (ST7735_SPI_GPIO == GPIOD) __HAL_RCC_GPIOD_CLK_ENABLE(); else
  if (ST7735_SPI_GPIO == GPIOE) __HAL_RCC_GPIOE_CLK_ENABLE();

  //GPIO Config
  GPIO_InitStruct.Pin = ST7735_SS|ST7735_DC|ST7735_RES|ST7735_BL;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(ST7735_SPI_GPIO, &GPIO_InitStruct);
  HAL_GPIO_WritePin(ST7735_SPI_GPIO, ST7735_SS|ST7735_DC|ST7735_RES, GPIO_PIN_SET); //Set NSS, DC, RES state high
  HAL_GPIO_WritePin(ST7735_SPI_GPIO, ST7735_BL, GPIO_PIN_RESET);                      //Set BL state low, backlight off

  GPIO_InitStruct.Pin = ST7735_SCK|ST7735_MOSI;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(ST7735_SPI_GPIO, &GPIO_InitStruct);

  //SPI Config
  hspi_ST7735.Instance = ST7735_SPI;
  hspi_ST7735.Init.Mode = SPI_MODE_MASTER;
  hspi_ST7735.Init.Direction = SPI_DIRECTION_2LINES;
  hspi_ST7735.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi_ST7735.Init.CLKPolarity = SPI_POLARITY_HIGH;
  hspi_ST7735.Init.CLKPhase = SPI_PHASE_2EDGE;
  hspi_ST7735.Init.NSS = SPI_NSS_SOFT;
  hspi_ST7735.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
  hspi_ST7735.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi_ST7735.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi_ST7735.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi_ST7735.Init.CRCPolynomial = 10;
  HAL_SPI_Init(&hspi_ST7735);
}


//ST7735 initialization
void ST7735_SPI_Init()
{

  ST7735settings.ColorMode=ST7735_COLOR_RGB666;
  ST7735settings.InversionMode=0;
  ST7735settings.Width=ST7735_OLED_WIDTH;
  ST7735settings.Height=ST7735_OLED_HEIGHT;
  ST7735settings.XOffset=ST7735_OLED_XOFFSET;
  ST7735settings.YOffset=ST7735_OLED_YOFFSET;
  ST7735settings.ScanDir=L2R_U2D;

  //reset
  HAL_GPIO_WritePin(ST7735_SPI_GPIO, ST7735_RES, GPIO_PIN_SET);
  HAL_Delay(100);
  HAL_GPIO_WritePin(ST7735_SPI_GPIO, ST7735_RES, GPIO_PIN_RESET);
  HAL_Delay(100);
  HAL_GPIO_WritePin(ST7735_SPI_GPIO, ST7735_RES, GPIO_PIN_SET);
  HAL_Delay(100);

  ST7735_SPI_WriteByte(ST7735_FRMCTR1, ST7735_CMD);                //Frame rate in normal mode (full colors)
  //ST7735_SPI_WriteByte(0x01, ST7735_DATA);
  //ST7735_SPI_WriteByte(0x2C, ST7735_DATA);
  //ST7735_SPI_WriteByte(0x2D, ST7735_DATA);
  ST7735_SPI_WriteByte(0x00, ST7735_DATA);
  ST7735_SPI_WriteByte(0x06, ST7735_DATA);
  ST7735_SPI_WriteByte(0x03, ST7735_DATA);

  ST7735_SPI_WriteByte(ST7735_FRMCTR2, ST7735_CMD);                //Frame rate in idle mode (8 colors)
  ST7735_SPI_WriteByte(0x01, ST7735_DATA);
  ST7735_SPI_WriteByte(0x2C, ST7735_DATA);
  ST7735_SPI_WriteByte(0x2D, ST7735_DATA);

  ST7735_SPI_WriteByte(ST7735_FRMCTR3, ST7735_CMD);                //Frame rate in partialmode (full colors)
  ST7735_SPI_WriteByte(0x01, ST7735_DATA);
  ST7735_SPI_WriteByte(0x2C, ST7735_DATA);
  ST7735_SPI_WriteByte(0x2D, ST7735_DATA);
  ST7735_SPI_WriteByte(0x01, ST7735_DATA);
  ST7735_SPI_WriteByte(0x2C, ST7735_DATA);
  ST7735_SPI_WriteByte(0x2D, ST7735_DATA);

  ST7735_SPI_WriteByte(ST7735_INVCTR, ST7735_CMD);                //Inversion modes
  ST7735_SPI_WriteByte(0x07, ST7735_DATA);

  //Power on
  ST7735_SPI_WriteByte(ST7735_PWCTR1, ST7735_CMD);
  ST7735_SPI_WriteByte(0xA2, ST7735_DATA);
  ST7735_SPI_WriteByte(0x02, ST7735_DATA);
  ST7735_SPI_WriteByte(0x84, ST7735_DATA);

  ST7735_SPI_WriteByte(ST7735_PWCTR2, ST7735_CMD);
  ST7735_SPI_WriteByte(0xC5, ST7735_DATA);

  ST7735_SPI_WriteByte(ST7735_PWCTR3, ST7735_CMD);
  ST7735_SPI_WriteByte(0x0A, ST7735_DATA);
  ST7735_SPI_WriteByte(0x00, ST7735_DATA);

  ST7735_SPI_WriteByte(ST7735_PWCTR4, ST7735_CMD);
  ST7735_SPI_WriteByte(0x8A, ST7735_DATA);
  ST7735_SPI_WriteByte(0x2A, ST7735_DATA);

  ST7735_SPI_WriteByte(ST7735_PWCTR5, ST7735_CMD);
  ST7735_SPI_WriteByte(0x8A, ST7735_DATA);
  ST7735_SPI_WriteByte(0xEE, ST7735_DATA);

  ST7735_SPI_WriteByte(ST7735_VMCTR1, ST7735_CMD);
  ST7735_SPI_WriteByte(0x0E, ST7735_DATA);


  ST7735_SPI_WriteByte(ST7735_COLMOD, ST7735_CMD);                      //Color mode
  ST7735_SPI_WriteByte(0x05, ST7735_DATA);
  ST7735settings.ColorMode=ST7735_COLOR_RGB565;

  ST7735_SPI_WriteByte(ST7735_SLOUT, ST7735_CMD);                       //Display sleep out
  HAL_Delay(120);
  ST7735_SPI_WriteByte(ST7735_DSIPON, ST7735_CMD);                      //Display on
  ST7735_ClearScreen(RGB(0,0,0));
  HAL_Delay(10);
  HAL_GPIO_WritePin(ST7735_SPI_GPIO, ST7735_BL, GPIO_PIN_SET);          //Backlight on

}


//Write one byte to ST7735 as Command (0) or Data (1)
void ST7735_SPI_WriteByte(unsigned char v, unsigned char DC)
{
  if (DC==ST7735_CMD)    //Command
	HAL_GPIO_WritePin(ST7735_SPI_GPIO, ST7735_DC, GPIO_PIN_RESET);
  else                    //Data
	HAL_GPIO_WritePin(ST7735_SPI_GPIO, ST7735_DC, GPIO_PIN_SET);

  HAL_GPIO_WritePin(ST7735_SPI_GPIO, ST7735_SS, GPIO_PIN_RESET);   //NSS enable
  HAL_SPI_Transmit(&hspi_ST7735, &v, 1, 50);
  HAL_GPIO_WritePin(ST7735_SPI_GPIO, ST7735_SS, GPIO_PIN_SET);     //NSS diable
  HAL_GPIO_WritePin(ST7735_SPI_GPIO, ST7735_DC, GPIO_PIN_SET);
}


//Clear Screen
void ST7735_ClearScreen(unsigned long int color)
{
  ST7735_FillRect(0,0,ST7735_OLED_WIDTH-1,ST7735_OLED_HEIGHT-1,color);
}

//Set display window
void ST7735_SetWindow(unsigned short int x0, unsigned short int y0, unsigned short int x1, unsigned short int y1)
{
  ST7735_SPI_WriteByte(ST7735_CASET, ST7735_CMD);       //set column
  ST7735_SPI_WriteByte((x0+ST7735settings.XOffset)>>8, ST7735_DATA);
  ST7735_SPI_WriteByte((x0+ST7735settings.XOffset),    ST7735_DATA);
  ST7735_SPI_WriteByte((x1+ST7735settings.XOffset)>>8, ST7735_DATA);
  ST7735_SPI_WriteByte((x1+ST7735settings.XOffset),    ST7735_DATA);

  ST7735_SPI_WriteByte(ST7735_RASET, ST7735_CMD);          //set row
  ST7735_SPI_WriteByte((y0+ST7735settings.YOffset)>>8, ST7735_DATA);
  ST7735_SPI_WriteByte((y0+ST7735settings.YOffset),    ST7735_DATA);
  ST7735_SPI_WriteByte((y1+ST7735settings.YOffset)>>8, ST7735_DATA);
  ST7735_SPI_WriteByte((y1+ST7735settings.YOffset),    ST7735_DATA);

  ST7735_SPI_WriteByte(ST7735_RAMWR, ST7735_CMD);         //memory write to prepare for transfer od data (pixel colors)
}


//Draw a pixel in x,y position with a given color
void ST7735_DrawPixel(unsigned short int x, unsigned short int y, unsigned long int color)
{
  if (x >= ST7735settings.Width || y >= ST7735settings.Height) {
	return;
  }

  if(ST7735settings.ColorMode==ST7735_COLOR_RGB565) color=RGB2RGB565(color);  //convert color to 16bit if 565 mode
  ST7735_SetWindow(x,y,x,y);

  if (ST7735settings.ColorMode==ST7735_COLOR_RGB666){
    ST7735_SPI_WriteByte(color >> 16, ST7735_DATA);     //send 24bit color, high byte
  }
  ST7735_SPI_WriteByte(color >> 8, ST7735_DATA);        //send 24bit color middle byte or 16 bit color high byte
  ST7735_SPI_WriteByte(color, ST7735_DATA);             //low byte
}


//Draw filled-in rectangle between x0,y0 and x1,y1
void ST7735_FillRect(unsigned short int x0, unsigned short int y0, unsigned short int x1, unsigned short int y1, unsigned long int fillColor)
{
  unsigned short int x,y;
  unsigned long int color;

  if ((x1<x0) | (y1<y0) | (x1>ST7735settings.Width) | (x1>ST7735settings.Height)) return;

  if (ST7735settings.ColorMode==ST7735_COLOR_RGB565) color=RGB2RGB565(fillColor);  //convert color to 16bit if 565 mode
  else color=fillColor;

  ST7735_SetWindow(x0,y0,x1,y1);

  for (x=0;x<(x1-x0+1);x++){
	for (y=0;y<(y1-y0+1);y++){
	  if (ST7735settings.ColorMode==ST7735_COLOR_RGB666)
        ST7735_SPI_WriteByte(color>>16, ST7735_DATA);       //send 24bit color, high byte
      ST7735_SPI_WriteByte(color>> 8, ST7735_DATA);         //send 24bit color middle byte or 16 bit color high byte
      ST7735_SPI_WriteByte(color    , ST7735_DATA);         //low byte
    }
  }
}


//Draw rectangle between x0,y0 and x1,y1
void ST7735_DrawRect(unsigned short int x0, unsigned short int y0, unsigned short int x1, unsigned short int y1, unsigned long int lineColor)
{
  ST7735_DrawLine(x0,y0,x1,y0,lineColor);
  ST7735_DrawLine(x1,y0,x1,y1,lineColor);
  ST7735_DrawLine(x1,y1,x0,y1,lineColor);
  ST7735_DrawLine(x0,y1,x0,y0,lineColor);
}



//Draw line from x0,y0 to x1, y1
void ST7735_DrawLine(unsigned short int x0, unsigned short int y0, unsigned short int x1, unsigned short int y1, unsigned long int lineColor)
{
  int dy = y1 - y0;
  int dx = x1 - x0;
  int stepx, stepy;
  int fraction;

  if (dy < 0) { dy = -dy; stepy = -1; } else { stepy = 1; }
  if (dx < 0) { dx = -dx; stepx = -1; } else { stepx = 1; }
  dy <<= 1; // dy is now 2*dy
  dx <<= 1; // dx is now 2*dx
  ST7735_DrawPixel(x0, y0, lineColor);
  if (dx > dy) {
    fraction = dy - (dx >> 1); // same as 2*dy - dx
    while (x0 != x1) {
      if (fraction >= 0) {
        y0 += stepy;
        fraction -= dx; // same as fraction -= 2*dx
      }
      x0 += stepx;
      fraction += dy; // same as fraction -= 2*dy
      ST7735_DrawPixel(x0, y0, lineColor);
    }
  } else {
    fraction = dx - (dy >> 1);
    while (y0 != y1) {
      if (fraction >= 0) {
        x0 += stepx;
        fraction -= dy;
      }
      y0 += stepy;
      fraction += dx;
      ST7735_DrawPixel(x0, y0, lineColor);
    }
  }
}


//Draw circle with a given color, center in x0,y0 and a given radius
void  ST7735_DrawCircle(unsigned short int x0, unsigned short int y0, unsigned short int radius, unsigned long int color) {
  int f = 1 - radius;
  int ddF_x = 0;
  int ddF_y = -2 * radius;
  int x = 0;
  int y = radius;

  ST7735_DrawPixel(x0, y0 + radius, color);     //s1
  ST7735_DrawPixel(x0, y0 - radius, color);     //s2
  ST7735_DrawPixel(x0 + radius, y0, color);     //s3
  ST7735_DrawPixel(x0 - radius, y0, color);     //s4
  while (x < y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x + 1;
    ST7735_DrawPixel(x0 + x, y0 + y, color);     //1
    ST7735_DrawPixel(x0 - x, y0 + y, color);     //2
    ST7735_DrawPixel(x0 + x, y0 - y, color);     //3
    ST7735_DrawPixel(x0 - x, y0 - y, color);     //4
    ST7735_DrawPixel(x0 + y, y0 + x, color);     //5
    ST7735_DrawPixel(x0 - y, y0 + x, color);     //6
    ST7735_DrawPixel(x0 + y, y0 - x, color);     //7
    ST7735_DrawPixel(x0 - y, y0 - x, color);     //8
  }
}


//Draw circle filled with a given color, circle center in x0, y0 and a given radius
void ST7735_FillCircle(unsigned short int x0, unsigned short int y0, unsigned short int radius, unsigned long int color) {
  int f = 1 - radius;
  int ddF_x = 0;
  int ddF_y = -2 * radius;
  int x = 0;
  int y = radius;

  ST7735_DrawLine(x0, y0 + radius, x0, y0 - radius, color);  //s1-2
  ST7735_DrawPixel(x0 + radius, y0, color);                  //s3
  ST7735_DrawPixel(x0 - radius, y0, color);                  //s4
  while (x < y) {
    if (f >= 0) {
      y--;
      ddF_y += 2;
      f += ddF_y;
    }
    x++;
    ddF_x += 2;
    f += ddF_x + 1;

    ST7735_DrawLine(x0 + x, y0 + y, x0 + x, y0 - y, color);  //1-3
    ST7735_DrawLine(x0 - x, y0 + y, x0 - x, y0 - y, color);  //2-4
    ST7735_DrawLine(x0 + y, y0 + x, x0 + y, y0 - x, color);  //5-7
    ST7735_DrawLine(x0 - y, y0 + x, x0 - y, y0 - x, color);  //6-8
  }

}


//Write text starting from X, Y position, set font and background color, use fontData as character pattern
void ST7735_WriteTextXY(const char *text, unsigned short int xs, unsigned short int ys, unsigned long int fontColor, unsigned long int bkgColor, const unsigned char * FontData) {
  //Write text at x,y position with a given foreground and background color
  unsigned int i, j, x, y;
  unsigned char pixelRow;
  unsigned char Mask=0x80;
  unsigned char charCode;

  if (ST7735settings.ColorMode==ST7735_COLOR_RGB565) {
	  fontColor=RGB2RGB565(fontColor);  //convert color to 16bit if 565 mode
	  bkgColor=RGB2RGB565(bkgColor);    //convert color to 16bit if 565 mode
  }

  x=xs;
  y=ys;
  while(*text!=0){
    if (x+FontData[0]>ST7735_OLED_WIDTH+ST7735_OLED_XOFFSET){         //if next character is out of display, move to the next line
      y+=FontData[1];
      x=xs;
    }


    ST7735_SetWindow(x,y,x+FontData[0]-1,y+FontData[1]-1);

    // Write char data
    charCode=*text-0x1F;
    for(i = 0; i <FontData[1]; i++) {
      pixelRow=FontData[(charCode)*FontData[1]+i];    //get one row of char pattern
      Mask = 0x80;
      for (j=0; j<FontData[0]; j++){
        if ((pixelRow & Mask) == 0){                  //put pixel by pixel and set its color
          if (ST7735settings.ColorMode==ST7735_COLOR_RGB666)
            ST7735_SPI_WriteByte(((bkgColor>>16)&0x00FF), ST7735_DATA);       //send 24bit color, high byte
          	ST7735_SPI_WriteByte(((bkgColor>> 8)&0x00FF), ST7735_DATA);
          	ST7735_SPI_WriteByte(((bkgColor    )&0x00FF), ST7735_DATA);
        } else {
          if (ST7735settings.ColorMode==ST7735_COLOR_RGB666)
            ST7735_SPI_WriteByte(((fontColor >> 16)&0x00FF), ST7735_DATA);       //send 24bit color, high byte
          ST7735_SPI_WriteByte(((fontColor>> 8)&0x00FF), ST7735_DATA);
          ST7735_SPI_WriteByte(((fontColor    )&0x00FF), ST7735_DATA);
        }
        Mask = Mask >> 1;
      }
    }
    text++;
    x+=FontData[0];
  }
}


//writes bitmap image stored in 565 format (1 pixel made of: 1st byte = RRRRRGGG & 2nd byte = GGGBBBB)
void ST7735_Write565Bitmap(const unsigned char *buffer, unsigned short int xPos, unsigned short int yPos, unsigned short int xSize, unsigned short int ySize) {
  unsigned long int i;

  ST7735_SetWindow(xPos,yPos,xPos+xSize-1,yPos+ySize-1);

  for(i = 0; i <xSize*ySize*2; i+=2){
    ST7735_SPI_WriteByte(((buffer[i  ]>>8)&0x00FF), ST7735_DATA);
    ST7735_SPI_WriteByte(((buffer[i+1]   )&0x00FF), ST7735_DATA);
  }
}



//writes bitmap image stored in 666 (or 888) format (1 pixel made of: 1st byte = RRRRRR__ & 2nd byte = GGGGGG__ & 3rd byte = BBBBBB__)
void ST7735_Write666Bitmap(const unsigned char *buffer, unsigned short int xPos, unsigned short int yPos, unsigned short int xSize, unsigned short int ySize) {
  unsigned long int i;

  ST7735_SetWindow(xPos,yPos,xPos+xSize-1,yPos+ySize-1);

  for(i = 0; i <xSize*ySize*3; i+=3){
	ST7735_SPI_WriteByte(((buffer[i  ]>>16)&0x00FF), ST7735_DATA);
    ST7735_SPI_WriteByte(((buffer[i+1]>> 8)&0x00FF), ST7735_DATA);
    ST7735_SPI_WriteByte(((buffer[i+2]    )&0x00FF), ST7735_DATA);
  }
}

//writes bitmap image stored in 666 (or 888) format (1 pixel made of: 1st byte = RRRRRR__ & 2nd byte = GGGGGG__ & 3rd byte = BBBBBB__)
void ST7735_Rotate(unsigned char RotMode) {
 unsigned char Settings=0;
  switch (RotMode){
    case  L2R_U2D:{
	                ST7735settings.Width=ST7735_OLED_WIDTH;
	                ST7735settings.Height=ST7735_OLED_HEIGHT;
	                ST7735settings.XOffset=ST7735_OLED_XOFFSET;
	                ST7735settings.YOffset=ST7735_OLED_YOFFSET;
	                ST7735settings.ScanDir=L2R_U2D;
	                Settings=0x00;
                  } break;
    case  D2U_L2R:{
                    ST7735settings.Width=ST7735_OLED_HEIGHT;
                    ST7735settings.Height=ST7735_OLED_WIDTH;
                    ST7735settings.XOffset=ST7735_OLED_YOFFSET;
                    ST7735settings.YOffset=ST7735_OLED_XOFFSET;
                    ST7735settings.ScanDir=D2U_L2R;
	                Settings=0xA0;
                  } break;

    case  R2L_D2U:{
                    ST7735settings.Width=ST7735_OLED_WIDTH;
                    ST7735settings.Height=ST7735_OLED_HEIGHT;
                    ST7735settings.XOffset=ST7735_OLED_XOFFSET;
                    ST7735settings.YOffset=ST7735_OLED_YOFFSET;
                    ST7735settings.ScanDir=R2L_D2U;
	                Settings=0xC0;
                  } break;

    case  U2D_R2L:{
                    ST7735settings.Width=ST7735_OLED_HEIGHT;
                    ST7735settings.Height=ST7735_OLED_WIDTH;
                    ST7735settings.XOffset=ST7735_OLED_YOFFSET;
                    ST7735settings.YOffset=ST7735_OLED_XOFFSET;
                    ST7735settings.ScanDir=U2D_R2L;
	                Settings=0x60;
                  } break;

    default:      {
                    ST7735settings.Width=ST7735_OLED_WIDTH;
                    ST7735settings.Height=ST7735_OLED_HEIGHT;
                    ST7735settings.XOffset=ST7735_OLED_XOFFSET;
                    ST7735settings.YOffset=ST7735_OLED_YOFFSET;
                    ST7735settings.ScanDir=L2R_U2D;
	                Settings=0;
                  }
   }
  ST7735_SPI_WriteByte(ST7735_MADCTL, ST7735_CMD);
  ST7735_SPI_WriteByte(Settings, ST7735_DATA);
}
