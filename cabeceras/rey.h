#include "globales.h"
#include "pieza.h"


//Clase Rey Herreda de la clase Pieza
class Rey : public Pieza{
    public:
	//Pasar Parametros del contructor de la clase Rey al constructor de la clase pieza
        Rey(const char* modelFile, const char* textureFile,int textureNum,char col, int row) : Pieza(modelFile, textureFile, textureNum,col,row){
            printf("Rey loaded\n");
            value = 9999;
        };
        ~Rey();

	//Metodos
        void listMoves(void);
        void move(unsigned int col, unsigned int row);
};

void Rey::listMoves(void){
    clearMovesList();
    highlight_tile_k(c_Col+1,c_Row,0);
    highlight_tile_k(c_Col-1,c_Row,1);
    highlight_tile_k(c_Col,c_Row+1,2);
    highlight_tile_k(c_Col,c_Row-1,3);
    highlight_tile_k(c_Col+1,c_Row+1,4);
    highlight_tile_k(c_Col+1,c_Row-1,5);
    highlight_tile_k(c_Col-1,c_Row+1,6);
    highlight_tile_k(c_Col-1,c_Row-1,7);
    if(!has_moved){
        if(!checkSquare(c_Col-3,c_Row) && !checkSquare(c_Col-2,c_Row) && !checkSquare(c_Col-1,c_Row)){
            highlight_tile(c_Col-2,c_Row,8);
        }
        if(!checkSquare(c_Col+2,c_Row) && !checkSquare(c_Col+1,c_Row)){
            highlight_tile(c_Col+2,c_Row,9);
        }
    }
}

void Rey::move(unsigned int col, unsigned int row){
    grid_pieces[c_Row-1][c_Col-1] = 0;
    if(c_Col-col == 2){
        if(color == WHITE){
            piece_at(1,1)->move((unsigned int)4,(unsigned int)1);
        }else if(color == BLACK){
            piece_at(1,8)->move((unsigned int)4,(unsigned int)8);
        }
    }else if(c_Col-col == -2){
        if(color == WHITE){
            piece_at(8,1)->move((unsigned int)6,(unsigned int)1);
        }else if(color == BLACK){
            piece_at(8,8)->move((unsigned int)6,(unsigned int)8);
        }
    }
    c_Col = col;
    c_Row = row;
    c_Column = column[col-1];
    grid_pieces[c_Row-1][c_Col-1] = color;
    has_moved = true;
}
