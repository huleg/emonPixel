/*
 * by Trystan Lea, Glyn Hudson, OpenEnergyMonitor
 * All adaptation GNU General Public License as below.
 *
 * -------------------------------------------------------------------
 *
 * This file is part of OpenEnergyMonitor.org project.
 * EmonESP is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * EmonESP is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License
 * along with EmonESP; see the file COPYING.  If not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#include <pixel.h>
//#include <NeoPixelBus.h>
#include <emonesp.h>

// ESP Bitbang method using GPIO4

// void pixel_setup(uint16_t PixelCount, uint16_t PixelPin)
// {
// NeoPixelBus<NeoGrbFeature, NeoEsp8266BitBang800KbpsMethod> strip(PixelCount, PixelPin);
// }

//NeoPixelBus<NeoGrbFeature, NeoEsp8266BitBang800KbpsMethod> strip(PixelCount, PixelPin);
WS2812FX strip = WS2812FX(PixelCount, PixelPin, NEO_GRB + NEO_KHZ400);

uint32_t red = strip.Color(colorSaturation, 0, 0);
uint32_t green= strip.Color(0, colorSaturation, 0);
uint32_t blue= strip.Color(0, 0, colorSaturation);
uint32_t white= strip.Color(255,255,255);
uint32_t black= strip.Color(0,0,0);

uint8_t c_count = 0;
uint8_t a_count = 0;
uint8_t c_target = 0;
uint8_t a_target = 0;
uint32_t c_colour= black;
uint32_t a_colour= black;

uint32_t bgColour = black;


void pixel_begin()
{
        // this resets all the neopixels to an off state
        //strip.Begin();
        strip.init();
        strip.Show();
}

void set_c_target(uint8_t target, uint8_t red, uint8_t green, uint8_t blue)
{

        c_colour = strip.Color(red,green,blue);
        c_target = target;
        c_count = 0;
}
void set_a_target(uint8_t target, uint8_t red, uint8_t green, uint8_t blue)
{

        a_colour = strip.Color(red,green,blue);
        a_target = target;
        a_count = 0;
}
void update_a_target()
{

        if (a_count == a_target || c_target != c_count) { // always go clockwise first
                return; //target reached do nothing
        }
        if (a_count % PixelCount == 0)
        {
                pixel_off();
                delay(400);
                yield();
                for(uint8_t loop_count = 0; loop_count < (int)(a_count / PixelCount); loop_count++) {
                        set_all_pixels(a_colour);
                        delay(3*FLASH_LOOP_DELAY);
                        yield();
                        pixel_off();
                        delay(2*FLASH_LOOP_DELAY);
                        yield();





                }
                refresh_background();
                set_c_target_state();
        }
        DEBUG.println(String(a_count) +  " : " + String(PixelCount - (a_count % PixelCount)));
        strip.SetPixelColor(PixelCount - ((a_count % PixelCount) + 1),a_colour);
        strip.Show();
        a_count++;
        delay(FLASH_LOOP_DELAY);

}
void update_c_target()
{
        if (c_count == c_target) {
                return; //target reached do nothing
        }
        if (c_count == 0) {
                //start of a new sequence
                refresh_background();
        } else if (c_count % PixelCount == 0)
        {
                pixel_off();
                delay(2*FLASH_LOOP_DELAY);
                yield();
                for(uint8_t loop_count = 0; loop_count < (int)(c_count / PixelCount); loop_count++) {
                        set_all_pixels(c_colour);
                        delay(3*FLASH_LOOP_DELAY);
                        yield();
                        pixel_off();
                        delay(2*FLASH_LOOP_DELAY);
                        yield();


                }
                refresh_background();
        }
        strip.SetPixelColor(c_count % PixelCount,c_colour);
        strip.Show();
        c_count++;
        delay(FLASH_LOOP_DELAY);

}
void set_c_target_state(){
        for (uint16_t pixel = 0; pixel < PixelCount && pixel < (c_count % PixelCount); pixel++)
        {
                strip.SetPixelColor(pixel % PixelCount,c_colour);
        }
        strip.Show();
}
void random_pixel_setup(){
        for (uint16_t pixel = 0; pixel < PixelCount; pixel++)
        {
                uint32_t color = strip.Color(random(255), random(255), random(255));
                strip.SetPixelColor(pixel, color);
        }
        strip.Show();

}

void pixel_rgb_demo(){
        for (uint16_t pixel = 0; pixel < PixelCount; pixel++)
        {
                switch(pixel%4)
                {
                case 0:
                        strip.SetPixelColor(pixel,red);
                        break;
                case 1:
                        strip.SetPixelColor(pixel,green);
                        break;
                case 2:
                        strip.SetPixelColor(pixel,blue);
                        break;
                case 3:
                        strip.SetPixelColor(pixel,white);
                        break;
                }
                strip.Show();
                delay(50);
        }

}

void pixel_off(){
        for (uint16_t pixel = 0; pixel < PixelCount; pixel++)
        {
                strip.SetPixelColor(pixel, black);
        }
        strip.Show();

}
void set_pixel(uint8_t pixel, uint8_t red, uint8_t green, uint8_t blue)
{
        strip.SetPixelColor(pixel, strip.Color(red,green,blue));
        strip.Show();
}
void set_background(uint8_t red, uint8_t green, uint8_t blue)
{
        DEBUG.println("Set backgorund to red: " + String(red) + " Green: " + String(green) + " Blue: " + String(blue) );
        bgColour =  strip.Color(red,green,blue);
        //refresh_background();
}

void refresh_background(){
        for (uint16_t pixel = 0; pixel < PixelCount; pixel++)
        {
                strip.SetPixelColor(pixel, bgColour);
        }
        strip.Show();

}
void set_all_pixels(uint32_t theColour){
        for (uint16_t pixel = 0; pixel < PixelCount; pixel++)
        {
                strip.SetPixelColor(pixel,theColour);
        }
        strip.Show();

}

void set_ap_mode(){
        strip.stop();
        strip.setSpeed(100);
        strip.setColor(blue);
        strip.setMode(FX_MODE_RAINBOW_CYCLE);
        //strip.setMode(FX_MODE_BREATH);

        strip.start();

}

void set_wifi_count(uint8_t count){
        strip.stop();

        uint32_t col=white;
        switch (count % 4) {
        case 0:
                col = red;
                break;
        case 1:
                col = green;
                break;
        case 2:
                col = blue;
                break;
        case 3:
                col = white;
                break;

        }
        strip.setBrightness(255);
        strip.setSpeed(255);
        strip.setColor(col);
        strip.setMode(FX_MODE_BREATH);
        strip.start();

}
void set_mqtt_connecting() {

  //connect blocks so can't do much here :()

        strip.stop();
        //pixel_rgb_demo()
        //strip.setSpeed(200);
        delay(500);
        strip.setColor(red);
        strip.setMode(FX_MODE_RAINBOW_CYCLE);
        for(int i =0; i < PixelCount; i++)
        strip.start();
        {
          strip.service();
        }

        strip.setSpeed(0);
}
void set_mqtt_connected() {
        strip.stop();

        strip.setSpeed(100);
        strip.setColor(blue);
        strip.setMode(FX_MODE_COLOR_WIPE);
        strip.start();
}
extern void pixel_loop(){
        if(strip.isRunning())
        {
                strip.service();
        }
        yield();
}
extern void stop_animations()
{
        if(strip.isRunning())
        {
                strip.stop();
        }
}
