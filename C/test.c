#include <Python.h>
#include <stdbool.h>
#include <stdint.h>

#define MAX_VERTS 10000

typedef struct {
    uint64_t* data;
    int nbits;
    int count;
    //Attention c'est juste le nb d'elems dans l'array data c'est tt
    int n_elems; 
} Bitmap;

Bitmap* bitmap_create(int nbits) {
    Bitmap* b = malloc(sizeof(Bitmap));
    b->nbits = nbits;
    b->count = 0;
    b->n_elems = (nbits + 63) / 64;
    b->data = calloc(b->n_elems, sizeof(uint64_t));
    return b;
}

void bitmap_free(Bitmap* b) {
    free(b->data);
    free(b);
}

void bitmap_set(Bitmap* b, int pos) {
    b->data[pos / 64] |= 1ULL << (pos % 64);
}

int bitmap_val(Bitmap* b, int pos) {
    return (b->data[pos / 64] >> (pos % 64)) & 1;
}

void bitmap_reset(Bitmap* b) {
    memset(b->data, 0, b->n_elems * sizeof(uint64_t));
}

void bitmap_and(Bitmap* dest, Bitmap* a, Bitmap* b) {
    for (int i = 0; i < a->n_elems; i++) {
        dest->data[i] = a->data[i] & b->data[i];
    }
}
void bitmap_or(Bitmap* dest, Bitmap* a, Bitmap* b) {
    for (int i = 0; i < a->n_elems; i++) {
        dest->data[i] = a->data[i] | b->data[i];
    }
}

void bitmap_or_inplace(Bitmap* a, Bitmap* b) {
    for (int i = 0; i < a->n_elems; i++) {
        a->data[i] = a->data[i] | b->data[i];
    }
}
void bitmap_and_inplace(Bitmap* a, Bitmap* b) {
    for (int i = 0; i < a->n_elems; i++) {
        a->data[i] = a->data[i] & b->data[i];
    }
}
bool bitmap_is_empty(Bitmap* b) {
    for (int i = 0; i < b->n_elems; i++) {
        if (b->data[i] != 0) {
            return false;
        }
    }
    return true;
}

void bitmap_comp(Bitmap* a, Bitmap* b) {
    for (int i = 0; i < b->n_elems-1; i++) {
        a->data[i] = ~b->data[i];
    }
    uint64_t mask = (1ULL << (b->nbits % 64)) - 1; 
    if (b->nbits % 64 != 0) {
        a->data[b->n_elems-1] = ~b->data[b->n_elems-1] & mask;
    }
}

unsigned int ctlz(uint64_t x) {
    unsigned long index;
    
    if (_BitScanForward64(&index, x)) {
        return (unsigned int)index; 
    }

    return 64;
}

//ULTRA LENT
void bitmap_print(Bitmap* b) {
    for (int i = 0; i < b->n_elems; i++) {
        uint64_t word = b->data[i];

        for (int j = 0; j < 64; j++) {
            if (bitmap_val(b, 64*i + j)) {
                printf("1");
            } else {
                printf("0");
            }
        }
        printf(" ");
    }
    printf("\n");
}
int bitmap_next_bit(Bitmap* b, int i) {
    int word_i = i / 64;
    int bit_pos = i % 64;

    //On regarde sur l'élément actuel si y'a pas de pb
    //Faut bien shift pck ctzl regarde tout à gauche et on veut le prochain à droite de l'élément
    uint64_t word = b->data[word_i];
    word >>= bit_pos;
    if (word != 0) {
        return i + ctlz(word);
    }

    //On regarde sur les prochains élems mais mtn pas besoin de shift
    for (word_i++; word_i < b->n_elems; word_i++) {
        word = b->data[word_i];
        if (word != 0) {
            return word_i * 64 + ctlz(word);
        }
    }

    return -1;
}

int bitmap_first_bit(Bitmap* b){
    uint64_t word = b->data[0];
    if (word == 0) {
        return -1; 
    }
    return ctlz(word);
}

//Si besoin de plus de rapidité y'a des algos sur : https://en.wikipedia.org/wiki/Hamming_weight
//Pour l'instant ça ira
int bitmap_count(Bitmap* b) {
    int count = 0;
    for (int i = 0; i < b->n_elems; i++) {
        uint64_t word = b->data[i];
        while (word) {
            count++;
            word &= (word - 1);
        }
    }
    return count;
}

void bitmap_copy(Bitmap* dest, Bitmap* src) {
    memcpy(dest->data, src->data, src->n_elems * sizeof(uint64_t));
    dest->count = src->count;
}

bool bitmap_is_superset(Bitmap* a, Bitmap* b) {
    for (int i = 0; i < a->n_elems; i++) {
        if ((a->data[i] & b->data[i]) != b->data[i]) {
            return false; 
        }
    }

    return true;
}

bool bitmap_have_superset(Bitmap** a, Bitmap* b, int len_arr) {
    
    for(int i = 0; i < len_arr; i++){
        if(bitmap_is_superset(b, a[i])) return true;
    }
    return false;
}

static PyObject* dong_li(PyObject* self, PyObject* args) {
    PyObject* py_edges;
    //https://docs.python.org/3/c-api/arg.html#c.PyArg_ParseTuple
    if (!PyArg_ParseTuple(args, "O", &py_edges))
        return NULL;

    //https://docs.python.org/3/c-api/list.html#c.PyList_Check
    if(!PyList_Check(py_edges)){
        //https://docs.python.org/3/c-api/exceptions.html#c.PyErr_SetString
        PyErr_SetString(PyExc_TypeError, "Attendait une liste de liste, a eu ???");
        return NULL;
    }

    //https://docs.python.org/3/c-api/list.html#c.PyList_Size
    Py_ssize_t nb_edges = PyList_Size(py_edges);
    int** edges = malloc(nb_edges * sizeof(int*));
    Py_ssize_t* len_edges = malloc(nb_edges * sizeof(Py_ssize_t));

    //On a besoin du nb de vertices aussi
    //Pour ça on va faire une bitmap avec 10000 bits (1Ko de mémoire normalement) 
    //On peut difficilement traiter des hypergraphes avec 100 sommets donc ça devrait aller
    Bitmap* vertices = bitmap_create(MAX_VERTS);

    for(Py_ssize_t i = 0; i < nb_edges; i++){
        //https://docs.python.org/3/c-api/list.html#c.PyList_GetItem
        PyObject* py_edge = PyList_GetItem(py_edges, i);
        if(!PyList_Check(py_edge)) {
            PyErr_SetString(PyExc_TypeError, "Attendait une liste de liste, a eu ???");
            return NULL;
        }

        Py_ssize_t nb_vertices = PyList_Size(py_edge);
        len_edges[i] = nb_vertices;
        edges[i] = malloc(nb_vertices * sizeof(int));
        
        for(Py_ssize_t j = 0; j < nb_vertices; j++) {
            PyObject* cur = PyList_GetItem(py_edge, j);
            //En fait en théorie on peut utiliser ça :
            //https://docs.python.org/3/c-api/long.html#c.PyLong_AsInt
            //Mais c'est tout nouveau (3.13) donc il faut utiliser AsLong en attendant
            //Ce serait bien de check au cas où on met n'imp
            int val = (int) PyLong_AsLong(cur);
            bitmap_set(vertices, val-1);
            edges[i][j] = val;
            //printf("%d ", val);
        }
        //printf("\n");
    }

    int nb_vertices = bitmap_count(vertices);
    //printf("Salut %d %d \n", nb_vertices, nb_edges);
    //Donc là on a bien un tableau de tableaux en C !
    //On peut faire Dong Li
    

    //Chaque itération ressemble à :
    // - On prend les transversaux qui couvrent déjà l'arête qu'on ajoute
    // - On prend les vertices de ces transversaux
    // - On fait une array avec les transversaux qui ne couvrent pas
    // - De même on prend les vertices qui ne sont pas utilisées pour couvrir
    // - On fusionne les 2 avec v et on les ajoute

    //On init avec littéralement la première arête en bitmap de bitmaps
    Bitmap** TrCur = malloc(len_edges[0] * sizeof(Bitmap));
    int len_TrCur = len_edges[0];

    for(int i = 0; i < len_TrCur; i++){
        TrCur[i] = bitmap_create(nb_vertices);
        bitmap_reset(TrCur[i]);
        bitmap_set(TrCur[i], edges[0][i]-1);
        TrCur[i]->count++;
    }

    //Une fois fait on itère de 1 à nb_edges-1
    for(Py_ssize_t i = 1; i < nb_edges; i++){
        //printf("On est %d\n", i);
        //On met e_i sous forme de bitmap !
        Bitmap* ei = bitmap_create(nb_vertices);
        for(int j = 0; j < len_edges[i]; j++){
            bitmap_set(ei, edges[i][j]-1);
        }
        //printf("ei : \n");
        //bitmap_print(ei);
        //printf("CP1\n");
        //Comme ça on peut aussi mettre covered sous forme de bitmap ? Qu'on remplit quand on trouve un non vide ?
        //Avec un or !! Rapide de fou en sah
        Bitmap* covered = bitmap_create(nb_vertices);
        bitmap_reset(covered);
        //printf("CP2\n");
        //On fait TrGuaranteed, on sait qu'il y a au plus lenTrCur * len_edges[i] éléments dedans
        //Pareil pour le TrT (Tr à traiter !)
        Bitmap** TrGuaranteed = malloc(len_edges[i] * len_TrCur * sizeof(Bitmap));
        Bitmap** TrT = malloc(len_edges[i] * len_TrCur * sizeof(Bitmap));
        int cur_lenTrG = 0;
        int cur_lenTrT = 0;

        for(int j = 0; j < len_TrCur; j++){
            //printf("On est %d %d\n", i, j);
            Bitmap* cur = TrCur[j];
            //for(int cur_pos = bitmap_first_bit(TrCur[j]); cur_pos != -1; cur_pos = bitmap_next_bit(TrCur[j], cur_pos+1)){
                //printf("%d ", cur_pos);
            //}
            //printf("\n");
            Bitmap* res = bitmap_create(nb_vertices);
            bitmap_and(res, cur, ei);

            //Pour res on a 2 choix :
            //Si c'est 0 alors y'a rien en commun =  ça va dans TrT
            //Sinon ça va dans TrGuaranteed et on regarde si cur est de taille 1 (si oui alors c un elem de ei !)

            if(bitmap_is_empty(res)){
                TrT[cur_lenTrT] = cur;
                cur_lenTrT++;
            }
            else {
                TrGuaranteed[cur_lenTrG] = cur;
                cur_lenTrG++;
                //On met à jour covered
                if(cur->count == 1)
                    bitmap_or_inplace(covered, res);
            }
        }

        //Le tri est fait, mais mtn il faut faire eicovered et eiuncov
        //eicov c'est juste covered and ei, uncov c'est juste son complémentaire
        //bitmap_and_inplace(covered,ei);
        Bitmap* uncovered = bitmap_create(nb_vertices);
        bitmap_comp(uncovered, covered);
        bitmap_and_inplace(uncovered,ei);
        //printf("uncov : \n");
        //bitmap_print(uncovered);
        //printf("CP4 len TrT %d TrG %d \n", cur_lenTrT, cur_lenTrG);
        //Si TrCur est déjà bien ordonné alors c'est bon
        //Par contre il faut une bitmap de bitmaps avec les nouveaux Tr pour les merge ensuite !
        Bitmap** newTrs = malloc(len_edges[i] * len_TrCur * sizeof(Bitmap));
        int cur_lenNew = 0;

        for(int j = 0; j < cur_lenTrT; j++){
            //printf("On est (postprocess) %d %d\n", i, j);
            for(int cur_pos = bitmap_first_bit(uncovered); cur_pos != -1; cur_pos = bitmap_next_bit(uncovered, cur_pos+1)){
                //printf("On est (postprocess) %d %d %d\n", i, j, cur_pos);
                Bitmap* new = bitmap_create(nb_vertices);
                bitmap_copy(new, TrT[j]);
                bitmap_set(new, cur_pos);
                //bitmap_print(new);
                new->count++;
                //printf("Sa taille : %d\n", new->count);
                //On peut faire le count++ ici parce que justement l'intersection est vide !
                if(!bitmap_have_superset(newTrs, new, cur_lenNew) && !bitmap_have_superset(TrGuaranteed, new, cur_lenTrG)){
                    //On l'ajoute à newTrs !!
                    newTrs[cur_lenNew] = new;
                    cur_lenNew++;
                }
                else{
                    //Là juste on free new pck il a flop
                    bitmap_free(new);
                }
            }
        }
        
        ////printf("CP5\n");

        //Maintenant on doit merge !
        Bitmap** final = malloc((cur_lenTrG + cur_lenNew) * sizeof(Bitmap));
        int cur_lenFinal = 0;

        int cur_inNew = 0;
        int cur_inTrg = 0;
        int cur_inFinal = 0;
        while(cur_inNew < cur_lenNew && cur_inTrg < cur_lenTrG){
            if(newTrs[cur_inNew]->count < TrGuaranteed[cur_inTrg]->count){
                final[cur_inFinal] = newTrs[cur_inNew];
                cur_inNew++;
            }
            else{
                //printf("YOUHOU\n");
                //bitmap_print(TrGuaranteed[cur_inTrg]);
                //printf("FIN YOUHOU\n");
                
                final[cur_inFinal] = TrGuaranteed[cur_inTrg];
                cur_inTrg++;
            }
            cur_inFinal++;
        }

        //Il reste des éléments dans TrG
        if(cur_inTrg < cur_lenTrG){
            while(cur_inTrg < cur_lenTrG){
                //printf("YOUHOU\n");
                //bitmap_print(TrGuaranteed[cur_inTrg]);
                //printf("FIN YOUHOU\n");
                final[cur_inFinal] = TrGuaranteed[cur_inTrg];
                cur_inTrg++;
                cur_inFinal++;
            }
        }
        else if(cur_inNew < cur_lenNew){
            while(cur_inNew < cur_lenNew){
                final[cur_inFinal] = newTrs[cur_inNew];
                cur_inNew++;
                cur_inFinal++;
            }
        }
        cur_lenFinal = cur_inFinal;
        
        //Là faudrait free TrT, TrG ayant été réutilisé
        for(int j = 0; j < cur_lenTrT; j++){
            bitmap_free(TrT[j]);
        }
        free(TrT);

        TrCur = final;
        len_TrCur = cur_lenFinal;

    }

    //printf("CP6\n");

    //https://docs.python.org/3/c-api/list.html#c.PyList_New
    //On met à 0 parce que je comprends pas trop le commentaire et je voulais utiliser append de toute façon
    PyObject* res = PyList_New(0);

    for(int i = 0; i < len_TrCur; i++){
        PyObject* curTr = PyList_New(0);
        for(int cur_pos = bitmap_first_bit(TrCur[i]); cur_pos != -1; cur_pos = bitmap_next_bit(TrCur[i], cur_pos+1)){
            //https://docs.python.org/3/c-api/list.html#c.PyList_Append
            //https://docs.python.org/3/c-api/long.html#c.PyLong_FromLong
            //J'ai pas capté le commentaire pour FromLong, mais bon ça devrait marcher 
            PyList_Append(curTr, PyLong_FromLong(cur_pos+1));
        }

        PyList_Append(res, curTr);
    }
    
    return res;
}

static PyMethodDef MyMethods[] = {
    {"dong_li", dong_li, METH_VARARGS, "Applique DL"},
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef module = {
    PyModuleDef_HEAD_INIT, "transversal_enumeration", NULL, -1, MyMethods
};

PyMODINIT_FUNC PyInit_transversal_enumeration(void) {
    return PyModule_Create(&module);
}