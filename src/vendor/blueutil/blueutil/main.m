/*
 * blueutil
 * Command-line utility to control Bluetooth.
 * Uses private API from IOBluetooth framework (i.e. IOBluetoothPreference*()).
 * http://www.frederikseiffert.de/blueutil
 *
 * This software is public domain. It is provided without any warranty whatsoever,
 * and may be modified or used without attribution.
 *
 * Written by Frederik Seiffert <ego@frederikseiffert.de>
 * Minor source code improvement by Takayama Fumihiko <tekezo@pqrs.org>
 */

#import <Foundation/Foundation.h>
// link against IOBluetooth.framework

// private methods
int IOBluetoothPreferencesAvailable(void);

int IOBluetoothPreferenceGetControllerPowerState(void);
void IOBluetoothPreferenceSetControllerPowerState(int state);

int IOBluetoothPreferenceGetDiscoverableState(void);
void IOBluetoothPreferenceSetDiscoverableState(int state);

static int BTPowerState(void)
{
  return IOBluetoothPreferenceGetControllerPowerState();
}

static int BTSetPowerState(int powerState)
{
  if (BTPowerState() == powerState) {
    printf("Bluetooth is already %s\n", powerState ? "on" : "off");
    return EXIT_SUCCESS;
  }

  IOBluetoothPreferenceSetControllerPowerState(powerState);

  usleep(2000000);       // wait until BT has been set
  if (BTPowerState() != powerState) {
    printf("Error: unable to turn Bluetooth %s\n", powerState ? "on" : "off");
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

static void BTStatus(void)
{
  printf("Status: %s\n", BTPowerState() ? "on" : "off");
}

int main(int argc, const char* argv[])
{
  int result = EXIT_SUCCESS;

  if (! IOBluetoothPreferencesAvailable()) {
    printf("Error: Bluetooth not available");
    result = EXIT_FAILURE;
  } else if (argc == 2 && strcmp(argv[1], "status") == 0) {
    BTStatus();
  } else if (argc == 2 && strcmp(argv[1], "on") == 0) {
    result = BTSetPowerState(1);
  } else if (argc == 2 && strcmp(argv[1], "off") == 0) {
    result = BTSetPowerState(0);
  } else if (argc == 2 && strcmp(argv[1], "toggle") == 0) {
    result = BTSetPowerState(BTPowerState() == 0 ? 1 : 0);
  } else {
    printf("Usage: %s [status|on|off|toggle]\n", argv[0]);
    result = EXIT_FAILURE;
  }

  return result;
}
