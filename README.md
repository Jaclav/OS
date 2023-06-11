# OS
Simple 16 bit operating system. My goal is to achive:
* errors management
* simple programs (file manager,file editor, games, image viewer)
* user's custom programs execution
* dynamic memory management
* change to protected mode
* protection rings
* multithreading

All sectors in BIOS **are counted from 1** and are 512B<br>
[Documentation ](http://192.168.0.60/pi/OS_doc/)<br>
Screenshot:<br>
![Screenshot](screenshot.png)
# Commands
| Command | Description                                      |
| :------ | :----------------------------------------------- |
| $       | Execute program $.com, see [Programs](#Programs) |
| cls     | clear screen                                     |
| pos     | print cursor position                            |
| key     | get information                                     |
| mode $1 | change video mode to $1                          |
| ls      | Print files and their sizes                      |
| map     | Show map of used sectors                         |
| rm $1   | Remove $1 file                                   |

## Programs
Programs<span id="Programs"></span> installed on OS are in directory disk/<br>
To call program.com with parameters enter "program parameters"
# SYSTEM Interruptions
0x20 is system IO interruption, see syscall()<br>
0x21 is file system intterruption, if error it will return negative value like [errno](https://chromium.googlesource.com/chromiumos/docs/+/master/constants/errnos.md) see int0x21()


# Program loading and executing
Program is loaded into address (CS+0x1000):0x100.<br>
Bytes from (CS+0x1000):0 to (CS+0x1000):0xff are reserved for OS i.e:
* 0x00 to 0x08 stores call function to 0x100
* 0x80 to 0xff stores string of program's paramaters

If size of program is $ sectors size, then on $*200+100 bytes is stored automatic return subprogram, it is executed when program don't end on it's own