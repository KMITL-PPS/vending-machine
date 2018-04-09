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
#define CONSUMER_AMOUNT 2

typedef struct good
{
    char name[257];
    int amount;
    pthread_mutex_t mutex;
} good_t;

typedef struct person
{
    good_t *good;
    int interval;
    int repeat;
    // char role;                  // 'S' = supplier , 'C' = consumer
    // pthread_t thread;
} person_t;

typedef struct arg
{
    int id;
    char role;                  // 'S' = supplier , 'C' = consumer
} arg_t;

char *cctime();
good_t *get_good_or_create(char *);
void *entry(void *);

// pthread_t thread_s[SUPPLIER_AMOUNT];
// pthread_t thread_c[CONSUMER_AMOUNT];
// pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_t threads[SUPPLIER_AMOUNT + CONSUMER_AMOUNT];
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

time_t now;
char s_time[50];

good_t goods[MAX_GOOD];
// person_t suppliers[SUPPLIER_AMOUNT];
// person_t consumers[CONSUMER_AMOUNT];

int good_count = 0;

int main()
{
    int i, j;

    // printf("%s STARTING...\nSupplier amount: %d\nConsumer amount: %d\nGood(s):", cctime(), SUPPLIER_AMOUNT, CONSUMER_AMOUNT);
    printf("%s STARTING...\nSupplier amount: %d\nConsumer amount: %d\n-------------------------------------------\n\n", cctime(), SUPPLIER_AMOUNT, CONSUMER_AMOUNT);

    for (i = 0; i < SUPPLIER_AMOUNT; i++)
    {
        arg_t *arg = malloc(sizeof(arg_t *));
        arg->id = i + 1;
        arg->role = 'S';
        if (pthread_create(&threads[i], NULL, &entry, arg) != 0)
        {
            printf("Error creating supplier thread %d!\n", i + 1);
        }
    }

    for (j = 0; j < CONSUMER_AMOUNT; i++, j++)
    {
        arg_t *arg = malloc(sizeof(arg_t *));
        arg->id = j + 1;
        arg->role = 'C';
        if (pthread_create(&threads[i], NULL, &entry, arg) != 0)
        {
            printf("Error creating consumer thread %d!\n", j + 1);
        }
    }

    // infinite loop
    while (1);

    return 0;
}

char *cctime()
{
    time(&now);
    struct tm *p = localtime(&now);
    strftime(s_time, 50, "%c", p);
    return s_time;
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
        // return NULL;
        exit(1);
    }

    // add information
    strcpy(goods[good_count].name, name);
    goods[good_count].amount = 0;
    pthread_mutex_init(&goods[good_count].mutex, NULL);

    // increase good count
    good_count++;

    return &goods[good_count - 1];
}

void *entry(void *arg)
{
    char tmp_file[15], tmp_name[257];
    person_t person;

    // get id & person role
    arg_t *arg_r = arg;
    int id = arg_r->id;
    char role = arg_r->role;

    // read from file
    sprintf(tmp_file, "%s%d.txt", (role == 'S' ? "supplier" : "consumer"), id);
    FILE *fp = fopen(tmp_file, "r");

    // check if file is exist
    if (!fp)
    {
        fprintf(stderr, "Cannot read from file %s!", tmp_file);
        exit(1);
    }

    // scan good name
    fscanf(fp, "%[^\n]", tmp_name);

    // add good to consumer
    pthread_mutex_lock(&mutex);
    person.good = get_good_or_create(tmp_name);
    pthread_mutex_unlock(&mutex);

    // add other information to consumer
    fscanf(fp, "%d\n%d", &person.interval, &person.repeat);

    // attempt count
    int i;
    // time to wait (default is interval)
    int time_to_wait = person.interval;
    for (i = 1; ; i++)
    {
        // try to lock thread
        if (pthread_mutex_trylock(&person.good->mutex) == 0)
        {
            // check if amount is ok
            if ((role == 'S' && person.good->amount < MAX_AMOUNT) || (role == 'C' && person.good->amount > 0))
            {
                // increase/decrease amount of good
                person.good->amount += (role == 'S' ? 1 : -1);

                printf("%s %s %s 1 unit. stock after = %d\n", cctime(), person.good->name, (role == 'S' ? "supplied" : "consumed"), person.good->amount);

                // reset time to wait and attempt count
                time_to_wait = person.interval;
                i = 0;
            }
            else
            {
                printf("%s %s %s going to wait.\n", cctime(), person.good->name, (role == 'S' ? "supplier" : "consumer"));
            }

            // unlock thread
            pthread_mutex_unlock(&person.good->mutex);
        }
        sleep(time_to_wait);

        // if attempt = repeat
        if (i == person.repeat)
        {
            // reset attempt count
            i = 0;
            // multiple time to wait by 2
            time_to_wait *= 2;
            // check if its exceed 60 sec
            if (time_to_wait >= 60)
                time_to_wait = 60;
        }
    }

    return NULL;
}