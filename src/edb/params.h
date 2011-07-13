#ifndef _PARAMS_H
#define _PARAMS_H

/*
 *  config.h
 *
 *  Configuration parameters.
 *
*/

//Postgres vs MySQL in config.h
#include "configs/config.h"

// Multi-principal vs. single-principal (1 vs. 0)
#define MULTIPRINC 1
//if cryptapp is set, webapp should also be set


// in web app mode, we no longer use adjustable encryption
#define WEBAPP 1
#define TRAINING 1


/************* EVAL/DEBUGGING FLAGS ************/

//Flags for evaluation of different scenarios:
#define PARSING 0
#define DECRYPTFIRST 0


#define ASSERTS_ON true

/******* VERBOSITY ****************/

//flag for debugging, particularly verbose
const bool VERBOSE_G = true;

const bool VERBOSE_KEYACCESS = true;
const bool VERBOSE_EDBCLIENT = false;
const bool VERBOSE_EDBCLIENT_VERY = false;

#endif   /* _PARAMS_H */