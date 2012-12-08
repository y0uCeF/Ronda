#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "define.h"

typedef struct {
        bool exit;
        int selected_card_hand;
        int selected_card_table;
} controller_data;

/*
 * initializes controller data with default values
 */ 
controller_data* controller_data_init();

/*
 * updates the data based on input results
 */
void controller_data_update(controller_data *c_data);
#endif
