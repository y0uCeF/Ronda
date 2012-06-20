#ifndef FUNCTIONS_H
#define FUNCTIONS_H

void init(card table[],player *p1, player *p2);
shortint distributeTable(card_num cardList[],card table[]);
shortint distributePlayer(card_num cardList[],player *pl);
short getSelectedHand(int x);
short getSelectedTable(int x, int y);
void play(player *p, card table[], short selectedHand, short selectedTable);
shortint render(card tabl[], player user, player computer, SDL_Surface *screen);
void delete(card table[], player user, player computer);
void treatEvent(SDL_Event event, shortint *running, player *user,short *selectedHand,short *selectedTable);

#endif
