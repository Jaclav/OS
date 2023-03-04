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
## Commands
| Command | Description                                              |
| :------ | :------------------------------------------------------- |
| cls     | clear screen                                             |
| pos     | print cursor position                                    |
| key     | get keycode                                              |
| mode $  | change video mode to $                                   |
| test    | test kernel library                                      |
| disk $  | Execute and print 512B from $th sector  (counted from 1) |
| pic     | Draw image.h file                                        |
## SYSTEM Interruptions
0x20 is system interruption
| AH   | Description               | Parameters            |
| :--- | :------------------------ | :-------------------- |
| 1    | print character on screen | AL - character        |
| 2    | print string on screen    | BX - address ofstring |
| 3    | print integer on screen   | BX - number           |