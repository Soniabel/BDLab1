#include "index.h"
#include "struct.h"

Index get_ind_by_pos(int pos) {
    FILE* f = fopen("M.ind", "rb");
    Index res = { -1, -1 ,0 };
    fseek(f, (pos)*I_SIZE, SEEK_SET);
    fread(&res, I_SIZE, 1, f);
    fclose(f);
    return res;
}

Index get_ind_by_key(int key) {
    Index temp;
    int m = 0;
    do {
        temp = get_ind_by_pos(m);
        ++m;
    } while (temp.key != key);
    if (temp.key == key)
        return temp;
    return (Index) { -1, -1, 0 };
}

int is_key(int key) {
    Index ind = get_ind_by_key(key);
    return (ind.key > 0 || ind.pos > 0);
}

int ind_pos_by_key(int key) {
    Index temp;
    int m = -1;
    do {
        ++m;
        temp = get_ind_by_pos(m);
    } while (!temp.key == key);
    if (temp.key == key)
        return m;
    return -10;
}
