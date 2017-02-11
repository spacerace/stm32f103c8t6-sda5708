# SDA5708 green 5x7 LED Display #


This is a small bit-banging driver for these displays, usually found in some Nokia DBOX1.

I recycled it together with four buttons, a small speaker+BC547 and a very cheap 
STM32F103C8T6-board from ebay (3$) and built an alarm clock.
That's why the charset only contains digits 0-9, chars 'a', ':' and a heart symbol.

**TODO**
- brightness control functions
- charset, after that:
- output functions:
  putc(c)			print charater to cursor position
  set_cursor(position)		set cursor to digit 0-7
  puts(s) 			print string, only first 8 chars will be displayed
  puts_scroll(speed, s)		print string, at the end of line, scroll text 
  scroll_lr_loop(s)		scroll in an endless loop a string from left to right and vv

**revisions**
- 0 - initial code
- 1 - added clear functions
- 2 - added readme
