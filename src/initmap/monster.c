#include "monster.h"


int initListMonsters()
{
    ListMonsters* listMonsters = malloc(sizeof(ListMonsters));
    DataMonsters** dataMonsters = malloc(sizeof(DataMonsters*) * 4);

    dataMonsters[SOLDER] = malloc(sizeof(DataMonsters));
    dataMonsters[HUGE_SOLDER] = malloc(sizeof(DataMonsters));
    dataMonsters[GERERAL] = malloc(sizeof(DataMonsters));
    dataMonsters[BOSS] = malloc(sizeof(DataMonsters));
    // TODO: Checker l'allocation

    listMonsters->monster_total = 0;
    listMonsters->firstMonster = NULL;

    // NOTE: Moins pratique le tableau de champs, mieux vaut faire des tableaux de structures
    //dataMonsters->PDV = malloc(sizeof(int)*4);
    //dataMonsters->strength = malloc(sizeof(int)*4);
    //dataMonsters->mass = malloc(sizeof(double)*4);

    dataMonsters[SOLDER]->PDV = 1;
    dataMonsters[SOLDER]->strength = 1;
    dataMonsters[SOLDER]->mass = 1.0;
    dataMonsters[SOLDER]->value = 1.0;

    dataMonsters[HUGE_SOLDER]->PDV = 2;
    dataMonsters[HUGE_SOLDER]->strength = 2;
    dataMonsters[HUGE_SOLDER]->mass = 2.0;
    dataMonsters[HUGE_SOLDER]->value = 2.0;

    dataMonsters[GERERAL]->PDV = 1;
    dataMonsters[GERERAL]->strength = 1;
    dataMonsters[GERERAL]->mass = 1.0;
    dataMonsters[GERERAL]->value = 1.0;

    dataMonsters[BOSS]->PDV = 1;
    dataMonsters[BOSS]->strength = 1;
    dataMonsters[BOSS]->mass = 1.0;
    dataMonsters[BOSS]->value = 1.0;
    
    listMonsters->dataMonsters = dataMonsters;
    
    plateau->listMonsters = listMonsters;
}

int addToList(Monster* monster)
{
  if(plateau->listMonsters->firstMonster == NULL) {
    plateau->listMonsters->firstMonster = monster;
    return 0;
  }

  Monster* currentMonster = plateau->listMonsters->firstMonster;
  while (currentMonster->next != NULL) {  
    currentMonster = currentMonster->next;
  }
  currentMonster->next = monster;
  return 0;
}


int monster_popMonster(InfosNodes* InfosNodes, TypeMonster type, int idIn) 
{
  // OPTIMIZE: Passer sur des pointeurs de données de DATAMONSTERS
    Monster* monster = malloc(sizeof(Monster)); 
    // TODO: Checker l'allocation

    monster->PDV = plateau->listMonsters->dataMonsters[type]->PDV;
    monster->strength = plateau->listMonsters->dataMonsters[type]->strength;
    monster->mass = plateau->listMonsters->dataMonsters[type]->mass;
    monster->value = plateau->listMonsters->dataMonsters[type]->value;

    monster->next = NULL;
    monster->idIn = idIn;
    monster->type = type;
    monster->status = ALIVE;
    monster->orientation = HAUT;
    monster->x = InfosNodes->nodes[idIn].x;
    monster->y = InfosNodes->nodes[idIn].y;
    initItineraire(monster, InfosNodes);

    addToList(monster);
    plateau->listMonsters->monster_total++;
    return 0;
}

void attackMonster(Projectile* projectile) 
{
    projectile->cible->PDV = projectile->cible->PDV - projectile->damage;
    if(projectile->cible->PDV <= 0) {
        killMonster(projectile->cible);
    }
}

void killMonster(Monster* monster) 
{
    monster->status = DEAD;
    deleteToList(monster);
    // TODO: Le joueur gagne datamonster[type].value en plus de son argent
}

int deleteToList(Monster* monster) {
    printf("kill");
    if(plateau->listMonsters->firstMonster == monster) {
        plateau->listMonsters->firstMonster = plateau->listMonsters->firstMonster->next;
    } else { 
        Monster* currentMonster = plateau->listMonsters->firstMonster;
        while (currentMonster->next != monster)
        {  
            currentMonster = currentMonster->next; 
        }
        currentMonster->next = currentMonster->next->next;
    }
    return 0;
}



void get_itineraire(Monster* monster) 
{
    free(monster);
}

int moveMonster(Monster* monster) {

  // NOTE: Le monstre a atteint l'arrivée.
    if (monster->itineraire->next == NULL) {
      // TODO: La partie est terminée.
        return 0;
    }
    
    /* Le monstre est sur un mouvement horizontal */
    if(monster->itineraire->next->node->x - monster->x > 0.01 || monster->itineraire->next->node->x - monster->x < -0.01) {
        if(monster->itineraire->next->node->x - monster->x < 0) {
            monster->x = monster->x - 0.01/monster->mass;
            monster->orientation = GAUCHE;
        } else {
            monster->x = monster->x + 0.01/monster->mass;
            monster->orientation = DROITE;
        }   
    }

    /* Le monstre est sur un mouvement vertical */
   else if (monster->itineraire->next->node->y - monster->y > 0.01 || monster->itineraire->next->node->y - monster->y < -0.01) {
        if(monster->itineraire->next->node->y - monster->y < 0) {
            monster->y = monster->y - 0.01/monster->mass;
            monster->orientation = HAUT;
        } else {
            monster->y = monster->y + 0.01/monster->mass;
            monster->orientation = BAS;
        }
    }
    
    /* Le monstre est sur un noeud, il passe au suivant */
    else {
        Etape* etape = monster->itineraire->next;
        monster->itineraire->next = monster->itineraire->next->next;
        free(etape);
        moveMonster(monster);   
    }

    return 0;
}

int moveAllMonster()
{
    
  // NOTE: (Nicolas) interpretation de la condition suivante.
    if(plateau->listMonsters->firstMonster == NULL) {
        return 0;
    }

    Monster* currentMonster = plateau->listMonsters->firstMonster;
    while (currentMonster != NULL) {  
        moveMonster(currentMonster);
        currentMonster = currentMonster->next;
    }
    return 0;
}

int findMonster(Tour* tour) {
    
    if(plateau->listMonsters->firstMonster == NULL) {
        tour->lastMonster = NULL;
        return 0;
    }
    
    Monster* currentMonster = plateau->listMonsters->firstMonster;
    double distance = 0;
    double distanceMin = plateau->Xsplit*plateau->Ysplit;
    while (currentMonster != NULL)
    {  
        distance = sqrt(pow(abs(tour->x - currentMonster->x), 2) + pow(abs(tour->y - currentMonster->y), 2));
        //printf("%lf -- ", distance);
        //printf("%d -- ", tour->radar);
        if(distance < distanceMin && distance <= tour->radar) {
            distanceMin = distance;
            tour->lastMonster = currentMonster;
        }
        currentMonster = currentMonster->next;   
    }
    return 0;
}

int refindMonster(Tour* tour) {
    double distance = 0;
    if(tour->lastMonster->status == DEAD || sqrt(pow(abs(tour->x - tour->lastMonster->x), 2) + pow(abs(tour->y - tour->lastMonster->y), 2)) > tour->radar) {
        tour->lastMonster = NULL;
    }
    return 0;
}
