/*---------------------------------------------------------------------------*/
//       Author : hiyohiyo
//         Mail : hiyohiyo@crystalmark.info
//          Web : https://crystalmark.info/
//      License : MIT License
/*---------------------------------------------------------------------------*/

#include "../stdafx.h"
#include "OsInfoFx.h"
#include "SystemInfoFx.h"
#include "UtilityFx.h"


//------------------------------------------------
// Get System Information by WMI
//------------------------------------------------

//warning : enum3, enum class
#pragma warning(disable : 26812)

#include <comdef.h>
#include <comutil.h>
#include <wbemcli.h>
#pragma comment(lib, "oleaut32.lib")
#pragma comment(lib, "wbemuuid.lib")

#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }
#ifndef safeCloseHandle
#define safeCloseHandle(h) { if( h != NULL ) { ::CloseHandle(h); h = NULL; } }
#endif
#ifndef safeVirtualFree
#define safeVirtualFree(h,b,c) { if( h != NULL ) { ::VirtualFree(h, b, c); h = NULL; } }
#endif

void GetCpuInfo(CString& cpuInfo, int* cores, int* threads)
{
	CString query = L"Select * from Win32_Processor";

	IWbemLocator*			pIWbemLocator = NULL;
	IWbemServices*			pIWbemServices = NULL;
	IEnumWbemClassObject*	pEnumCOMDevs = NULL;
	IWbemClassObject*		pCOMDev = NULL;
	ULONG					uReturned = 0;
	BOOL					flag = FALSE;

	try
	{
		if (SUCCEEDED(CoCreateInstance(CLSID_WbemLocator, NULL, CLSCTX_INPROC_SERVER,
			IID_IWbemLocator, (LPVOID*)&pIWbemLocator)))
		{
			long securityFlag = 0;
			if (IsWindowsVersionOrGreaterFx(6, 0)) { securityFlag = WBEM_FLAG_CONNECT_USE_MAX_WAIT; }
			if (SUCCEEDED(pIWbemLocator->ConnectServer(_bstr_t(L"root\\cimv2"),
				NULL, NULL, 0L, securityFlag, NULL, NULL, &pIWbemServices)))
			{
				if (SUCCEEDED(CoSetProxyBlanket(pIWbemServices, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE,
					NULL, RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE)))
				{
					if (SUCCEEDED(pIWbemServices->ExecQuery(_bstr_t(L"WQL"),
						_bstr_t(query), WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, NULL, &pEnumCOMDevs)))
					{
						while (pEnumCOMDevs && SUCCEEDED(pEnumCOMDevs->Next(10000, 1, &pCOMDev, &uReturned)) && uReturned == 1)
						{
							CString name;
							UINT32 speed = 0;
							// UINT32 cores = 0;
							// UINT32 threads = 0;

							VARIANT pVal;
							VariantInit(&pVal);
							if (pCOMDev->Get(L"Name", 0L, &pVal, NULL, NULL) == WBEM_S_NO_ERROR && pVal.vt > VT_NULL)
							{
								name = pVal.bstrVal;
								name.TrimLeft();
								name.TrimRight();
								VariantClear(&pVal);
							}
							if (pCOMDev->Get(L"MaxClockSpeed", 0L, &pVal, NULL, NULL) == WBEM_S_NO_ERROR && pVal.vt > VT_NULL)
							{
								speed = pVal.intVal;
								VariantClear(&pVal);
							}
							if (pCOMDev->Get(L"NumberOfCores", 0L, &pVal, NULL, NULL) == WBEM_S_NO_ERROR && pVal.vt > VT_NULL)
							{
								*cores = pVal.intVal;
								VariantClear(&pVal);
							}
							if (pCOMDev->Get(L"NumberOfLogicalProcessors", 0L, &pVal, NULL, NULL) == WBEM_S_NO_ERROR && pVal.vt > VT_NULL)
							{
								*threads = pVal.intVal;
								VariantClear(&pVal);
							}

							if (speed > 0 && cores > 0 && threads > 0)
							{
								cpuInfo.Format(L"%s %dMHz (%d cores/%d threads)", name.GetString(), speed, *cores, *threads);
							}
							else
							{
								cpuInfo = name;
							}
						}
					}
				}
			}
		}
	}
	catch (...)
	{

	}

	SAFE_RELEASE(pCOMDev);
	SAFE_RELEASE(pEnumCOMDevs);
	SAFE_RELEASE(pIWbemServices);
	SAFE_RELEASE(pIWbemLocator);

	if (cpuInfo.IsEmpty())
	{
		TCHAR str[256];
		DWORD value = 0;
		DWORD type = REG_SZ;
		ULONG size = 256 * sizeof(TCHAR);
		HKEY  hKey = NULL;

		if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0", 0, KEY_READ, &hKey) == ERROR_SUCCESS)
		{
			if (RegQueryValueExW(hKey, L"ProcessorNameString", NULL, &type, (LPBYTE)str, &size) == ERROR_SUCCESS)
			{
				cpuInfo = str;
				cpuInfo.TrimLeft();
				cpuInfo.TrimRight();
			}
			type = REG_DWORD;
			size = sizeof(DWORD);
			if (RegQueryValueExW(hKey, L"~MHz", NULL, &type, (LPBYTE)&value, &size) == ERROR_SUCCESS)
			{
				cpuInfo.Format(L"%s %dMHz", str, value);
				cpuInfo.TrimLeft();
				cpuInfo.TrimRight();
			}
			RegCloseKey(hKey);
		}
	}
}

void GetGpuInfo(CString& gpuInfo)
{
	CString query = L"Select * from Win32_VideoController";

	IWbemLocator*			pIWbemLocator = NULL;
	IWbemServices*			pIWbemServices = NULL;
	IEnumWbemClassObject*	pEnumCOMDevs = NULL;
	IWbemClassObject*		pCOMDev = NULL;
	ULONG					uReturned = 0;
	BOOL					flag = FALSE;

	try
	{
		if (SUCCEEDED(CoCreateInstance(CLSID_WbemLocator, NULL, CLSCTX_INPROC_SERVER,
			IID_IWbemLocator, (LPVOID*)&pIWbemLocator)))
		{
			long securityFlag = 0;
			if (IsWindowsVersionOrGreaterFx(6, 0)) { securityFlag = WBEM_FLAG_CONNECT_USE_MAX_WAIT; }
			if (SUCCEEDED(pIWbemLocator->ConnectServer(_bstr_t(L"root\\cimv2"),
				NULL, NULL, 0L, securityFlag, NULL, NULL, &pIWbemServices)))
			{
				if (SUCCEEDED(CoSetProxyBlanket(pIWbemServices, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE,
					NULL, RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE)))
				{
					if (SUCCEEDED(pIWbemServices->ExecQuery(_bstr_t(L"WQL"),
						_bstr_t(query), WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, NULL, &pEnumCOMDevs)))
					{
						while (pEnumCOMDevs && SUCCEEDED(pEnumCOMDevs->Next(10000, 1, &pCOMDev, &uReturned)) && uReturned == 1)
						{
							CString name;

							VARIANT pVal;
							VariantInit(&pVal);
							if (pCOMDev->Get(L"Name", 0L, &pVal, NULL, NULL) == WBEM_S_NO_ERROR && pVal.vt > VT_NULL)
							{
								name = pVal.bstrVal;
								name.TrimLeft();
								name.TrimRight();
								VariantClear(&pVal);
							}
							gpuInfo = name;								
						}
					}
				}
			}
		}
	}
	catch (...)
	{

	}

	SAFE_RELEASE(pCOMDev);
	SAFE_RELEASE(pEnumCOMDevs);
	SAFE_RELEASE(pIWbemServices);
	SAFE_RELEASE(pIWbemLocator);

	if (gpuInfo.IsEmpty())
	{
		TCHAR str[256];
		DWORD value = 0;
		DWORD type = REG_SZ;
		ULONG size = 256 * sizeof(TCHAR);
		HKEY  hKey = NULL;

		// GPU
		if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"SYSTEM\\CurrentControlSet\\Control\\Class\\{4d36e968-e325-11ce-bfc1-08002be10318}\\0000", 0, KEY_READ, &hKey) == ERROR_SUCCESS)
		{
			type = REG_SZ;
			size = 256 * sizeof(TCHAR);
			if (RegQueryValueExW(hKey, L"DriverDesc", NULL, &type, (LPBYTE)str, &size) == ERROR_SUCCESS)
			{
				gpuInfo = str;
				gpuInfo.TrimLeft();
				gpuInfo.TrimRight();
			}
			RegCloseKey(hKey);
		}
		if (gpuInfo.IsEmpty())
		{
			if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"SYSTEM\\CurrentControlSet\\Control\\Class\\{4d36e968-e325-11ce-bfc1-08002be10318}\\0001", 0, KEY_READ, &hKey) == ERROR_SUCCESS)
			{
				type = REG_SZ;
				size = 256 * sizeof(TCHAR);
				if (RegQueryValueExW(hKey, L"DriverDesc", NULL, &type, (LPBYTE)str, &size) == ERROR_SUCCESS)
				{
					gpuInfo = str;
					gpuInfo.TrimLeft();
					gpuInfo.TrimRight();
				}
				RegCloseKey(hKey);
			}
		}
	}
}

void GetBaseBoardInfo(CString& baseBoardInfo)
{
	CString query = L"Select * from Win32_BaseBoard";

	IWbemLocator*			pIWbemLocator = NULL;
	IWbemServices*			pIWbemServices = NULL;
	IEnumWbemClassObject*	pEnumCOMDevs = NULL;
	IWbemClassObject*		pCOMDev = NULL;
	ULONG					uReturned = 0;
	BOOL					flag = FALSE;

	try
	{
		if (SUCCEEDED(CoCreateInstance(CLSID_WbemLocator, NULL, CLSCTX_INPROC_SERVER,
			IID_IWbemLocator, (LPVOID*)&pIWbemLocator)))
		{
			long securityFlag = 0;
			if (IsWindowsVersionOrGreaterFx(6, 0)) { securityFlag = WBEM_FLAG_CONNECT_USE_MAX_WAIT; }
			if (SUCCEEDED(pIWbemLocator->ConnectServer(_bstr_t(L"root\\cimv2"),
				NULL, NULL, 0L, securityFlag, NULL, NULL, &pIWbemServices)))
			{
				if (SUCCEEDED(CoSetProxyBlanket(pIWbemServices, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE,
					NULL, RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE)))
				{
					if (SUCCEEDED(pIWbemServices->ExecQuery(_bstr_t(L"WQL"),
						_bstr_t(query), WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, NULL, &pEnumCOMDevs)))
					{
						while (pEnumCOMDevs && SUCCEEDED(pEnumCOMDevs->Next(10000, 1, &pCOMDev, &uReturned)) && uReturned == 1)
						{
							CString manufacturer;
							CString product;

							VARIANT pVal;
							VariantInit(&pVal);
							if (pCOMDev->Get(L"Manufacturer", 0L, &pVal, NULL, NULL) == WBEM_S_NO_ERROR && pVal.vt > VT_NULL)
							{
								manufacturer = pVal.bstrVal;
								manufacturer.TrimLeft();
								manufacturer.TrimRight();
								VariantClear(&pVal);
							}
							if (pCOMDev->Get(L"Product", 0L, &pVal, NULL, NULL) == WBEM_S_NO_ERROR && pVal.vt > VT_NULL)
							{
								product = pVal.bstrVal;
								product.TrimLeft();
								product.TrimRight();
								VariantClear(&pVal);
							}
							baseBoardInfo = manufacturer + L" " + product;

							baseBoardInfo.Replace(L"To Be Filled By O.E.M.", L"");
							baseBoardInfo.Replace(L"To be filled by O.E.M.", L"");
							baseBoardInfo.Replace(L"Not Available", L"");
							baseBoardInfo.TrimLeft();
							baseBoardInfo.TrimRight();
						}
					}
				}
			}
		}
	}
	catch (...)
	{

	}

	SAFE_RELEASE(pCOMDev);
	SAFE_RELEASE(pEnumCOMDevs);
	SAFE_RELEASE(pIWbemServices);
	SAFE_RELEASE(pIWbemLocator);

	if (baseBoardInfo.IsEmpty())
	{
		TCHAR str[256];
		DWORD value = 0;
		DWORD type = REG_SZ;
		ULONG size = 256 * sizeof(TCHAR);
		HKEY  hKey = NULL;

		// BaseBoard/System
		if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"HARDWARE\\DESCRIPTION\\System\\BIOS", 0, KEY_READ, &hKey) == ERROR_SUCCESS)
		{
			size = 256 * sizeof(TCHAR);
			if (RegQueryValueExW(hKey, L"BaseBoardManufacturer", NULL, &type, (LPBYTE)str, &size) == ERROR_SUCCESS)
			{
				baseBoardInfo = str;

			}
			size = 256 * sizeof(TCHAR);
			if (RegQueryValueExW(hKey, L"BaseBoardProduct", NULL, &type, (LPBYTE)str, &size) == ERROR_SUCCESS)
			{
				baseBoardInfo += L" ";
				baseBoardInfo += str;
			}

			baseBoardInfo.Replace(L"To Be Filled By O.E.M.", L"");
			baseBoardInfo.Replace(L"To be filled by O.E.M.", L"");
			baseBoardInfo.Replace(L"Not Available", L"");
			baseBoardInfo.TrimLeft();
			baseBoardInfo.TrimRight();

			RegCloseKey(hKey);
		}
	}
}

void GetComputerSystemInfo(CString& computerSystemInfo)
{
	CString query = L"Select * from Win32_ComputerSystem";

	IWbemLocator*			pIWbemLocator = NULL;
	IWbemServices*			pIWbemServices = NULL;
	IEnumWbemClassObject*	pEnumCOMDevs = NULL;
	IWbemClassObject*		pCOMDev = NULL;
	ULONG					uReturned = 0;
	BOOL					flag = FALSE;

	try
	{
		if (SUCCEEDED(CoCreateInstance(CLSID_WbemLocator, NULL, CLSCTX_INPROC_SERVER,
			IID_IWbemLocator, (LPVOID*)&pIWbemLocator)))
		{
			long securityFlag = 0;
			if (IsWindowsVersionOrGreaterFx(6, 0)) { securityFlag = WBEM_FLAG_CONNECT_USE_MAX_WAIT; }
			if (SUCCEEDED(pIWbemLocator->ConnectServer(_bstr_t(L"root\\cimv2"),
				NULL, NULL, 0L, securityFlag, NULL, NULL, &pIWbemServices)))
			{
				if (SUCCEEDED(CoSetProxyBlanket(pIWbemServices, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE,
					NULL, RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE)))
				{
					if (SUCCEEDED(pIWbemServices->ExecQuery(_bstr_t(L"WQL"),
						_bstr_t(query), WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, NULL, &pEnumCOMDevs)))
					{
						while (pEnumCOMDevs && SUCCEEDED(pEnumCOMDevs->Next(10000, 1, &pCOMDev, &uReturned)) && uReturned == 1)
						{
							CString manufacturer;
							CString model;

							VARIANT pVal;
							VariantInit(&pVal);
							if (pCOMDev->Get(L"Manufacturer", 0L, &pVal, NULL, NULL) == WBEM_S_NO_ERROR && pVal.vt > VT_NULL)
							{
								manufacturer = pVal.bstrVal;
								manufacturer.TrimLeft();
								manufacturer.TrimRight();
								VariantClear(&pVal);
							}
							if (pCOMDev->Get(L"Model", 0L, &pVal, NULL, NULL) == WBEM_S_NO_ERROR && pVal.vt > VT_NULL)
							{
								model = pVal.bstrVal;
								model.TrimLeft();
								model.TrimRight();
								VariantClear(&pVal);
							}
							computerSystemInfo = manufacturer + L" " + model;

							computerSystemInfo.Replace(L"To Be Filled By O.E.M.", L"");
							computerSystemInfo.Replace(L"To be filled by O.E.M.", L"");
							computerSystemInfo.Replace(L"Not Available", L"");
							computerSystemInfo.TrimLeft();
							computerSystemInfo.TrimRight();
						}
					}
				}
			}
		}
	}
	catch (...)
	{

	}

	SAFE_RELEASE(pCOMDev);
	SAFE_RELEASE(pEnumCOMDevs);
	SAFE_RELEASE(pIWbemServices);
	SAFE_RELEASE(pIWbemLocator);

	if (computerSystemInfo.IsEmpty())
	{
		TCHAR str[256];
		DWORD value = 0;
		DWORD type = REG_SZ;
		ULONG size = 256 * sizeof(TCHAR);
		HKEY  hKey = NULL;

		// BaseBoard/System
		if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"HARDWARE\\DESCRIPTION\\System\\BIOS", 0, KEY_READ, &hKey) == ERROR_SUCCESS)
		{
			size = 256 * sizeof(TCHAR);
			if (RegQueryValueExW(hKey, L"SystemManufacturer", NULL, &type, (LPBYTE)str, &size) == ERROR_SUCCESS)
			{
				computerSystemInfo = str;
			}
			size = 256 * sizeof(TCHAR);
			if (RegQueryValueExW(hKey, L"SystemProductName", NULL, &type, (LPBYTE)str, &size) == ERROR_SUCCESS)
			{
				computerSystemInfo += L" ";
				computerSystemInfo += str;
			}

			computerSystemInfo.Replace(L"To Be Filled By O.E.M.", L"");
			computerSystemInfo.Replace(L"To be filled by O.E.M.", L"");
			computerSystemInfo.Replace(L"Not Available", L"");
			computerSystemInfo.Replace(L"System Product Name", L"");
			computerSystemInfo.Replace(L"System manufacturer", L"");
			computerSystemInfo.TrimLeft();
			computerSystemInfo.TrimRight();

			RegCloseKey(hKey);
		}
	}
}

void GetScreenInfo(CString& screenInfo)
{
	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);

	HDC hDC = ::GetDC(NULL);
	int bitsPerPixel = ::GetDeviceCaps(hDC, BITSPIXEL);
	int planes = ::GetDeviceCaps(hDC, PLANES);
	int colorDepth = bitsPerPixel * planes;
	::ReleaseDC(NULL, hDC);

	DWORD fontSmoothingType = 0;
	SystemParametersInfoW(SPI_GETFONTSMOOTHINGTYPE, 0, &fontSmoothingType, 0);

	screenInfo.Format(L"%dx%d %dbit", screenWidth, screenHeight, colorDepth);
	if (fontSmoothingType == FE_FONTSMOOTHINGSTANDARD)
	{
		screenInfo += L" (Smoothing)";
	}
	else if (fontSmoothingType == FE_FONTSMOOTHINGCLEARTYPE)
	{
		screenInfo += L" (ClearType)";
	}
}