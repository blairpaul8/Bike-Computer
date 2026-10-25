#pragma once

#include <stdint.h>

static int blecent_gap_event(struct ble_gap_event *event, void *arg);

static int blecent_gap_event(struct ble_gap_event *event, void *arg);

static void blecent_scan(void);

static void blecent_on_disc_complete(const struct peer *peer, int status, void *arg);

static int blecent_on_subscribe(uint16_t conn_handle, const struct ble_gatt_error *error, struct ble_gatt_attr *attr, void *arg);

static void blecent_on_disc_complete(const struct peer *peer, int status, void *arg);

static void blecent_on_sync(void);

static void host_task(void *param);

