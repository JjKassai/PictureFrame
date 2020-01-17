# PictureFrame

Notes on Hardware Setup:

Arduino Nano, bought on ebay.  Bootloader installed using Atmel Studio - first file in the bootloader directory under ATMEGA.
RTC is the DSxxxx from Maxim bought on a breakout with CR2032 from ebay

Notes on software requirements:
Use the normal bootloader option when loading in arduino software (Rather than old bootloader).
Requires the lady ada graphics library for the OLED (chip name?)
Requires a specific RTC library, have to find.  Library has an example used to set the RTC with a specific syntax over the serial monitor.
