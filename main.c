#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>

// max goods amount
#define MAX_GOOD 5

// supplier amount
#define SUPPLIER_AMOUNT 2

// consumer amount
#define CONSUMER_AMOUNT 8

typedef struct good
{
    char name[100];
    int amount;
} good_t;

typedef struct people
{
    good_t *good;
    int interval;
    int repeat;
} people_t;

good_t *getGoodFromName(char *);

good_t goods[MAX_GOOD];
people_t suppliers[SUPPLIER_AMOUNT];
people_t consumers[CONSUMER_AMOUNT];

int good_count = 0;

int main()
{
    int i;
    char tmp_file[15], tmp_name[100];

    // collect supplier config file
    for (i = 0; i < SUPPLIER_AMOUNT; i++)
    {
        // read from file
        sprintf(tmp_file, "supplier%d.txt", i + 1);
        FILE *fp = fopen(tmp_file, "r");
        // scan good name
        fscanf(fp, "%[^\n]", tmp_name);

        // get good
        good_t *good = getGoodFromName(tmp_name);
        // if not exist then insert it
        if (good == 0)
        {
            // check if good amount exceed MAX_GOOD
            if (good_count >= MAX_GOOD)
            {
                fprintf(stderr, "Good amount exceed maximum number of good!");
                return 1;
            }

            // add information
            strcpy(goods[good_count].name, tmp_name);
            goods[good_count].amount = 0;

            // get good
            good = &goods[good_count];
            good_count++;
        }

        // add good to supplier
        suppliers[i].good = good;

        // add other information to supplier
        fscanf(fp, "%d\n%d", &suppliers[i].interval, &suppliers[i].repeat);
    }

    return 0;
}

good_t *getGoodFromName(char *name)
{
    int i;
    for (i = 0; i < good_count; i++)
    {
        if (!strcmp(goods[i].name, name))
        {
            return &goods[i];
        }
    }
    return 0;
}