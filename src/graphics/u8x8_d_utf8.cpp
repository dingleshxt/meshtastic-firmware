#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <U8g2lib.h>

#define CHGR_WIDTH 128
#define CHGR_HEIGHT 64

bool ch_bitmap[CHGR_HEIGHT][CHGR_WIDTH];

void chgr_clear(void)
{
  unsigned x, y;
  for( y = 0; y < CHGR_HEIGHT; y++)
  {
    for( x = 0; x < CHGR_WIDTH; x++)
    {
      ch_bitmap[y][x] = false;
    }
  }
}

void chgr_set_8pixel(unsigned x, unsigned y, uint8_t pixel)
{
  int cnt = 8;
  while( cnt > 0 )
  {
    if ( (pixel & 1) != 0 )
    {
      ch_bitmap[y][x] = true;
    }
    else
    {
      ch_bitmap[y][x] = false;
    }
    pixel >>= 1;
    y+=1;
    cnt--;
  }
}

void chgr_set_multiple_8pixel(int x, int y, int cnt, uint8_t *pixel)
{
  uint8_t b;
  while( cnt > 0 )
  {
    b = *pixel;
    chgr_set_8pixel(x, y, b);
    x+=1;
    pixel++;
    cnt--;
  }
}




/*==========================================*/
/* tga description procedures */


static const u8x8_display_info_t u8x8_utf8_info =
{
  /* chip_enable_level = */ 0,
  /* chip_disable_level = */ 1,
  
  /* post_chip_enable_wait_ns = */ 0,
  /* pre_chip_disable_wait_ns = */ 0,
  /* reset_pulse_width_ms = */ 0, 
  /* post_reset_wait_ms = */ 0, 
  /* sda_setup_time_ns = */ 0,		
  /* sck_pulse_width_ns = */ 0,
  /* sck_clock_hz = */ 4000000UL,	/* since Arduino 1.6.0, the SPI bus speed in Hz. Should be  1000000000/sck_pulse_width_ns */
  /* spi_mode = */ 1,		
  /* i2c_bus_clock_100kHz = */ 0,
  /* data_setup_time_ns = */ 0,
  /* write_pulse_width_ns = */ 0,
  /* tile_width = */ 16,
  /* tile_hight = */ 8,
  /* default_x_offset = */ 0,
  /* flipmode_x_offset = */ 0,
  /* pixel_width = */ 128,
  /* pixel_height = */ 64
};


uint8_t u8x8_d_utf8(u8x8_t *u8g2, uint8_t msg, uint8_t arg_int, void *arg_ptr)
{
  u8g2_uint_t x, y, c;
  uint8_t *ptr;
  switch(msg)
  {
    case U8X8_MSG_DISPLAY_SETUP_MEMORY:
      u8x8_d_helper_display_setup_memory(u8g2, &u8x8_utf8_info);
      break;
    case U8X8_MSG_DISPLAY_INIT:
      u8x8_d_helper_display_init(u8g2);
      chgr_clear();
      // printf("chgr_clear\n");
      break;
    case U8X8_MSG_DISPLAY_SET_POWER_SAVE:
      break;
    case U8X8_MSG_DISPLAY_SET_FLIP_MODE:
      break;
    case U8X8_MSG_DISPLAY_SET_CONTRAST:
      break;
    case U8X8_MSG_DISPLAY_DRAW_TILE:
          
      x = ((u8x8_tile_t *)arg_ptr)->x_pos;
      // printf("U8X8_MSG_DISPLAY_DRAW_TILE x=%d, ", x);
      x *= 8;
      x += u8g2->x_offset;	
    
      y = ((u8x8_tile_t *)arg_ptr)->y_pos;
      // printf("y=%d, c=%d\n", y, ((u8x8_tile_t *)arg_ptr)->cnt);
      y *= 8;
    
      do
      {
        c = ((u8x8_tile_t *)arg_ptr)->cnt;
        ptr = ((u8x8_tile_t *)arg_ptr)->tile_ptr;
        chgr_set_multiple_8pixel(x, y, c*8, ptr);
        arg_int--;
        x += c*8;
      } while( arg_int > 0 );

      break;
    default:
      return 0;
  }
  return 1;
}


void u8x8_Setup_Utf8(u8x8_t *u8x8)
{
  /* setup defaults */
  u8x8_SetupDefaults(u8x8);
  
  /* setup specific callbacks */
  u8x8->display_cb = u8x8_d_utf8;

  /* setup display info */
  u8x8_SetupMemory(u8x8);
}

void u8g2_SetupBuffer_Utf8(u8g2_t *u8g2, const u8g2_cb_t *u8g2_cb)
{
  static uint8_t buf[CHGR_WIDTH*8];   // i guess this is to big. it should be (CHGR_WIDTH+7)/8
  
  u8x8_Setup_Utf8(u8g2_GetU8x8(u8g2));

  //  u8g2_ll_hvline_vertical_top_lsb
  //  u8g2_ll_hvline_horizontal_right_lsb
  
  u8g2_SetupBuffer(u8g2, buf, 8, u8g2_ll_hvline_vertical_top_lsb, u8g2_cb);
}
