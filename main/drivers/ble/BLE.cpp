// ble_driver.cpp
#include "ble_driver.hpp"
#include "esp_log.h"
#include "nvs_flash.h"
#include "nimble/nimble_port.h"
#include "nimble/nimble_port_freertos.h"
#include "host/ble_hs.h"
#include "host/util/util.h"
#include "services/gap/ble_svc_gap.h"
#include "esp_central.h"

static const char *TAG = "BleDriver";

// shared state (was: own_addr_type, plus the HR #defines)
static uint16_t s_serviceUuid;
static uint16_t s_charUuid;
static BleNotifyCallback s_notifyCb;
static void *s_notifyCtx;
static uint8_t s_ownAddrType;
static bool s_connected;

// forward declarations (they call each other)
static void scan(void);
static void onDiscComplete(const struct peer *peer, int status, void *arg);

// the old functions, moved here as-is
static bool shouldConnect(const struct ble_gap_disc_desc *disc)
{
    // was blecent_should_connect: compare against s_serviceUuid instead of BLECENT_SVC_HR_UUID
}

static int gapEvent(struct ble_gap_event *event, void *arg)
{
    // was blecent_gap_event:
    //  - NOTIFY_RX: copy the mbuf, then call s_notifyCb(buf, len, s_notifyCtx). No HR parsing.
    //  - CONNECT: set s_connected = true on success
    //  - DISCONNECT: set s_connected = false
}

static void onDiscComplete(const struct peer *peer, int status, void *arg)
{
    // was blecent_on_disc_complete: use s_serviceUuid / s_charUuid
}

static void scan(void)
{
    // was blecent_scan: use s_ownAddrType
}

static void onSync(void)
{
    // was blecent_on_sync: infer address into s_ownAddrType, then scan()
}

static void hostTask(void *param)
{
    nimble_port_run();
    nimble_port_freertos_deinit();
}

// ---- the public API from the header ----
bool BleDriver::begin(uint16_t serviceUuid, uint16_t charUuid, BleNotifyCallback cb, void *ctx)
{
    s_serviceUuid = serviceUuid;
    s_charUuid = charUuid;
    s_notifyCb = cb;
    s_notifyCtx = ctx;

    // was blue_init()
    nvs_flash_init();
    nimble_port_init();
    ble_hs_cfg.sync_cb = onSync;
    peer_init(MYNEWT_VAL(BLE_MAX_CONNECTIONS), 64, 64, 64);
    nimble_port_freertos_init(hostTask);
    return true;
}

bool BleDriver::isConnected() const
{
    return s_connected;
}