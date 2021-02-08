#ifndef DB_INDEX_H
#define DB_INDEX_H
#include "struct.h"
Index get_ind_by_pos(int pos);

Index get_ind_by_key(int key);

int is_key(int key);

int ind_pos_by_key(int key);

#endif //DB_INDEX_H
