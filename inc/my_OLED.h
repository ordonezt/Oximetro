/**
 * This Library was originally written by Olivier Van den Eede (4ilo) in 2016.
 * Some refactoring was done and SPI support was added by Aleksander Alekseev (afiskon) in 2018.
 *
 * https://github.com/afiskon/stm32-ssd1306
 */

#ifndef __SSD1306_H__
#define __SSD1306_H__

#include <stddef.h>
#include "my_include.h"

#define SSD1306_USE_SPI

//------------EX-ssd1306_fonts.h---------------------
typedef struct {
	const uint8_t FontWidth;    /*!< Font width in pixels */
	uint8_t FontHeight;   /*!< Font height in pixels */
	const uint16_t *data; /*!< Pointer to data font data array */
} FontDef;


extern FontDef Font_7x10;
extern FontDef Font_11x18;
extern FontDef Font_16x26;
//------------------------------------------------------

#ifndef SSD1306_CS_Port
#define SSD1306_CS_Port
#endif
#ifndef SSD1306_CS_Pin
#define SSD1306_CS_Pin
#endif

#ifndef SSD1306_DC_Port
#define SSD1306_DC_Port         0
#endif
#ifndef SSD1306_DC_Pin
#define SSD1306_DC_Pin          7
#endif

#ifndef SSD1306_Reset_Port
#define SSD1306_Reset_Port      0
#endif
#ifndef SSD1306_Reset_Pin
#define SSD1306_Reset_Pin       6
#endif

/* ^^^ SPI config ^^^ */

// SSD1306 OLED height in pixels
#ifndef SSD1306_HEIGHT
#define SSD1306_HEIGHT          64
#endif

// SSD1306 width in pixels
#ifndef SSD1306_WIDTH
#define SSD1306_WIDTH           128
#endif

// some LEDs don't display anything in first two columns
// #define SSD1306_WIDTH           130

// Enumeration for screen colors
typedef enum {
    Black = 0x00, // Black color, no pixel
    White = 0x01  // Pixel is set. Color depends on OLED
} SSD1306_COLOR;

// Struct to store transformations
typedef struct {
    uint16_t CurrentX;
    uint16_t CurrentY;
    uint8_t Inverted;
    uint8_t Initialized;
} SSD1306_t;

extern SPI_CONFIG_FORMAT_T spi_format;
extern SPI_DATA_SETUP_T spi_xf;

// Procedure definitions
void initOLED(void);
void OLED_Fill(SSD1306_COLOR color);
void OLED_UpdateScreen(void);
void OLED_DrawPixel(uint8_t x, uint8_t y, SSD1306_COLOR color);
char OLED_WriteChar(char ch, FontDef Font, SSD1306_COLOR color);
char OLED_WriteString(char* str, FontDef Font, SSD1306_COLOR color);
void OLED_SetCursor(uint8_t x, uint8_t y);
void OLED_Signal(float dato);

// Low-level procedures
void ssd1306_Reset(void);
void ssd1306_WriteCommand(uint8_t byte);
void ssd1306_WriteData(uint8_t* buffer, size_t buff_size);

void HAL_SPI_Transmit(uint8_t* data, uint32_t size);
void HAL_Delay(uint32_t t);

#endif // __SSD1306_H__
