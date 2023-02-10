

// Rewards:
// Sword (Rewarded after completing level 2)
// Shield (Rewarded after completing level 3)
// Armour (Rewarded after completing level 4)
// Bow (Rewarded after completing level 5)

// After Every Level:
// Increase the Character's stats after each level.
// Increase Max HP and reset the maxHealth to max,

// Special Ability:
// Depending upon the probability of activation of these special abilities - the
// abilities will get triggered when a Character chooses to do an attack

// Level Details
// Level 1

// There will be only 1 enemy.
// For enemies, code randomly generated attacks and defences against our
// Character. Our hero gets a map in level 1.
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
enum CharacterType { PlayerType = 1, EnemyType, BossEnemyType };
class Character {
private:
  int maxHealth;
  int curHealth;
  int attackStats; // based on level
  int defenceStats;
  int attackPow; // Based on weapon
  int level;
  CharacterType type;
  string charName;
  bool isAttacking = false;
  bool isAttacked = false;
  bool isHealing = false;

public:
  Character(string name, int hp, int as, int ds, int lev) {
    maxHealth = hp;
    curHealth = maxHealth;
    attackStats = as;
    defenceStats = ds;
    charName = name;
    level = lev;
  }
  //--Seters--
  void SetCurHealth(int hp) { curHealth = hp; }
  void SetAttack(int value) { attackStats = value; }
  void SetDefence(int value) { defenceStats = value; }
  void SetIsAttacking(bool value) { isAttacking = value; }
  void SetIsAttacked(bool value) { isAttacked = value; }
  void SetIsHealing(bool value) { isHealing = value; }
  void SetLevel(int value) { level = value; }
  void SetAttackPower(int value) { attackPow = value; }
  void ResetActivity() {
    isAttacking = false;
    isAttacked = false;
    isHealing = false;
  }

  //--Geters--

  int GetLevel() { return level; }
  int GetMaxHealth() { return maxHealth; }
  int GetHealth() { return curHealth; }
  int GetAttackStat() { return attackStats; }
  int GetDefence() { return defenceStats; }
  int GetAttackPower() { return attackPow; }
  bool IsAttacking() { return isAttacking; }
  bool IsAttacked() { return isAttacked; }
  bool IsHealinh() { return isHealing; }
  string GetName() { return charName; }
  CharacterType GetType() { return type; }
  bool IsAlive() {
    if (maxHealth <= 0)
      return false;
    return true;
  }
  virtual void ShowStats() {
    cout << "\n\nCurrent Stats  of " << charName << " are:\nHealth --> "
         << curHealth << "\nAttack -->" << attackStats << "\nDefence --> "
         << defenceStats << "\n";
  }
  // Special Ability
  virtual void SpecialAbility(Character *enemyCharacter = NULL) = 0;

  // Deal Damage to enemy Character
  void DealDamage(Character &enemyCharacter, int attackPower) {

    this->SetIsAttacking(true);
    enemyCharacter.SetIsAttacked(true);
    int damageAmt = (attackPower + attackStats) - enemyCharacter.GetDefence();
    int newHealth = enemyCharacter.GetMaxHealth() - damageAmt;

    cout << "\n\n"
         << this->GetName() << " tried dealing -> " << damageAmt << " to "
         << enemyCharacter.GetName() << "\n";
    enemyCharacter.SpecialAbility();
    if (!(enemyCharacter.IsAttacked())) {
      cout << endl
           << enemyCharacter.GetName()
           << " blocked the attack, no damage done.";
      return;
    }
    if (newHealth <= 0) {
      enemyCharacter.SetCurHealth(0);
      cout << enemyCharacter.GetName() << " has Died!! \n\n";
      return;
    }
    enemyCharacter.SetCurHealth(newHealth);
    this->SpecialAbility(&enemyCharacter);
  }

  // virtual destructor
  virtual ~Character() {}
};
class Player : public Character {
private:
  int magicStats; // For heall
  int critActivationPercentage = 10;
  int blockActivationPercentage = 10;

public:
  Player(string name, int hp, int as, int ds, int ms, int lev = 1)
      : Character(name, hp, as, ds, lev) {
    magicStats = ms;
  }
  void ShowStats() {
    Character::ShowStats();
    cout << "Magic --> " << magicStats << "\n";
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
        DealDamage(*enemyCharacter, attackPower);
      }
    }
    if (GetLevel() == 2)
      return;
    // Blocker (will get 0 damage on enemy hit) Probability - 10 %
    if (IsAttacked()) {
      if (activate(blockActivationPercentage)) {
        cout<<"\n"<<GetName()<<" Special Ability Blocker activated\n";
        SetIsAttacked(false);
      }
    }
    if (GetLevel() == 3)
      return;
    
  }

  // Healing
  void Heal() {
    if (GetHealth() == GetMaxHealth()) {
      cout << "Already full HP, round wasted noob\n";
      return;
    }
    int healAmt = (GetMaxHealth() / magicStats) + (magicStats / 2);
    int newHealth = GetHealth() + healAmt;
    if (newHealth > GetMaxHealth()) {
      newHealth = GetMaxHealth();
    }
    SetCurHealth(newHealth);
    SetIsHealing(true);
    SpecialAbility();
  }
};
int main() {
  Character *player1 = new Player("Player 1", 100, 15, 10, 10);
  Character *player2 = new Player("Player 2", 100, 15, 10, 10);
  player1->ShowStats();
  player2->ShowStats();
  player1->SetLevel(2);
  player2->SetLevel(3);
  player1->DealDamage(*player2, 20);
  player2->ShowStats();
  delete player1;
  delete player2;
}