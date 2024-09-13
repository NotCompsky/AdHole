#include <systemd/sd-bus.h> // for sd_bus_error
#include <systemd/sd-daemon.h>
#include <systemd/sd-journal.h>

int restart_dnsmasq(){
	// Connect to the system manager
	sd_bus_error error = SD_BUS_ERROR_NULL;
	sd_bus *bus = nullptr;
	
	const int r1 = sd_bus_open_system(&bus);
	if (r1 < 0){
		[[unlikely]]
		sd_journal_print(LOG_ERR, "Failed to connect to system bus: %s", error.message);
		return r1;
	}
	
	const int r2 = sd_bus_call_method(
		bus,
		"org.freedesktop.systemd1",  // service name
		"/org/freedesktop/systemd1",
		"org.freedesktop.systemd1.Manager", // interface name
		"RestartUnit", // action
		&error,
		nullptr,
		"ss",
		"dnsmasq.service",
		"replace"
	);
	if (r2 < 0){
		[[unlikely]]
		sd_journal_print(LOG_ERR, "Failed to restart dnsmasq service: %s", error.message);
		sd_bus_error_free(&error);
		sd_bus_unref(bus);
		return r2;
	}
	
	sd_bus_unref(bus);
	return 0;
}