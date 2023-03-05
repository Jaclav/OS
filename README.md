# OS
Simple 16 bit operating system. My goal is to achive:
* errors management
* disk management
* file read/write
* simple programs (file manager,file editor, games, image viewer)
* user's custom programs execution
* dynamic memory management
* change to protected mode
* protection rings
* multithreading

All sectors in BIOS *are counted from 1*
## Commands
| Command | Description                     |
| :------ | :------------------------------ |
| cls     | clear screen                    |
| pos     | print cursor position           |
| key     | get keycode                     |
| mode $  | change video mode to $          |
| test    | test kernel library             |
| load $  | Execute program from $th sector |
| pic     | Draw image.h file               |
| sec     | Print $th sector                |
| ls      | Print files and their sizes     |
## SYSTEM Interruptions
0x20 is system interruption
| AH   | Description               | Parameters            |
| :--- | :------------------------ | :-------------------- |
| 1    | print character on screen | AL - character        |
| 2    | print string on screen    | BX - address ofstring |
| 3    | print integer on screen   | BX - number           |