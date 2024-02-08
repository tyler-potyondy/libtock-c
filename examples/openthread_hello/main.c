#include <assert.h>
#include <openthread/dataset_ftd.h>
#include <openthread/instance.h>
#include <stdio.h>

static void setNetworkConfiguration(otInstance *aInstance);

int main(int argc, char *argv[])
{
    size_t   otInstanceBufferLength = 0;
    uint8_t *otInstanceBuffer       = NULL;

    otInstance *instance;

    // obtain required buffer size
    (void)otInstanceInit(NULL, &otInstanceBufferLength);

    // Call to allocate the buffer
    otInstanceBuffer = (uint8_t *)malloc(otInstanceBufferLength);
    assert(otInstanceBuffer);

    // Initialize OpenThread with the buffer
    /* Hangs within the below function call (see comment in loop)*/
    instance = otInstanceInit(otInstanceBuffer, &otInstanceBufferLength); 
    assert(instance);

    /* As part of the initialization, we will also configure / init 
    the thread network.
        - Init dataset with the following properties:
            - channel: 26
            - network key: 0x00112233445566778899aabbccddeeff 
            - PAN ID: 0xabcd
        - configure ip addr (ifconfig up cmd)
        - start thread network (thread start cmd) */
    setNetworkConfiguration(instance);

    /* Start the Thread network interface (CLI cmd > ifconfig up) */
    otIp6SetEnabled(instance, true);

    // Initialize UDP socket TODO (see guide: https://openthread.io/codelabs/openthread-apis#7)

    /* Start the Thread stack (CLI cmd > thread start) */
    otThreadSetEnabled(instance, true);

    for(int i = 0;;i++)
    {
        /* Until the platform library files are implemented, this will not
        be called. With the empty platform files, the Instance() constructor 
        that is called within the otInstanceInit function hangs. */
        if (i % 1000000 == 0) fprintf(stderr, ".");

        otTaskletsProcess(instance);
        otSysProcessDrivers(instance); 
    
    }

    return 0;
}

/**
 * Override default network settings, such as panid, so the devices can join a
 network
 */
void setNetworkConfiguration(otInstance *aInstance)
{
    static char          aNetworkName[] = "OTCodelab";
    otOperationalDataset aDataset;

    memset(&aDataset, 0, sizeof(otOperationalDataset));

    /*
     * Fields that can be configured in otOperationDataset to override defaults:
     *     Network Name, Mesh Local Prefix, Extended PAN ID, PAN ID, Delay Timer,
     *     Channel, Channel Mask Page 0, Network Key, PSKc, Security Policy
     */
    aDataset.mActiveTimestamp.mSeconds             = 1;
    aDataset.mActiveTimestamp.mTicks               = 0;
    aDataset.mActiveTimestamp.mAuthoritative       = false;
    aDataset.mComponents.mIsActiveTimestampPresent = true;

    /* Set Channel to 26 */
    aDataset.mChannel                      = 26;
    aDataset.mComponents.mIsChannelPresent = true;

    /* Set Pan ID to abcd */
    aDataset.mPanId                      = (otPanId)0xabcd;
    aDataset.mComponents.mIsPanIdPresent = true;

    /* Set Extended Pan ID to C0DE1AB5C0DE1AB5 */
    uint8_t extPanId[OT_EXT_PAN_ID_SIZE] = {0xC0, 0xDE, 0x1A, 0xB5, 0xC0, 0xDE, 0x1A, 0xB5};
    memcpy(aDataset.mExtendedPanId.m8, extPanId, sizeof(aDataset.mExtendedPanId));
    aDataset.mComponents.mIsExtendedPanIdPresent = true;

    /* Set network key to 00112233445566778899aabbccddeeff */
    uint8_t key[OT_NETWORK_KEY_SIZE] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
                                        0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff};
    memcpy(aDataset.mNetworkKey.m8, key, sizeof(aDataset.mNetworkKey));
    aDataset.mComponents.mIsNetworkKeyPresent = true;

    /* Set Network Name to OTCodelab */
    size_t length = strlen(aNetworkName);
    assert(length <= OT_NETWORK_NAME_MAX_SIZE);
    memcpy(aDataset.mNetworkName.m8, aNetworkName, length);
    aDataset.mComponents.mIsNetworkNamePresent = true;

    otDatasetSetActive(aInstance, &aDataset);
    /* Set the router selection jitter to override the 2 minute default.
       CLI cmd > routerselectionjitter 20
       Warning: For demo purposes only - not to be used in a real product */
    uint8_t jitterValue = 20;
    otThreadSetRouterSelectionJitter(aInstance, jitterValue);
}