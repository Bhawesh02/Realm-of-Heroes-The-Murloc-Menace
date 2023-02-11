

// Rewards:
// Sword (Rewarded after completing level 2)
// Shield (Rewarded after completing level 3)
// Armour (Rewarded after completing level 4)
// Bow (Rewarded after completing level 5)

// After Every Level:
// Increase the Character's stats after each level.
// Increase Max HP and reset the maxHealth to max,

// Level Details

// Level 2

// There will be two enemies.
// For enemies, code randomly generated attacks and defences against our
// Character. Special Ability Awarded - Critical hits (performs attack with
// massive damage boost) Probability - 10% Special Item Awarded - Sword
// Level 3

// There will be three enemies.
// For enemies, code randomly generated attacks and defences against our
// Character. Special Ability Awarded - Blocker (will get 0 damage on enemy hit)
// Probability - 10 %
// Special Item Awarded - Shield
// Level 4

// There will be four enemies.
// For enemies, code randomly generated attacks and defences against our
// Character. Special Ability Awarded - Life steal (Recovering a small amount of
// HP after giving damage) Probability - 10 % Special Item Awarded - Armour
// Level 5

// There will be five enemies.
// For enemies, code randomly generated attacks and defences against our
// Character. Special Ability Awarded - Ranged Attack (After performing this
// attack, the Hero won't take damage for the next attack of the enemy)
// Probability - 10 % Special Item Awarded - Bow Level 6

// Boss Fight Murlocs.
// For enemies, code randomly generated attacks and defences against our
// Character. Add special abilities to the Murlocs of your choice, some examples
// Ground Slash
// Speed Dash
// maxHealth Regeneration

#include <iostream>
#include <memory>
#include <random>
#include <string>
using namespace std;
// Genertae random int between range
random_device rd;
mt19937 gen(rd());
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

// Character needs to have:
// maxHealth
// Melee Damage
// Defence
enum CharacterState {
  Idle = 1,
  Attacking,
  Attacked,
  Healing,
  Invincible,
  Critical,
  Defence
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
  virtual void LevelUp() {
    maxHealth += 50;
    attackStats += 2;
    defenceStats += 2;
    level += 1;
    curHealth = maxHealth;
  }
  virtual void ShowStats() {
    cout << "\n\nCurrent Stats  of " << charName << " are:\nHealth --> "
         << curHealth << "\nAttack -->" << attackStats << "\nDefence --> "
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
    if (enemyCharacter->GetState() != Defence &&
        enemyCharacter->GetType() != EnemyType) {
      enemyCharacter->SetState(Attacked);
      enemyCharacter->SpecialAbility();
    }
    if (GetType() != EnemyType)
      this->SpecialAbility();

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
    int newHealth = enemyCharacter->GetHealth() - damageAmt;
    if (newHealth <= 0) {
      enemyCharacter->SetCurHealth(0);
      cout << enemyCharacter->GetName() << " has Died!! \n\n";
      return;
    }
    enemyCharacter->SetCurHealth(newHealth);
    ResetActivity();
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
  int lifeStealActivationPercentage = 10;

public:
  Player(string name, int hp, int as, int ds, int ms, int ap = 10)
      : Character(name, hp, as, ds, ap, PlayerType, ms) {}
  void ShowStats() {
    Character::ShowStats();
    cout << "Magic --> " << GetMagic() << "\n";
  }
  void LevelUp() {
    Character::LevelUp();
    int swordAttackPower = 20;
    int shieldDefeceneIncrease = 5;
    int armourDefenceIncrease = 2;
    int armourMagicIncrease = 5;
    int bowAttackPower = 15;
    switch (GetLevel()) {
    case 2:
      cout << "\nPlayer got a map\n";
      break;
    case 3:
      cout << "\nPlayer got sword\n Mele damage attack power increase to "
           << swordAttackPower
           << "\nSpecial Ability Awarded - Critical hits (performs attack "
              "with massive damage boost)\n";
      SetAttackPower(swordAttackPower);
      break;
    case 4:
      cout << "\nPlayer got a shield\nDeferce rose by "
           << shieldDefeceneIncrease
           << "\nSpecial Ability Awarded - Blocker (will get 0 damage on "
              "enemy hit)";
      SetDefence(GetDefence() + shieldDefeceneIncrease);
      break;
    case 5:
      cout << "\nPlayer got a armour\nDeferce rose by " << armourDefenceIncrease
           << "\nMagic rose by " << armourMagicIncrease
           << "\nSpecial Ability Awarded - Blocker (will get 0 damage on "
              "enemy hit)";
      SetDefence(GetDefence() + armourDefenceIncrease);
      SetMagic(GetMagic() + armourMagicIncrease);
      break;
    case 6:
      cout << "\nPlayer got a Bow:\nCan do ranged damage next round will get "
              "half damage\n";
      break;
    }
  }
  // Healing
  void Heal(int healAmt = 0) {
    if (GetHealth() == GetMaxHealth()) {
      cout << "Already full HP\n";
      return;
    }
    SetState(Healing);
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
      if (activate(critActivationPercentage)) {
        SetState(Critical);
        return;
      }
    }
    if (GetLevel() < 4)
      return;
    // Blocker (will get 0 damage on enemy hit) Probability - 10 %
    if (GetState() == Attacked) {
      if (activate(blockActivationPercentage)) {
        cout << "\n" << GetName() << " Special Ability Blocker activated\n";
        SetState(Invincible);
        return;
      }
    }
    if (GetLevel() < 5)
      return;
    // Life steal (Recovering a small amount of HP after giving damage)
    // Probability - 10 %
    if (GetState() == Attacking) {
      if (activate(lifeStealActivationPercentage)) {
        cout << "\n" << GetName() << " Life Steal Ability Activated\n";
        int healAmt =
            ((GetAttackStat() + GetMagic()) - enemyCharacter->GetDefence()) / 2;
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
public:
  BossEnemy(string name, int hp, int as, int ds, int ms, int ap = 25)
      : Character(name, hp, as, ds, ap, BossEnemyType, ms) {
    cout << "\nBoss is here\n\n";
  }
void Heal(int healAmt = 0){
  if(GetHealth() == GetMaxHealth())
  {
    cout<<"\nHealth full so nothing happend\n";
    return;
  }
  float healthpercentage = 0.2;
  healAmt = (int)((0.2*GetMaxHealth()));
  int newHealth = healAmt + GetHealth();
  if (newHealth > GetMaxHealth())
    newHealth = GetMaxHealth();
  SetCurHealth(newHealth);
}
  // Defence, After succefull attack 20% chance to recover 25% of max hp, After attack 10 % chance of stuning player for next round
  void SpecialAbility(Character *enemyCharacter = NULL) {
    if (GetState() == Idle)
      SetState(Defence);
    if (GetState() == Attacking) {
      int healActivationPercentage = 20;
      if (activate(healActivationPercentage)) {
        cout << "\n"
             << GetName()
             << " Special Ability activated, health regentrated by 25%\n";
        Heal();
      }
    }
  }
};

class GamePlay {
private:
  int gameOver = false;
  int bossLevel = 6;

public:
  GamePlay() { cout << "Lets Start the game:\n\n"; }
  void SpawnEnemy(int num, Character *enemyies[num]) {
    int enemyHp = 100;
    int attackStat = 15;
    int defenseStat = 5;
    for (int i = 0; i < num; i++) {
      enemyies[i] = new Enemy("Enemy " + to_string(i + 1), enemyHp, attackStat,
                              defenseStat);
    }
  }

  void SpawnBossEnemy(Character *bossEnemy[1]) {
    int enemyHp = 200;
    int attackStat = 35;
    int defenseStat = 30;
    int magicStat = 20;
    bossEnemy[0] = new BossEnemy("Boss Enemy", enemyHp, attackStat, defenseStat,
                                 magicStat);
  }

  void DeleteEnemy(int num, Character *enemyies[num]) {
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
      cout << "\nPlayer used bow to attack\n";
      break;
    }
    player->DealDamage(enemy, player->GetAttackPower());
  }

  void PlayerTurn(Character *player, int num, Character *enemyies[num]) {
    int userInput = 0;
    int flag = 1;
    while (flag) {
      flag = 0;
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
        flag = 1;
        break;
      }
    }
  }

  void MobEnemyTurn(Character *enemy, Character *player) {
    int attackChancePercentage = 75;
    if (activate(attackChancePercentage)) {
      enemy->DealDamage(player, enemy->GetAttackPower());
    } else {
      enemy->SpecialAbility();
    }
  }

  void BossEnemyTurn(Character *enemy, Character *player) {
    int attackChancePercentage = 75;
    if (activate(attackChancePercentage)) {
      enemy->DealDamage(player, enemy->GetAttackPower());
    } else {
      enemy->SpecialAbility();
    }
  }

  void EnemyTurn(Character *player, int num, Character *enemyies[num]) {
    for (int i = 0; i < num; i++) {
      if (enemyies[i]->IsAlive()) {
        if (enemyies[i]->GetType() != BossEnemyType)
          MobEnemyTurn(enemyies[i], player);
        else
          BossEnemyTurn(enemyies[i], player);
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

  void Play(Character *player) {
    int enyNum = 0;
    player->ShowStats();
    for (int lev = 1; lev < bossLevel && player->IsAlive(); lev++) {
      enyNum = lev;
      Character *enemyies[enyNum];
      SpawnEnemy(enyNum, enemyies);
      Combat(player, enyNum, enemyies);
      DeleteEnemy(enyNum, enemyies);
      if (player->IsAlive())
        player->LevelUp();
    }
    if (!player->IsAlive())
      return;
    enyNum = 1;
    Character *bossEnemy[enyNum];
    SpawnBossEnemy(bossEnemy);
    Combat(player, enyNum, bossEnemy);
    DeleteEnemy(enyNum, bossEnemy);
  }
};

int main() {
  unique_ptr<GamePlay> game(new GamePlay());
  int playerHp = 100, playerAttackStat = 15, playerDeffenceStat = 10,
      playerMagicStat = 10;
  Character *player = new Player("Player", playerHp, playerAttackStat,
                                 playerDeffenceStat, playerMagicStat);
  game->Play(player);
  if (!(player->IsAlive()))
    cout << "You lost!! \n\n";
  else
    cout << "You won!!\n\n";

  delete player;
}