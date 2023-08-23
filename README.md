For main controller code - Everything works currently, but there's an Issue when trying to make the code more efficient. Using a loop to repeat code for easy, med, and hard messes up wireless communication. Will try to update with more efficient code, reducing the lines to 400-500.

# Interactive-shooting-game



5 ESP-32 ARE NEEDED. CAN USE 4 IF DONT WANT MUSIC

There are 4 files,

MAC address retriever: *Do this first* returns the MAC address of the esp32 board. Upload to each board, save the MAC address from the serial monitor, and label all 5 accordingly.

Main Controller: has the code for the ... main controller. Upload to only the main board with the buttons, scoreboard, LCD screen etc...

3 targets: code for 3 targets. Upload to the 3 target boards

Music: code for background music, upload to one board

