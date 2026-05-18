#include <raylib.h>
#include <iostream>
#include <vector>
#include <string> 
#include <algorithm>


struct Piece
{
    int type = 0;
    int rotation = 0;
    int x = 3;
    int y = 0;
};

struct SpawnData
{
    int x;
    int y;
    int rotation;
};

struct GameData
{
	const int tileCountX = 10;
	const int tileCountY = 20;
	const int paddingX = 1;
	const int paddingY = 1;
	const int tileSize = 50;
	Texture2D tileSet[4];
    Piece cPiece;
	Vector2 position = { 3,0 };
    const int tetrominoes[7][4][4][4] =
    {
        // I
        {
            {
                {0,0,0,0},
                {1,1,1,1},
                {0,0,0,0},
                {0,0,0,0}
            },
            {
                {0,0,1,0},
                {0,0,1,0},
                {0,0,1,0},
                {0,0,1,0}
            },
            {
                {0,0,0,0},
                {1,1,1,1},
                {0,0,0,0},
                {0,0,0,0}
            },
            {
                {0,1,0,0},
                {0,1,0,0},
                {0,1,0,0},
                {0,1,0,0}
            }
        },

        // O
        {
            {
                {0,1,1,0},
                {0,1,1,0},
                {0,0,0,0},
                {0,0,0,0}
            },
            {
                {0,1,1,0},
                {0,1,1,0},
                {0,0,0,0},
                {0,0,0,0}
            },
            {
                {0,1,1,0},
                {0,1,1,0},
                {0,0,0,0},
                {0,0,0,0}
            },
            {
                {0,1,1,0},
                {0,1,1,0},
                {0,0,0,0},
                {0,0,0,0}
            }
        },

        // T
        {
            {
                {0,1,0,0},
                {1,1,1,0},
                {0,0,0,0},
                {0,0,0,0}
            },
            {
                {0,1,0,0},
                {0,1,1,0},
                {0,1,0,0},
                {0,0,0,0}
            },
            {
                {0,0,0,0},
                {1,1,1,0},
                {0,1,0,0},
                {0,0,0,0}
            },
            {
                {0,1,0,0},
                {1,1,0,0},
                {0,1,0,0},
                {0,0,0,0}
            }
        },

        // S
        {
            {
                {0,1,1,0},
                {1,1,0,0},
                {0,0,0,0},
                {0,0,0,0}
            },
            {
                {0,1,0,0},
                {0,1,1,0},
                {0,0,1,0},
                {0,0,0,0}
            },
            {
                {0,1,1,0},
                {1,1,0,0},
                {0,0,0,0},
                {0,0,0,0}
            },
            {
                {0,1,0,0},
                {0,1,1,0},
                {0,0,1,0},
                {0,0,0,0}
            }
        },

        // Z
        {
            {
                {1,1,0,0},
                {0,1,1,0},
                {0,0,0,0},
                {0,0,0,0}
            },
            {
                {0,0,1,0},
                {0,1,1,0},
                {0,1,0,0},
                {0,0,0,0}
            },
            {
                {1,1,0,0},
                {0,1,1,0},
                {0,0,0,0},
                {0,0,0,0}
            },
            {
                {0,0,1,0},
                {0,1,1,0},
                {0,1,0,0},
                {0,0,0,0}
            }
        },

        // J
        {
            {
                {1,0,0,0},
                {1,1,1,0},
                {0,0,0,0},
                {0,0,0,0}
            },
            {
                {0,1,1,0},
                {0,1,0,0},
                {0,1,0,0},
                {0,0,0,0}
            },
            {
                {0,0,0,0},
                {1,1,1,0},
                {0,0,1,0},
                {0,0,0,0}
            },
            {
                {0,1,0,0},
                {0,1,0,0},
                {1,1,0,0},
                {0,0,0,0}
            }
        },

        // L
        {
            {
                {0,0,1,0},
                {1,1,1,0},
                {0,0,0,0},
                {0,0,0,0}
            },
            {
                {0,1,0,0},
                {0,1,0,0},
                {0,1,1,0},
                {0,0,0,0}
            },
            {
                {0,0,0,0},
                {1,1,1,0},
                {1,0,0,0},
                {0,0,0,0}
            },
            {
                {1,1,0,0},
                {0,1,0,0},
                {0,1,0,0},
                {0,0,0,0}
            }
        }
    };
    const float gravityTable[20] =
    {
        0.800f, // 0
        0.717f, // 1
        0.633f, // 2
        0.550f, // 3
        0.467f, // 4
        0.383f, // 5
        0.300f, // 6
        0.217f, // 7
        0.133f, // 8
        0.100f, // 9
        0.083f, // 10
        0.083f, // 11
        0.083f, // 12
        0.067f, // 13 
        0.067f, // 14
        0.067f, // 15 
        0.050f, // 16
        0.050f, // 17 
        0.033f, // 18 
        0.017f  // 19
    };
    const SpawnData spawnData[7] =
    {
        // I
        {3, -1, 0},

        // O
        {4, 0, 0},

        // T
        {4, -1, 2},

        // S
        {4, 0, 0},

        // Z
        {4, 0, 0},

        // J
        {4, -1, 2},

        // L
        {4, -1, 2}
    };

    int lines = 0;

    float softDropSpeed = 0.06f;
	float timer = 0;
	unsigned char board[200];
    float delay = 0.0f;
    std::vector<int> linesToClear;
    float DASChargeR = 0;
    float DASChargeL = 0;

    float DASR = 0;
    float DASL = 0;

    float ARRR = 0;
    float ARRL = 0;

    int score = 0;

    int nextPiece;

    bool gameOver = false;

    int chosenLevel = 0;

    int level = 0;
	GameData() {
        linesToClear.reserve(4);
		for (int i = 0; i < 200; i++) {
			board[i] = 255;
		}
	}
    
};
GameData gameData;
int windowWidth;
int windowHeight;
struct MenuData {
    bool levels[20];
    int buttonSize = 80;
    int buttonFont = 40;
    Vector2 getButtonPos(int idx){
        
        Vector2 result;
        result.x = buttonSize * (idx % 10) + (50);
        result.y = buttonSize * (int)(idx / 10) + 100 + buttonSize;
        return result;
    
    } 

    bool ButtonHovered(int idx) {
        Vector2 pos = getButtonPos(idx);
        Vector2 mousePos = GetMousePosition();

        if (mousePos.x > pos.x && mousePos.x < pos.x + buttonSize &&
            mousePos.y > pos.y && mousePos.y < pos.y + buttonSize) {
            return true;
        }
        return false;
    }
    MenuData() {
        for (int i = 0; i < 20; i++) {
            levels[i] = false;
        }
    }
};
MenuData menuData;
void spawnTetroid() {
    gameData.cPiece.type = gameData.nextPiece;
    gameData.nextPiece = GetRandomValue(0, 6);
    
    gameData.cPiece.rotation = gameData.spawnData[gameData.cPiece.type].rotation;
    gameData.cPiece.x = gameData.spawnData[gameData.cPiece.type].x;
    gameData.cPiece.y = gameData.spawnData[gameData.cPiece.type].y;
	
}

void drawTile(int posX, int posY, int type)
{
	if (posX >= gameData.tileCountX || posY >= gameData.tileCountY) { return; }
    int color = 3;
    if (type == 0 || type == 1 || type == 2) {  color = 0; }
    else if (type == 3 || type == 5) {  color = 1; }
    else {  color = 2; }
	DrawTexture(gameData.tileSet[color], gameData.tileSize * (posX + 1), gameData.tileSize * (posY), WHITE);
}

void drawNextPiece() {
    int type = gameData.nextPiece;
    for (int row = 0; row < 4; row++)
    {
        for (int col = 0; col < 4; col++)
        {
            if (gameData.tetrominoes[type][gameData.spawnData[type].rotation][row][col])
            {
                
                int color = 3;
                if (type == 0 || type == 1 || type == 2) { color = 0; }
                else if (type == 3 || type == 5) { color = 1; }
                else { color = 2; }

                DrawTexture(gameData.tileSet[color], 
                    (gameData.tileCountX + gameData.paddingX * 2) * gameData.tileSize + 30 + gameData.tileSize * (col),
                    260 + gameData.tileSize * (row),
                    WHITE);

                //drawTile(gameData.cPiece.x + col, gameData.cPiece.y + row, gameData.cPiece.type);
                

            }
        }
    }
}

bool canDrop() {
    for (int row = 0; row < 4; row++)
    {
        for (int col = 0; col < 4; col++)
        {
            if (gameData.tetrominoes[gameData.cPiece.type][gameData.cPiece.rotation][row][col])
            {
                int x = (int)(gameData.cPiece.x + col);
                int y = (int)(gameData.cPiece.y + row + 1);
                // check vertical bounds
                if (y >= gameData.tileCountY) {
                    return false;
                }
                int idx = y * gameData.tileCountX + x;
                if (gameData.board[idx] != 255) {
                    return false;
                }
            }
        }
    }
	return true;
}
void move(bool direction) {

    int shift = 1;
    if (!direction) { shift *= -1; }
    for (int row = 0; row < 4; row++)
    {
        for (int col = 0; col < 4; col++)
        {
            if (gameData.tetrominoes[gameData.cPiece.type][gameData.cPiece.rotation][row][col])
            {
                int x = (int)(gameData.cPiece.x + col + shift );
                int y = (int)(gameData.cPiece.y + row);
                if (x < 0 || x >= gameData.tileCountX) {
                    return;
                }
                if (y >= 0) {
                    if (gameData.board[y * gameData.tileCountX + x] != 255) {
                        return;
                    }
                }
                    
            }
        }
    }
    gameData.cPiece.x += shift;

}


void rotate(bool direction) {

    if (direction) {
        gameData.cPiece.rotation = (gameData.cPiece.rotation + 1) % 4;
    }
    else {
        gameData.cPiece.rotation = (gameData.cPiece.rotation + 3) % 4;
    }
    bool reject = false;
    for (int row = 0; row < 4; row++)
    {
        for (int col = 0; col < 4; col++)
        {
            if (gameData.tetrominoes[gameData.cPiece.type][gameData.cPiece.rotation][row][col])
            {
                int x = (int)(gameData.cPiece.x + col);
                int y = (int)(gameData.cPiece.y + row);
                if (x < 0 || x >= gameData.tileCountX || y >= gameData.tileCountY) { reject = true; }
                if (y >= 0) {
                    if (gameData.board[y * gameData.tileCountX + x] != 255) {
                        reject = true;
                    }
                }
            }
        }
    }

    if (reject) {
        if (direction) {
            gameData.cPiece.rotation = (gameData.cPiece.rotation + 3) % 4;
        }
        else {
            gameData.cPiece.rotation = (gameData.cPiece.rotation + 1) % 4;
        }
    }
}

void reset() {
    gameData.nextPiece = GetRandomValue(0, 6);
    spawnTetroid();
    for (int i = 0; i < 200; i++) {
        gameData.board[i] = 255;
    }
    gameData.DASChargeR = 0;
    gameData.DASChargeL = 0;

    gameData.DASR = 0;
    gameData.DASL = 0;

    gameData.ARRR = 0;
    gameData.ARRL = 0;

    gameData.linesToClear.clear();

    gameData.lines = 0;
    gameData.score = 0;

    spawnTetroid();

    gameData.gameOver = false;
}
char Scene = 'm';



int main(void) {

    windowWidth = (gameData.tileCountX + gameData.paddingX * 2 + 6) * gameData.tileSize;
    windowHeight = (gameData.tileCountY + gameData.paddingY) * gameData.tileSize;

    InitWindow(windowWidth,windowHeight , "Tetris");
    
    
    SetExitKey(KEY_NULL);
    reset();

    // loads the textures into the tileSet array -> put this into a constructor 
    for (int i = 0; i < 4; i++) {
        char temp = 48 + (i + 1);
        std::string path = std::string(RESOURCES_PATH) + "tile" + temp + ".png";
        Image tile = LoadImage(path.c_str());
        gameData.tileSet[i] = LoadTextureFromImage(tile);
        UnloadImage(tile);
        gameData.tileSet[i].width *= gameData.tileSize / 10;
        gameData.tileSet[i].height *= gameData.tileSize / 10;
    }

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground({ 10,10,10,255 });

        switch (Scene) {
        case 'm':
            if (IsKeyPressed(KEY_ESCAPE)) {
                Scene = 'g';
            }

            DrawText("Tetris",
                windowWidth / 2 - 124,
                40,
                80,
                WHITE);

            for (int i = 0; i < 20; i++) {
                Vector2 temp = menuData.getButtonPos(i);
                
                if (menuData.ButtonHovered(i)) {
                    DrawRectangle(temp.x,
                        temp.y,
                        menuData.buttonSize,
                        menuData.buttonSize,
                        YELLOW);
                    menuData.levels[i] = true;

                    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                        gameData.chosenLevel = i;
                        Scene = 'g';
                        reset();
                    }
                }
                else {
                    menuData.levels[i] = false;
                }
                DrawRectangleLinesEx(
                    {
                        temp.x,
                        temp.y,
                        (float)menuData.buttonSize,
                        (float)menuData.buttonSize
                    },
                    4,
                    DARKGREEN
                );

                DrawText(TextFormat("%01i", i),
                    temp.x + (menuData.buttonSize - menuData.buttonFont) / 2,
                    temp.y + (menuData.buttonSize - menuData.buttonFont) / 2,
                    menuData.buttonFont, 
                    RED);
            }
            break;
        case 'g': // this is the game 
            // draws the stats

            if (IsKeyPressed(KEY_ESCAPE)) {
                Scene = 'm';
            }

            DrawText(TextFormat("Level: %01i", gameData.level),
                (gameData.tileCountX + gameData.paddingX * 2) * gameData.tileSize + 30,
                80, 40, WHITE);
            DrawText(TextFormat("Lines: %01i", gameData.lines),
                (gameData.tileCountX + gameData.paddingX * 2) * gameData.tileSize + 30,
                120, 40, WHITE);
            DrawText(TextFormat("Score: %01i", gameData.score),
                (gameData.tileCountX + gameData.paddingX * 2) * gameData.tileSize + 30,
                160, 40, WHITE);
            DrawText("Next:",
                (gameData.tileCountX + gameData.paddingX * 2) * gameData.tileSize + 30,
                200, 40, WHITE);

            drawNextPiece();

            //draw Border sides 
            for (int i = 0; i < gameData.tileCountY + gameData.paddingY; i++) {
                DrawTexture(gameData.tileSet[3], 0, gameData.tileSize * i, WHITE);
                DrawTexture(gameData.tileSet[3], gameData.tileSize * (gameData.tileCountX - 1 + gameData.paddingX * 2), gameData.tileSize * i, WHITE);
            }
            //draw border bottom 
            for (int i = 0; i < gameData.tileCountX + gameData.paddingX * 2; i++) {
                DrawTexture(gameData.tileSet[3], gameData.tileSize * i, gameData.tileSize * (gameData.tileCountY - 1 + gameData.paddingY), WHITE);
            }

            //draw board state

            for (int x = 0; x < gameData.tileCountX; x++)
            {
                for (int y = 0; y < gameData.tileCountY; y++)
                {
                    if (gameData.board[y * gameData.tileCountX + x] != 255)
                    {
                        drawTile(x, y, (int)gameData.board[y * gameData.tileCountX + x]);
                    }
                }
            }

            //draw Current Tetroid
            for (int row = 0; row < 4; row++)
            {
                for (int col = 0; col < 4; col++)
                {
                    if (gameData.tetrominoes[gameData.cPiece.type][gameData.cPiece.rotation][row][col])
                    {
                        if (gameData.cPiece.y + row >= 0) {
                            drawTile(gameData.cPiece.x + col, gameData.cPiece.y + row, gameData.cPiece.type);
                        }

                    }
                }
            }
            if (gameData.gameOver) {
                DrawText("Press 'R' To Reset",
                    ((gameData.tileCountX + gameData.paddingX * 2) * gameData.tileSize) / 2 - 200,
                    gameData.tileCountY * gameData.tileSize / 2 - 20, 40, WHITE);

                if (IsKeyPressed(KEY_R)) {
                    reset();
                }
            }
            else {



                if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D) || IsGamepadButtonDown(0, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT))
                {
                    gameData.DASChargeR += GetFrameTime();


                }
                else
                {
                    gameData.DASChargeR = 0;
                    gameData.ARRR = 0;
                }



                if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A) || IsGamepadButtonDown(0, GAMEPAD_BUTTON_RIGHT_FACE_LEFT))
                {
                    gameData.DASChargeL += GetFrameTime();


                }
                else
                {
                    gameData.DASChargeL = 0;
                    gameData.ARRL = 0;
                }



                if (gameData.delay != 0) {
                    gameData.delay -= GetFrameTime();



                    if (gameData.delay < 0)
                    {
                        gameData.delay = 0;

                        if (gameData.linesToClear.size() > 0)

                            for (int row : gameData.linesToClear)
                            {
                                memcpy(&gameData.board[gameData.tileCountX], &gameData.board[0], row * gameData.tileCountX);
                                for (int i = 0; i < gameData.tileCountX; i++) {
                                    gameData.board[i] = 255;
                                }
                            }

                        gameData.linesToClear.clear();
                    }
                }
                else {

                    if (gameData.DASChargeR >= 0.267f)
                    {
                        gameData.ARRR += GetFrameTime();

                        if (gameData.ARRR >= 0.1f)
                        {
                            move(true);
                            gameData.ARRR = 0;
                        }
                    }

                    if (gameData.DASChargeL >= 0.267f)
                    {
                        gameData.ARRL += GetFrameTime();

                        if (gameData.ARRL >= 0.1f)
                        {
                            move(false);
                            gameData.ARRL = 0;
                        }
                    }



                    if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D) || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT))
                    {
                        move(true);

                        gameData.DASChargeR = 0;
                        gameData.ARRR = 0;

                        gameData.DASChargeL = 0;
                    }
                    if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A) || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_LEFT))
                    {
                        move(false);

                        gameData.DASChargeR = 0;
                        gameData.ARRR = 0;

                        gameData.DASChargeL = 0;
                    }


                    if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W) || IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_UP)) { rotate(true); }
                    else if (IsKeyPressed(KEY_S)) { rotate(false); }


                    gameData.timer += GetFrameTime();
                    int firstTransition = std::min(gameData.chosenLevel * 10 + 10, 
                        std::max(100, gameData.chosenLevel * 10 - 50));
                    
                    if (gameData.lines < firstTransition) {
                        gameData.level = gameData.chosenLevel;
                    }
                    else {
                        gameData.level = gameData.chosenLevel + 1 + ((gameData.lines - firstTransition) / 10);
                    }
                     
                    float gravity = gameData.gravityTable[std::min(gameData.level,18)];
                    if (IsKeyDown(KEY_LEFT_SHIFT) || IsKeyDown(KEY_SPACE) || IsKeyDown(KEY_DOWN) || IsGamepadButtonDown(0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN)) { gravity = std::min(gameData.softDropSpeed, gravity); }

                    if (gameData.timer > gravity)
                    {
                        gameData.timer = 0;

                        if (canDrop())
                        {
                            gameData.cPiece.y += 1;
                        }
                        else
                        {
                            for (int row = 0; row < 4; row++)
                            {
                                for (int col = 0; col < 4; col++)
                                {
                                    if (gameData.tetrominoes[gameData.cPiece.type][gameData.cPiece.rotation][row][col])
                                    {
                                        int x = (int)(gameData.cPiece.x + col);
                                        int y = (int)(gameData.cPiece.y + row);
                                        if (y >= 0) {
                                            gameData.board[y * gameData.tileCountX + x] = (int)(gameData.cPiece.type);
                                        }

                                    }
                                }
                            }

                            for (int y = 0; y < gameData.tileCountY; y++) {
                                bool lineFull = true;
                                for (int x = 0; x < gameData.tileCountX; x++) {
                                    if (gameData.board[y * gameData.tileCountX + x] == 255) { lineFull = false; }
                                }
                                if (lineFull) { gameData.linesToClear.push_back(y); }

                            }
                            for (int row : gameData.linesToClear) {
                                for (int x = 0; x < gameData.tileCountX; x++) {
                                    gameData.board[row * gameData.tileCountX + x] = 255;
                                }
                            }
                            gameData.delay = 0.2;
                            if (gameData.linesToClear.size() > 0) {
                                gameData.delay += 0.283;
                                gameData.lines += gameData.linesToClear.size();
                                int basePoint = gameData.linesToClear.size() == 1 ? 40 :
                                    gameData.linesToClear.size() == 2 ? 100 :
                                    gameData.linesToClear.size() == 3 ? 300 : 1200;

                                gameData.score += basePoint * (gameData.lines / 10 + 1);
                            }


                            spawnTetroid();

                            for (int row = 0; row < 4; row++)
                            {
                                for (int col = 0; col < 4; col++)
                                {
                                    if (gameData.tetrominoes[gameData.cPiece.type][gameData.cPiece.rotation][row][col])
                                    {
                                        int x = gameData.cPiece.x + col;
                                        int y = row + gameData.cPiece.y;
                                        if (gameData.board[y * gameData.tileCountX + x] != 255) {
                                            gameData.gameOver = true;
                                        }
                                    }
                                }
                            }
                        }


                    }
                }
            }
            break;
        }
        
        EndDrawing();
	}

	CloseWindow();
	for (auto& t : gameData.tileSet) {
		UnloadTexture(t);
	}
	return 0;
}