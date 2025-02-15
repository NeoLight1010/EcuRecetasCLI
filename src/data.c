#include "constants.h"
#include <data.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <style.h>
#include <utils.h>

void init_recipe(Recipe *recipe, char name[BUFFER_SIZE], int n_ingredients,
                 float quantities_of_ingredients[BUFFER_SIZE],
                 char ingredients[BUFFER_SIZE][BUFFER_SIZE], int n_instructions,
                 char instructions[BUFFER_SIZE][BUFFER_SIZE]) {
  strcpy(recipe->name, name);

  recipe->n_ingredients = n_ingredients;
  memcpy(recipe->quantities_of_ingredients, quantities_of_ingredients,
         sizeof recipe->quantities_of_ingredients);
  memcpy(recipe->ingredients, ingredients, sizeof recipe->ingredients);

  recipe->n_instructions = n_instructions;
  memcpy(recipe->instructions, instructions, sizeof recipe->instructions);
}

/* Searches a recipe by name and returns its index. */
int search_recipe_by_name(char name[BUFFER_SIZE], int n_recipes,
                          Recipe recipes[]) {
  for (int i = 0; i < n_recipes; i++) {
    if (strcmp(name, recipes[i].name) == 0)
      return i;
  }

  return -1;
}

void modify_recipe(int i, Recipe recipes[], char name[BUFFER_SIZE],
                   int new_n_ingredients,
                   float new_quantities_of_ingredients[BUFFER_SIZE],
                   char new_ingredients[BUFFER_SIZE][BUFFER_SIZE],
                   int new_n_instructions,
                   char new_instructions[BUFFER_SIZE][BUFFER_SIZE]) {
  init_recipe(&recipes[i], name, new_n_ingredients,
              new_quantities_of_ingredients, new_ingredients,
              new_n_instructions, new_instructions);
}

void show_recipe(int i, Recipe recipes[]) {
  printf(BLU "Nombre: " RESET "%s\n", recipes[i].name);
  puts(BLU "Ingredientes: " RESET);

  for (int j = 0; j < recipes[i].n_ingredients; j++)
    printf("\t- %s\t%.2f\n", recipes[i].ingredients[j],
           recipes[i].quantities_of_ingredients[j]);

  puts(BLU "\nInstrucciones:" RESET);
  for (int j = 0; j < recipes[i].n_instructions; j++)
    printf("\t%i. %s\n", j + 1, recipes[i].instructions[j]);
}

void show_all_recipes_names(int n_recipes, Recipe recipes[]) {
  for (int i = 0; i < n_recipes; i++) {
    printf(BLU "%i. " RESET "%s\n", i + 1, recipes[i].name);
  }
}

/*
 * "Moves" each recipe on the right if i one index to the left.
 */
void remove_recipe_from_array(int i, int *n_recipes, int array_length,
                              Recipe recipes[]) {
  for (int j = i; j < *n_recipes - 1; j++)
    if (j != array_length - 1)
      recipes[j] = recipes[j + 1];

  *n_recipes = *n_recipes - 1;
}

/*
 * Saves an array of recipes in a file.
 */
void dump_data(int n_recipes, Recipe recipes[], char *file_path) {
  FILE *file = fopen(file_path, "w");

  if (file == NULL) {
    puts(RED "El archivo no se abrió correctamente.");
    return;
  }

  for (int i = 0; i < n_recipes; i++) {
    Recipe recipe = recipes[i];

    fprintf(file, "%s\n", recipe.name);
    fprintf(file, "%i\n", recipe.n_ingredients);

    for (int j = 0; j < recipe.n_ingredients; j++)
      fprintf(file, "%f\n", recipe.quantities_of_ingredients[j]);

    for (int j = 0; j < recipe.n_ingredients; j++)
      fprintf(file, "%s\n", recipe.ingredients[j]);

    fprintf(file, "%i\n", recipe.n_instructions);

    for (int j = 0; j < recipe.n_instructions; j++)
      fprintf(file, "%s\n", recipe.instructions[j]);
  }

  fclose(file);
}

void load_data(int *n_recipes, Recipe recipes[], char *file_path) {
  FILE *file = fopen(file_path, "r");

  if (file == NULL) {
    puts(RED "El archivo no se abrió correctamente.");
    return;
  }

  for (*n_recipes = 0; *n_recipes < BUFFER_SIZE; *n_recipes = *n_recipes + 1) {
    Recipe recipe;
    char temp[BUFFER_SIZE];

    char *is_eof = fgets(recipe.name, BUFFER_SIZE, file);
    if (is_eof == NULL)
      break;
    remove_last_char(recipe.name);

    fgets(temp, BUFFER_SIZE, file);
    recipe.n_ingredients = atoi(temp);

    for (int i = 0; i < recipe.n_ingredients; i++) {
      fgets(temp, BUFFER_SIZE, file);
      recipe.quantities_of_ingredients[i] = atof(temp);
    }

    for (int i = 0; i < recipe.n_ingredients; i++) {
      fgets(recipe.ingredients[i], BUFFER_SIZE, file);
      remove_last_char(recipe.ingredients[i]);
    }

    fgets(temp, BUFFER_SIZE, file);
    recipe.n_instructions = atoi(temp);

    for (int i = 0; i < recipe.n_instructions; i++) {
      fgets(recipe.instructions[i], BUFFER_SIZE, file);
      remove_last_char(recipe.instructions[i]);
    }

    recipes[*n_recipes] = recipe;
  }

  fclose(file);
}
