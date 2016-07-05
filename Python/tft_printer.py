#!/usr/bin/env python

"""tft_printer.py: Prints a wrapped string to the Sensorian TFT LCD"""

import textwrap
from PIL import Image
from PIL import ImageDraw
from PIL import ImageFont
import TFT as GLCD

__author__ = "Michael Lescisin"
__copyright__ = "Copyright Sensorian 2015"
__maintainer__ = "Dylan Kauling"
__status__ = "Development"

LINE_SPACING = 12
LINES_VERTICAL = 10
LINES_HORIZONTAL = 9

height = GLCD.TFT_HEIGHT
width = GLCD.TFT_WIDTH

disp = GLCD.TFT()  # Create TFT LCD display class.
disp.initialize()  # Initialize display.
disp.clear()  # Alternatively can clear to a black screen by calling:
draw = disp.draw()  # Get a PIL Draw object to start drawing on the display buffer
font = ImageFont.truetype('/usr/share/fonts/truetype/freefont/FreeSansBold.ttf', 14)  # use a truetype font


# Print a string to the LCD at a given position and angle, called by screen_print_rotated
def draw_rotated_text(text, position, angle, draw_passed, colour, background, font_passed=font):
    # Get width and height/size of font to be fit on the screen
    text_width, text_height = draw_passed.textsize(text, font=font_passed)
    # Create a new image with transparent background to store the text.
    text_image = Image.new('RGBA', (text_width, text_height), background)
    # Render the text.
    text_draw = ImageDraw.Draw(text_image)
    text_draw.text((0, 0), text, font=font_passed, fill=colour)
    # Rotate the text image.
    rotated = text_image.rotate(angle, expand=1)
    # Paste the text into the image, using it as a mask for transparency.
    draw_passed.bitmap(position, rotated)


# Prints a string to the LCD in portrait mode
def screen_print(text):
    line_length = 16  # The number of characters that fit in a line in portrait mode with the default font
    text = textwrap.wrap(text, line_length)  # Wrap the passed string to fit the screen as best as possible
    if len(text) > LINES_VERTICAL:  # If the string won't fit the screen, only use how much will fit
        text = text[0:LINES_VERTICAL]
    disp.clear()  # Clears the screen to black before printing again
    for i in range(0, len(text)):  # Draw each of the lines of text on the screen
        draw.text((0, LINE_SPACING * i), text[i], font=font)
    disp.display()  # Display the drawn text


# Prints text to the TFT-LCD display based on the specified orientation, calls draw_rotated_text
def screen_print_rotated(text, orientation, colour=(255, 255, 255), background=(0, 0, 0, 0)):
    """
    Prints text to the TFT-LCD display based on the specified orientation.
        Orientations:
            0: Text is printed so that it is readable with the SD card pointing
                upwards and the screen facing the user.

            1: RPi + Sensorian is in direction of +Z towards the user, +Y
                towards ceiling, and +X towards the user's right hand.
                Capacitive touch button texts 'B1', 'B2', 'B3' are in their
                normal reading orientation.
    """
    if orientation == 0:  # If drawing in portrait mode
        screen_print(text)  # Call the function specifically simplified for portrait mode

    elif orientation == 1:  # If drawing in landscape mode
        draw_position = (0, 160)  # The position to draw the text, (0, 160) when landscape
        draw_angle = 90    # The angle at which to draw the text, 90 degrees for landscape
        line_length = 20  # The number of characters that fit in a line in landscape mode with the default font
        text = textwrap.wrap(text, line_length)  # Wrap the passed string to fit the screen as best as possible
        if len(text) > LINES_HORIZONTAL:  # If the string won't fit the screen, only use how much will fit
            text = text[0:LINES_HORIZONTAL]
        disp.clear()  # Clears the screen to black before printing again
        for i in range(0, len(text)):  # Draw each of the lines of text on the screen
            t_width, t_height = draw.textsize(text[i], font=font)  # Get the size of the font to be fit on the screen
            draw_rotated_text(text=text[i], position=(draw_position[0] + LINE_SPACING * i, draw_position[1] - t_width),
                              angle=draw_angle, draw_passed=draw, colour=colour, background=background)
        disp.display()  # Display the drawn text
