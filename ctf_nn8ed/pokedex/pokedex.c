/*
 *
 *  CTF Navaja Negra 2018
 *  Pokedex - by @danigargu
 *
 *  gcc -no-pie pokedex_nn2k18.c -o pokedex_nn2k18
 *  socat tcp-l:1337,reuseaddr,fork exec:"timeout 60 ./pokedex",chroot=/home/ctf/jail,su-d=ctf
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_POKEMONS 160
#define NAME_LEN_MAX 2048

typedef struct _entry {
    float height;
    float weight;
    int power;
    int name_len;
    char *name;
} entry;

static entry *pokemons[MAX_POKEMONS] = {0};

int choose_id(char *msg)
{
    char buf[5] = {0};
    int idx = -1;
    do {
        printf("%s", msg);
        read(0, buf, 4);
        idx = atoi(buf);

        if (idx < 0 || idx >= MAX_POKEMONS)
            printf("Error: Invalid ID\n");

    } while (idx < 0 || idx >= MAX_POKEMONS);
    return idx;
}

void create()
{
    int idx;
    char name[NAME_LEN_MAX] = {0};

    idx = choose_id("Enter the new pokemon ID: ");
    if (pokemons[idx] != NULL)
    {
        printf("Pokemon already exists :(\n");
        return;
    }
    entry *pokemon = (entry *)malloc(sizeof(entry));
    if (pokemon != NULL)
    {
        int n;
        printf("Name: ");
        if ((n = read(0, name, sizeof(name))) > 0)
        {
            name[n-1] = '\0';
            pokemon->name_len = n-1;
            pokemon->name = (char *)malloc(n);

            if (pokemon->name != NULL)
                memcpy(pokemon->name, name, n);
        }

        printf("Height: ");
        scanf("%f", &pokemon->height);

        printf("Weight: ");
        scanf("%f", &pokemon->weight);

        printf("Power: ");
        scanf("%d", &pokemon->power);

        pokemons[idx] = pokemon;
        printf("[*] Pokemon saved!\n");
    }
}

void edit()
{
    int idx;
    int name_size;

    idx = choose_id("Enter the ID to edit: ");
    entry *pokemon = pokemons[idx];

    if (pokemon == NULL)
    {
        printf("[-] Empty index\n");
        return;
    }

    printf("New name: ");
    int n = read(0, pokemon->name, pokemon->name_len);
    if (n > 0)
    {
        pokemon->name[n-1] = '\0';
        pokemon->name_len = strlen(pokemon->name);
    }

    printf("Height: ");
    scanf("%f", &pokemon->height);
    printf("Weight: ");
    scanf("%f", &pokemon->weight);
    printf("Power: ");
    scanf("%d", &pokemon->power);
    printf("[*] Pokemon saved!\n");
}

void view(void)
{
    int idx;
    idx = choose_id("Enter the ID to print: ");

    if (pokemons[idx] == NULL)
    {
        printf("[-] Empty index\n");
        return;
    }
    entry *pokemon = pokemons[idx];
    printf("-------------------------------\n");
    printf("ID: %d\n", idx);
    printf("Name: %s\n", pokemon->name);
    printf("Height: %.02f\n", pokemon->height);
    printf("Weight: %.02f\n", pokemon->weight);
    printf("Power: %d\n", pokemon->power);
    printf("-------------------------------\n");
}

void delete()
{
    int idx;
    idx = choose_id("Insert the ID to delete: ");

    if (pokemons[idx] == NULL)
    {
        printf("Invalid index");
        return;
    }
    free(pokemons[idx]->name);
    free(pokemons[idx]);

    printf("[*] Pokemon deleted!");
}

int menu(void)
{
    char opt[6];
    
    printf("\nMenu:\n");
    printf("1) Create pokemon\n");
    printf("2) Edit pokemon\n");
    printf("3) Delete pokemon\n");
    printf("4) View pokemon\n");
    printf("5) Exit\n\n");
    printf("option> ");

    read(0, opt, 5);
    return atoi(opt);
}

int main(int argc, char *argv[])
{
    int opt;
    setvbuf(stdin,  NULL, _IONBF, 0);
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);

    printf("-=== Welcome to the POKEDEX ===-\n");
    do
    {
        opt = menu();
        switch (opt)
        {
            case 1:
            create();
            break;

            case 2:
            edit();
            break;

            case 3:
            delete();
            break;

            case 4:
            view();
            break;

            case 5:
            printf("bye!\n");
            break;

            default:
            printf("ERROR: invalid option: %d\n", opt);
            break;
        }

    } while (opt != 5);
}

