/*!
 * @brief Tiny C++ wrapper on the pure ANSI-C command-line parsing library Argtable3
 * https://www.argtable.org (https://github.com/argtable/argtable3)
 * Implementation file
 * 	@file	argtable.cpp
 *	@author	(Solomatov A.A. (aso)
 *	@date Created 08.10.2024
 *	      Updated 10.10.2024
 *	@version 0.1
 */


//#define LOG_LOCAL_LEVEL ESP_LOG_DEBUG	// 4 - set 'DEBUG' logging level

#include <cstring>
//#include <sys/unistd.h>
#include <vector>

#include <esp_system.h>
#include <esp_err.h>

#include <esp_log.h>
#include <esp_console.h>
#include <argtable3/argtable3.h>

#include "argtable"


//TODO Будущая реализация: 1) создать иерархию виртуальных наследников базового класса ArgBase для каждого конкретного типа arg's
//TODO Будущая реализация: 2) Argtable хранит список std::list элементов ArgBase и генерирует "на лету" массив std::vector из указателей void* на элементы списка аргументов

/// default constructor
Argtable::Argtable(int mx_err_cnt):
    stor(1, (void*)arg_end(mx_err_cnt))	// add terminal for commands sequention
{ };


/// destructor
Argtable::~Argtable()
{
    /* deallocate each non-null entry in argtable[] */
    arg_freetable(stor.data(), stor.size());
}; /* Argtable::~Argtable() */


/// Add new item in argtable
esp_err_t Argtable::addoption(void* item_ptr)
{
    ESP_LOGI(__FUNCTION__, "Length of Argtable is %i, capasity is %i", stor.size(), stor.capacity());
    if (stor.empty())
	stor.insert(stor.begin(), item_ptr);	// insert the new item before the "end" position of data vector

//    if (stor.size() + 1 > stor.capacity())
//	stor.resize(stor.size() + 1);

    stor.insert(std::prev(stor.end()), item_ptr);	// insert the new item before the "end" position of data vector
    return ESP_OK;
}; /* Argtable::addoption() */




//--[ argtable.cpp ]---------------------------------------------------------------------------------------------------
