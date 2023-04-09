#pragma once
class CONFIGURATION_WDS {
public:
	class CAPTURE {
	public:
		bool Unattended;
		char VolumeToCapture[MAX_PATH];
		char SystemRoot[MAX_PATH];
		char ImageName[MAX_PATH];
		char ImageDescription[MAX_PATH];
		char DestinationFile[MAX_PATH];
		bool Overwrite;
	}capture;
	class WDS {
	public:
		bool UploadToWDSServer;
		char WDSServerName[MAX_PATH];
		char WDSImageGroup[MAX_PATH];
		char Username[MAX_PATH];
		char Password[MAX_PATH];
		bool DeleteLocalWimOnSuccess;
	}wds;
};