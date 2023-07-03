# Chess
I'm currently working on this Chess app, written in C++ with rendering handled in OpenGL. You can get a sense for my workflow and current progress by looking at the trello board I use for this project at https://trello.com/b/t6SrmE43/chess-app. I've written about some of my goals, motivation, and experiences with this project below. I hope my discussion of various design choices and implementation details can be helpful for anyone reading this that might be interested in writing their own Chess app.

In the source you will find two folders. Engine contains all of the application framework, which I kickstarted by using the boilerplate available in one of my other repos. I have added to it, settling on a layer representation. The application class contains a single (when I need multiple I will just have a stack of them) Layer object, which must implement the usual game loop functions (update, render, event handling, enable, disable, etc). The application handles all windowing events, and propagates relevant ones to the layerstack to be handled by the game itself. The Chess folder unsurprisingly contains all the Chess code. It contains a Game class, derived from the abstract base Layer class described above. This serves as the hook for the user of the app framework to interface with the engine.

## Why Chess?
Firstly, I enjoy Chess. It's a deceptively simple game which rewards intuition and pattern recognition, this presents a decent level of challenge for anyone trying to write a (competent) AI player. It's also a turn based game, which lends itself to an event-driven game loop, a style of program I haven't had much experience in writing. The rendering and visuals are simple enough that the bulk of my focus can be on writing the actual game logic, while still leaving scope for me to work on UI and test out different design choices.

Once the core functionality is in, and you can enjoy a quick game against a relatively dimwitted AI opponent, there are a multitude of ways in which I may look to extend the program. Some simple examples that spring to mind include:
- Serialization and Deserialization of game states using FEN notation https://www.chessprogramming.org/Forsyth-Edwards_Notation
- Implementation of accesibility options, the most obvious and simple of which being customisable color palettes for colorblind users.
- Advanced AI, and even AI "personalities", i.e. adjusted parameters for the model which result in different playstyles.
- Challenges and mating practice.
- Detection of common openers.

Last Edited (03/07/2023)
