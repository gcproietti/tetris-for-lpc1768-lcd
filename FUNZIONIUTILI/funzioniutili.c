/**DEFINIZIONE DEI TETRAMINI SU MATRICI */
int tetramino_I[4][4] = {
    {1, 1, 1, 1},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0}
};  
int tetramino_O[4][4] = {
    {0, 1, 1, 0},
    {0, 1, 1, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0}
};
int tetramino_T[4][4] = {
    {1, 1, 1, 0},
    {0, 1, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0}
};
int tetramino_S[4][4] = {
    {0, 1, 1, 0},
    {1, 1, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0}
};
int tetramino_Z[4][4] = {
    {1, 1, 0, 0},
    {0, 1, 1, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0}
};
int tetramino_J[4][4] = {
    {0, 1, 0, 0},
    {0, 1, 0, 0},
    {1, 1, 0, 0},
    {0, 0, 0, 0}
};
int tetramino_L[4][4] = {
    {1, 0, 0, 0},
    {1, 0, 0, 0},
    {1, 1, 0, 0},
    {0, 0, 0, 0}
};

int** ruota_90_gradi(int** matrice, int righe, int colonne){
    int ruotata[4][4];
    for(int i = 0; i < colonne; i++){
        for(int j = 0; j < righe; j++){
            ruotata[i][j] = matrice[righe - 1 - j][i];
        }
    }
    return ruotata;
}

// funzione che crea la matrice di gioco
int** crea_matrice(int righe, int colonne){
    int matrice[10][20];
    for(int i = 0; i < 10; i++){
        for(int j = 0; j < 20; j++){
            matrice[i][j] = 0; // inizializza la matrice con zeri
        }
    }
    return matrice;
}

typedef struct {
    int x;
    int y;
} Coord;

Coord coordinate_su_schermo(Coord coord_matrice){
    Coord coord_schermo;
    coord_schermo.x = coord_matrice.x * 14; // supponendo che ogni cella sia 14 pixel di larghezza
    coord_schermo.y = coord_matrice.y * 14; // supponendo che ogni cella sia 14 pixel di altezza
    return coord_schermo;
}

