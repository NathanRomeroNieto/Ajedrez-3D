#include "pieces.h"

GLuint texture[32];
float angle = 0.0f;
GLuint p,f,v,tile;
int grid_row, grid_col;
char grid_column;
//int highlighted_tiles[28][2] = {0};
int highlighted_tiles[28][2] = {0};
GAMESTATE gamestate;

void initDLs(void){
    printf("%i\n",A);
    tile = glGenLists(1);
    glNewList(tile,GL_COMPILE);
    glBegin(GL_QUADS);
        glVertex3f(1,0,1);
        glVertex3f(-1,0,1);
        glVertex3f(-1,0,-1);
        glVertex3f(1,0,-1);
    glEnd();
    glEndList();
}

void drawGrid(void){
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glColor4f(1,1,1,0);
    int counter = 0;
    for(int i = 0; i <= 7; i++){
        for(int j = 0; j <= 7; j++){
            
            /*if(i+1 == grid_col && j+1 == grid_row){
                glColor4f(1,0,0,0.3);
                glTranslatef(-7+(i*2),-0.97,-(-7+(j*2)));
                glCallList(tile);
                glColor4f(1,1,1,0.3);
            }else{*/
                /*bool success = false;
                for(int k = 0; k < 28; k++){
                    if(highlighted_tiles[k] != 0){
                        if(highlighted_tiles[k][0] == j+1 && highlighted_tiles[k][1] == i+1){
                            counter++;
                            glLoadName(100+counter);
                            glPushMatrix();
                            //printf("row = %i col = %i\n",highlighted_tiles[k][0],highlighted_tiles[k][1]);
                            glColor4f(1,0.1,0.1,0);
                            glTranslatef(-7+(i*2),-0.97,-(-7+(j*2)));
                            glCallList(tile);
                            glColor4f(1,0,0,1);
                            glBegin(GL_LINES);
                                glVertex3f(1,0,1);
                                glVertex3f(0.4,0,1);
                                glVertex3f(1,0,-0.9);
                                glVertex3f(0.4,0,-0.9);
                                glVertex3f(-1,0,1);
                                glVertex3f(-0.4,0,1);
                                glVertex3f(-1,0,-0.9);
                                glVertex3f(-0.4,0,-0.9);
                                glVertex3f(-0.9,0,1);
                                glVertex3f(-0.9,0,0.4);
                                glVertex3f(-0.9,0,-1);
                                glVertex3f(-0.9,0,-0.4);
                                glVertex3f(0.9,0,1);
                                glVertex3f(0.9,0,0.4);
                                glVertex3f(0.9,0,-1);
                                glVertex3f(0.9,0,-0.4);
                            glEnd();
                            success = true;
                            glPopMatrix();
                            break;
                        }
                    }
                }*/
                //if(!success){
                    counter++;
                    glLoadName(100+counter);
                    glPushMatrix();
                    glColor4f(1,1,1,0);
                    glTranslatef(-7+(i*2),-0.97,(-7+(j*2)));
                    glCallList(tile);
                    glPopMatrix();
                //}
            //}
            //glPopMatrix();
        }
    }
    for(int k = 0; k < 28; k++){
        if(highlighted_tiles[k] != 0){
            int row = highlighted_tiles[k][0]-1;
            int col = highlighted_tiles[k][1]-1;
            //if(highlighted_tiles[k][0] == j+1 && highlighted_tiles[k][1] == i+1){
            if(col >= 0 && col >= 0 && col <= 7 && col <= 7){
                //counter++;
                //glLoadName(100+counter);
                glPushMatrix();
                //printf("row = %i col = %i\n",highlighted_tiles[k][0],highlighted_tiles[k][1]);
                glColor4f(1,0,0,0.5);
                glTranslatef(-7+(col*2),-0.96,-(-7+(row*2)));
                glCallList(tile);
                glColor4f(1,0,0,1);
                glBegin(GL_LINES);
                glVertex3f(1,0,1);
                glVertex3f(0.4,0,1);
                glVertex3f(1,0,-0.9);
                glVertex3f(0.4,0,-0.9);
                glVertex3f(-1,0,1);
                glVertex3f(-0.4,0,1);
                glVertex3f(-1,0,-0.9);
                glVertex3f(-0.4,0,-0.9);
                glVertex3f(-0.9,0,1);
                glVertex3f(-0.9,0,0.4);
                glVertex3f(-0.9,0,-1);
                glVertex3f(-0.9,0,-0.4);
                glVertex3f(0.9,0,1);
                glVertex3f(0.9,0,0.4);
                glVertex3f(0.9,0,-1);
                glVertex3f(0.9,0,-0.4);
                glEnd();
                //success = true;
                glPopMatrix();
                //break;
            }
        }
    }
    glEnable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);
}

void setShaders(void){
    char *vs = NULL,*fs = NULL;

    v = glCreateShader(GL_VERTEX_SHADER);
    f = glCreateShader(GL_FRAGMENT_SHADER);

    vs = textFileRead("shaders/swag.vert");
    fs = textFileRead("shaders/swag.frag");

    const char *vv = vs;
    const char *ff = fs;

    glShaderSource(v,1,&vv,NULL);
    glShaderSource(f,1,&ff,NULL);

    free(vs);free(fs);

    glCompileShader(v);
    glCompileShader(f);

    p = glCreateProgram();
    
    glAttachShader(p,f);
    glAttachShader(p,v);

    glLinkProgram(p);
    glUseProgram(p);
}

bool loadOBJ(const char* filePath,float *&vertexArray,float *&normalArray,float *&uvArray,int &numVerts){
    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(filePath, aiProcessPreset_TargetRealtime_Fast);
    if(!scene){
        printf("failed to load\n");
        return false;
    }
    aiMesh *mesh = scene->mMeshes[0];
    numVerts = mesh->mNumFaces*3;
    vertexArray = new float[mesh->mNumFaces*3*3];
    normalArray = new float[mesh->mNumFaces*3*3];
    uvArray = new float[mesh->mNumFaces*3*2];

    for(unsigned int i = 0; i<mesh->mNumFaces;i++){
        const aiFace& face = mesh->mFaces[i];
        for(int j = 0; j < 3; j++){
            aiVector3D uv = mesh->mTextureCoords[0][face.mIndices[j]];
            memcpy(uvArray,&uv,sizeof(float)*2);
            uvArray+=2;

            aiVector3D pos = mesh->mVertices[face.mIndices[j]];
            memcpy(vertexArray,&pos,sizeof(float)*3);
            vertexArray+=3;

            aiVector3D normal = mesh->mNormals[face.mIndices[j]];
            memcpy(normalArray, &normal, sizeof(float)*3);
            normalArray+=3;
        }
    }

    uvArray-=mesh->mNumFaces*3*2;
    normalArray-=mesh->mNumFaces*3*3;
    vertexArray-=mesh->mNumFaces*3*3;
}

bool loadGLTexture(const char* filePath, unsigned int textureNum){
    texture[textureNum] = SOIL_load_OGL_texture(
    filePath,
    SOIL_LOAD_AUTO,
    SOIL_CREATE_NEW_ID,
    SOIL_FLAG_INVERT_Y
    );

    if(texture[textureNum] == 0){
        printf("Could not load texture\n");
        return false;
    }

    glBindTexture(GL_TEXTURE_2D, texture[textureNum]);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    return true;
}

Piece::Piece(const char* modelFile, const char* textureFile,int textureNum,char col,int row){
    angle = 0.0f;
    loadOBJ(modelFile,vertexArray,normalArray,uvArray,numVerts);
    textureNumber = textureNum;
    loadGLTexture(textureFile,textureNumber);
    c_Row = row;
    for(unsigned int i = 0; i <= sizeof(column)/sizeof(char); i++){
        if(tolower(column[i]) == tolower(col)){
            c_Col = i+1; //letter to int
        }
    }
    c_Column = col;
    if(row == 1 || row == 2){
        color = WHITE;
    }else if(row == 7 || row == 8){
        color = BLACK;
    }else{
        color = -1;
    }
}

void Piece::draw(){
    glPushMatrix();
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
    if(picked){
        //y = 2.0f;
    }
    switch(c_Column){
        case 'a':
            x = -8.8f;
            break;
        case 'b':
            x = -6.3f;
            break;
        case 'c':
            x = -3.8f;
            break;
        case 'd':
            x = -1.3f;
            break;
        case 'e':
            x = 1.2f;
            break;
        case 'f':
            x = 3.7f;
            break;
        case 'g':
            x = 6.2f;
            break;
        case 'h':
            x = 8.7f;
            break;
    };
    switch(c_Row){
        case 1:
            z = 8.8f;
            break;
        case 2:
            z = 6.3f;
            break;
        case 3:
            z = 3.8f;
            break;
        case 4:
            z = 1.3f;
            break;
        case 5:
            z = -1.2f;
            break;
        case 6:
            z = -3.7f;
            break;
        case 7:
            z = -6.2f;
            break;
        case 8:
            z = -8.7f;
            break;
    };
    if(c_Column != 'z'){ //normal pieces
        glColor3f(1,1,1);
        glScalef(0.8,0.8,0.8);
        glTranslatef(x,-1.25+y,z);
    }else if(c_Column == 'z'){ //board
        glColor3f(0.5,0.5,1);
        glTranslatef(0,-2,0);
    }
    glBindTexture(GL_TEXTURE_2D,texture[textureNumber]);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glVertexPointer(3,GL_FLOAT,0,vertexArray);
    glNormalPointer(GL_FLOAT,0,normalArray);
            
    glClientActiveTexture(GL_TEXTURE0);
    glTexCoordPointer(2,GL_FLOAT,0,uvArray);

    glDrawArrays(GL_TRIANGLES,0,numVerts);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);

    glPopMatrix();
    angle+=1.0f;
}

void Knight::draw(){
    glPushMatrix();
    //glRotatef(angle,0,1,0);
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
    if(picked){
        //y = 2.0f;
    }
    switch(c_Column){
        case 'a':
            x = -8.8f;
            break;
        case 'b':
            x = -6.3f;
            break;
        case 'c':
            x = -3.8f;
            break;
        case 'd':
            x = -1.3f;
            break;
        case 'e':
            x = 1.2f;
            break;
        case 'f':
            x = 3.7f;
            break;
        case 'g':
            x = 6.2f;
            break;
        case 'h':
            x = 8.7f;
            break;
    };
    switch(c_Row){
        case 1:
            z = 8.8f;
            break;
        case 2:
            z = 6.3f;
            break;
        case 3:
            z = 3.8f;
            break;
        case 4:
            z = 1.3f;
            break;
        case 5:
            z = -1.2f;
            break;
        case 6:
            z = -3.7f;
            break;
        case 7:
            z = -6.2f;
            break;
        case 8:
            z = -8.7f;
            break;
    };
    glColor3f(1,1,1);
    glScalef(0.8,0.8,0.8);
    glTranslatef(-7.3+x,-2.25+y,-0.8+z);
    glBindTexture(GL_TEXTURE_2D,texture[textureNumber]);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glVertexPointer(3,GL_FLOAT,0,vertexArray);
    glNormalPointer(GL_FLOAT,0,normalArray);
            
    //glClientActiveTexture(GL_TEXTURE0);
    glTexCoordPointer(2,GL_FLOAT,0,uvArray);

    glDrawArrays(GL_TRIANGLES,0,numVerts);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);

    glPopMatrix();
    angle+=1.0f;
}

void highlight_tile(int col, int row,unsigned int tile){
    if(col <= 8 && row <= 8 && col > 0 && row > 0){
        highlighted_tiles[tile][0] = row;
        highlighted_tiles[tile][1] = col;
    }
}

void Piece::pick(void){
    picked = true;
    listMoves();
    grid_row = c_Row;
    grid_col = c_Col;
    grid_column = c_Column;
    //printf("selected row = %i col = %i\n",grid_row,grid_col,grid_column);
}

void Piece::unpick(void){
    picked = false;
    //memset(highlighted_tiles,0,sizeof(highlighted_tiles[0][0])*28*2); //clear the array
    //grid_row = 0;
    //grid_col = 0;
}

void Piece::listMoves(void){
    memset(highlighted_tiles,0,sizeof(highlighted_tiles[0][0])*28*2); //clear the array
}

void Pawn::listMoves(void){
    memset(highlighted_tiles,0,sizeof(highlighted_tiles[0][0])*28*2); //clear the array
    if(color == BLACK){
            highlight_tile(c_Col,c_Row-1,0);
            highlight_tile(c_Col,c_Row-2,1);
    }else if(color == WHITE){
            highlight_tile(c_Col,c_Row+1,0);
            highlight_tile(c_Col,c_Row+2,1);
    }
}

void Knight::listMoves(void){
    memset(highlighted_tiles,0,sizeof(highlighted_tiles[0][0])*28*2); //clear the array
    highlight_tile(c_Col-1,c_Row+2,0);
    highlight_tile(c_Col+1,c_Row+2,1);
    highlight_tile(c_Col-1,c_Row-2,2);
    highlight_tile(c_Col+1,c_Row-2,3);
    highlight_tile(c_Col-2,c_Row+1,4);
    highlight_tile(c_Col-2,c_Row-1,5);
    highlight_tile(c_Col+2,c_Row+1,6);
    highlight_tile(c_Col+2,c_Row-1,7);
}

void Bishop::listMoves(void){
    memset(highlighted_tiles,0,sizeof(highlighted_tiles[0][0])*28*2); //clear the array
    highlight_tile(c_Col-1,c_Row+1,0);
    highlight_tile(c_Col-1,c_Row-1,1);
    highlight_tile(c_Col+1,c_Row+1,2);
    highlight_tile(c_Col+1,c_Row-1,3);
    highlight_tile(c_Col-2,c_Row+2,4);
    highlight_tile(c_Col-2,c_Row-2,5);
    highlight_tile(c_Col+2,c_Row+2,6);
    highlight_tile(c_Col+2,c_Row-2,7);
    highlight_tile(c_Col-3,c_Row+3,8);
    highlight_tile(c_Col-3,c_Row-3,9);
    highlight_tile(c_Col+3,c_Row+3,10);
    highlight_tile(c_Col+3,c_Row-3,11);
    highlight_tile(c_Col-4,c_Row+4,12);
    highlight_tile(c_Col-4,c_Row-4,13);
    highlight_tile(c_Col+4,c_Row+4,14);
    highlight_tile(c_Col+4,c_Row-4,15);
    highlight_tile(c_Col-5,c_Row+5,16);
    highlight_tile(c_Col-5,c_Row-5,17);
    highlight_tile(c_Col+5,c_Row+5,18);
    highlight_tile(c_Col+5,c_Row-5,19);
    highlight_tile(c_Col-6,c_Row+6,20);
    highlight_tile(c_Col-6,c_Row-6,21);
    highlight_tile(c_Col+6,c_Row+6,22);
    highlight_tile(c_Col+6,c_Row-6,23);
    highlight_tile(c_Col-7,c_Row+7,24);
    highlight_tile(c_Col-7,c_Row-7,25);
    highlight_tile(c_Col+7,c_Row+7,26);
    highlight_tile(c_Col+7,c_Row-7,27);
    
}

void Piece::move(unsigned int col, unsigned int row){
    c_Col = col;
    c_Row = row;
    c_Column = column[col-1];
}

void Piece::move(char col, unsigned int row){
    for(unsigned int i = 0; i <= sizeof(column)/sizeof(char); i++){
        if(tolower(column[i]) == tolower(col)){
            c_Col = i+1;
        }
    }
    c_Row = row;
    c_Column = col;
}
