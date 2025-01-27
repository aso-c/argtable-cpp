/*!
 * @brief Older variant of the C++ wrapper upon the pure ANSI-C command-line parsing library Argtable3 -
 * https://www.argtable.org (https://github.com/argtable/argtable3)
 * non-static arrays implementation
 * Development stopped
 * Implementation file
 * 	@file	argtable.cpp
 *	@author	(Solomatov A.A. (aso)
 *	@date Created 27.01.2025
 *	      Updated --.--.----
 *	@version 0.6.0
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
#include "argtable-legacy"


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


/// assign values of "srs" to "dest"
void Arg::head::set(struct arg_hdr& dest, const arg_hdr& src)
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
void Arg::head::set(arg_hdr& dest, arg_hdr&& tsrc)
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



const arg_hdr& Arg::item::hdr() const
////const arg_hdr& Arg::item::header() const
{
    return const_cast<item*>(this)->hdr();
}; /* const arg_hdr& Arg::item::hdr() const */

/// Set stored hdr field
void Arg::item::hdr(const arg_hdr& src)
{
    Arg::head::set(hdr(), src);
}; /* Arg::item::hdr(const arg_hdr&) */


void Arg::item::hdr(arg_hdr&& tmpsrc)
{
    Arg::head::set(hdr(), std::move(tmpsrc));
}; /* Arg::item::hdr(arg_hdr&&) */





#if 0
typedef struct arg_rem {
    struct arg_hdr hdr; /* The mandatory argtable header struct */
} arg_rem_t;
#endif

#if 0
/// copy constructor
//Arg:rem::rem(const struct arg_rem* arg)
template <>
Arg::rem::seed(const struct arg_rem* arg)
{
    assign(*arg);
}; /* Arg::rem::rem(const arg_rem*) */
#endif

/// assignment the value from other item
template <>
Arg::rem& Arg::rem::assign(const struct arg_rem& other)
{
    ESP_LOGI(__FUNCTION__, "Assign the [const arg_rem&] value other: [%p] to Arg::rem [%p]", &other, this);
    if (this != &other)	///< prevent autoassigment
	Arg::head::set(arg_rem::hdr, other.hdr);
    return *this;
}; /* Arg::rem::assign(const arg_rem&) */





#if 0

typedef struct arg_lit {
    struct arg_hdr hdr; /* The mandatory argtable header struct */
    int count;          /* Number of matching command line args */
} arg_lit_t;
#endif

#if 0
/// copy constructor
//Arg:lit::lit(const struct arg_lit* arg)
template <>
Arg::lit::seed(const struct arg_lit* arg)
{
    assign(*arg);
}; /* Arg::lit::lit(const arg_lit*) */
#endif

/// assignment the value from other item
template <>
Arg::lit& Arg::lit::assign(const struct arg_lit& other)
{
    ESP_LOGI(__FUNCTION__, "Assign the [const arg_lit&] value other: [%p] to Arg::lit [%p]", &other, this);
    if (this != &other)	///< prevent autoassigment
    {
	Arg::head::set(arg_lit::hdr, other.hdr);
	count = other.count;
    }; /* if this != &other */
    return *this;
}; /* Arg::lit::assign(const arg_lit&) */



#if 0
typedef struct arg_int {
    struct arg_hdr hdr; /* The mandatory argtable header struct */
    int count;          /* Number of matching command line args */
    int* ival;          /* Array of parsed argument values */
} arg_int_t;
#endif

#if 0
/// copy constructor
//Arg:integer::integer(const struct arg_lit* arg)
template <>
Arg::integer::seed(const struct arg_int* arg)
{
    assign(*arg);
}; /* Arg::integer::integer(const arg_int*) */
#endif
/// assignment the value from other item
template <>
Arg::integer& Arg::integer::assign(const struct arg_int& other)
{
    ESP_LOGI(__FUNCTION__, "Assign the [const arg_int&] value other: [%p] to Arg::integer [%p]", &other, this);
    if (this != &other)	///< prevent autoassigment
    {
	Arg::head::set(arg_int::hdr, other.hdr);
	count = other.count;
	ival  = other.ival;       /* Array of parsed argument values */
    }; /* if this != &other */
    return *this;
}; /* Arg::integer::assign(const arg_int&) */




#if 0
typedef struct arg_dbl {
    struct arg_hdr hdr; /* The mandatory argtable header struct */
    int count;          /* Number of matching command line args */
    double* dval;       /* Array of parsed argument values */
} arg_dbl_t;
#endif

#if 0
/// copy constructor
//Arg:dbl::dbl(const struct arg_dbl* arg)
template <>
Arg::dbl::seed(const struct arg_dbl* arg)
{
    assign(*arg);
}; /* Arg::dbl::dbl(const arg_dbl*) */
#endif

/// assignment the value from other item
template <>
Arg::dbl& Arg::dbl::assign(const struct arg_dbl& other)
{
    ESP_LOGI(__FUNCTION__, "Assign the [const arg_dbl&] value other: [%p] to Arg::dbl [%p]", &other, this);
    if (this != &other)	///< prevent autoassigment
    {
	Arg::head::set(arg_dbl::hdr, other.hdr);
	count = other.count;
	dval  = other.dval;       /* Array of parsed argument values */
    }; /* if this != &other */
    return *this;
}; /* Arg::dbl::assign(const arg_dbl&) */




#if 0
typedef struct arg_str {
    struct arg_hdr hdr; /* The mandatory argtable header struct */
    int count;          /* Number of matching command line args */
    const char** sval;  /* Array of parsed argument values */
} arg_str_t;
#endif

#if 0
/// copy constructor
//Arg:str::str(const struct arg_str* arg)
template <>
Arg::str::seed(const struct arg_str* arg)
{
    assign(*arg);
}; /* Arg::str::str(const arg_str*) */
#endif

/// assignment the value from other item
template <>
Arg::str& Arg::str::assign(const struct arg_str& other)
{
    ESP_LOGI(__FUNCTION__, "Assign the [const arg_str&] value other: [%p] to Arg::str [%p]", &other, this);
    if (this != &other)	///< prevent autoassigment
    {
	Arg::head::set(arg_str::hdr, other.hdr);
	count = other.count;
	sval  = other.sval;  /* Array of parsed argument values */
    }; /* if this != &other */
    return *this;
}; /* Arg::str::assign(const arg_str&) */





#if 0
typedef struct arg_rex {
    struct arg_hdr hdr; /* The mandatory argtable header struct */
    int count;          /* Number of matching command line args */
    const char** sval;  /* Array of parsed argument values */
} arg_rex_t;
#endif

#if 0
/// copy constructor
//Arg:rex::rex(const struct arg_rex* arg)
template <>
Arg::rex::seed(const struct arg_rex* arg)
{
    assign(*arg);
}; /* Arg::rex::rex(const arg_rex*) */
#endif

/// assignment the value from other item
template <>
Arg::rex& Arg::rex::assign(const struct arg_rex& other)
{
    ESP_LOGI(__FUNCTION__, "Assign the [const arg_rex&] value other: [%p] to Arg::rex [%p]", &other, this);
    if (this != &other)	///< prevent autoassigment
    {
	Arg::head::set(arg_rex::hdr, other.hdr);
	count = other.count;
	sval  = other.sval;  /* Array of parsed argument values */
    }; /* if this != &other */
    return *this;
}; /* Arg::rex::assign(const arg_rex&) */





#if 0
typedef struct arg_file {
    struct arg_hdr hdr;     /* The mandatory argtable header struct */
    int count;              /* Number of matching command line args*/
    const char** filename;  /* Array of parsed filenames  (eg: /home/foo.bar) */
    const char** basename;  /* Array of parsed basenames  (eg: foo.bar) */
    const char** extension; /* Array of parsed extensions (eg: .bar) */
} arg_file_t;
#endif

#if 0
/// copy constructor
//Arg:file::file(const struct arg_file* arg)
template <>
Arg::file::seed(const struct arg_file* arg)
{
    assign(*arg);
}; /* Arg::file::file(const arg_file*) */
#endif

/// assignment the value from other item
template <>
Arg::file& Arg::file::assign(const struct arg_file& other)
{
    ESP_LOGI(__FUNCTION__, "Assign the [const arg_file&] value other: [%p] to Arg::file [%p]", &other, this);
    if (this != &other)	///< prevent autoassigment
    {
	Arg::head::set(arg_file::hdr, other.hdr);
	count    = other.count;
	filename = other.filename;  /* Array of parsed filenames  (eg: /home/foo.bar) */
	basename = other.filename;  /* Array of parsed basenames  (eg: foo.bar) */
	extension = other.filename; /* Array of parsed extensions (eg: .bar) */
    }; /* if this != &other */
    return *this;
}; /* Arg::file::assign(const arg_file&) */




#if 0
typedef struct arg_date {
    struct arg_hdr hdr; /* The mandatory argtable header struct */
    const char* format; /* strptime format string used to parse the date */
    int count;          /* Number of matching command line args */
    struct tm* tmval;   /* Array of parsed time values */
} arg_date_t;
#endif

#if 0
/// copy constructor
//Arg:date::date(const struct arg_date* arg)
template <>
Arg::date::seed(const struct arg_date* arg)
{
    assign(*arg);
}; /* Arg::date::date(const arg_date*) */
#endif

/// assignment the value from other item
template <>
Arg::date& Arg::date::assign(const struct arg_date& other)
{
    ESP_LOGI(__FUNCTION__, "Assign the [const arg_date&] value other: [%p] to Arg::date [%p]", &other, this);
    if (this != &other)	///< prevent autoassigment
    {
	Arg::head::set(arg_date::hdr, other.hdr);
	count  = other.count;
	format = other.format; /* strptime format string used to parse the date */
	count  = other.count;  /* Number of matching command line args */
	tmval  = other.tmval;  /* Array of parsed time values */
    }; /* if this != &other */
    return *this;
}; /* Arg::date::assign(const arg_date&) */




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

#if 0
/// copy constructor
template <>
Arg::end::seed(const struct arg_end* arg)
//Arg::end::end(const struct arg_end* arg)
{
    assign(*arg);
}; /* Arg::end::end(const arg_end*) */
#endif

/// assignment the value from other item
template <>
Arg::end& Arg::end::assign(const struct arg_end& other)
{
    ESP_LOGI(__FUNCTION__, "Assign the [const arg_end&] value other: [%p] to Arg::end [%p]", &other, this);
    if (this != &other)	///< prevent autoassigment
    {
	Arg::head::set(arg_end::hdr, other.hdr);
	count  = other.count;
	error  = other.error;
	parent = other.parent;
	argval = other.argval;
    }; /* if this != &other */
    return *this;
}; /* Arg::end::assign(const arg_end&) */



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
Arg::table::table(int mx_err_cnt)//:
//    stor(1, static_cast<Arg::item*&&>(new end(arg_end(mx_err_cnt))))	// add terminal for commands sequention
    //stor{/*(1,*/ xcmd(arg_end(mx_err_cnt))/*)*/}	// add terminal for commands sequention
{ stor.insert(stor.end(), xcmd(arg_end(mx_err_cnt))); };


/// destructor
Arg::table::~table()
{
    /* deallocate each non-null entry in argtable[] */
    //arg_freetable(stor.data(), stor.size());
}; /* Argtable::~Argtable() */


/// Add new item in argtable
//template <>
//Arg::iterator Arg::table::add<Arg::cmd&&>(cmd&& cmd_item)
Arg::iterator Arg::table::addcmd(xcmd&& cmd_item)
{
    dirty = true;
    return stor.insert(std::prev(stor.end()), std::move(cmd_item));	// insert the new item before the "end" position of data vector
}; /* Arg::table::addo() */


/// Return the converted this to the void*
template <class kind>
void * Arg::seed<kind>::pure() {
    return reinterpret_cast<void*>(/*static_cast<kind*>(*/this/*)*/);
};

template void* Arg::seed<struct arg_rem>::pure();
template void* Arg::seed<struct arg_lit>::pure();
template void* Arg::seed<struct arg_int>::pure();
template void* Arg::seed<struct arg_dbl>::pure();
template void* Arg::seed<struct arg_rex>::pure();
template void* Arg::seed<struct arg_file>::pure();
template void* Arg::seed<struct arg_date>::pure();
template void* Arg::seed<struct arg_end>::pure();



template <typename It>
Arg::iterator Arg::table::add(It* &&it) {

    return addcmd(xcmd(it));

}; /* table::add<It, C>(It* &&it) */


template Arg::iterator Arg::table::add<struct arg_rem>(struct arg_rem* &&it);
template Arg::iterator Arg::table::add<arg_lit>(struct arg_lit* &&it);
template Arg::iterator Arg::table::add<arg_int>(struct arg_int* &&it);
template Arg::iterator Arg::table::add<arg_dbl>(struct arg_dbl* &&it);
template Arg::iterator Arg::table::add<arg_rex>(struct arg_rex* &&it);
template Arg::iterator Arg::table::add<arg_file>(struct arg_file* &&it);
template Arg::iterator Arg::table::add<arg_date>(struct arg_date* &&it);


template <std::size_t n, typename ... Stored>
inline void* xget(std::variant<Stored...> stored)
{
    if constexpr (std::variant_size_v<std::variant<Stored...>> != n)
    {
	if (stored.index() == n)
	    return static_cast<void*>(std::get<n>(stored));

	return xget<n + 1, Stored...>(stored);
    }; /* if constexpr std::variant_size_v<std::variant<Stored...>> != n */

    return nullptr;

}; /* xget() */



void* Arg::xcmd::get()
{
    return xget<0>(held);
}; /* Arg::xcmd::get() */


Arg::xcmd::~xcmd()
{
    void * ptr = get();
    arg_freetable( &ptr, 1);
}; /* Arg::xcmd::~xcmd() */





/// Get the syntax table as array to void* data
std::vector<void*>& Arg::table::syntax()
{
    /// if data is dirty - refresh the data
    if (dirty)
    {
	data.clear();
	data.reserve(stor.size());

	for (auto c = stor.begin(); c != stor.end(); c++)
	    data.push_back((void*)c->get());
	dirty = false;

    }; /* if dirty */
    return data;
}; /* std::vector<void*>& Arg::table::syntax() */



//--[ argtable.cpp ]---------------------------------------------------------------------------------------------------
