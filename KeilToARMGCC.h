#ifndef KEIL2GCC___H
#define KEIL2GCC___H

#include "TString.h"
#include "TStringList.h"
#include "TXMLTag.h"
#include "TList.h"

enum KeilConversionState
{
	kcsHeader, 
	kcsParams,
	kcsVectors,
	kcsEnd
};

struct MemoryFragment
{
	unsigned long beginAddress;
	unsigned long endAddress;
};
 
class KeilToARMGCC
{
	private:
		KeilConversionState m_state;
		FILE* m_fileHandle;

		int   m_counter;
		int   m_stackSize;
		int   m_heapSize;
		bool  m_useFPU;
		int   m_fpuVersion;

		TStringList m_irqList;
		TStringList m_incList;
		TStringList m_srcList;
		TStringList m_defineList;

		TString m_targetName;
		TString m_startupFile;
		TString m_ldScriptFile;
		TString m_eclipseFile;

        TString         m_cpuType;
		TString         m_cpuPlatform;
        TString         m_cpuVendor;
        unsigned long   m_cpuClock;
		bool            m_fpuPresent;

		TList<MemoryFragment> m_romFragments;
		TList<MemoryFragment> m_ramFragments;

        TString         m_cflags;
        TString         m_defines;
		TString         m_newStartupFile;
        TString         m_systemFile;

        void SetState (KeilConversionState state);

        void ReplaceUIDAndWriteLine(const char* text);
        void WriteLine(const char* text);
        void WriteInt(int number);
        void WriteHex(int number);

		bool CreateStartupFile(const char* vectorFileName);
		bool CreateMakeFile(const char* makeFileName);
		bool CreateJLinkFile(const char* jlinkFileName);
		bool CreateLDScript(const char* ldScriptFileName);
		bool CreateProjectFile(const char* projectFileName);
		bool CreateCProjectFile(const char* cProjectFileName);

		void ParseLine(TString& line);
		void ParseParams(TString& line);
		void ParseVectors(TString& line);

        void ScanLibs(const char* makefilePath, const char* relativePath);

		bool FindMdk5IRom(TXMLTag* xmlTag, unsigned long& startAddress, unsigned long& endAddress);
		bool FindMdk5IRam1(TXMLTag* xmlTag, unsigned long &startAddress, unsigned long& endAddress);
		bool FindMdk5IRam2(TXMLTag* xmlTag, unsigned long &startAddress, unsigned long& endAddress);
		bool FindMdk5IRam(TXMLTag* xmlTag, unsigned long memoryIndex, unsigned long& startAddress, unsigned long& endAddress);
		bool ReadMemoryBlock(TXMLTag* ramEnabledTag, TXMLTag* ramAddressTag, unsigned long& startAddress, unsigned long& endAddress);

	public:
		KeilToARMGCC();
		bool ParseKeilProjectSettings(const char* keilProjectFile, 
			                          int keilTargetIndex, 
									  const char* targetDirectory,
									  bool        forceSoftFPU);

        void KeilToARMGCC::DoConversion(const char*  uv4ProjectFile,
                                        int         keilTargetIndex,
                                        const char* targetDirectory, 
                                        bool        forceSoftFPU,
                                        bool        scanlibs);

};


#endif