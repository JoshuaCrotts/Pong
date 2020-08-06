//=============================================================================================//
// FILENAME :       text.c
//
// DESCRIPTION :
//        This file defines font and text-drawing functions.
//
// NOTES :
//        Permission is hereby granted, free of charge, to any person obtaining a copy
//        of this software and associated documentation files (the "Software"), to deal
//        in the Software without restriction, including without limitation the rights
//        to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//        copies of the Software, and to permit persons to whom the Software is
//        furnished to do so, subject to the following conditions:
//
//        The above copyright notice and this permission notice shall be included in all
//        copies or substantial portions of the Software.
//
//        THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//        IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//        FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//        AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//        LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//        OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//        SOFTWARE.
//
// AUTHOR :   Joshua Crotts        START DATE :    18 Jun 2020
//
//=============================================================================================//

#include "../include/draw.h"

#define DEFAULT_FONT_COLOR 0xFF

static char text_buffer[MAX_LINE_LENGTH];

static TTF_Font *Stds_GetFont( const char *f, const uint16_t s );
static void      Stds_LoadFonts( void );
void      Stds_AddFont( const char *f, const uint16_t s );

/**
 * Initializes the TTF font library for use.
 *
 * @param void.
 *
 * @return void.
 */
void
Stds_InitFonts( void ) {
  if ( TTF_Init() == -1 ) {
    SDL_LogInfo( SDL_LOG_CATEGORY_APPLICATION, "Failed to initialize TTF_Init: %s.\n",
                 SDL_GetError() );
    exit( EXIT_FAILURE );
  }

  g_app.font_tail = &g_app.font_head;
  Stds_LoadFonts();
}

/**
 * Draws a string of text specified by the const char *parameter, supplemented
 * by whatever formatting arguments are necessary.
 *
 * @param float x coordinate (top-left) of string.
 * @param float y coordinate (top-left) of string.
 * @param const char *font name (use the file name itself with the extension).
 * @param uint16_t font size.
 * @param SDL_Color * pointer to color of text.
 * @param const char *string to draw.
 * @param ... formatting args.
 *
 * To center a string, call Stds_GetStringSize() or Stds_GetStringSizeFont (if using a non
 * standard font), and draw the string at SCREEN_WIDTH / 2 - fontWidth / 2.
 *
 * @return void.
 */
void
Stds_DrawText( const float x, float y, const char *font_string, const uint16_t font_size,
               const SDL_Color *c, const char *text, ... ) {
  SDL_Rect message_rect;
  message_rect.x = ( int32_t ) x;
  message_rect.y = ( int32_t ) y;

  va_list args;
  memset( &text_buffer, '\0', sizeof( text_buffer ) );

  va_start( args, text );
  vsprintf( text_buffer, text, args );
  va_end( args );

  TTF_Font *   font            = Stds_GetFont( font_string, font_size );
  SDL_Surface *message_surface = TTF_RenderText_Solid( font, text_buffer, *c );
  TTF_SizeText( font, text_buffer, &message_rect.w, &message_rect.h );

  if ( message_surface == NULL ) {
    SDL_LogInfo( SDL_LOG_CATEGORY_APPLICATION, "Failed to write message: %s.\n", SDL_GetError() );
    exit( EXIT_FAILURE );
  }

  SDL_Texture *message_texture = SDL_CreateTextureFromSurface( g_app.renderer, message_surface );
  SDL_RenderCopy( g_app.renderer, message_texture, NULL, &message_rect );

  /* Destroys the available message texture and surface to prevent a memory leak,
     this seems superfluous every time through a call, but PLEASE do it. */
  SDL_DestroyTexture( message_texture );
  SDL_FreeSurface( message_surface );
}

/**
 * Frees the fonts that are in use by the standards library.
 *
 * @param void.
 *
 * @return void.
 */
  void Stds_FreeFonts( void ) {
    struct font_t *f;
    SDL_LogDebug( SDL_LOG_CATEGORY_APPLICATION, "Freeing font.\n" );

    /* Frees the font linked list. */
    while ( g_app.font_head.next ) {
      f                    = g_app.font_head.next;
      g_app.font_head.next = f->next;
      free( f );
    }

    TTF_Quit();
  }

  /**
   * Computes the size of the string with the default font were it to be drawn
   * to the screen in pixels. This is useful for positioning the string in the middle
   * of the screen if need-be.
   *
   * @param const char *string.
   * @param const char *font name.
   * @param uint16_t font size.
   * @param pointer to integer (int32_t) where the width of the string is stored.
   * @param pointer to integer (int32_t) where the height of the string is stored.
   *
   * @return void.
   */
  void Stds_GetStringSize( const char *s, const char *font, const uint16_t size, int32_t *w,
                           int32_t *h ) {
    TTF_Font *f;
    f = Stds_GetFont( font, size );

    if ( f != NULL ) {
      TTF_SizeText( f, s, w, h );
    } else {
      exit( EXIT_FAILURE );
    }
  }

  /**
   * Iterate through the linked list of fonts already loaded into the system.
   * If it is found, we return the font with the corresponding size. Otherwise,
   * NULL is returned.
   *
   * @param const char * font name.
   * @param const uint16_t font size.
   *
   * @return TTF_Font * pointer to font object.
   */
  static TTF_Font *Stds_GetFont( const char *font_str, const uint16_t font_size ) {
    struct font_t *f;

    for ( f = g_app.font_head.next; f != NULL; f = f->next ) {
      if ( strcmp( f->name, font_str ) == 0 && f->size == font_size ) {
        return f->font;
      }
    }

    if ( f == NULL ) {
      SDL_LogInfo( SDL_LOG_CATEGORY_APPLICATION, "Could not find font %s, %d.", font_str,
                   font_size );
    }

    return NULL;
  }

  /**
   *
   *
   * @param void.
   *
   * @return void.
   */
  static void Stds_LoadFonts( void ) {
    // Stds_AddFont( "res/fonts/nes.ttf", 12 );
    // Stds_AddFont( "res/fonts/nes.ttf", 18 );
    // Stds_AddFont( "res/fonts/nes.ttf", 24 );
  }

  /**
   * Adds a font to the font linked list in the app struct. A font can be loaded
   * in multiple times, each with different sizes.
   *
   * @param const char * font name.
   * @param const uint16_t size of font.
   *
   * @return void.
   */
  void Stds_AddFont( const char *font_file, const uint16_t size ) {
    struct font_t *f;
    f = malloc( sizeof( struct font_t ) );

    if ( f == NULL ) {
      SDL_LogInfo( SDL_LOG_CATEGORY_APPLICATION, "Could not allocate memory for font_t. %s.\n",
                   SDL_GetError() );
      exit( EXIT_FAILURE );
    }

    memset( f, 0, sizeof( struct font_t ) );

    f->font = TTF_OpenFont( font_file, size );

    if ( f->font == NULL ) {
      SDL_LogInfo( SDL_LOG_CATEGORY_APPLICATION,
                   "Could not load font_t %s, %d. Is the path correct?", font_file, size );
    }

    /* Push the font to the linked list. */
    strncpy( f->name, font_file, strlen( font_file ) + 1 );
    f->size = size;

    g_app.font_tail->next = f;
    g_app.font_tail       = f;
  }