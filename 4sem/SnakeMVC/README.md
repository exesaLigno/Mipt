# Model-View-Controller Snake Game

For the introduction - this is revision of classic NOKIA snake game.
It works with two modes - single and with ai.

## Structure

Controller getting events from sfml window and remakes it to specific MVC format.
After that event going to Model, that is parsing it and making some staff inside.

When event successfully parsed and all internal actions completed, we can
get "Game Representation" from model. Game Representation is very simple class,
contains all blocks, needed to draw and center text.

After getting representation, we going directly to View, sfml window, that drawing
all staff from representation with internal logic.
