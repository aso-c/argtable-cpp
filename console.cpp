/*!
 * @file console.cpp
 *
 * @brief Tiny C++ wrapper at ESP32 console library
 *
 * @detail Definition of the classes, types & procedures for the C++ wrapper on ESP32 console library
 * Implementation file.
 *
 * @section LICENCE
 *
 * This code is in the Public Domain (or CC0 licensed, at your option.)
 *
 * Unless required by applicable law or agreed to in writing, this
 * software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
 * CONDITIONS OF ANY KIND, either express or implied.
 *
 * @author: Solomatov A.A. (aso)
 * @version 0.6.4
 * @date Created on: 25 дек. 2024 г.
 *	    Updated: 30.01.2025
 */

#if 0
#include <errno.h>
#include <stdlib.h>
#endif	// if 0

//#include <stdint.h>
//#include <string.h>
//#include <stdbool.h>
//#include <stdio.h>
//#include <inttypes.h>

#include "esp_console.h"
#include "argtable3/argtable3.h"

//#include <freertos/FreeRTOS.h>
//#include <freertos/task.h>
#include <esp_log.h>
#include <esp_err.h>

//#include <time.h>
//#include <sys/time.h>

#include "argtable"
#include "console"


namespace esp
{
    namespace console
    {
	/// constructror for command execute w/o context
	cmd_complex::cmd_complex(/*.command =*/ const char name[] /*"bt"*//* | bluetooth"*/,
			esp_console_cmd_func_t /*exec_t*/ /*.func*/ exe /* = global_lambda*/,
			void* /*.argtable*/ syntaxtable /* = nullptr*/ /*bt::syntax*/,
			const char /*.help*/ help_str[] /* = nullptr*/ /*"General Bluetooth command"*/,
			const char /*.hint*/ hint_str[] /* = nullptr*/ /*"Bluetooth command exec"*/
//			esp_console_cmd_func_with_context_t /*.func_w_context*/ context_exe /*= nullptr*/,
//			void* /*.context*/ cntxt = nullptr
	):
		esp_console_cmd_t {
			.command = name,
		        .help = help_str,
		        .hint = hint_str,
		        .func = exe,
			.argtable = syntaxtable,
			.func_w_context = nullptr,
			.context = nullptr
	}
	{};

	/// constructor for command execute implementation with context
	cmd_complex::cmd_complex(/*.command =*/ char name[] /*"bt"*//* | bluetooth"*/,
//			esp_console_cmd_func_t /*.func*/ exe /* = global_lambda*/,
			esp_console_cmd_func_with_context_t /*cntxt_exec_t*/ /*.func_w_context*/ exec_wcontext /*= nullptr*/,
			void* /*.argtable*/ syntaxtable /* = nullptr*/ /*bt::syntax*/,
			char /*.help*/ help_str[] /* = nullptr*/ /*"General Bluetooth command"*/,
			char /*.hint*/ hint_str[] /* = nullptr*/ /*"Bluetooth command exec"*/,
			void* /*.context*/ cntxt /* = nullptr*/
	):
//	cmd::cmd(char name[], esp_console_cmd_func_with_context_t /*cntxt_exec_t*/ context_exec,
//		void* syntaxtable, char help_str[], char hint_str[], void* cntxt):
		esp_console_cmd_t {
			.command = name,
		        .help = help_str,
		        .hint = hint_str,
		        .func = nullptr,
			.argtable = syntaxtable,
			.func_w_context = exec_wcontext,
			.context = cntxt
	}
	{};

	//const esp::console::cmd bt_cmd ({
	//	.command = "bt"/* | bluetooth"*/,
	//        .help = "General Bluetooth command",
	//        .hint = nullptr/*"Bluetooth command exec"*/,
	//        .func = global_lambda,
	//	.argtable = bt::syntax,
	//	.func_w_context = nullptr,
	//	.context = nullptr
	//}); /* bt_cmd */

	/// register the current command
	esp_err_t cmd_complex::enreg() const
	{
	    return esp_console_cmd_register(this);
	}; /* esp::console::cmd::enreg() */

    }; /* namespace esp::console */

}; /* namespace esp */


//--[ console.cpp ]----------------------------------------------------------------------------------------------------
