#pragma once

#include "GenericGraph.h"
#include "GenericGraphNode.h"
#include "GenericGraphEdge.h"

// You should place include statements to your module's private header files here.  You only need to
// add includes for headers that are used in most of your module's source files though.
#include "GenericGraphEditor.h"

#define LOG_INFO(FMT, ...) UE_LOG(LogGenericGraph, Display, (FMT), ##__VA_ARGS__)
#define LOG_WARNING(FMT, ...) UE_LOG(LogGenericGraph, Warning, (FMT), ##__VA_ARGS__)
#define LOG_ERROR(FMT, ...) UE_LOG(LogGenericGraph, Error, (FMT), ##__VA_ARGS__)
