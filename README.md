# Chess
I'm currently working on this Chess app, written in C++ with rendering handled in OpenGL. You can get a sense for my workflow and current progress by looking at the trello board I use for this project at https://trello.com/b/t6SrmE43/chess-app. I've written about some of my goals, motivation, and experiences with this project below. I hope my discussion of various design choices and implementation details can be helpful for anyone reading this that might be interested in writing their own Chess app.

In the source you will find two folders. Engine contains all of the application framework, which I kickstarted by using the boilerplate available in one of my other repos. I have added to it, settling on a layer representation. The application class contains a single (when I need multiple I will just have a stack of them) Layer object, which must implement the usual game loop functions (update, render, event handling, enable, disable, etc). The application handles all windowing events, and propagates relevant ones to the layerstack to be handled by the game itself. The Chess folder unsurprisingly contains all the Chess code. It contains a Game class, derived from the abstract base Layer class described above, which is where we override the update, render, event functions etc to add desired functionality.

## Why Chess?
Firstly, I enjoy Chess. It's a deceptively simple game which rewards intuition and pattern recognition, this presents a decent level of challenge for anyone trying to write a (competent) AI player. It's also a turn based game, which lends itself to an event-driven game loop, a style of program I haven't had much experience in writing. The rendering and visuals are simple enough that the bulk of my focus can be on writing the actual game logic, while still leaving scope for me to work on UI and test out different design choices.

Once the core functionality is in, and you can enjoy a quick game against a relatively dimwitted AI opponent, there are a few ways I might look to extend the program. Some simple examples that spring to mind include:
- (De)serialization of game states with FEN notation https://www.chessprogramming.org/Forsyth-Edwards_Notation
- Implementation of accesibility options, i.e. customisable color palettes for colorblind users.
- Advanced AI, and even AI "personalities", i.e. adjusted parameters for the model which result in different playstyles.
- Challenges and mating practice.
- Detection of common openers.

Last Edited (03/07/2023)
