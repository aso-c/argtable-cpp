/*!
 * @brief Tiny C++ wrapper on the pure ANSI-C command-line parsing library Argtable3
 * https://www.argtable.org (https://github.com/argtable/argtable3)
 * Implementation file
 * 	@file	argtable.cpp
 *	@author	(Solomatov A.A. (aso)
 *	@date Created 08.10.2024
 *	      Updated 22.10.2024
 *	@version 0.3
 */


//#define LOG_LOCAL_LEVEL ESP_LOG_DEBUG	// 4 - set 'DEBUG' logging level

#include <cstring>
//#include <sys/unistd.h>
#include <utility>
#include <vector>
#include <list>


#include <esp_system.h>
#include <esp_err.h>

#include <esp_log.h>
#include <esp_console.h>
#include <argtable3/argtable3.h>

#include "argtable"


//TODO Будущая реализация: 1) создать иерархию виртуальных наследников базового класса ArgBase для каждого конкретного типа arg's
//TODO Будущая реализация: 2) Argtable хранит список std::list элементов ArgBase и генерирует "на лету" массив std::vector из указателей void* на элементы списка аргументов

#if 0
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


// arg_hdr& Arg::header() = 0;
// virtual void Arg::header(arg_hdr&&) = 0;
// virtual void Arg::header(const arg_hdr&) = 0;

/// assign values of "srs" to "dest"
void Arg::header::set(arg_hdr& dest, const arg_hdr& src)
{
    ESP_LOGI(__FUNCTION__, "Set Arg Header from !!!long lifetime value");
    dest.flag  = src.flag /*char flag*/;	/* Modifier flags: ARG_TERMINATOR, ARG_HASVALUE. */
    dest.shortopts = src.shortopts /*const char* shortopts*/; /* String defining the short options */
    dest.longopts = src.longopts /*const char* longopts*/;  /* String defiing the long options */
    dest.datatype = src.datatype /*const char* datatype*/;  /* Description of the argument data type */
    dest.glossary = src.glossary /*const char* glossary*/;  /* Description of the option as shown by arg_print_glossary function */
    dest.mincount = src.mincount /*int mincount*/;          /* Minimum number of occurences of this option accepted */
    dest.maxcount = src.maxcount /*int maxcount*/;          /* Maximum number of occurences if this option accepted */
    dest.parent  = src.parent /*void* parent*/;          /* Pointer to parent arg_xxx struct */
    dest.resetfn = src.resetfn /*arg_resetfn* resetfn*/;  /* Pointer to parent arg_xxx reset function */
    dest.scanfn  = src.scanfn /*arg_scanfn* scanfn*/;    /* Pointer to parent arg_xxx scan function */
    dest.checkfn = src.checkfn /*arg_checkfn* checkfn*/;  /* Pointer to parent arg_xxx check function */
    dest.errorfn = src.errorfn /*arg_errorfn* errorfn*/;  /* Pointer to parent arg_xxx error function */
    dest.priv  = src.priv /*void* priv*/;            /* Pointer to private header data for use by arg_xxx functions */
}; /* Arg::item::set_hdr(arg_hdr&, const arg_hdr&) */


/// move values of temporary "tsrs" to "dest"
void Arg::header::set(arg_hdr& dest, arg_hdr&& tsrc)
{
    ESP_LOGI(__FUNCTION__, "Set Arg Header from $$$temporary lifetime value");
    set(dest, tsrc);

    // clear pointer in the the temporary value
    dest.flag = '\0' /*char flag*/;/* ? */	/* Modifier flags: ARG_TERMINATOR, ARG_HASVALUE. */
    dest.shortopts = nullptr /*const char* shortopts*/; /* String defining the short options */
    dest.longopts = nullptr /*const char* longopts*/;  /* String defiing the long options */
    dest.datatype = nullptr /*const char* datatype*/;  /* Description of the argument data type */
    dest.glossary = nullptr /*const char* glossary*/;  /* Description of the option as shown by arg_print_glossary function */
    dest.mincount = 0 /*int mincount*/; /* ? */         /* Minimum number of occurences of this option accepted */
    dest.maxcount = 0/*int maxcount*/;  /* ? */        /* Maximum number of occurences if this option accepted */
    dest.parent   = nullptr /*void* parent*/;          /* Pointer to parent arg_xxx struct */
    dest.resetfn  = nullptr /*arg_resetfn* resetfn*/;  /* Pointer to parent arg_xxx reset function */
    dest.scanfn   = nullptr/*arg_scanfn* scanfn*/;    /* Pointer to parent arg_xxx scan function */
    dest.checkfn  = nullptr/*arg_checkfn* checkfn*/;  /* Pointer to parent arg_xxx check function */
    dest.errorfn  = nullptr/*arg_errorfn* errorfn*/;  /* Pointer to parent arg_xxx error function */
    dest.priv  = nullptr/*void* priv*/;            /* Pointer to private header data for use by arg_xxx functions */

}; /* Arg::item::set_hdr(arg_hdr&, arg_hdr&&) */



const arg_hdr& Arg::item::header() const
{
    return const_cast<item*>(this)->header();
}; /* const arg_hdr& Arg::item::header() const */

/// Set stored hdr field
void Arg::item::header(const arg_hdr& src)
{
    Arg::header::set(header(), src);
}; /* Arg::item::header(const arg_hdr&) */


void Arg::item::header(arg_hdr&& tmpsrc)
{
    Arg::header::set(header(), std::move(tmpsrc));
}; /* Arg::item::header(arg_hdr&&) */





#if 0
typedef struct arg_rem {
    struct arg_hdr hdr; /* The mandatory argtable header struct */
} arg_rem_t;
#endif

/// Get stored hdr field
arg_hdr& Arg::rem::header()
{
    return hdr;
}; /* Arg::rem::header() */




#if 0

typedef struct arg_lit {
    struct arg_hdr hdr; /* The mandatory argtable header struct */
    int count;          /* Number of matching command line args */
} arg_lit_t;
#endif

/// Get stored hdr field
arg_hdr& Arg::lit::header()
{
    return hdr;
}; /* Arg::lit::header() */




#if 0
typedef struct arg_int {
    struct arg_hdr hdr; /* The mandatory argtable header struct */
    int count;          /* Number of matching command line args */
    int* ival;          /* Array of parsed argument values */
} arg_int_t;
#endif

/// Get stored hdr field
arg_hdr& Arg::integer::header()
{
    return hdr;
}; /* Arg::integer::header() */





#if 0
typedef struct arg_dbl {
    struct arg_hdr hdr; /* The mandatory argtable header struct */
    int count;          /* Number of matching command line args */
    double* dval;       /* Array of parsed argument values */
} arg_dbl_t;
#endif

/// Get stored hdr field
arg_hdr& Arg::dbl::header()
{
    return hdr;
}; /* Arg::dbl::header() */





#if 0
typedef struct arg_str {
    struct arg_hdr hdr; /* The mandatory argtable header struct */
    int count;          /* Number of matching command line args */
    const char** sval;  /* Array of parsed argument values */
} arg_str_t;
#endif

/// Get stored hdr field
arg_hdr& Arg::str::header()
{
    return hdr;
}; /* Arg::str::header() */




#if 0
typedef struct arg_rex {
    struct arg_hdr hdr; /* The mandatory argtable header struct */
    int count;          /* Number of matching command line args */
    const char** sval;  /* Array of parsed argument values */
} arg_rex_t;
#endif

/// Get stored hdr field
arg_hdr& Arg::rex::header()
{
    return hdr;
}; /* Arg::rex::header() */




#if 0
typedef struct arg_file {
    struct arg_hdr hdr;     /* The mandatory argtable header struct */
    int count;              /* Number of matching command line args*/
    const char** filename;  /* Array of parsed filenames  (eg: /home/foo.bar) */
    const char** basename;  /* Array of parsed basenames  (eg: foo.bar) */
    const char** extension; /* Array of parsed extensions (eg: .bar) */
} arg_file_t;
#endif

/// Get stored hdr field
arg_hdr& Arg::file::header()
{
    return hdr;
}; /* Arg::file::header() */




#if 0
typedef struct arg_date {
    struct arg_hdr hdr; /* The mandatory argtable header struct */
    const char* format; /* strptime format string used to parse the date */
    int count;          /* Number of matching command line args */
    struct tm* tmval;   /* Array of parsed time values */
} arg_date_t;
#endif

/// Get stored hdr field
arg_hdr& Arg::date::header()
{
    return hdr;
}; /* Arg::date::header() */




#if 0
enum { ARG_ELIMIT = 1, ARG_EMALLOC, ARG_ENOMATCH, ARG_ELONGOPT, ARG_EMISSARG };
typedef struct arg_end {
    struct arg_hdr hdr;  /* The mandatory argtable header struct */
    int count;           /* Number of errors encountered */
    int* error;          /* Array of error codes */
    void** parent;       /* Array of pointers to offending arg_xxx struct */
    const char** argval; /* Array of pointers to offending argv[] string */
} arg_end_t;
#endif


/// copy constructor
Arg::end::end(const arg_end* arg)
{
    assign(arg);
}; /* Arg::end::end(const arg_end&) */

/// assignment the value from other item
Arg::end& Arg::end::assign(const arg_end& other)
{
    if (this != &other)	///< prevent autoassigment
    {
	Arg::header::set(this->hdr, other.hdr);
	count  = other.count;
	error  = other.error;
	parent = other.parent;
	argval = other.argval;
    };
    return *this;
}; /* Arg::end::assign(const arg_end&) */

/// clone the Arg::end instance into allocated memory object
Arg::end* Arg::end::clone() const
{
    end* p = new end(*this);
    return p;
}; /* Arg::end::clone() */


/// Get stored hdr field
arg_hdr& Arg::end::header()
{
    return hdr;
}; /* Arg::end::header() */





///// Initializing the Arg::cmd with l-value, long lifetime pointer
//Arg::cmd::cmd(item* const pitm):
//	arg(new item(pitm))
//{
//    ESP_LOGI(__FUNCTION__, "Create Arg::cmd object from the [const Arg::item*]");
//}; /* Arg::cmd::cmd(const item*) */


/// Initializing the Arg::cmd with temporary lifetime pointer
Arg::cmd::cmd(item* && pitm):
    arg(pitm)
{
    ESP_LOGI(__FUNCTION__, "Create Arg::cmd object from the [Arg::item* &&]");
}; /* Arg::cmd::cmd(item* &&) */


/// Initializing the Arg::cmd with l-value, long lifetime ref
Arg::cmd::cmd(const cmd& itm):
	arg(itm.arg->clone())
{
    ESP_LOGI(__FUNCTION__, "Create Arg::cmd object from the [const Arg::cmd&]");
}; /* Arg::cmd::cmd(const cmd&) */

/// Initializing the Arg::cmd with temporary lifetime - rvalue ref
Arg::cmd::cmd(cmd&& itm):
    arg(itm.arg)
{
    ESP_LOGI(__FUNCTION__, "Create Arg::cmd object from the [Arg::cmd&&]");
}; /* Arg::cmd::cmd(cmd&&) */


//Arg::cmd& Arg::cmd::assign(cmd&& other) { arg = other.arg; other.arg = nullptr; return *this; };

Arg::cmd& Arg::cmd::assign(const cmd& other)
{
    if (this != &other)	///< prevent autoassinment
    {
	delete arg;
	arg = other.arg;
    }; /* if this != &other */
    return *this;
}; /* Arg::cmd& Arg::cmd::assign(const cmd&) */

//Arg::cmd& Arg::cmd::assign(item* &&otharg) { arg = otharg; return *this; };

Arg::cmd& Arg::cmd::assign(const item* otharg)
{
    arg = otharg->clone();
    return *this;
}; /* Arg::cmd::assign(const item*) */

/// swap the args
void Arg::cmd::swap(cmd& other) noexcept
{
    item* tmp = other.arg;
    other.arg = arg;
    arg = tmp;
}; /* Arg::cmd::swap(cmd&) noexcept */





/// default constructor
Arg::table::table(int mx_err_cnt):
    stor(1, new end(arg_end(mx_err_cnt)))	// add terminal for commands sequention
{ };


/// destructor
Arg::table::~table()
{
    /* deallocate each non-null entry in argtable[] */
    //arg_freetable(stor.data(), stor.size());
}; /* Argtable::~Argtable() */


/// Add new item in argtable
template <>
Arg::iterator Arg::table::add<Arg::cmd&&>(cmd&& cmd_item)
{
    return stor.insert(std::prev(stor.end()), std::move(cmd_item));	// insert the new item before the "end" position of data vector
}; /* Arg::table::addo() */

#if 0
typedef struct arg_rem {
    ...
} arg_rem_t;
#endif

template <>
Arg::iterator Arg::table::add<arg_rem_t*>(arg_rem_t* && item)
{
    return add(rem(item));
}; /* Arg::table::add<arg_rem* &&>(arg_rem* &&) */


#if 0
typedef struct arg_lit {
    ...
} arg_lit_t;
#endif

template <>
Arg::iterator Arg::table::add<arg_lit_t*>(arg_lit_t* && item)
{
    return add(lit(item));
}; /* Arg::table::add<arg_lit* &&>(arg_lit* &&) */


#if 0

typedef struct arg_int {
    struct arg_hdr hdr; /* The mandatory argtable header struct */
    int count;          /* Number of matching command line args */
    int* ival;          /* Array of parsed argument values */
} arg_int_t;

typedef struct arg_dbl {
    struct arg_hdr hdr; /* The mandatory argtable header struct */
    int count;          /* Number of matching command line args */
    double* dval;       /* Array of parsed argument values */
} arg_dbl_t;

typedef struct arg_str {
    struct arg_hdr hdr; /* The mandatory argtable header struct */
    int count;          /* Number of matching command line args */
    const char** sval;  /* Array of parsed argument values */
} arg_str_t;

typedef struct arg_rex {
    struct arg_hdr hdr; /* The mandatory argtable header struct */
    int count;          /* Number of matching command line args */
    const char** sval;  /* Array of parsed argument values */
} arg_rex_t;

typedef struct arg_file {
    struct arg_hdr hdr;     /* The mandatory argtable header struct */
    int count;              /* Number of matching command line args*/
    const char** filename;  /* Array of parsed filenames  (eg: /home/foo.bar) */
    const char** basename;  /* Array of parsed basenames  (eg: foo.bar) */
    const char** extension; /* Array of parsed extensions (eg: .bar) */
} arg_file_t;

typedef struct arg_date {
    struct arg_hdr hdr; /* The mandatory argtable header struct */
    const char* format; /* strptime format string used to parse the date */
    int count;          /* Number of matching command line args */
    struct tm* tmval;   /* Array of parsed time values */
} arg_date_t;

enum { ARG_ELIMIT = 1, ARG_EMALLOC, ARG_ENOMATCH, ARG_ELONGOPT, ARG_EMISSARG };
typedef struct arg_end {
    struct arg_hdr hdr;  /* The mandatory argtable header struct */
    int count;           /* Number of errors encountered */
    int* error;          /* Array of error codes */
    void** parent;       /* Array of pointers to offending arg_xxx struct */
    const char** argval; /* Array of pointers to offending argv[] string */
} arg_end_t;

typedef struct arg_cmd_info {
    char name[ARG_CMD_NAME_LEN];
    char description[ARG_CMD_DESCRIPTION_LEN];
    arg_cmdfn* proc;
} arg_cmd_info_t;


#endif



//--[ argtable.cpp ]---------------------------------------------------------------------------------------------------
