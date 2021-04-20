 Connect Four Game
 Author: Klaudia Jaros

### Description:

Connect Four is a two-player connection board game, in which the players choose a colour and then take turns dropping coloured discs into a seven-column, 
six-row vertically suspended grid. The pieces fall straight down, occupying the lowest available space within the column. 
The objective of the game is to be the first to form a horizontal, vertical, or diagonal line of four of one's own discs

My implementation of Connect Four is a command line application with a Text User Interface and it is written in the C language using standard libraries.

### Game options:

* Multiplayer

Play with your Friend

* Single player

Play against a Computer  

* Undo and redo moves

Both Single Player and Multiplayer games can be played in assisted mode where the players can undo and redo their moves. Great for beginners!

* View game history

Go to Game History to see a replay of your previous games.

* Change board sizes

Size variations include: 7x6 (classic), 5×4, 6×5, 8×7, 9×7, 10×7, 8×8

### Requirements:

Visual Studio Developer Command Prompt and Windows OS
I used: Visual Studio 2019 Developer Command Prompt v16.8.2

### How to install:

Open your Visual Studio Developer Command Prompt and use the command: `nmake install` to install the application.
Nmake uses makefile instructions to compile and link the main connectFour.c file with the libraries written by me: stack.c and gameHistory.c 

You are ready to use the application now.

### How to run:

Use the command: `nmake run` or simply type `connectFour.exe`

### How to delete:

Open your Visual Studio Developer Command Prompt and use the command: `nmake delete`