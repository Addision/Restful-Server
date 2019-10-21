#pragma once

#include <mutex>

typedef std::recursive_mutex MyRecursiveLock;
typedef std::lock_guard<std::recursive_mutex> RecursiveLock;
typedef std::mutex MyLock;
typedef std::unique_lock<std::mutex> AutoLock;
