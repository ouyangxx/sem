#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sem.h"
#include "random.h"

#if 0
sem_t * sem_open(const char *name, int oflag, mode_t mode, unsigned int value);
int sem_close(sem_t *sem);
int sem_unlink(const char *name);

int sem_init(sem_t *sem, int pshared, unsigned int value);
int sem_wait(sem_t *sem);
int sem_post(sem_t *sem);
int sem_destroy(sem_t *sem);
#endif

int sem_node_init(sem_node_t *sem_node, int pshared, unsigned int value)
{
	int ret = -1;
	sem_node->sem = NULL;
	memset(sem_node->sem_name, 0, sizeof(sem_node->sem_name));

    sprintf(sem_node->sem_name, "%u", random_uint());
	#ifdef __APPLE__
	sem_node->sem = sem_open(sem_node->sem_name, O_CREAT|O_EXCL, S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH, value);
	if (sem_node->sem != NULL)
	{
		ret = 0;
	}
	#else
	sem_node->sem = (sem_t *)malloc(sizeof(sem_t));
	ret = sem_init(sem_node->sem, pshared, value);
	#endif
	return ret;
}

int sem_node_destroy(sem_node_t *sem_node)
{
	int ret = -1;
	#ifdef __APPLE__
	if (sem_close(sem_node->sem) == 0 && sem_unlink(sem_node->sem_name) == 0)
	{
		ret = 0;
	}
	#else
	ret = sem_destroy(sem_node->sem);
	if (sem_node->sem != NULL)
	{
		free(sem_node->sem);
		sem_node->sem = NULL;
	}
	#endif
	return ret;
}

int sem_node_wait(sem_node_t *sem_node)
{
	return sem_wait(sem_node->sem);
}

int sem_node_post(sem_node_t *sem_node)
{
	return sem_post(sem_node->sem);
}
