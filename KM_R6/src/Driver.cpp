#include "Communication.hpp"

Communication* communication = nullptr;

extern "C"
NTSTATUS DriverMain(DRIVER_OBJECT* driver, UNICODE_STRING*) {
	driver->DriverUnload = [](DRIVER_OBJECT*) {
		if (!communication) return;

		communication->UndoHook();

		delete communication;
	};

	communication = new Communication("dxgkrnl.sys", "NtDxgkGetTrackedWorkloadStatistics");
	
	CommunicationServer;

	return communication->MakeHook()?STATUS_SUCCESS:STATUS_UNSUCCESSFUL;
}