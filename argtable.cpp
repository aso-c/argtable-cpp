/*!
 * @brief Tiny C++ wrapper on the pure ANSI-C command-line parsing library Argtable3
 * https://www.argtable.org (https://github.com/argtable/argtable3)
 * Implementation file
 * 	@file	argtable.cpp
 *	@author	(Solomatov A.A. (aso)
 *	@date Created 08.10.2024
 *	      Updated 12.02.2025
 *	@version 0.6.11
 */


//#define LOG_LOCAL_LEVEL ESP_LOG_DEBUG	// 4 - set 'DEBUG' logging level

#include <cstring>
//#include <sys/unistd.h>
#include <utility>
#include <vector>
#include <list>
#include <variant>


#include <esp_system.h>
#include <esp_err.h>

#include <esp_log.h>
#include <esp_console.h>
#include <argtable3/argtable3.h>

#include "argtable"


namespace arg
{

}; /* namespace arg */





#if 0

//TODO Future implementation: 1) создать иерархию виртуальных наследников базового класса ArgBase для каждого конкретного типа arg's
//TODO Future implementation: 2) Argtable хранит список std::list элементов ArgBase и генерирует "на лету" массив std::vector из указателей void* на элементы списка аргументов

/*
 * The arg_hdr struct defines properties that are common to all arg_xxx structs.
 * The argtable library requires each arg_xxx struct to have an arg_hdr
 * struct as its first data member.
 * The argtable library functions then use this data to identify the
 * properties of the command line option, such as its option tags,
 * datatype string, and glossary strings, and so on.
 * Moreover, the arg_hdr struct contains pointers to custom functions that
 * are provided by each arg_xxx struct which perform the tasks of parsing
 * that particular arg_xxx arguments, performing post-parse checks, and
 * reporting errors.
 * These functions are private to the individual arg_xxx source code
 * and are the pointer to them are initiliased by that arg_xxx struct's
 * constructor function. The user could alter them after construction
 * if desired, but the original intention is for them to be set by the
 * constructor and left unaltered.
 */
typedef struct arg_hdr {
    char flag;             /* Modifier flags: ARG_TERMINATOR, ARG_HASVALUE. */
    const char* shortopts; /* String defining the short options */
    const char* longopts;  /* String defiing the long options */
    const char* datatype;  /* Description of the argument data type */
    const char* glossary;  /* Description of the option as shown by arg_print_glossary function */
    int mincount;          /* Minimum number of occurences of this option accepted */
    int maxcount;          /* Maximum number of occurences if this option accepted */
    void* parent;          /* Pointer to parent arg_xxx struct */
    arg_resetfn* resetfn;  /* Pointer to parent arg_xxx reset function */
    arg_scanfn* scanfn;    /* Pointer to parent arg_xxx scan function */
    arg_checkfn* checkfn;  /* Pointer to parent arg_xxx check function */
    arg_errorfn* errorfn;  /* Pointer to parent arg_xxx error function */
    void* priv;            /* Pointer to private header data for use by arg_xxx functions */
} arg_hdr_t;
#endif


//--[ argtable.cpp ]---------------------------------------------------------------------------------------------------
