#ifndef STRUCTURES_H_
#define STRUCTURES_H_

#include <SDL2/SDL.h>

typedef enum MapDataContent {
  MDATA_IMG = 1,
  MDATA_ENERGY = 2,
  MDATA_IN = 4,
  MDATA_OUT = 8,
  MDATA_PATH = 16,
  MDATA_CONSTRUCT = 32,
  MDATA_NODE = 64,
  MDATA_INFOSNODE = 128,
  MDATA_WAVES = 256
} MapDataContent;

typedef enum Operations {
  CHK_SUCCESS = 0,
  CHK_NULL = 0,
  CHK_ERROR_ALLOC = 1,
  CHK_ERROR_FILE = 2
} Operations;

typedef enum TypeMonster {
  SOLDER,
  HUGE_SOLDER,
  GERERAL,
  BOSS
} TypeMonster;

typedef struct Wave {
  int wave_id;
  float timeBegin;
  float freq_pop;
  float random;
  float nextMonster_timer;
  int monster_total;
  TypeMonster* monsters;
  struct Wave* next;
} Wave;

typedef struct ListWaves {
  int wave_total;
  Wave* next;
} ListWaves;


typedef struct RGBcolor {
  unsigned char red;
  unsigned char green;
  unsigned char blue;
} RGBcolor;

typedef struct Node {
  int id;
  int type;
  double x;
  double y;
  int link[4];
} Node;

typedef struct InfosNodes {
  int nbNoeud;
  int entrance_total;
  int idOut;
  int* idEntrees;
  Node* nodes;
  Node** shortPaths;
} InfosNodes;


typedef struct MapData {
  char* mapFile;
  int energy;
  RGBcolor pathCol;
  RGBcolor nodeCol;
  RGBcolor buildingCol;
  RGBcolor inCol;
  RGBcolor outCol;
  unsigned int contentState; 
  InfosNodes* infosNodes;
  ListWaves* listWaves;
} MapData;
typedef enum GeneralType {
  TOUR = 1,
  BATIMENT = 2,
  OTHER
} GeneralType;

typedef enum TypeCase {
  LASER = 0,
  MISSILE,
  RADAR,
  ARMEMENT,
  CENTRALE,
  MUNITION,
  TERRAIN,
  CHEMIN,
  NOEUD,
  ENTREE,
  SORTIE
} TypeCase;

typedef struct Monster Monster;

typedef enum Bool {
  FALSE = 0,
  TRUE = 1
} Bool;

typedef struct Tour {
  TypeCase type;
  int armement;
  int centrale;
  int munition;
  float rechargement;
  int radar;
  double x;
  double y;
  Uint32 time_tir;
  Monster* targetMonster;
  struct Tour* next;
} Tour;

typedef struct ListTours {
  int nbTours;
  Tour* next;
} ListTours;

typedef enum Action {
  ADD,
  GETINFO,
  REMOVE
} Action;

typedef struct ConstructionData {
  TypeCase type;
  int degats;
  int alimentation;
  int cadence;
  int portee;
  int range;
  int valeur_achat;
  int valeur_revente;
} ConstructionData;

typedef struct Etat {
  TypeCase type;
  Action action;
  int argent;
} Etat;

typedef enum Orientation {
   HAUT,
   BAS,
   GAUCHE,
   DROITE,
   VERTICAL,
   HORIZONTAL,
   IMMOBILE
} Orientation;

typedef enum Statut {
   ALIVE,
   DEAD
} Statut;

typedef struct Etape {
    Node* node;
    struct Etape* next;
} Etape;


typedef struct Itineraire {
   int nbEtape;
   Etape* next;
} Itineraire;

typedef struct Monster {
   float PDV;
   int strength;
   double mass;
   int value;
   int idIn;
   TypeMonster type;
   Statut status;
   Orientation orientation;
   double x;
   double y;
   Itineraire* itineraire;
   struct Monster* next; 
} Monster;

typedef enum ProjectileType {
    LAS,
    MIS
} ProjectileType;

typedef struct Projectile {
  Monster* cible;
  ProjectileType type;
  float damage;
  double x;
  double y;
  struct Projectile* next;
} Projectile;

typedef struct ListProjectiles {
    int nbProjectile;
    Projectile* next;
} ListProjectiles;

typedef struct DataMonsters {
   int PDV;
   int strength;
   double mass;
   int value;
} DataMonsters;

typedef struct ListMonsters {
   int monster_total;
   Monster* firstMonster;
   DataMonsters** dataMonsters;
} ListMonsters;

typedef struct Plateau {
  int Xsplit;
  int Ysplit;
  Etat joueur;
  ConstructionData constructionData[6];
  ListTours* listTours;
  ListMonsters* listMonsters;
  ListProjectiles* listProjectiles;
  Tour **tours;
  TypeCase *cases;
} Plateau;
#endif //STRUCTURES_H_