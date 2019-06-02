#include "gui.h"

ButtonName gui_getButtonNameFromAction(Action action)
{
  ButtonName button_name;
  switch(action) {
    case GETINFO:
      button_name = GETINFO_BTN;
      break;
    case ADD:
      button_name = ADD_BTN;
      break;
    case REMOVE:
      button_name = REMOVE_BTN;
      break;
  }
  return button_name;
}

ButtonName gui_getButtonNameFromTypeCase(TypeCase type)
{;
  ButtonName button_name;
  switch(type) {
    case LASER:
      button_name = LASER_BTN;
      break;
    case MISSILE:
      button_name = MISSILE_BTN;
      break;
    case RADAR:
      button_name = RADAR_BTN;
      break;
    case ARMEMENT:
      button_name = ARMEMENT_BTN;
      break;
    case CENTRALE:
      button_name = CENTRALE_BTN;
      break;
    case MUNITION:
      button_name = MUNITION_BTN;
      break;
  }
  return button_name;
}

Display gui_getTowerButtonState(TypeCase type)
{
  ButtonName button_name = gui_getButtonNameFromTypeCase(type);
  Button *currentButton = gameData->buttonGUI->buttons;
  while (currentButton->name != button_name) {
    currentButton = currentButton->next;
  }
  return currentButton->display;
}

void gui_changeTowerButtonState(TypeCase type, Display state)
{
  ButtonName button_name = gui_getButtonNameFromTypeCase(type);
  Button *currentButton = gameData->buttonGUI->buttons;
  while (currentButton->name != button_name) {
    if (currentButton->next == NULL) {
      return;
    }
    currentButton = currentButton->next;
  }
  currentButton->display = state;
}

void gui_changeActionButtonState(Action action, Display state)
{
  ButtonName button_name = gui_getButtonNameFromAction(action);
  Button *currentButton = gameData->buttonGUI->buttons;
  while (currentButton->name != button_name) {
    currentButton = currentButton->next;
  }
  currentButton->display = state;
}

GUI *gui_addChildren(SectionName name, int x, int y, int width, int height, GUI *parent)
{
  Div *dimensions = calloc(1, sizeof(Div));
  dimensions->x = x;
  dimensions->y = y;
  dimensions->width = width;
  dimensions->height = height;

  GUI *new = calloc(1, sizeof(GUI));
  new->parent = parent;
  new->childen = NULL;
  new->name = name;
  new->dimensions = dimensions;
  new->next = parent->childen;
  parent->childen = new;
  return new;
}

void gui_init()
{
  gameData->bodyGUI = calloc(1, sizeof(GUI));
  // TODO: check alloc
  gameData->bodyGUI->name = BODY;

  Div *dimensions = calloc(1, sizeof(Div));
  dimensions->width = WINDOW_WIDTH;
  dimensions->height = WINDOW_HEIGHT;
  gameData->bodyGUI->dimensions = dimensions;


  gameData->topGUI = gui_addChildren(HEADER, 0, 0, gameData->bodyGUI->dimensions->width, 40, gameData->bodyGUI);
  gui_addButton(gameData->topGUI, 600, 20, 45, 45, PAUSE_BTN, CLICKED);

  gameData->plateauGUI = gui_addChildren(PLATEAU, 0, 40, gameData->bodyGUI->dimensions->width, 600, gameData->bodyGUI);

  gameData->bottomGUI = gui_addChildren(FOOTER, 0, 640, gameData->bodyGUI->dimensions->width, 160, gameData->bodyGUI);

  gameData->infoGUI = gui_addChildren(INFO_SECTION, 0, 0, 600, gameData->bottomGUI->dimensions->height, gameData->bottomGUI);

  gameData->buttonGUI = gui_addChildren(TOWER_BUTTONS_SECTION, 600, 0, 200, gameData->bottomGUI->dimensions->height, gameData->bottomGUI);

  gui_addButton(gameData->buttonGUI, 30, 30, 45, 45, LASER_BTN, CLICKED);
  gui_addButton(gameData->buttonGUI, 90, 30, 45, 45, MISSILE_BTN, ACTIVE);

  gui_addButton(gameData->buttonGUI, 30, 80, 45, 45, ARMEMENT_BTN, ACTIVE);
  gui_addButton(gameData->buttonGUI, 90, 80, 45, 45, MUNITION_BTN, ACTIVE);
  gui_addButton(gameData->buttonGUI, 30, 130, 45, 45, RADAR_BTN, ACTIVE);
  gui_addButton(gameData->buttonGUI, 90, 130, 45, 45, CENTRALE_BTN, ACTIVE);

  gui_addButton(gameData->buttonGUI, 170, 30, 45, 45, ADD_BTN, CLICKED);
  gui_addButton(gameData->buttonGUI, 170, 80, 45, 45, GETINFO_BTN, ACTIVE);
  gui_addButton(gameData->buttonGUI, 170, 130, 45, 45, REMOVE_BTN, ACTIVE);


  gameData->mainMenuGUI = calloc(1, sizeof(GUI));
  gameData->mainMenuGUI->name = MAIN;
  Div *dimensions_main = calloc(1, sizeof(Div));
  dimensions_main->width = WINDOW_WIDTH;
  dimensions_main->height = WINDOW_HEIGHT;
  gameData->mainMenuGUI->dimensions = dimensions_main;
  gui_addButton(gameData->mainMenuGUI, 400, 400, 250, 45, LEVEL1_BTN, ACTIVE);
  gui_addButton(gameData->mainMenuGUI, 400, 470, 250, 45, LEVEL2_BTN, ACTIVE);
  gui_addButton(gameData->mainMenuGUI, 400, 540, 250, 45, LEVEL3_BTN, ACTIVE);

  gameData->endMenuGUI = calloc(1, sizeof(GUI));
  gameData->endMenuGUI->name =LOSEMENU ;
  Div *dimensions_end = calloc(1, sizeof(Div));
  dimensions_end->width = WINDOW_WIDTH;
  dimensions_end->height = WINDOW_HEIGHT;
  gameData->endMenuGUI->dimensions = dimensions_main;
  gui_addButton(gameData->endMenuGUI, 400, 400, 250, 45, MAINMENU_BTN, ACTIVE);
  gui_addButton(gameData->endMenuGUI, 400, 470, 250, 45, REPLAY_BTN, ACTIVE);
}

void gui_getAbsoluteDimensionsButton(GUI *section, Div *button)
{
  int button_x = 0;
  int button_y = 0;
  gui_getAbsoluteCoordinates(section, &button_x, &button_y);

  button->x = button_x + button->x - button->width / 2;
  button->y = button_y + button->y - button->height / 2;
}

void gui_getAbsoluteCoordinates(GUI *section, int *X, int *Y)
{
  if (section->parent == NULL) {
    return 1;
  }
  else {
    *X += section->dimensions->x;
    *Y += section->dimensions->y;
    gui_getAbsoluteCoordinates(section->parent, X, Y);
  }
}

void gui_addButton(GUI *section, int x, int y, int width, int height, ButtonName name, Display display_mode)
{
  Button *new_button = calloc(1, sizeof(Button));

  Div *dimensions = calloc(1, sizeof(Div));
  dimensions->x = x;
  dimensions->y = y;
  dimensions->width = width;
  dimensions->height = height;
  new_button->dimensions = dimensions;

  new_button->name = name;
  new_button->display = display_mode;
  new_button->next = section->buttons;

  section->buttons = new_button;
}
