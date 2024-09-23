const int playerSize = 40;
const int groundHeight = 80;
const int enemySize = 30;

class Player {
public:
    Rectangle rect;
    Vector2 velocity;
    bool isJumping;

    Player(float x, float y) {
        rect = { x, y, playerSize, playerSize };
        velocity = { 0, 0 };
        isJumping = false;
    }

    void Update(float levelWidth) {
        // Update
        if (IsKeyDown(KEY_D)) {
            velocity.x = 5;
        } else if (IsKeyDown(KEY_A)) {
            velocity.x = -5;
        } else {
            velocity.x = 0;
        }

        if (IsKeyPressed(KEY_SPACE) && !isJumping) {
            velocity.y = -10;
            isJumping = true;
        }

        // Apply gravity
        velocity.y += 0.5f;

        // Move player horizontally
        rect.x += velocity.x;

        // Move player vertically
        rect.y += velocity.y;

        // Check collision with ground
        if (rect.y + rect.height >= GetScreenHeight() - groundHeight) {
            rect.y = GetScreenHeight() - groundHeight - rect.height;
            velocity.y = 0;
            isJumping = false;
        }

        // Check collision with screen boundaries
        if (rect.x < 0) {
            rect.x = 0;
        }
        if (rect.x + rect.width > levelWidth) {
            rect.x = levelWidth - rect.width;
        }
    }

    static std::vector<Player> InitializePlayers(float x, float y) {
        std::vector<Player> players;
        for (int i = 0; i < 1; i++) {
            players.push_back(Player(x, y));
        }
        return players;
    }

    void Draw() {
        DrawRectangleRec(rect, RED);
    }
};

class Tile {
public:
    Rectangle rect;
    Color color;

    Tile(float x, float y, float width, float height, Color tileColor) {
        rect = { x, y, width, height };
        color = tileColor;
    }

    virtual void Update() {

    }

    virtual void Draw() {
        DrawRectangleRec(rect, color);
    }
};

class GroundTile : public Tile {
public:
    GroundTile(float x, float y, float width, float height, Color tileColor)
        : Tile(x, y, width, height, tileColor) {}


    void Draw() override {
        DrawRectangleRec(rect, color);
    }
};


class Enemy {
    static const int enemyCount = 8;
public:
    Rectangle rect;
    Vector2 velocity;
    bool isActive;


    Enemy(float x, float y, float vx) {
        rect = { x, y, enemySize, enemySize };
        velocity = { vx, 0 };
        isActive = true;
    }

    static std::vector<Enemy> InitializeEnemies(float levelWidth, float levelHeight) {
        std::vector<Enemy> enemies;
        for (int i = 0; i < enemyCount; i++) {
            enemies.push_back(Enemy(GetRandomValue(0, levelWidth - enemySize), GetScreenHeight() - groundHeight - enemySize, GetRandomValue(-2, 2)));
        }
        return enemies;
    }

    void Update(float levelWidth) {
        // Move enemy horizontally
        rect.x += velocity.x;

        // Check collision with screen boundaries
        if (rect.x < 0 || rect.x + rect.width > levelWidth) {
            velocity.x *= -1;
        }
    }

    void Draw() {
        DrawRectangleRec(rect, BLUE);
    }
};

class Flagpole {
public:
    Rectangle rect;

    Flagpole(float x, float y, float height) {
        rect = { x, y, 10, height };
    }
    
    Flagpole() {
        rect = { 0, 0, 10, 0 };
    }

    void Draw() {
        DrawRectangleRec(rect, GREEN);
    }
};

class Level {
public:
    std::vector<Tile> tiles;
    std::vector<Enemy> enemies;
	std::vector<Player> players;
    Flagpole flagpole;
    Color backgroundColor;
    float scrollOffset;
	float levelWidth;
	float levelHeight;

    Level(float levelWidth, float levelHeight): levelWidth(levelWidth), levelHeight(levelHeight) {
        // Initialize background color
        backgroundColor = SKYBLUE;

        // Initialize tiles
        tiles.push_back(GroundTile(0, GetScreenHeight() - groundHeight, levelWidth, groundHeight, BROWN));

        // Initialize enemies
        enemies = Enemy::InitializeEnemies(levelWidth, levelHeight);

        // Initialize flagpole
        flagpole = Flagpole(levelWidth - 20, GetScreenHeight() - groundHeight - 200, 200);

		// Initialize players
		players = Player::InitializePlayers(GetScreenWidth() / 2 - playerSize / 2, GetScreenHeight() - groundHeight - playerSize);

        // Initialize scroll offset
        scrollOffset = 0;
    }

    void Update() {
        // Update tiles
        for (auto& tile : tiles) {
            tile.Update();
        }

        // Update enemies
        for (auto& enemy : enemies) {
            enemy.Update(levelWidth);
        }

		// Update players
		for (auto& player : players) {
			player.Update(levelWidth);
		}

		// Update scroll offset
		if (players[0].rect.x > scrollOffset + GetScreenWidth() * 0.6f) {
			scrollOffset = players[0].rect.x - GetScreenWidth() * 0.6f;
		}
        if (players[0].rect.x < scrollOffset + GetScreenWidth() * 0.4f) {
            scrollOffset = players[0].rect.x - GetScreenWidth() * 0.4f;
        }

		//Clamp scroll offset
		if (scrollOffset < 0) scrollOffset = 0;
		if (scrollOffset > levelWidth - GetScreenWidth()) scrollOffset = levelWidth - GetScreenWidth();



    }

    void Draw() {
        ClearBackground(backgroundColor);

        // Draw tiles
        for (auto& tile : tiles) {
            DrawRectangleRec({ tile.rect.x - scrollOffset, tile.rect.y, tile.rect.width, tile.rect.height }, tile.color);
        }

        // Draw enemies
        for (auto& enemy : enemies) {
            DrawRectangleRec({ enemy.rect.x - scrollOffset, enemy.rect.y, enemy.rect.width, enemy.rect.height }, BLUE);
        }

        // Draw flagpole
        DrawRectangleRec({ flagpole.rect.x - scrollOffset, flagpole.rect.y, flagpole.rect.width, flagpole.rect.height }, GREEN);

		// Draw player
        for (auto& player : players) {
            DrawRectangleRec({ player.rect.x - scrollOffset, player.rect.y, player.rect.width, player.rect.height }, RED);
        }
    }
};