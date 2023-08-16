
#include <iostream>
#include <memory>
#include <random>
#include <string>
using namespace std;
random_device rd;
mt19937 gen(rd());
// Genertae random int between range
class TollKit {
public:
  int random(int min, int max) {
    uniform_int_distribution<> dist(min, max);
    return dist(gen);
  }
  // activate based on probabilty percentage
  bool activate(int percentage) {
    int ran = random(1, 100);
    if (ran <= percentage)
      return true;
    return false;
  }
};

enum CharacterState {
  Idle = 1,
  Attacking,
  Attacked,
  Healing,
  Invincible,
  Critical,
  Defence,
  Stunned
};
enum ChacterType { PlayerType = 1, EnemyType, BossEnemyType };
class Character {
private:
  int maxHealth;
  int curHealth;
  int attackStats; // based on level
  int defenceStats;
  int attackPow; // Based on weapon
  int level;
  string charName;
  CharacterState state = Idle;
  ChacterType type;
  int magicStats;

public:
  Character(string name, int hp, int as, int ds, int ap, ChacterType tp,
            int ms = 0, int lev = 1) {
    maxHealth = hp;
    curHealth = maxHealth;
    attackStats = as;
    defenceStats = ds;
    charName = name;
    level = lev;
    attackPow = ap;
    type = tp;
    magicStats = ms;
  }
  //--Seters--
  void SetCurHealth(int hp) { curHealth = hp; }
  void SetMaxHealth(int hp) { maxHealth = hp; }
  void SetAttack(int value) { attackStats = value; }
  void SetDefence(int value) { defenceStats = value; }
  void SetLevel(int value) { level = value; }
  void SetAttackPower(int value) { attackPow = value; }
  void SetState(CharacterState value) { state = value; }
  void ResetActivity() { state = Idle; }
  void SetMagic(int value) { magicStats = value; }
  //--Geters--

  int GetLevel() { return level; }
  int GetMaxHealth() { return maxHealth; }
  int GetHealth() { return curHealth; }
  int GetAttackStat() { return attackStats; }
  int GetDefence() { return defenceStats; }
  int GetAttackPower() { return attackPow; }
  int GetMagic() { return magicStats; }
  CharacterState GetState() { return state; }

  string GetName() { return charName; }
  bool IsAlive() {
    if (curHealth <= 0)
      return false;
    return true;
  }
  ChacterType GetType() { return type; }

  virtual void ShowStats() {
    cout << "\n\nCurrent Stats  of " << charName << " are:\nMax Health --> "
         << maxHealth << "\nAttack -->" << attackStats << "\nDefence --> "
         << defenceStats << "\n";
  }
  // Special Ability
  virtual void SpecialAbility(Character *enemyCharacter = NULL) = 0;

  // Deal Damage to enemy Character
  void DealDamage(Character *enemyCharacter, int attackPower) {

    this->SetState(Attacking);
    int damageAmt = (attackPower + attackStats) - enemyCharacter->GetDefence();
    if (damageAmt <= 0)
      damageAmt = 1;
    cout << "\n\n"
         << this->GetName() << " tried dealing -> " << damageAmt << " to "
         << enemyCharacter->GetName() << "\n";

    if (enemyCharacter->GetState() == Invincible) {
      cout << endl
           << enemyCharacter->GetName()
           << " blocked the attack, no damage done.";
      ResetActivity();
      enemyCharacter->ResetActivity();
      return;
    }
    if (GetType() != EnemyType)
      this->SpecialAbility(enemyCharacter);

    cout << "\nAttack was successfull\n";

    if (GetState() == Critical) {
      cout << "\nIt was a Critical Hit 2x Damage\n";
      damageAmt *= 2;
    }
    if (enemyCharacter->GetState() == Defence) {
      cout << "\n"
           << enemyCharacter->GetName()
           << " defened , damage reduced by half\n\n\n";
      damageAmt /= 2;
    }
    if (enemyCharacter->GetState() != Defence &&
        enemyCharacter->GetType() != EnemyType &&
        enemyCharacter->GetState() != Stunned) {
      enemyCharacter->SetState(Attacked);
      enemyCharacter->SpecialAbility(this);
    }

    int newHealth = enemyCharacter->GetHealth() - damageAmt;
    if (newHealth <= 0) {
      enemyCharacter->SetCurHealth(0);
      cout << enemyCharacter->GetName() << " has Died!! \n\n";
      return;
    }
    enemyCharacter->SetCurHealth(newHealth);
    if (GetState() == Attacking)
      ResetActivity();
    if (enemyCharacter->GetState() == Attacked)
      enemyCharacter->ResetActivity();
  }
  virtual void Heal(int healAmt = 0) {
    cout << GetName() << " Cant use heal\n\n";
  }
  // virtual destructor
  virtual ~Character() {}
};
class Player : public Character {
private:
  int critActivationPercentage = 10;
  int blockActivationPercentage = 10;
  int lifeStealActivationPercentage = 100;
  TollKit tool;

public:
  Player(string name, int hp, int as, int ds, int ms, int ap = 10)
      : Character(name, hp, as, ds, ap, PlayerType, ms) {}
  void ShowStats() {
    Character::ShowStats();
    cout << "Magic --> " << GetMagic() << "\n";
  }

  // Healing
  void Heal(int healAmt = 0) {
    if (GetHealth() == GetMaxHealth()) {
      cout << "Already full HP\n";
      return;
    }
    SetState(Healing);
    cout << "\nhealing\n";
    if (!healAmt)
      healAmt = (GetMaxHealth() / GetMagic()) + (GetMagic() / 2);
    int newHealth = GetHealth() + healAmt;
    cout << "\n" << GetName() << " is healed by " << healAmt << " points\n\n";
    if (newHealth > GetMaxHealth()) {
      newHealth = GetMaxHealth();
    }
    SetCurHealth(newHealth);

    ResetActivity();
  }
  // Special Ability
  void SpecialAbility(Character *enemyCharacter = NULL) {
    if (GetLevel() < 3)
      return;
    // Critical hits (performs attack with massive damage boost) Probability -
    // 10%
    if (GetState() == Attacking) {
      if (tool.activate(critActivationPercentage)) {
        SetState(Critical);
        return;
      }
    }
    if (GetLevel() < 4)
      return;
    // Blocker (will get 0 damage on enemy hit) Probability - 10 %
    if (GetState() == Attacked) {
      if (tool.activate(blockActivationPercentage)) {
        cout << "\n"
             << GetName() << " Special Ability Blocker tool.activated\n";
        SetState(Invincible);
        return;
      }
    }
    if (GetLevel() < 5)
      return;
    // Life steal (Recovering a small amount of HP after giving damage)
    // Probability - 10 %
    if (GetState() == Attacking) {
      if (tool.activate(lifeStealActivationPercentage)) {
        cout << "\n" << GetName() << " Life Steal Ability tool.activated\n";
        int healAmt = (int)(((GetAttackStat() + GetMagic()) -
                             enemyCharacter->GetDefence()) /
                            2);
        Heal(healAmt);
        return;
      }
    }
  }
};

class Enemy : public Character {
public:
  Enemy(string name, int hp, int as, int ds, int ap = 10)
      : Character(name, hp, as, ds, ap, EnemyType) {
    cout << "\n" << name << " has spwawned\n";
  }
  // Will waste a round and in next round will defend to take half damage and
  // might counterattack or defend again
  void SpecialAbility(Character *enemyCharacter = NULL) {
    cout << "\n\n" << GetName() << " will take half damage in next round\n";
    SetState(Defence);
  }
};

class BossEnemy : public Character {
private:
  int healActivationPercentage = 20;
  int stunActivationPercentage = 10;
  float healthpercentage = 0.2;
  TollKit tool;
public:
  BossEnemy(string name, int hp, int as, int ds, int ms, int ap = 25)
      : Character(name, hp, as, ds, ap, BossEnemyType, ms) {
    cout << "\nMurlocs is here, he callenges you to duel.\n\n";
  }
  void Heal(int healAmt = 0) {
    if (GetHealth() == GetMaxHealth()) {
      cout << "\nHealth full so nothing happend\n";
      return;
    }
    healAmt = (int)((0.2 * GetMaxHealth()));
    int newHealth = healAmt + GetHealth();
    if (newHealth > GetMaxHealth())
      newHealth = GetMaxHealth();
    SetCurHealth(newHealth);
  }
  // Defence, After succefull attack 20% chance to recover 25% of max hp, After
  // attack 10 % chance of stuning player for next round
  void SpecialAbility(Character *player = NULL) {
    if (GetState() == Idle)
      SetState(Defence);
    if (GetState() == Attacking) {
      if (tool.activate(healActivationPercentage)) {
        cout << "\n"
             << GetName()
             << " Special Ability tool.activated, health regentrated by 25%\n";
        Heal();
      }
    }
    if (GetState() == Attacked) {
      if (tool.activate(stunActivationPercentage)) {
        cout << "\nBoss special ability stun tool.activated\n";
        player->SetState(Stunned);
      }
    }
  }
};

int bossLevel = 6;

class Level {
private:
  bool gameOver = false;
  int mobEnemyhealth = 100;
  int mobEnemyAttackStat = 15;
  int mobEnemyDefenceStat = 5;
  int bossEnemyhealth = 200;
  int bossEnemyAttackStat = 35;
  int bossEnemyDefenceStat = 30;
  int bossMagicStat = 20;
  int level;
  Character *player;
  int statIncrease = 2;
  int heealthIncrease = 50;
  TollKit tool;
public:
  Level(Character *players) {
    player = players;
    level = player->GetLevel();
  }
  void LevelUp() {
    level += 1;
    player->SetLevel(level);
    player->SetAttack(player->GetAttackStat() + statIncrease);
    player->SetDefence(player->GetDefence() + statIncrease);
    player->SetMagic(player->GetMagic() + statIncrease);
    player->SetMaxHealth(player->GetMaxHealth() + heealthIncrease);
    player->SetCurHealth(player->GetMaxHealth());
    cout << endl
         << player->GetName() << " has leveled up !!\nHealth restored to full!";
    int swordAttackPower = 20;
    // int shieldDefeceneIncrease = 5;
    // int armourDefenceIncrease = 2;
    int armourMagicIncrease = 5;
    int bowAttackPower = 15;
    switch (level) {
    case 2:
      cout << "\n"
           << player->GetName()
           << " got a map, in that it was writen after sending 5 waves of "
              "minion Murlocs will apear\n\n";
      break;
    case 3:
      cout << "\n"
           << player->GetName()
           << " got sword\n Mele damage attack power increase to "
           << swordAttackPower
           << "\nSpecial Ability Awarded - Critical hits (performs attack "
              "with massive damage boost)\n";
      player->SetAttackPower(swordAttackPower);
      break;
    case 4:
      cout << "\n"
           << player->GetName() << " got a shield\n "
           << "\nSpecial Ability Awarded - Blocker (will get 0 damage on "
              "enemy hit)";
      // SetDefence(GetDefence() + shieldDefeceneIncrease);
      break;
    case 5:
      cout << "\n"
           << player->GetName() << " got a armour\n"
           << "\nMagic rose by " << armourMagicIncrease
           << "\nSpecial Ability Awarded - Blocker (will get 0 damage on "
              "enemy hit)";
      // SetDefence(GetDefence() + armourDefenceIncrease);
      player->SetMagic(player->GetMagic() + armourMagicIncrease);
      break;
    case 6:
      cout << "\n"
           << player->GetName()
           << " got a Bow:\nCan do ranged damage and next round will "
              "get "
              "half damage\n";
      break;
    }
  }
  void SpawnEnemy(int num, Character *enemyies[num]) {
    for (int i = 0; i < num; i++) {
      enemyies[i] = new Enemy("Enemy " + to_string(i + 1), mobEnemyhealth,
                              mobEnemyAttackStat, mobEnemyDefenceStat);
    }
  }

  void SpawnBossEnemy(Character *bossEnemy[1]) {
    string name = "Murlocs";
    bossEnemy[0] = new BossEnemy(name, bossEnemyhealth, bossEnemyAttackStat,
                                 bossEnemyDefenceStat, bossMagicStat);
  }

  void DeleteCharacter(int num, Character *enemyies[num]) {
    for (int i = 0; i < num; i++) {
      delete enemyies[i];
    }
  }

  bool IsAnyEnemyAlive(int num, Character *enemyies[num]) {
    int DeadEnemy = 0;
    for (int i = 0; i < num; i++) {
      if (!(enemyies[i]->IsAlive()))
        DeadEnemy++;
    }
    if (DeadEnemy == num)
      return false;
    return true;
  }

  int ChooseEnemy(int num, Character *enemyies[num]) {
    if (num == 1)
      return 0;
    cout << "\nChoose Enemy:\n";
    int userInput;
    for (int i = 0; i < num; i++)
      if (enemyies[i]->IsAlive())
        cout << "\nEnemy " + to_string(i + 1) << "\n";
    int flag = 1;
    cout << "\nEnter you selection:\n";
    while (flag) {
      while (!(cin >> userInput) || userInput < 1 || userInput > num) {
        cout << "Choose a Enemy between 1 and " << num << " which is alive\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
      }
      if (enemyies[userInput - 1]->IsAlive())
        flag = 0;
      else {
        cout << "Enemy is dead, choose another:\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
      }
    }

    return userInput - 1;
  }

  void PlayerChooseAttack(Character *player, Character *enemy) {

    int swordAttackPower = 20;
    int bowAttackPower = 15;
    int userInput = 0;
    cout << "Choose Attack: "
         << "\nPlayer choose what to do (1 -> Melle, 2 -> Ranged\n";
    while (!(cin >> userInput) || userInput < 1 || userInput > 2) {
      cout << "Choose between 1 and 2:\n";
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    switch (userInput) {
    case 1:
      player->SetAttackPower(swordAttackPower);
      cout << "\nPlayer used sword to attack\n";
      break;
    case 2:
      player->SetAttackPower(bowAttackPower);
      cout << "\nPlayer used bow to attack\nPlayer will Defend in this round";
      break;
    }
    player->DealDamage(enemy, player->GetAttackPower());
    if (player->GetAttackPower() == bowAttackPower)
      player->SetState(Defence);
  }

  void PlayerTurn(Character *player, int num, Character *enemyies[num]) {
    if (player->GetState() == Stunned) {
      cout << "Player is stuned can't move\n";
      player->ResetActivity();
      return;
    }
    int userInput = 0;
    cout << "Player Current Health: " << player->GetHealth()
         << "\nPlayer choose what to do (1 -> Attack, 2 -> Heal, 3 -> Show "
            "Current Stats)\n";
    while (!(cin >> userInput) || userInput < 1 || userInput > 3) {
      cout << "Choose a number between 1 and 3\n";
      cin.clear();
      cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    int enemyToAttack;
    switch (userInput) {
    case 1:
      enemyToAttack = ChooseEnemy(num, enemyies);
      if (num > 1)
        cout << "You Choose " << enemyies[enemyToAttack]->GetName() << "\n\n";
      if (player->GetLevel() < bossLevel)
        player->DealDamage(enemyies[enemyToAttack], player->GetAttackPower());
      else
        PlayerChooseAttack(player, enemyies[enemyToAttack]);
      break;

    case 2:
      player->Heal();
      break;
    case 3:
      player->ShowStats();
      PlayerTurn(player, num, enemyies);
      break;
    }
  }

  void EnemyTurn(Character *player, int num, Character *enemyies[num]) {
    int attackChancePercentage = 75;

    for (int i = 0; i < num; i++) {
      if (enemyies[i]->IsAlive()) {
        if (tool.activate(attackChancePercentage)) {
          enemyies[i]->DealDamage(player, enemyies[i]->GetAttackPower());
        } else {
          enemyies[i]->SpecialAbility();
        }
      }
    }
  }

  void Combat(Character *player, int num, Character *enemyies[num]) {

    while (player->IsAlive() && IsAnyEnemyAlive(num, enemyies)) {
      cout << endl;
      for (int i = 0; i < num; i++)
        cout << enemyies[i]->GetName()
             << " health is : " << enemyies[i]->GetHealth() << "\n";
      PlayerTurn(player, num, enemyies);

      EnemyTurn(player, num, enemyies);
    }
  }

  void LevelPlayOut() {
    int enyNum;
    if (level != bossLevel) {
      enyNum = level;
      Character *enemyies[enyNum];
      SpawnEnemy(enyNum, enemyies);
      Combat(player, enyNum, enemyies);
      DeleteCharacter(enyNum, enemyies);
      if (player->IsAlive())
        LevelUp();
      return;
    }
    enyNum = 1;
    Character *bossEnemy[enyNum];
    SpawnBossEnemy(bossEnemy);
    Combat(player, enyNum, bossEnemy);
    DeleteCharacter(enyNum, bossEnemy);
    if (player->IsAlive())
      cout << "The village has been saved!!";
  }
};

class GamePlay {
private:
  bool gameOver = false;
  int playerHp = 100;
  int playerAttackStat = 15;
  int playerDeffenceStat = 10;
  int playerMagicStat = 10;

public:
  GamePlay() {
    cout << "Sir, Hero"
         << " while you were out fighting Demon king army, they found out "
            "about you village and sent Murlocs along with his minions to "
            "destory it. You have to hurry and save your village from getting "
            "destroyed.\n";
  }

  void Play() {
    string name;

    cout << "\nHero, before you go can I know your name :\n";
    cin >> name;
    cout
        << "\n\nRules: \n1)First Player will make turn after that "
           "enemy.\n2)Player can either attack, heal or see his "
           "stats\n3)After each wave player will find a item that will help "
           "him.\n4)Enemyies will either defend or attack\n5)Player need to "
           "defeat the Boss enemy to win the game.\n5)After each wave of enemy "
           "player will Level up which will increase its stats and restore "
           "health.\n6)Boss Enemy has special ability beware of that \n\n";

    Character *player = new Player(name, playerHp, playerAttackStat,
                                   playerDeffenceStat, playerMagicStat);
    unique_ptr<Level> level(new Level(player));

    cout << "You have reached the village and minions have started appearing\n";

    for (int lev = 1; lev < bossLevel && player->IsAlive(); lev++) {
      level->LevelPlayOut();
    }
    if (!player->IsAlive()) {
      cout << "You lost!! \n\n";
      delete player;

      return;
    }

    delete player;
  }
};

int main() {
  unique_ptr<GamePlay> game(new GamePlay());

  game->Play();
}