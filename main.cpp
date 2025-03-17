#include <iostream>
#include <cstdlib>
#include <ctime>

#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"

using namespace std;

// 定义巨人结构体
struct Giant {
    string type;
    int health;
    int weakness; // 弱点编号，对应不同的道具
};

// 生成随机巨人
Giant generateRandomGiant() {
    int randomType = rand() % 3;
    Giant giant;
    switch (randomType) {
        case 0:
            giant.type = "普通巨人";
            giant.health = 1;
            giant.weakness = 0;
            break;
        case 1:
            giant.type = "强壮巨人";
            giant.health = 2;
            giant.weakness = 1;
            break;
        case 2:
            giant.type = "敏捷巨人";
            giant.health = 1;
            giant.weakness = 2;
            break;
    }
    return giant;
}

// 道具结构体
struct Item {
    string name;
    int effect; // 道具效果，如增加子弹、提高命中率等
};

// 玩家结构体
struct Player {
    int level;
    int bullets;
    int hitRate; // 命中率
    Item items[3]; // 最多携带 3 种道具
    int itemCount;
};

// 初始化玩家
void initPlayer(Player& player) {
    player.level = 0;
    player.bullets = 5;
    player.hitRate = 2; // 初始命中率为 2/5
    player.itemCount = 0;
}

// 显示道具信息
void showItems(const Player& player) {
    if (player.itemCount == 0) {
        cout << "你没有任何道具。" << endl;
    } else {
        cout << "你拥有的道具：" << endl;
        for (int i = 0; i < player.itemCount; i++) {
            cout << i + 1 << ". " << player.items[i].name << endl;
        }
    }
}

// 使用道具
void useItem(Player& player, Giant& giant) {
    if (player.itemCount == 0) {
        cout << "你没有任何道具，无法使用。" << endl;
        return;
    }
    showItems(player);
    cout << "请选择要使用的道具编号（输入 0 取消）：";
    int choice;
    cin >> choice;
    if (choice >= 1 && choice <= player.itemCount) {
        Item& item = player.items[choice - 1];
        if (item.effect == giant.weakness) {
            cout << GREEN << "你使用了 " << item.name << "，对 " << giant.type << " 造成了额外伤害！" << RESET << endl;
            giant.health = 0;
        } else if (item.effect == 3) {
            cout << GREEN << "你使用了 " << item.name << "，增加了 3 发子弹！" << RESET << endl;
            player.bullets += 3;
        } else if (item.effect == 4) {
            cout << GREEN << "你使用了 " << item.name << "，提高了命中率！" << RESET << endl;
            player.hitRate = 3; // 提高命中率到 3/5
        } else if (item.effect == 5) {
            cout << GREEN << "你使用了 " << item.name << "，对 " << giant.type << " 造成了致命伤害！" << RESET << endl;
            giant.health = 0;
        }
        // 移除使用过的道具
        for (int i = choice - 1; i < player.itemCount - 1; i++) {
            player.items[i] = player.items[i + 1];
        }
        player.itemCount--;
    }
}

// 升级奖励
void levelUpReward(Player& player) {
    cout << GREEN << "你升级了！你可以选择一个奖励：" << RESET << endl;
    cout << "1. 增加 3 发子弹" << endl;
    cout << "2. 提高命中率" << endl;
    cout << "3. 获得一个随机道具" << endl;
    int choice;
    cin >> choice;
    switch (choice) {
        case 1:
            player.bullets += 3;
            cout << GREEN << "你获得了 3 发子弹！" << RESET << endl;
            break;
        case 2:
            // 增加命中率并检查是否超过上限
            if (player.hitRate < 4) {
                player.hitRate++;
            }
            cout << GREEN << "你的命中率提高了！当前命中率为：" << player.hitRate << "/5" << RESET << endl;
            break;
        case 3:
            if (player.itemCount < 3) {
                int randomItem = rand() % 4; // 新增一种道具，范围改为 0 - 3
                Item item;
                switch (randomItem) {
                    case 0:
                        item.name = "普通药剂";
                        item.effect = 0;
                        break;
                    case 1:
                        item.name = "力量药剂";
                        item.effect = 1;
                        break;
                    case 2:
                        item.name = "速度药剂";
                        item.effect = 2;
                        break;
                    case 3:
                        item.name = "万能药剂";
                        item.effect = 5; // 万能药剂效果编号设为 5
                        break;
                }
                player.items[player.itemCount++] = item;
                cout << GREEN << "你获得了 " << item.name << "！" << RESET << endl;
            } else {
                cout << RED << "你的道具栏已满，无法获得新道具。" << RESET << endl;
            }
            break;
    }
}

// 掉落奖励
void dropReward(Player& player) {
    int randomReward = rand() % 3;
    switch (randomReward) {
        case 0: {
            Item item;
            int randomItem = rand() % 4;
            switch (randomItem) {
                case 0:
                    item.name = "普通药剂";
                    item.effect = 0;
                    break;
                case 1:
                    item.name = "力量药剂";
                    item.effect = 1;
                    break;
                case 2:
                    item.name = "速度药剂";
                    item.effect = 2;
                    break;
                case 3:
                    item.name = "万能药剂";
                    item.effect = 5;
                    break;
            }
            if (player.itemCount < 3) {
                player.items[player.itemCount++] = item;
                cout << GREEN << "巨人掉落了 " << item.name << "！" << RESET << endl;
            } else {
                cout << RED << "你的道具栏已满，巨人掉落的 " << item.name << " 未拾取。" << RESET << endl;
            }
            break;
        }
        case 1:
            player.bullets += 2;
            cout << GREEN << "巨人掉落了 2 发子弹！" << RESET << endl;
            break;
        case 2:
            if (player.hitRate < 4) {
                player.hitRate++;
                cout << GREEN << "巨人掉落了命中提升，你的命中率提高了！当前命中率为：" << player.hitRate << "/5" << RESET << endl;
            } else {
                cout << RED << "你的命中率已达到上限，巨人掉落的命中提升未生效。" << RESET << endl;
            }
            break;
    }
}

int main() {
    srand(static_cast<unsigned int>(time(nullptr)));
    Player player;
    initPlayer(player);

    cout << YELLOW << "================ 巨人消灭游戏 ================" << RESET << endl;

    while (true) {
        cout << BLUE << "当前等级为：" << player.level << RESET << endl;
        cout << BLUE << "当前子弹数为：" << player.bullets << RESET << endl;
        cout << BLUE << "当前命中率为：" << player.hitRate << "/5" << RESET << endl;
        showItems(player);

        int giantsBeaten = 0;
        for (int i = 0; i < 3; i++) {
            Giant giant = generateRandomGiant();
            cout << RED << "有一个 " << giant.type << "，你要去消灭他" << RESET << endl;
            cout << YELLOW << "你要做什么？" << RESET << endl;
            cout << YELLOW << "1. 开枪" << RESET << endl;
            cout << YELLOW << "2. 使用道具" << RESET << endl;
            cout << YELLOW << "3. 退出" << RESET << endl;

            int choice;
            cin >> choice;

            if (choice == 1) {
                if (player.bullets == 0) {
                    cout << RED << "没有子弹了，游戏结束" << RESET << endl;
                    return 0;
                }
                player.bullets--;
                if (rand() % 5 + 1 > player.hitRate) {
                    cout << RED << "你开枪了，但是没有击中" << RESET << endl;
                } else {
                    giant.health--;
                    if (giant.health == 0) {
                        cout << GREEN << "你开枪了，" << giant.type << " 被消灭了" << RESET << endl;
                        giantsBeaten++;
                        dropReward(player);
                    } else {
                        cout << RED << "你开枪了，但是 " << giant.type << " 还活着！" << RESET << endl;
                    }
                }
            } else if (choice == 2) {
                useItem(player, giant);
                if (giant.health == 0) {
                    cout << GREEN << giant.type << " 被消灭了" << RESET << endl;
                    giantsBeaten++;
                    dropReward(player);
                }
            } else if (choice == 3) {
                cout << YELLOW << "你退出了游戏" << RESET << endl;
                return 0;
            }
        }

        if (giantsBeaten < 2) {
            cout << RED << "你在这一级别至少需要打败 2 个巨人才能升级，重新挑战这一级别。" << RESET << endl;
        } else {
            player.level++;
            levelUpReward(player);
            if (player.level == 10) {
                cout << GREEN << "你赢了" << RESET << endl;
                break;
            }
        }
    }

    cout << YELLOW << "================ 游戏结束 ================" << RESET << endl;

    return 0;
}