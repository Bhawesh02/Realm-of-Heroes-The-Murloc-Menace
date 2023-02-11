

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
enum CharacterState { Idle = 1, Attacking, Attacked, Healing, Invincible };
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

public:
  Character(string name, int hp, int as, int ds, int ap, int lev = 1) {
    maxHealth = hp;
    curHealth = maxHealth;
    attackStats = as;
    defenceStats = ds;
    charName = name;
    level = lev;
    attackPow = ap;
  }
  //--Seters--
  void SetCurHealth(int hp) { curHealth = hp; }
  void SetAttack(int value) { attackStats = value; }
  void SetDefence(int value) { defenceStats = value; }
  void SetLevel(int value) { level = value; }
  void SetAttackPower(int value) { attackPow = value; }
  void SetState(CharacterState value) { state = value; }
  void ResetActivity() { state = Idle; }

  //--Geters--

  int GetLevel() { return level; }
  int GetMaxHealth() { return maxHealth; }
  int GetHealth() { return curHealth; }
  int GetAttackStat() { return attackStats; }
  int GetDefence() { return defenceStats; }
  int GetAttackPower() { return attackPow; }
  bool IsInvicible() {
    if (state == Invincible)
      return true;
    return false;
  }
  bool IsAttacking() { if (state == Attacking)
      return true;
    return false; }
  bool IsAttacked() {if (state == Attacked)
      return true;
    return false; }
  bool IsHealing() { if (state == Healing)
      return true;
    return false; }
  string GetName() { return charName; }
  bool IsAlive() {
    if (maxHealth <= 0)
      return false;
    return true;
  }
  virtual void LevelUp() {
    maxHealth += 2;
    attackStats += 2;
    defenceStats += 2;
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
    enemyCharacter->SetState(Attacked);
    int damageAmt = (attackPower + attackStats) - enemyCharacter->GetDefence();
    int newHealth = enemyCharacter->GetMaxHealth() - damageAmt;

    cout << "\n\n"
         << this->GetName() << " tried dealing -> " << damageAmt << " to "
         << enemyCharacter->GetName() << "\n";
    enemyCharacter->SpecialAbility();
    if (enemyCharacter->IsInvicible()) {
      cout << endl
           << enemyCharacter->IsInvicible() << enemyCharacter->GetName()
           << " blocked the attack, no damage done.";
      enemyCharacter->SetState(Invincible);
      return;
    }
    cout << "\nAttack was successfull\n";
    if (newHealth <= 0) {
      enemyCharacter->SetCurHealth(0);
      cout << enemyCharacter->GetName() << " has Died!! \n\n";
      return;
    }
    enemyCharacter->SetCurHealth(newHealth);
    SpecialAbility(enemyCharacter);
    ResetActivity();
    enemyCharacter->ResetActivity();
  }

  // virtual destructor
  virtual ~Character() {}
};
class Player : public Character {
private:
  int magicStats; // For heall
  int critActivationPercentage = 10;
  int blockActivationPercentage = 10;
  int lifeStealActivationPercentage = 10;

public:
  Player(string name, int hp, int as, int ds, int ms, int ap = 10)
      : Character(name, hp, as, ds, ap) {
    magicStats = ms;
  }
  void ShowStats() {
    Character::ShowStats();
    cout << "Magic --> " << magicStats << "\n";
  }
  void LevelUp() {
    Character::LevelUp();
    magicStats += 2;
  }
  // Healing
  void Heal(int healAmt = 0) {
    if (GetHealth() == GetMaxHealth()) {
      cout << "Already full HP\n";
      return;
    }
    SetState(Healing);
    if (!healAmt)
      healAmt = (GetMaxHealth() / magicStats) + (magicStats / 2);
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
    if (GetLevel() == 1)
      return;
    // Critical hits (performs attack with massive damage boost) Probability -
    // 10%
    if (IsAttacking()) {
      if (activate(critActivationPercentage)) {
        // Deal Damge with Attack Power = 2 * attackStats
        cout << "\nCritical Hit!!  Addition Damage done to "
             << enemyCharacter->GetName() << "\n";
        int attackPower = 2 * GetAttackStat();
        DealDamage(enemyCharacter, attackPower);
      }
    }
    if (GetLevel() == 2)
      return;
    // Blocker (will get 0 damage on enemy hit) Probability - 10 %
    if (IsAttacked()) {
      if (activate(blockActivationPercentage)) {
        cout << "\n" << GetName() << " Special Ability Blocker activated\n";
        SetState(Invincible);
      }
    }
    if (GetLevel() == 3)
      return;
    // Life steal (Recovering a small amount of HP after giving damage)
    // Probability - 10 %
    if (IsAttacking()) {
      if (activate(lifeStealActivationPercentage)) {
        cout << "\n" << GetName() << " Life Steal Ability Activated\n";
        int healAmt =
            ((GetAttackStat() + magicStats) - enemyCharacter->GetDefence()) / 2;
        Heal(healAmt);
      }
    }
  }
};

// In case I want add more feature to mob enemy
class Enemy : public Character {
public:
  Enemy(string name, int hp, int as, int ds, int ap = 10)
      : Character(name, hp, as, ds, ap) {
    cout << "\n" << name << " has spwawned\n";
  }
  void SpecialAbility(Character *enemyCharacter = NULL) {}
};

class GamePlay {
private:
  int gameOver = false;
};
// Max Num of enies allowed to spawn at a time
int maxNumEmies = 100;

// spwan num of enemies
void SpawnEnemy(Character *enemyies[maxNumEmies], int num) {
  int enemyHp = 100;
  int attackStat = 10;
  int defenseStat = 5;
  for (int i = 0; i < num; i++) {
    enemyies[i] = new Enemy("Enemy " + to_string(i + 1), enemyHp, attackStat,
                            defenseStat);
  }
}

void DeleteEnemy(Character *enemyies[maxNumEmies], int num) {
  for (int i = 0; i < num; i++) {
    delete enemyies[i];
  }
}

bool AllEnemyAlive(Character *enemyies[maxNumEmies], int num) {
  int DeadEnemy = 0;
  for (int i = 0; i < num; i++) {
    if (!(enemyies[i]->IsAlive()))
      DeadEnemy++;
  }
  if (DeadEnemy == num)
    return false;
  return true;
}

void Play(Character *player, Character *enemyies[maxNumEmies], int num) {
  // while ((player->IsAlive()) && AllEnemyAlive(enemyies, num)) {
  // //Start Here
  // }
}

int main() {
  bool gameOver = false;
  Character *player = new Player("Player", 100, 15, 10, 10);
  Character *enemy[maxNumEmies];
  while (player->IsAlive() && !gameOver) {
    // Level 1
    // There will be only 1 enemy.
    // For enemies, code randomly generated attacks and defences against our
    // Character. Our hero gets a map in level 1.
    int numOfEnemy = 1;
    SpawnEnemy(enemy, numOfEnemy);
    Play(player, enemy, numOfEnemy);
    DeleteEnemy(enemy, numOfEnemy);
    gameOver = true;
  }
  // if (!(player->IsAlive()))
  //   cout << "You lost!! \n\n";
  // else
  //   cout << "You won!!\n\n";

  delete player;
}