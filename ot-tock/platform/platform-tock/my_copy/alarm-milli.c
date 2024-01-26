#include <alarm-milli.h> // from OpenThread

// openthread instance structure
// https://openthread.io/reference/group/api-instance
// no idea what otInstance is

/**
 * Set the alarm to fire at @p aDt milliseconds after @p aT0.
 *
 * For @p aT0 the platform MUST support all values in [0, 2^32-1].
 * For @p aDt, the platform MUST support all values in [0, 2^31-1].
 *
 * @param[in] aInstance  The OpenThread instance structure.
 * @param[in] aT0        The reference time.
 * @param[in] aDt        The time delay in milliseconds from @p aT0.
 */
void otPlatAlarmMilliStartAt(otInstance *aInstance, uint32_t aT0, uint32_t aDt);

/**
 * Stop the alarm.
 *
 * @param[in] aInstance  The OpenThread instance structure.
 */
void otPlatAlarmMilliStop(otInstance *aInstance);

/**
 * Get the current time.
 *
 * The current time MUST represent a free-running timer. When maintaining current time, the time value MUST utilize the
 * entire range [0, 2^32-1] and MUST NOT wrap before 2^32.
 *
 * @returns The current time in milliseconds.
 */
uint32_t otPlatAlarmMilliGetNow(void);

/**
 * Signal that the alarm has fired.
 *
 * @param[in] aInstance  The OpenThread instance structure.
 */
extern void otPlatAlarmMilliFired(otInstance *aInstance);

/**
 * Signal diagnostics module that the alarm has fired.
 *
 * @param[in] aInstance  The OpenThread instance structure.
 */
extern void otPlatDiagAlarmFired(otInstance *aInstance);

