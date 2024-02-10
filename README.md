#Airsoft target shooting game.

*****************I MADE THIS PROJECT BEFORE I HAD KNOWLEDGE OF OBJECT-ORIENTED PROGRAMMING AND DATA STRUCTURES. LOTS OF THIS CODE IS  REPETITIVE BUT IT WORKS. AM MUCH BETTER NOW *********************************

Players select difficulty and mode from an LCD menu using buttons.
LED targets randomly light up and stay on based on selected difficulty (2 sec for hard, 4 sec for medium, 6 sec for easy).
Hitting a target before it turns off updates the score and triggers a new target.
In 2-player mode, the game saves and compares scores, announcing the winner on the LCD screen at the end.


5 ESP-32 ARE NEEDED. CAN USE 4 IF DONT WANT MUSIC

There are 4 files,

MAC address retriever (esp32_now_macadress_getter) : *Do this first* returns the MAC address of the esp32 board. Upload to each board, save the MAC address from the serial monitor, and label all 5 accordingly.

Main Controller (gun_game_main): has the code for the main controller. Upload to only the main board with the buttons, scoreboard, LCD screen, etc...

3 targets (3nodes): code for 3 targets. Upload to the 3 target boards

Music: code for background music, upload to one board

