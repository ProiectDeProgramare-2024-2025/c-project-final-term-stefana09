#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_APPS 10
#define MAX_CATEGORIES 5
#define MAX_NAME_LEN 50
#pragma warning(disable:4996)

#define COLOR_RESET "\033[0m"
#define COLOR_GREEN "\033[32m"
#define COLOR_BLUE "\033[34m"
#define COLOR_YELLOW "\033[33m"
#define COLOR_RED "\033[31m"

typedef struct {
    char name[MAX_NAME_LEN];
    int is_installed;
} Application;

typedef struct {
    char category_name[MAX_NAME_LEN];
    Application apps[MAX_APPS];
    int num_apps;
} Category;

Category catalog[MAX_CATEGORIES];
int nr_categorii;

typedef struct {
    char username[MAX_NAME_LEN];
    int installed_apps[MAX_CATEGORIES];
} User;

void show_menu();
void show_category_menu(Category* category);
void show_category_menu_short(Category* category);
void display_catalog();
void display_catalog_short();
void register_user(User* user);
void install_app(User* user);
void uninstall_app(User* user);
void show_statistics(User* user);
void citire_din_fisier();
void scriere_in_fisier();
void clear_screen();


void citire_din_fisier() {
    FILE* f = fopen("aplicatii.txt", "r");
    if (!f) {
        printf(COLOR_RED "Eroare la deschiderea fisierului de intrare.\n" COLOR_RESET);
        return;
    }

    fscanf(f, "%d", &nr_categorii);
    for (int i = 0; i < nr_categorii; i++) {
        fscanf(f, "%s %d", catalog[i].category_name, &catalog[i].num_apps);
        for (int j = 0; j < catalog[i].num_apps; j++) {
            fscanf(f, "%s %d", catalog[i].apps[j].name, &catalog[i].apps[j].is_installed);
        }
    }

    fclose(f);
}

void scriere_in_fisier() {
    FILE* f = fopen("aplicatii2.txt", "w");
    if (!f) {
        printf(COLOR_RED "Nu s-a putut deschide fisierul pentru scriere.\n" COLOR_RESET);
        return;
    }

    fprintf(f, "%d\n", nr_categorii);
    for (int i = 0; i < nr_categorii; i++) {
        fprintf(f, "%s %d\n", catalog[i].category_name, catalog[i].num_apps);
        for (int j = 0; j < catalog[i].num_apps; j++) {
            fprintf(f, "%s %d\n", catalog[i].apps[j].name, catalog[i].apps[j].is_installed);
        }
    }

    fclose(f);
}

void register_user(User* user) {
    printf(COLOR_YELLOW "Introduceti numele utilizatorului (fara spatii): " COLOR_RESET);
    scanf("%s", user->username);
    for (int i = 0; i < MAX_CATEGORIES; i++) {
        user->installed_apps[i] = 0;
    }
    printf(COLOR_GREEN "Inregistrare completa pentru utilizatorul '%s'.\n" COLOR_RESET, user->username);
}

void show_menu() {
    printf("\n" COLOR_BLUE "-> Sistem pentru gestionarea aplicatiilor <-\n" COLOR_RESET);
    printf("1. Vizualizeaza catalogul de aplicatii\n");
    printf("2. Instaleaza o aplicatie\n");
    printf("3. Dezinstaleaza o aplicatie\n");
    printf("4. Statistici aplicatii instalate\n");
    printf("5. Iesire\n");
}

void show_category_menu(Category* category) {
    printf("\n--- Categoria: " COLOR_BLUE "%s" COLOR_RESET " ---\n", category->category_name);
    for (int i = 0; i < category->num_apps; i++) {
        printf("%d. " COLOR_GREEN "%s" COLOR_RESET " %s\n", i + 1,
            category->apps[i].name,
            category->apps[i].is_installed ? "(Instalat)" : "");
    }
}

void show_category_menu_short(Category* category) {
    printf("\n--- Categoria: " COLOR_BLUE "%s" COLOR_RESET " ---\n", category->category_name);
}

void display_catalog() {
    for (int i = 0; i < nr_categorii; i++) {
        show_category_menu(&catalog[i]);
    }
}

void display_catalog_short() {
    for (int i = 0; i < nr_categorii; i++) {
        show_category_menu_short(&catalog[i]);
    }
}

void install_app(User* user) {
    int category_choice, app_choice;
    printf("Selectati categoria (1-%d): ", nr_categorii);
    scanf("%d", &category_choice);
    if (category_choice < 1 || category_choice > nr_categorii) {
        printf(COLOR_RED "❌ Optiune invalida!\n" COLOR_RESET);
        return;
    }

    Category* category = &catalog[category_choice - 1];
    show_category_menu(category);

    printf("Selectati aplicatia de instalat (1-%d): ", category->num_apps);
    scanf("%d", &app_choice);
    if (app_choice < 1 || app_choice > category->num_apps) {
        printf(COLOR_RED " Optiune invalida!\n" COLOR_RESET);
        return;
    }

    Application* app = &category->apps[app_choice - 1];
    if (app->is_installed) {
        printf(COLOR_YELLOW "Aplicatia este deja instalata!\n" COLOR_RESET);
    }
    else {
        app->is_installed = 1;
        user->installed_apps[category_choice - 1]++;
        printf(COLOR_GREEN " Aplicatia '%s' a fost instalata.\n" COLOR_RESET, app->name);
    }
}

void uninstall_app(User* user) {
    int category_choice, app_choice;
    printf("Selectati categoria (1-%d): ", nr_categorii);
    scanf("%d", &category_choice);
    if (category_choice < 1 || category_choice > nr_categorii) {
        printf(COLOR_RED " Optiune invalida!\n" COLOR_RESET);
        return;
    }

    Category* category = &catalog[category_choice - 1];
    show_category_menu(category);

    printf("Selectati aplicatia de dezinstalat (1-%d): ", category->num_apps);
    scanf("%d", &app_choice);
    if (app_choice < 1 || app_choice > category->num_apps) {
        printf(COLOR_RED " Optiune invalida!\n" COLOR_RESET);
        return;
    }

    Application* app = &category->apps[app_choice - 1];
    if (!app->is_installed) {
        printf(COLOR_YELLOW "Aplicatia nu este instalata.\n" COLOR_RESET);
    }
    else {
        app->is_installed = 0;
        user->installed_apps[category_choice - 1]--;
        printf(" Aplicatia ");
        printf(COLOR_GREEN "%s" COLOR_RESET, app->name);
        printf("  a fost dezinstalata.\n");
    }
}

void show_statistics(User* user) {
    printf("\n" COLOR_BLUE "Statistici aplicatii instalate:\n" COLOR_RESET);
    for (int i = 0; i < nr_categorii; i++) {
        printf("%s: " COLOR_YELLOW "%d aplicatii instalate\n" COLOR_RESET, catalog[i].category_name, user->installed_apps[i]);
    }
}

void clear_screen() {
    printf("\033[H\033[J");
}

int main() {
    User user;
    int choice;

    citire_din_fisier();
    register_user(&user);

    while (1) {
        show_menu();
        printf("Selectati o optiune: ");
        scanf("%d", &choice);
        switch (choice) {
        case 1:
            clear_screen();
            display_catalog();
            printf("Apasa 1 pentru a reveni la meniu: ");
            scanf("%d", &choice);
            break;
        case 2:
            clear_screen();
            display_catalog_short();
            install_app(&user);
            break;
        case 3:
            clear_screen();
            display_catalog_short();
            uninstall_app(&user);
            break;
        case 4:
            clear_screen();
            show_statistics(&user);
            break;
        case 5:
            scriere_in_fisier();
            printf("Iesire...\n");
            return 0;
        default:
            printf(COLOR_RED " Optiune invalida!\n" COLOR_RESET);
        }
    }

    return 0;
}