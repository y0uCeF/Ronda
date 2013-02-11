ReadMe
======

Introduction
------------

**Ronda** is a fishing game, it has Mediterranean origins, it is traced back 
to Spain and north Africa (specially Morocco). Ronda is played with 
Spanish/Italian deck : 40 cards containing 4 suits : coins, clubs, swords 
and cups. each suit having 10 cards :
cards from 1 to 7 plus : 10 (Sota/Jack), 11 (Caballo/Horse) and 12 (Rey/King).

License: GPLv2

You can get the latest sources from the git repository:  

```
git clone git://github.com/y0uCeF/Ronda.git
```

Building Ronda
--------------

This was only tested on *Linux*,  
Ronda requires *SDL-1.2+*, *SDL_image* and *SDL_ttf* to be installed in your
system. You can get them from [here](http://libsdl.org/).
Once you got the requirements you can run :  

```
make
make install
```

In case you want to run Ronda *in-place* without installit it, then
running :  

```
make 'INPLACE=y'
```
should work perfectly.

You can uninstall Ronda by running  

```
make uninstall
```

How to play
-----------

### Game start/ending ###

In the first deal each player is given 3 cards, and other 4 cards are 
put on the table (not containing duplicates), and when all 6 cards are 
used a new round starts with the distribution of other 6 cards from the
 deck.
 
The game ends when no cards remain in the deck.

### Announces ###

After each deal the player can announce :

- Ronda : when he has 2 similar cards (same card number - same rank).

- Tringla : when he obtains 3 similar cards.

Announces result extra points as explained in Points_ section.

### Main game ###

* Each player (in his turn) has 2 options:

 - He can capture a card from the table by using one similar card from 
 his hand and keep taking cards incrementally starting from that number.
 - He can put a card on the table when he can't capture any card.

* The player can also gain points by doing one of this acts:

  * Mesa : when a player takes all the cards on the table (except when 
  it's the final deal and the player is the last one)
  * Este : when a player takes the last card dropped by his opponent 
  (check Points_ section)

* When in the last deal, and after the last turn. If there still are 
cards on the table, the last player who captured a card takes them.

### Points ###

A player can get points when : 

- He announced Ronda and the opponent didn't : 1 point
- Both players announced Ronda with equal rank : 1 point
- Both players announced Ronda and he has the bigger number : 2 points
- He announced Tringla and the opponent didn't : 5 point
- Both players announced Tringla with equal rank : 1 point
- Both players announced Tringla and he has the bigger number : 10 points
- He announced Tringla and the opponent announced Ronda : 6 points
- He makes an Este : 1 point
- When the game ends, all the cards above the 20th are counted as points

### Controls ###

Choose a card from your hand by clicking on it with left_mouse_button, 
choose a valid location on the table (similar card/empty slot) and you 
finished your turn (It's simple as that).


### Notes ###

Ronda and Tringla are announced automatically and their points are calculated
at the end of the deal.
