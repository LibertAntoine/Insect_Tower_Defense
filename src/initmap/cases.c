#include "cases.h"

int case_initPlateau(MapData* mapdata)
{
  plateau = malloc(sizeof(Plateau));
   if (!plateau) {
    return EXIT_FAILURE;
  }

  player_init();
  tour_initConstructionData();
  initListMonsters();

  unsigned char* pixel_data;
  pixel_data = ppm_loadImage(mapdata->mapFile);
   
  if(plateau->Xsplit*plateau->Ysplit != mapdata->energy) {

    return 0;
  }     
    
  TypeCase* cases = malloc(sizeof(int)*mapdata->energy);
  RGBcolor* pixel_ppm = malloc(sizeof(RGBcolor));
  int nbSortie = 0;
  for(int i = 0; i < plateau->Xsplit*plateau->Ysplit; i++) {
    pixel_ppm->red = (char) pixel_data[i*3];
    pixel_ppm->green = (char) pixel_data[i*3+1];
    pixel_ppm->blue = (char) pixel_data[i*3+2];
    if(case_RGBCompare(*pixel_ppm, mapdata->pathCol)) {
        cases[i] = CHEMIN;
    } else if (case_RGBCompare(*pixel_ppm, mapdata->nodeCol)) {
        cases[i] = NOEUD;   
    } else if (case_RGBCompare(*pixel_ppm, mapdata->buildingCol)) {
        cases[i] = TERRAIN;
    } else if (case_RGBCompare(*pixel_ppm, mapdata->inCol)) {
        cases[i] = ENTREE;
    } else if (case_RGBCompare(*pixel_ppm, mapdata->outCol)) {
        cases[i] = SORTIE;
        nbSortie++;
    } else {
      return 0;
    }
    //printf("%d ", cases[i]);
  }
  if(nbSortie != 1) {
    return 0;
  }
  plateau->cases = cases;
  plateau->listTours = malloc(sizeof(ListTours));
  plateau->listTours->nbTours = 0;
  plateau->listTours->next = NULL;
  plateau->listProjectiles = malloc(sizeof(ListProjectiles));
  plateau->listProjectiles->nbProjectile = 0;
  plateau->listProjectiles->next = NULL;

  plateau->tours = calloc(plateau->Xsplit*plateau->Ysplit, sizeof(Tour*));
  if (!plateau->tours) {
    return EXIT_FAILURE;
  }
  itineraire_checkChemin(mapdata);

  return 1;
}

int case_RGBCompare(RGBcolor color1, RGBcolor color2) {
  if(color1.green != color2.green) {
    return 0;
  } else if (color1.red != color2.red) {
    return 0;
  } else if (color1.blue != color2.blue) {
    return 0;
  } else {
    return 1;
  }
}

int case_getCaseIndex(int caseX, int caseY)
{
  return (caseY)*plateau->Xsplit + (caseX);
}

void case_getCasePosition(int caseIndex, int* caseX, int* caseY)
{
  *caseY = caseIndex / plateau->Xsplit + 1;
  *caseX = caseIndex % plateau->Xsplit + 1;
}

TypeCase case_getType(int caseX, int caseY)
{
  int index_case = case_getCaseIndex(caseX, caseY);
  return plateau->cases[index_case];
}

int case_getCaseCoordFromPixels(int positionX, int positionY, int *caseX, int *caseY, int px_width, int px_height)
{
  int case_width = px_width / plateau->Xsplit;
  int case_height = px_height / plateau->Ysplit;

  if (positionX >= 0 && positionX < px_width && positionY >= 0 && positionY < px_height) {
    *caseX = (positionX / case_width);
    *caseY = (positionY / case_height);
    return 1;
  }
  else {
    return 0;
  }
}

int case_isConstructible(int caseX, int caseY)
{
  int index_case = case_getCaseIndex(caseX, caseY);
  if (plateau->cases[index_case] != TERRAIN) {
    return 0;
  }
  else {
    return 1;
  }
}

void case_update(int caseX, int caseY, TypeCase newType)
{
  int index_case = case_getCaseIndex(caseX, caseY);
  plateau->cases[index_case] = newType;
}

GeneralType case_getGeneralConstructionType(TypeCase type)
{
  switch(type) {
    case LASER:
    case MISSILE:
      return TOUR;
      break;

    case RADAR:
    case ARMEMENT:
    case CENTRALE:
    case MUNITION:
      return BATIMENT;
      break;

    default:
      return OTHER;
      break;
  }
}

void case_addConstruction(int caseX, int caseY)
{
  int index_case = case_getCaseIndex(caseX, caseY);

  TypeCase type = plateau->joueur.type;

  GeneralType generalType = case_getGeneralConstructionType(type);

  case_update(caseX, caseY, type);

  if (generalType == TOUR) {
    printf("adding tower\n");
    tour_add(type, index_case);
  }
  else if (generalType == BATIMENT) {
    updateAllTower();
    printf("adding building\n");
    // Fonction de placement de batiment
  }
}

int case_isUserPlaced(int caseX, int caseY)
{
  int index_case = case_getCaseIndex(caseX, caseY);
  TypeCase type = plateau->cases[index_case];
  GeneralType generalType = case_getGeneralConstructionType(type);
  if (generalType == OTHER) {
    return 0;
  }
  else {
    return 1;
  }
}

void case_removeConstruction(int caseX, int caseY)
{
  int index_case = case_getCaseIndex(caseX, caseY);
  TypeCase type = case_getType(caseX, caseY);
  GeneralType generalType = case_getGeneralConstructionType(type);
  plateau->joueur.argent += tour_getPrixRevente(type);

  plateau->cases[index_case] = TERRAIN;
  if (generalType == TOUR) {
    free(plateau->tours[index_case]);
  }
}

Tour* case_getTourPointer(int caseX, int caseY)
{
  int index_case = case_getCaseIndex(caseX, caseY);
  Tour* tour = plateau->tours[index_case];
  if (tour != NULL) {
    return tour;
  }
}

void case_printInfos(int caseX, int caseY)
{
  int index_case = case_getCaseIndex(caseX, caseY);
  TypeCase type = plateau->cases[index_case];
  switch (type) {
    case TERRAIN:
      printf("Ceci est un terrain constructible\n");
      return 1;
      break;
    case CHEMIN:
      printf("Ceci est un chemin de passage\n");
      return 1;
      break;
    case LASER:
      printf("Ceci est une tour LASER\n");
      break;
    case MISSILE:
      printf("Ceci est une tour MISSILE\n");
      break;
    case CENTRALE:
      printf("Ceci est un batiment CENTRALE\n");
      break;
    case ARMEMENT:
      printf("Ceci est un batiment ARMEMENT\n");
      break;
    case MUNITION:
      printf("Ceci est un batiment MUNITION\n");
      break;
    case RADAR:
      printf("Ceci est un batiment RADAR\n");
      break;
  }
  GeneralType generalType = case_getGeneralConstructionType(type);

  if (generalType == BATIMENT) {
    int degats = plateau->constructionData[type].degats;
    int alimentation = plateau->constructionData[type].alimentation;
    int cadence = plateau->constructionData[type].cadence;
    int portee = plateau->constructionData[type].portee;
    int range = plateau->constructionData[type].range;
    int valeur_achat = plateau->constructionData[type].valeur_achat;
    int valeur_revente = plateau->constructionData[type].valeur_revente;
    printf("stats du batiment :\n");
    printf("Apport dégats : %d\n", degats);
    printf("Apport alimentation : %d\n", alimentation);
    printf("Apport portee : %d\n", portee);
    printf("Apport cadence : %d\n", cadence);
    printf("Portée d'effet : %d\n", range);
    printf("Cout à l'achat : %d\n", valeur_achat);
    printf("Cout à la revente : %d\n", valeur_revente);
  }
  else if (generalType == TOUR) {
    int degats = plateau->constructionData[type].degats;
    int alimentation = plateau->constructionData[type].alimentation;
    int cadence = plateau->constructionData[type].cadence;
    int portee = plateau->constructionData[type].portee;
    int valeur_achat = plateau->constructionData[type].valeur_achat;
    int valeur_revente = plateau->constructionData[type].valeur_revente;
    printf("stats de la tour à l'origine:\n");
    printf("dégats : %d\n", degats);
    printf("Besoins en alimentation : %d\n", alimentation);
    printf("portee : %d\n", portee);
    printf("cadence : %f\n", 60.0 / (float) cadence);
    printf("Cout à l'achat : %d\n", valeur_achat);
    printf("Cout à la revente : %d\n", valeur_revente);

    Tour* tour = plateau->tours[index_case];

    int armement_c = tour->armement;
    int centrale_c = tour->centrale;
    int munition_c = tour->munition;
    int radar_c = tour->radar;
    printf("\n");
    printf("La toure est reliée à :\n");
    printf("Usines d'armement : %d\n", armement_c);
    printf("Radar : %d\n", radar_c);
    printf("Entrepot de munitions : %d\n", munition_c);
    printf("Centrales à énergie : %d\n", centrale_c);

    float degats_calc = tour_calculDegats(tour);
    float cadence_calc = tour_calculCadence(tour);
    float portee_calc = tour_calculPortee(tour);
    printf("\n");
    printf("Portée calculée : %f\n", portee_calc);
    printf("Cadence calculée : %f\n", cadence_calc);
    printf("Degats calculés : %f\n", degats_calc);
  }
}

void get_casesi(int *caseX, int *caseY, Div *plateau_div)
{
  int mouseX, mouseY;
  SDL_GetMouseState(&mouseX, &mouseY);

  int RmouseX = mouseX - plateau_div->x;
  int RmouseY = mouseY - plateau_div->y;

  int stepX = plateau_div->width / plateau->Xsplit;
  int stepY = plateau_div->height / plateau->Ysplit;

  printf("%d %d\n", RmouseX, RmouseY);

  if (RmouseX >= 0 && RmouseY >= 0) {
    if (RmouseX <= plateau_div->width && RmouseY <= plateau_div->height) {
      // on est bien dans le plateau,
      *caseX = RmouseX / stepX;
      *caseY = RmouseY / stepY;
    }
  }
}

void get_casesf(float *caseX, float *caseY, Div *plateau_div)
{
  int mouseX, mouseY;
  SDL_GetMouseState(&mouseX, &mouseY);

  float RmouseX = mouseX - plateau_div->x;
  float RmouseY = mouseY - plateau_div->y;

  float stepX = plateau_div->width / plateau->Xsplit;
  float stepY = plateau_div->height / plateau->Ysplit;

  if (RmouseX >= 0 && RmouseY >= 0) {
    if (RmouseX <= plateau_div->width && RmouseY <= plateau_div->height) {
      // On est bien dans le plateau
      *caseX = RmouseX / stepX;
      *caseY = RmouseY / stepY;
    }
  }
}

void case_actionAdd(int caseX, int caseY)
{
  if(case_isConstructible(caseX, caseY)) {
    printf("Porte monaie : %d \n", plateau->joueur.argent);
    if (player_acheteConstruction(caseX, caseY)) {
      printf("Ajout de tour avec succes, -%d\n", tour_getPrixAchat(case_getType(caseX, caseY)));
      printf("Porte monaie : %d \n", plateau->joueur.argent);
    }
    else {
      printf("Vous n'avez pas assez\n");
    }
  }
  else {
    printf("This place is not available\n");
  }
}

void case_actionRemove(int caseX, int caseY)
{
  printf("invoking remove\n");

  if (case_isUserPlaced(caseX, caseY)) {
    printf("Suppression de tour avec succes, +%d\n", tour_getPrixRevente(case_getType(caseX, caseY)));
    case_removeConstruction(caseX, caseY);
    printf("Porte monaie : %d \n", plateau->joueur.argent);
  }
  else {
    printf("You can only remove your buildings\n");
  }
}

void case_actionInfo(int caseX, int caseY)
{
  case_printInfos(caseX, caseY);
}

void case_handleAction(int caseX, int caseY)
{
  switch(plateau->joueur.action) {
    case ADD:
      case_actionAdd(caseX, caseY);
      break;

    case GETINFO:
      case_actionInfo(caseX, caseY);
      break;

    case REMOVE:
      case_actionRemove(caseX, caseY);
      break;
  }
}

