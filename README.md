# Realm of Heroes: The Murloc Menace

"Realm of Heroes: The Murloc Menace" is a console-based RPG game where you, as a hero, must save your village from the impending threat of Murlocs and their minions. Embark on an epic adventure, fight waves of enemies, level up, and face the ultimate challenge - the Boss Enemy!

## Introduction

In this text-based RPG game, players take on the role of a hero who must defend their village from the invading forces of Murlocs and their minions. The game is divided into levels, with each level consisting of waves of enemies to defeat. As you progress, you'll gain experience, level up, and unlock special abilities and equipment to aid you in your battles.

## Gameplay

1. Players start by entering their hero's name.
2. Engage in turn-based combat with waves of enemies, choosing to attack, heal, or check your stats each turn.
3. After defeating each wave, you'll level up and potentially gain new equipment and special abilities.
4. The final challenge is the Boss Enemy, where you'll need all your skills and strategies to emerge victorious.

## Features

- Turn-based combat system.
- Level progression with stat upgrades and special abilities.
- Unique enemies with different behaviours and abilities.
- Equipment and items to enhance your hero's capabilities.
- Challenging Boss Enemy encounter.
- Interactive console-based interface.

## Object-Oriented Programming in "Realm of Heroes: The Murloc Menace"

The "Realm of Heroes: The Murloc Menace" game demonstrates the effective use of Object-Oriented Programming (OOP) concepts to design and implement its gameplay mechanics. Here are some key ways in which OOP principles are utilized in the game's design:

### Classes and Objects

The game defines several classes, such as Character, Player, Enemy, BossEnemy, Level, GamePlay, and TollKit. These classes serve as blueprints for creating objects with specific properties and behaviors. For example, each character (player, enemy, or boss) is an instance of the Character class.

### Encapsulation

OOP promotes encapsulation, which means bundling data (attributes) and methods (functions) that operate on that data into a single unit. In the game, the attributes like maxHealth, attackStats, and charName, as well as the methods like DealDamage and Heal, are encapsulated within the character classes. This helps in maintaining data integrity and providing a clear interface to interact with the objects.

### Inheritance

Inheritance is a key OOP concept that allows new classes to inherit properties and methods from existing classes. In the game, the Player, Enemy, and BossEnemy classes inherit from the Character class. This allows for code reusability and a hierarchy of character types.

### Polymorphism

Polymorphism allows different classes to be treated as instances of the same class through a common interface. The game uses polymorphism through the Character class's virtual function SpecialAbility(). Each subclass (Player, Enemy, BossEnemy) overrides this function to provide its own unique implementation.

### Abstraction

Abstraction focuses on providing essential features while hiding unnecessary details. The game abstracts away complex internal mechanics and interactions behind a user-friendly interface. Players don't need to know the intricate details of damage calculations; they just interact with the game through attacks, heals, and special abilities.

### Single Responsibility Principle

Each class in the game follows the Single Responsibility Principle, meaning each class has a specific and well-defined role. For instance, the Character class is responsible for character attributes and actions, the Level class handles level progression and combat mechanics, and the GamePlay class manages the overall gameplay flow.

### Modularity and Reusability

OOP encourages building modular and reusable code. The game's design allows for easy addition of new character types, items, or gameplay features without extensively modifying existing code. This promotes scalability and maintainability.

