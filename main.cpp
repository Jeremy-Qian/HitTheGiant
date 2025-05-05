#include <iostream>
#include <cstdlib>
#include <ctime>

#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"

using namespace std;

// Define the Giant structure
struct Giant {
    string type;
    int health;
    int weakness; // Weakness number, corresponding to different items
};

// Generate a random giant
Giant generateRandomGiant() {
    int randomType = rand() % 3;
    Giant giant;
    switch (randomType) {
        case 0:
            giant.type = "Ordinary Giant";
            giant.health = 1;
            giant.weakness = 0;
            break;
        case 1:
            giant.type = "Strong Giant";
            giant.health = 2;
            giant.weakness = 1;
            break;
        case 2:
            giant.type = "Agile Giant";
            giant.health = 1;
            giant.weakness = 2;
            break;
    }
    return giant;
}

// Item structure
struct Item {
    string name;
    int effect; // Item effect, such as adding bullets, increasing hit rate, etc.
};

// Player structure
struct Player {
    int level;
    int bullets;
    int hitRate; // Hit rate
    Item items[3]; // Carry up to 3 types of items
    int itemCount;
};

// Initialize the player
void initPlayer(Player& player) {
    player.level = 0;
    player.bullets = 8;
    player.hitRate = 45; // Initial hit rate is 45%
    player.itemCount = 0;
}

void showValues(Player& player) {
    cout << "Current Level: " << player.level << endl;
    cout << "Current Bullets: " << player.bullets << endl;
    cout << "Current Hit Rate: " << player.hitRate << "%" << endl;
}

// Show item information
void showItems(const Player& player) {
    if (player.itemCount == 0) {
        cout << "You don't have any items." << endl;
    } else {
        cout << "Items you have: " << endl;
        int validItemIndex = 1;
        for (int i = 0; i < player.itemCount; i++) {
            if (!player.items[i].name.empty()) {
                cout << validItemIndex << ". " << player.items[i].name << endl;
                validItemIndex++;
            }
        }
    }
}

// Use an item
void useItem(Player& player, Giant& giant) {
    if (player.itemCount == 0) {
        cout << "You don't have any items, so you can't use them." << endl;
        return;
    }
    showItems(player);
    cout << "Please select the item number to use (enter 0 to cancel): ";
    int choice;
    cin >> choice;
    if (choice >= 1 && choice <= player.itemCount) {
        Item& item = player.items[choice - 1];
        if (item.effect == giant.weakness) {
            cout << GREEN << "You used " << item.name << " and dealt extra damage to " << giant.type << "!" << RESET << endl;
            giant.health = 0;
        } else if (item.effect == 3) {
            cout << GREEN << "You used " << item.name << " and gained 3 bullets!" << RESET << endl;
            player.bullets += 3;
        } else if (item.effect == 4) {
            cout << GREEN << "You used " << item.name << " and increased your hit rate!" << RESET << endl;
            player.hitRate = 3; // Increase hit rate to 3/5
        } else if (item.effect == 5) {
            cout << GREEN << "You used " << item.name << " and dealt fatal damage to " << giant.type << "!" << RESET << endl;
            giant.health = 0;
        }
        // Remove the used item
        for (int i = choice - 1; i < player.itemCount - 1; i++) {
            player.items[i] = player.items[i + 1];
        }
        player.itemCount--;
    }
}

// Level-up reward
void levelUpReward(Player& player) {
    cout << GREEN << "You've leveled up! You can choose a reward: " << RESET << endl;

    // Define four reward options
    int rewards[4] = {1, 2, 3, 4};
    // Randomly shuffle the reward options
    for (int i = 3; i > 0; --i) {
        int j = rand() % (i + 1);
        int temp = rewards[i];
        rewards[i] = rewards[j];
        rewards[j] = temp;
    }

    // Show the first three random reward options
    for (int i = 0; i < 3; ++i) {
        switch (rewards[i]) {
            case 1:
                cout << i + 1 << ". Add 10 bullets" << endl;
                break;
            case 2:
                cout << i + 1 << ". Increase hit rate" << endl;
                break;
            case 3:
                cout << i + 1 << ". Get a random item" << endl;
                break;
            case 4:
                cout << i + 1 << ". Add 1 item slot" << endl;
                break;
        }
    }

    int choice;
    cin >> choice;
    if (choice >= 1 && choice <= 3) {
        switch (rewards[choice - 1]) {
            case 1:
                player.bullets += 10;
                cout << GREEN << "You've gained 10 bullets!" << RESET << endl;
                break;
            case 2:
                // Increase hit rate and check if it exceeds the upper limit
                if (player.hitRate < 90) {
                    player.hitRate = player.hitRate + 5;
                }
                cout << GREEN << "Your hit rate has increased! Current hit rate: " << player.hitRate << "%" << RESET << endl;
                break;
            case 3:
                if (player.itemCount < 10) { // 假设最大物品槽数量为 10
                    int randomItem = rand() % 4; 
                    Item item;
                    switch (randomItem) {
                        case 0:
                            item.name = "Ordinary Potion";
                            item.effect = 0;
                            break;
                        case 1:
                            item.name = "Power Potion";
                            item.effect = 1;
                            break;
                        case 2:
                            item.name = "Speed Potion";
                            item.effect = 2;
                            break;
                        case 3:
                            item.name = "Universal Potion";
                            item.effect = 5; 
                            break;
                    }
                    player.items[player.itemCount++] = item;
                    cout << GREEN << "You've obtained " << item.name << "!" << RESET << endl;
                } else {
                    cout << RED << "Your item slots are full. You can't get a new item." << RESET << endl;
                }
                break;
            case 4:
                if (player.itemCount < 10) {
                    // 确保新增的物品槽被初始化为空
                    Item emptyItem = {"", 0};
                    player.items[player.itemCount] = emptyItem;
                    player.itemCount++;
                    cout << GREEN << "You've added 1 item slot!" << RESET << endl;
                } else {
                    cout << RED << "Your item slots have reached the maximum capacity. You can't add more." << RESET << endl;
                }
                break;
        }
    }
}

// Drop reward
void dropReward(Player& player) {
    int randomReward = rand() % 3;
    switch (randomReward) {
        case 0: {
            Item item;
            int randomItem = rand() % 4;
            switch (randomItem) {
                case 0:
                    item.name = "Ordinary Potion";
                    item.effect = 0;
                    break;
                case 1:
                    item.name = "Power Potion";
                    item.effect = 1;
                    break;
                case 2:
                    item.name = "Speed Potion";
                    item.effect = 2;
                    break;
                case 3:
                    item.name = "Universal Potion";
                    item.effect = 5;
                    break;
            }
            if (player.itemCount < 6) {
                player.items[player.itemCount++] = item;
                cout << GREEN << "The giant dropped " << item.name << "!" << RESET << endl;
            } else {
                cout << RED << "Your item slots are full. The " << item.name << " dropped by the giant was not picked up." << RESET << endl;
            }
            break;
        }
        case 1:
            player.bullets += 5;
            cout << GREEN << "The giant dropped 5 bullets!" << RESET << endl;

            break;
        case 2:
            if (player.hitRate < 90) {
                player.hitRate = player.hitRate + 5;
                cout << GREEN << "The giant dropped a hit rate boost. Your hit rate has increased! Current hit rate: " << player.hitRate << "%" << RESET << endl;
            } else {
                cout << RED << "Your hit rate has reached the upper limit. The hit rate boost dropped by the giant did not take effect." << RESET << endl;
            }
            break;
    }
}

int main() {
    srand(static_cast<unsigned int>(time(nullptr)));
    Player player;
    initPlayer(player);

    cout << YELLOW << "================ Giant Elimination Game ================" << RESET << endl;
    cout << "Please select a game mode: " << endl;
    cout << "1. Target Mode" << endl;
    cout << "2. Endless Mode" << endl;
    int modeChoice;
    cin >> modeChoice;
    int targetLevel = 0;
    if (modeChoice == 1) {
        cout << "Please set the target level: " << endl;
        cin >> targetLevel;
    }

    while (true) {
        if (modeChoice == 1 && player.level == targetLevel) {
            cout << GREEN << "You've reached the target level. You win!" << RESET << endl;
            showValues(player);
            break;
        }
        cout << BLUE << "Current Level: " << player.level << RESET << endl;
        cout << BLUE << "Current Bullets: " << player.bullets << RESET << endl;
        cout << BLUE << "Current Hit Rate: " << player.hitRate << "%" << RESET << endl;
        showItems(player);

        int giantsBeaten = 0;
        for (int i = 0; i < 3; i++) {
            Giant giant = generateRandomGiant();
            cout << RED << "There's an " << giant.type << ". You need to eliminate it." << RESET << endl;
            cout << YELLOW << "What will you do?" << RESET << endl;
            cout << YELLOW << "1. Shoot" << RESET << endl;
            cout << YELLOW << "2. Use an item" << RESET << endl;
            cout << YELLOW << "3. Quit" << RESET << endl;

            int choice;
            cin >> choice;

            if (choice == 1) {
                if (player.bullets == 0) {
                    cout << RED << "You're out of bullets. Game over." << RESET << endl;
                    return 0;
                }
                player.bullets--;
                if (rand() % 100 + 1 > player.hitRate) {
                    cout << RED << "You fired, but missed." << RESET << endl;
                } else {
                    giant.health--;
                    if (giant.health == 0) {
                        cout << GREEN << "You fired and eliminated the " << giant.type << "!" << RESET << endl;
                        giantsBeaten++;
                        dropReward(player);
                    } else {
                        cout << RED << "You fired, but the " << giant.type << " is still alive!" << RESET << endl;
                    }
                }
            } else if (choice == 2) {
                useItem(player, giant);
                if (giant.health == 0) {
                    cout << GREEN << "The " << giant.type << " has been eliminated." << RESET << endl;
                    giantsBeaten++;
                    dropReward(player);
                }
            } else if (choice == 3) {
                cout << YELLOW << "You've quit the game." << RESET << endl;
                showItems(player);
                return 0;
            }
        }

        if (giantsBeaten < 2) {
            cout << RED << "You need to defeat at least 2 giants at this level to level up. Try this level again." << RESET << endl;
        } else {
            player.level++;
            levelUpReward(player);
        }
    }

    cout << YELLOW << "================ Game Over ================" << RESET << endl;

    return 0;
}
