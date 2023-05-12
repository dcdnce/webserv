#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "utils/Logger.hpp"

#ifndef MAX_CLIENTS
# define MAX_CLIENTS	10
#endif

#ifndef BUFFER_SIZE
# define BUFFER_SIZE	128
#endif

namespace http
{

	// TODO: Add defines for redundant things

}
