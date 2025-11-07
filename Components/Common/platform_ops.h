/**
 * @file platform_ops.h
 * @brief Common platform operations interface for embedded drivers
 * 
 * This header defines a standardized platform operations interface that can be
 * shared across multiple drivers requiring platform-specific functionality.
 * By centralizing these definitions, we ensure consistency and avoid duplication
 * across different driver implementations.
 * 
 * The platform operations typically include:
 * - Timing functions (delays, tick counters)
 * - Memory operations (allocation, copying)
 * - Synchronization primitives (mutexes, semaphores)
 * - Platform-specific utilities
 * 
 * @author Santiago Rincon
 * @date 2025-11-07
 */

#ifndef PLATFORM_OPS_H
#define PLATFORM_OPS_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/**
 * @brief Common platform operations structure
 */
typedef struct {
    /**
     * @brief Platform-specific millisecond delay function
     * 
     * @param[in] ms Delay time in milliseconds
     */
    void (*delay_ms)(uint32_t ms);
    
} platform_ops_t;

/**
 * @brief Validate platform operations structure
 * 
 * @param[in] ops Pointer to platform operations structure to validate
 * @return true if validation passes, false otherwise
 */
static inline bool platform_ops_validate(const platform_ops_t *ops) {
    if ((ops == NULL) || (ops->delay_ms == NULL)) {
        return false;
    }
    
    return true;
}

const platform_ops_t* platform_ops_get_instance(void);

#endif /* PLATFORM_OPS_H */
