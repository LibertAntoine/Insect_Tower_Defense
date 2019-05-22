#include "projectile.h"


int create_projectile(Tour* tour, Monster* monster, float degats) {
  Projectile* projectile = malloc(sizeof(Projectile));
  projectile->cible = monster;
  projectile->x = tour->x;
  projectile->y = tour->y;
  projectile->damage = degats;
  projectile->next = NULL;

  addToProjectiles(projectile);
  plateau->listProjectiles->nbProjectile++;
}


int addToProjectiles(Projectile* projectile) {
  if(plateau->listProjectiles->next == NULL) {
    plateau->listProjectiles->next = projectile;
    return 0;
  }
  Projectile* currentProjectile = plateau->listProjectiles->next;
  while (currentProjectile->next != NULL)
  {  
    currentProjectile = currentProjectile->next;
  }
  currentProjectile->next = projectile;
  return 0;
}

int moveAllProjectiles() {
  if(plateau->listProjectiles->next == NULL) {
    return 0;
  }

  Projectile* currentProjectile = plateau->listProjectiles->next;
  int status;
  Projectile* projectileDelete;

  while (currentProjectile != NULL) {  
    projectileDelete = currentProjectile;

    if(currentProjectile->cible->status == DEAD) {
      currentProjectile = currentProjectile->next;
      deleteToProjectile(projectileDelete);
    } else {
      status = moveProjectile(currentProjectile);
      currentProjectile = currentProjectile->next;
      if(status == 1) {
        deleteToProjectile(projectileDelete);
      }  
    }

  }
  return 0;
}

int moveProjectile(Projectile* projectile) {
  if(projectile->cible->x - projectile->x > 0.15 || projectile->cible->x - projectile->x < -0.15) {
    if(projectile->cible->x - projectile->x < 0) {
      projectile->x = projectile->x - 0.15;

    } else {
      projectile->x = projectile->x + 0.15;
    }   
  }
  if (projectile->cible->y - projectile->y > 0.15 || projectile->cible->y - projectile->y < -0.15) {
    if(projectile->cible->y - projectile->y < 0) {
      projectile->y = projectile->y - 0.15;
    } else {
      projectile->y = projectile->y + 0.15;
    }
  }
  
  if (!(projectile->cible->x - projectile->x > 0.15 || projectile->cible->x - projectile->x < -0.15)) {
    if (!(projectile->cible->y - projectile->y > 0.15 || projectile->cible->y - projectile->y < -0.15)) {
      monster_attack(projectile);
      return 1;
    }
  }
  return 0;
}

int deleteToProjectile(Projectile* projectile) {
  if(plateau->listProjectiles->next == projectile) {
    plateau->listProjectiles->next = plateau->listProjectiles->next->next;
  } else { 
    Projectile* currentProjectile = plateau->listProjectiles->next;
    while (currentProjectile->next != projectile)
    {  
      currentProjectile = currentProjectile->next; 
    }
    currentProjectile->next = currentProjectile->next->next;
  }
  plateau->listProjectiles->nbProjectile--;
  free(projectile);
  return 0;
}
