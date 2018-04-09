#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>

// max good amount
#define MAX_GOOD 5

// max amount per good
#define MAX_AMOUNT 100

// supplier amount
#define SUPPLIER_AMOUNT 2

// consumer amount
#define CONSUMER_AMOUNT 1

typedef struct good
{
    char name[257];
    int amount;
    pthread_mutex_t mutex;
} good_t;

typedef struct people
{
    good_t *good;
    int interval;
    int repeat;
    char role;                  // 'S' = supplier , 'C' = consumer
    pthread_t thread;
} people_t;

void print_time();
good_t *get_good_or_create(char *);
void *entry(void *);

// pthread_t thread_s[SUPPLIER_AMOUNT];
// pthread_t thread_c[CONSUMER_AMOUNT];
// pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

time_t now;
char s_time[50];

good_t goods[MAX_GOOD];
people_t suppliers[SUPPLIER_AMOUNT];
people_t consumers[CONSUMER_AMOUNT];

int good_count = 0;

int main()
{
    int i;
    char tmp_file[15], tmp_name[257];

    // collect supplier config file
    for (i = 0; i < SUPPLIER_AMOUNT; i++)
    {
        // read from file
        sprintf(tmp_file, "supplier%d.txt", i + 1);
        FILE *fp = fopen(tmp_file, "r");

        // check if file is exist
        if (!fp)
        {
            fprintf(stderr, "Cannot read from file %s!", tmp_file);
            return 1;
        }

        // scan good name
        fscanf(fp, "%[^\n]", tmp_name);

        // add good to supplier
        suppliers[i].good = get_good_or_create(tmp_name);

        // add other information to supplier
        fscanf(fp, "%d\n%d", &suppliers[i].interval, &suppliers[i].repeat);
    }
// TODO: merge both as people?
    // collect consumer config file
    for (i = 0; i < CONSUMER_AMOUNT; i++)
    {
        // read from file
        sprintf(tmp_file, "consumer%d.txt", i + 1);
        FILE *fp = fopen(tmp_file, "r");

        // check if file is exist
        if (!fp)
        {
            fprintf(stderr, "Cannot read from file %s!", tmp_file);
            return 1;
        }

        // scan good name
        fscanf(fp, "%[^\n]", tmp_name);

        // add good to consumer
        consumers[i].good = get_good_or_create(tmp_name);

        // add other information to consumer
        fscanf(fp, "%d\n%d", &consumers[i].interval, &consumers[i].repeat);
    }

    print_time();
    printf("STARTING...\nSupplier amount: %d\nConsumer amount: %d\nGood(s):", SUPPLIER_AMOUNT, CONSUMER_AMOUNT);
    for (i = 0; i < good_count; i++)
    {
        printf(" %s", goods[i].name);
    }
    printf("\n-------------------------------------------\n\n");

    // for (i = 0; i < SUPPLIER_AMOUNT; i++)
    // {
    //     if (pthread_create(&suppliers[i].thread, NULL, &supply, (void *) &suppliers[i]) != 0)
    //     {
    //         printf("Error creating supplier thread %d!\n", i);
    //     }
    // }

    // for (i = 0; i < CONSUMER_AMOUNT; i++)
    // {
    //     if (pthread_create(&consumers[i].thread, NULL, &consume, (void *) &consumers[i]) != 0)
    //     {
    //         printf("Error creating consumer thread %d!\n", i);
    //     }
    // }

    return 0;
}

void print_time()
{
    time(&now);
    struct tm *p = localtime(&now);
    strftime(s_time, 50, "%c", p);
    printf("[%s] ", s_time);
}

good_t *get_good_or_create(char *name)
{
    int i;
    // check if good is exist then return
    for (i = 0; i < good_count; i++)
    {
        if (!strcmp(goods[i].name, name))
        {
            return &goods[i];
        }
    }

    // good is not exist
    // check if good amount exceed MAX_GOOD
    if (good_count >= MAX_GOOD)
    {
        fprintf(stderr, "Good amount exceed maximum number of good!");
        return 1;
    }

    // add information
    strcpy(goods[good_count].name, name);
    goods[good_count].amount = 0;
    pthread_mutex_init(&goods[good_count].mutex, NULL);

    // increase good count
    good_count++;

    return &goods[good_count - 1];
}

void *supply(void *arg)
{
    int id = (int) arg;

    while (1)
    {
        // 
    }

    return NULL;
}

void *consume(void *arg)
{
    int id = (int) arg;

    while (1)
    {
        // 
    }

    return NULL;
}