# SDA5708 green 5x7 LED Display #


This is a small bit-banging driver for these displays, usually found in some Nokia DBOX1.

I recycled it together with four buttons, a small speaker+BC547 and a very cheap 
STM32F103C8T6-board from ebay (3$) and built an alarm clock.
That's why the charset only contains digits 0-9, chars 'a', ':' and a heart symbol.

**TODO**
- brightness control functions

**revisions**
- 0 - initial code
- 1 - added clear functions
- 2 - added readme
- 3 - added full charset, puts, putc, putc_at  
