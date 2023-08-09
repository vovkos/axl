//..............................................................................
//
//  This file is part of the AXL library.
//
//  AXL is distributed under the MIT license.
//  For details see accompanying license.txt file,
//  the public copy of which is also available at:
//  http://tibbo.com/downloads/archive/axl/license.txt
//
//..............................................................................

#include "pch.h"
#include "axl_io_HidUsage.h"
#include "axl_sl_CallOnce.h"

namespace axl {
namespace io {

//..............................................................................

class InitHidUsagePageStringTable {
public:
	void
	operator () (const char* stringTable[]) const {
		for (size_t i = 0; i < 256; i++)
			stringTable[i] = "Unknown";

		stringTable[HidUsagePage_Undefined]               = "Undefined";
		stringTable[HidUsagePage_GenericDesktop]          = "GenericDesktop";
		stringTable[HidUsagePage_SimulationControls]      = "SimulationControls";
		stringTable[HidUsagePage_VrControls]              = "VrControls";
		stringTable[HidUsagePage_SportControls]           = "SportControls";
		stringTable[HidUsagePage_GameControls]            = "GameControls";
		stringTable[HidUsagePage_GenericDeviceControls]   = "GenericDeviceControls";
		stringTable[HidUsagePage_Keyboard]                = "Keyboard";
		stringTable[HidUsagePage_Led]                     = "Led";
		stringTable[HidUsagePage_Button]                  = "Button";
		stringTable[HidUsagePage_Ordinal]                 = "Ordinal";
		stringTable[HidUsagePage_TelephonyDevice]         = "TelephonyDevice";
		stringTable[HidUsagePage_Consumer]                = "Consumer";
		stringTable[HidUsagePage_Digitizers]              = "Digitizers";
		stringTable[HidUsagePage_Haptics]                 = "Haptics";
		stringTable[HidUsagePage_PhysicalInputDevice]     = "PhysicalInputDevice";
		stringTable[HidUsagePage_Unicode]                 = "Unicode";
		stringTable[HidUsagePage_SoC]                     = "SoC";
		stringTable[HidUsagePage_EyeHeadTrackers]         = "EyeHeadTrackers";
		stringTable[HidUsagePage_AuxiliaryDisplay]        = "AuxiliaryDisplay";
		stringTable[HidUsagePage_Sensors]                 = "Sensors";
		stringTable[HidUsagePage_MedicalInstrument]       = "MedicalInstrument";
		stringTable[HidUsagePage_BrailleDisplay]          = "BrailleDisplay";
		stringTable[HidUsagePage_LightingAndIllumination] = "LightingAndIllumination";
		stringTable[HidUsagePage_Monitor]                 = "Monitor";
		stringTable[HidUsagePage_MonitorEnumerated]       = "MonitorEnumerated";
		stringTable[HidUsagePage_VesaVirtualControls]     = "VesaVirtualControls";
		stringTable[HidUsagePage_Power]                   = "Power";
		stringTable[HidUsagePage_BatterySystem]           = "BatterySystem";
		stringTable[HidUsagePage_BarcodeScanner]          = "BarcodeScanner";
		stringTable[HidUsagePage_Scales]                  = "Scales";
		stringTable[HidUsagePage_MagneticStripeReader]    = "MagneticStripeReader";
		stringTable[HidUsagePage_CameraControl]           = "CameraControl";
		stringTable[HidUsagePage_Arcade]                  = "Arcade";
		stringTable[HidUsagePage_GamingDevice]            = "GamingDevice";
	}
};

const char*
getHidUsagePageString(HidUsagePage page) {
	static const char* stringTable[256] = { 0 };
	sl::callOnce(InitHidUsagePageStringTable(), stringTable);
	return
		page == HidUsagePage_FidoAlliance ? "FidoAlliance" :
		(size_t)page < 256 ? stringTable[page] : "Unknown";
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class InitGenericDesktopUsageIdStringTable {
public:
	void
	operator () (const char* stringTable[]) const {
		for (size_t i = 0; i < 256; i++)
			stringTable[i] = "Unknown";

		stringTable[HidGenericDesktopUsageId_Mouse]                                 = "Mouse";
		stringTable[HidGenericDesktopUsageId_Joystick]                              = "Joystick";
		stringTable[HidGenericDesktopUsageId_Gamepad]                               = "Gamepad";
		stringTable[HidGenericDesktopUsageId_Keyboard]                              = "Keyboard";
		stringTable[HidGenericDesktopUsageId_Keypad]                                = "Keypad";
		stringTable[HidGenericDesktopUsageId_MultiAxisController]                   = "MultiAxisController";
		stringTable[HidGenericDesktopUsageId_TabletPcSystemControls]                = "TabletPcSystemControls";
		stringTable[HidGenericDesktopUsageId_WaterCoolingDevice]                    = "WaterCoolingDevice";
		stringTable[HidGenericDesktopUsageId_ComputerChassisDevice]                 = "ComputerChassisDevice";
		stringTable[HidGenericDesktopUsageId_WirelessRadioControls]                 = "WirelessRadioControls";
		stringTable[HidGenericDesktopUsageId_PortableDeviceControl]                 = "PortableDeviceControl";
		stringTable[HidGenericDesktopUsageId_SystemMultiAxisController]             = "SystemMultiAxisController";
		stringTable[HidGenericDesktopUsageId_SpatialController]                     = "SpatialController";
		stringTable[HidGenericDesktopUsageId_AssistiveControl]                      = "AssistiveControl";
		stringTable[HidGenericDesktopUsageId_DeviceDock]                            = "DeviceDock";
		stringTable[HidGenericDesktopUsageId_DockableDevice]                        = "DockableDevice";
		stringTable[HidGenericDesktopUsageId_CallStateManagementControl]            = "CallStateManagementControl";
		stringTable[HidGenericDesktopUsageId_X]                                     = "X";
		stringTable[HidGenericDesktopUsageId_Y]                                     = "Y";
		stringTable[HidGenericDesktopUsageId_Z]                                     = "Z";
		stringTable[HidGenericDesktopUsageId_Rx]                                    = "Rx";
		stringTable[HidGenericDesktopUsageId_Ry]                                    = "Ry";
		stringTable[HidGenericDesktopUsageId_Rz]                                    = "Rz";
		stringTable[HidGenericDesktopUsageId_Slider]                                = "Slider";
		stringTable[HidGenericDesktopUsageId_Dial]                                  = "Dial";
		stringTable[HidGenericDesktopUsageId_Wheel]                                 = "Wheel";
		stringTable[HidGenericDesktopUsageId_HatSwitch]                             = "HatSwitch";
		stringTable[HidGenericDesktopUsageId_CountedBuffer]                         = "CountedBuffer";
		stringTable[HidGenericDesktopUsageId_ByteCount]                             = "ByteCount";
		stringTable[HidGenericDesktopUsageId_MotionWakeup]                          = "MotionWakeup";
		stringTable[HidGenericDesktopUsageId_Start]                                 = "Start";
		stringTable[HidGenericDesktopUsageId_Select]                                = "Select";
		stringTable[HidGenericDesktopUsageId_Vx]                                    = "Vx";
		stringTable[HidGenericDesktopUsageId_Vy]                                    = "Vy";
		stringTable[HidGenericDesktopUsageId_Vz]                                    = "Vz";
		stringTable[HidGenericDesktopUsageId_Vbrx]                                  = "Vbrx";
		stringTable[HidGenericDesktopUsageId_Vbry]                                  = "Vbry";
		stringTable[HidGenericDesktopUsageId_Vbrz]                                  = "Vbrz";
		stringTable[HidGenericDesktopUsageId_Vno]                                   = "Vno";
		stringTable[HidGenericDesktopUsageId_FeatureNotification]                   = "FeatureNotification";
		stringTable[HidGenericDesktopUsageId_ResolutionMultiplier]                  = "ResolutionMultiplier";
		stringTable[HidGenericDesktopUsageId_Qx]                                    = "Qx";
		stringTable[HidGenericDesktopUsageId_Qy]                                    = "Qy";
		stringTable[HidGenericDesktopUsageId_Qz]                                    = "Qz";
		stringTable[HidGenericDesktopUsageId_Qw]                                    = "Qw";
		stringTable[HidGenericDesktopUsageId_SystemControl]                         = "SystemControl";
		stringTable[HidGenericDesktopUsageId_SystemPowerDown]                       = "SystemPowerDown";
		stringTable[HidGenericDesktopUsageId_SystemSleep]                           = "SystemSleep";
		stringTable[HidGenericDesktopUsageId_SystemWakeUp]                          = "SystemWakeUp";
		stringTable[HidGenericDesktopUsageId_SystemContextMenu]                     = "SystemContextMenu";
		stringTable[HidGenericDesktopUsageId_SystemMainMenu]                        = "SystemMainMenu";
		stringTable[HidGenericDesktopUsageId_SystemAppMenu]                         = "SystemAppMenu";
		stringTable[HidGenericDesktopUsageId_SystemMenuHelp]                        = "SystemMenuHelp";
		stringTable[HidGenericDesktopUsageId_SystemMenuExit]                        = "SystemMenuExit";
		stringTable[HidGenericDesktopUsageId_SystemMenuSelect]                      = "SystemMenuSelect";
		stringTable[HidGenericDesktopUsageId_SystemMenuRight]                       = "SystemMenuRight";
		stringTable[HidGenericDesktopUsageId_SystemMenuLeft]                        = "SystemMenuLeft";
		stringTable[HidGenericDesktopUsageId_SystemMenuUp]                          = "SystemMenuUp";
		stringTable[HidGenericDesktopUsageId_SystemMenuDown]                        = "SystemMenuDown";
		stringTable[HidGenericDesktopUsageId_SystemColdRestart]                     = "SystemColdRestart";
		stringTable[HidGenericDesktopUsageId_SystemWarmRestart]                     = "SystemWarmRestart";
		stringTable[HidGenericDesktopUsageId_DpadUp]                                = "DpadUp";
		stringTable[HidGenericDesktopUsageId_DpadDown]                              = "DpadDown";
		stringTable[HidGenericDesktopUsageId_DpadRight]                             = "DpadRight";
		stringTable[HidGenericDesktopUsageId_DpadLeft]                              = "DpadLeft";
		stringTable[HidGenericDesktopUsageId_IndexTrigger]                          = "IndexTrigger";
		stringTable[HidGenericDesktopUsageId_PalmTrigger]                           = "PalmTrigger";
		stringTable[HidGenericDesktopUsageId_Thumbstick]                            = "Thumbstick";
		stringTable[HidGenericDesktopUsageId_SystemFunctionShift]                   = "SystemFunctionShift";
		stringTable[HidGenericDesktopUsageId_SystemFunctionShiftLock]               = "SystemFunctionShiftLock";
		stringTable[HidGenericDesktopUsageId_SystemFunctionShiftLockIndicator]      = "SystemFunctionShiftLockIndicator";
		stringTable[HidGenericDesktopUsageId_SystemDismissNotification]             = "SystemDismissNotification";
		stringTable[HidGenericDesktopUsageId_SystemDoNotDisturb]                    = "SystemDoNotDisturb";
		stringTable[HidGenericDesktopUsageId_SystemDock]                            = "SystemDock";
		stringTable[HidGenericDesktopUsageId_SystemUndock]                          = "SystemUndock";
		stringTable[HidGenericDesktopUsageId_SystemSetup]                           = "SystemSetup";
		stringTable[HidGenericDesktopUsageId_SystemBreak]                           = "SystemBreak";
		stringTable[HidGenericDesktopUsageId_SystemDebuggerBreak]                   = "SystemDebuggerBreak";
		stringTable[HidGenericDesktopUsageId_ApplicationBreak]                      = "ApplicationBreak";
		stringTable[HidGenericDesktopUsageId_ApplicationDebuggerBreak]              = "ApplicationDebuggerBreak";
		stringTable[HidGenericDesktopUsageId_SystemSpeakerMute]                     = "SystemSpeakerMute";
		stringTable[HidGenericDesktopUsageId_SystemHibernate]                       = "SystemHibernate";
		stringTable[HidGenericDesktopUsageId_SystemMicrophoneMute]                  = "SystemMicrophoneMute";
		stringTable[HidGenericDesktopUsageId_SystemDisplayInvert]                   = "SystemDisplayInvert";
		stringTable[HidGenericDesktopUsageId_SystemDisplayInternal]                 = "SystemDisplayInternal";
		stringTable[HidGenericDesktopUsageId_SystemDisplayExternal]                 = "SystemDisplayExternal";
		stringTable[HidGenericDesktopUsageId_SystemDisplayBoth]                     = "SystemDisplayBoth";
		stringTable[HidGenericDesktopUsageId_SystemDisplayDual]                     = "SystemDisplayDual";
		stringTable[HidGenericDesktopUsageId_SystemDisplayToggleIntExtMode]         = "SystemDisplayToggleIntExtMode";
		stringTable[HidGenericDesktopUsageId_SystemDisplaySwapPrimarySecondary]     = "SystemDisplaySwapPrimarySecondary";
		stringTable[HidGenericDesktopUsageId_SystemDisplayToggleLcdAutoscale]       = "SystemDisplayToggleLcdAutoscale";
		stringTable[HidGenericDesktopUsageId_SensorZone]                            = "SensorZone";
		stringTable[HidGenericDesktopUsageId_Rpm]                                   = "Rpm";
		stringTable[HidGenericDesktopUsageId_CoolantLevel]                          = "CoolantLevel";
		stringTable[HidGenericDesktopUsageId_CoolantCriticalLevel]                  = "CoolantCriticalLevel";
		stringTable[HidGenericDesktopUsageId_CoolantPump]                           = "CoolantPump";
		stringTable[HidGenericDesktopUsageId_ChassisEnclosure]                      = "ChassisEnclosure";
		stringTable[HidGenericDesktopUsageId_WirelessRadioButton]                   = "WirelessRadioButton";
		stringTable[HidGenericDesktopUsageId_WirelessRadioLed]                      = "WirelessRadioLed";
		stringTable[HidGenericDesktopUsageId_WirelessRadioSliderSwitch]             = "WirelessRadioSliderSwitch";
		stringTable[HidGenericDesktopUsageId_SystemDisplayRotationLockButton]       = "SystemDisplayRotationLockButton";
		stringTable[HidGenericDesktopUsageId_SystemDisplayRotationLockSliderSwitch] = "SystemDisplayRotationLockSliderSwitch";
		stringTable[HidGenericDesktopUsageId_ControlEnable]                         = "ControlEnable";
		stringTable[HidGenericDesktopUsageId_DockableDeviceUniqueID]                = "DockableDeviceUniqueID";
		stringTable[HidGenericDesktopUsageId_DockableDeviceVendorID]                = "DockableDeviceVendorID";
		stringTable[HidGenericDesktopUsageId_DockableDevicePrimaryUsagePage]        = "DockableDevicePrimaryUsagePage";
		stringTable[HidGenericDesktopUsageId_DockableDevicePrimaryUsageId]          = "DockableDevicePrimaryUsageId";
		stringTable[HidGenericDesktopUsageId_DockableDeviceDockingState]            = "DockableDeviceDockingState";
		stringTable[HidGenericDesktopUsageId_DockableDeviceDisplayOcclusion]        = "DockableDeviceDisplayOcclusion";
		stringTable[HidGenericDesktopUsageId_DockableDeviceObjectType]              = "DockableDeviceObjectType";
		stringTable[HidGenericDesktopUsageId_CallActiveLed]                         = "CallActiveLed";
		stringTable[HidGenericDesktopUsageId_CallMuteToggle]                        = "CallMuteToggle";
		stringTable[HidGenericDesktopUsageId_CallMuteLed]                           = "CallMuteLed";
	}
};

const char*
getHidGenericDesktopUsageIdString(HidGenericDesktopUsageId id) {
	static const char* stringTable[256] = { 0 };
	sl::callOnce(InitGenericDesktopUsageIdStringTable(), stringTable);
	return (size_t)id < 256 ? stringTable[id] : "Unknown";
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class InitHidKeboardUsageIdStringTable {
public:
	void
	operator () (const char* stringTable[]) const {
		for (size_t i = 0; i < 256; i++)
			stringTable[i] = "Unknown";

		stringTable[HidKeboardUsageId_ErrorRollOver]          = "ErrorRollOver";
		stringTable[HidKeboardUsageId_PostFail]               = "PostFail";
		stringTable[HidKeboardUsageId_ErrorUndefined]         = "ErrorUndefined";
		stringTable[HidKeboardUsageId_A]                      = "A";
		stringTable[HidKeboardUsageId_B]                      = "B";
		stringTable[HidKeboardUsageId_C]                      = "C";
		stringTable[HidKeboardUsageId_D]                      = "D";
		stringTable[HidKeboardUsageId_E]                      = "E";
		stringTable[HidKeboardUsageId_F]                      = "F";
		stringTable[HidKeboardUsageId_G]                      = "G";
		stringTable[HidKeboardUsageId_H]                      = "H";
		stringTable[HidKeboardUsageId_I]                      = "I";
		stringTable[HidKeboardUsageId_J]                      = "J";
		stringTable[HidKeboardUsageId_K]                      = "K";
		stringTable[HidKeboardUsageId_L]                      = "L";
		stringTable[HidKeboardUsageId_M]                      = "M";
		stringTable[HidKeboardUsageId_N]                      = "N";
		stringTable[HidKeboardUsageId_O]                      = "O";
		stringTable[HidKeboardUsageId_P]                      = "P";
		stringTable[HidKeboardUsageId_Q]                      = "Q";
		stringTable[HidKeboardUsageId_R]                      = "R";
		stringTable[HidKeboardUsageId_S]                      = "S";
		stringTable[HidKeboardUsageId_T]                      = "T";
		stringTable[HidKeboardUsageId_U]                      = "U";
		stringTable[HidKeboardUsageId_V]                      = "V";
		stringTable[HidKeboardUsageId_W]                      = "W";
		stringTable[HidKeboardUsageId_X]                      = "X";
		stringTable[HidKeboardUsageId_Y]                      = "Y";
		stringTable[HidKeboardUsageId_Z]                      = "Z";
		stringTable[HidKeboardUsageId_1]                      = "1";
		stringTable[HidKeboardUsageId_2]                      = "2";
		stringTable[HidKeboardUsageId_3]                      = "3";
		stringTable[HidKeboardUsageId_4]                      = "4";
		stringTable[HidKeboardUsageId_5]                      = "5";
		stringTable[HidKeboardUsageId_6]                      = "6";
		stringTable[HidKeboardUsageId_7]                      = "7";
		stringTable[HidKeboardUsageId_8]                      = "8";
		stringTable[HidKeboardUsageId_9]                      = "9";
		stringTable[HidKeboardUsageId_0]                      = "0";
		stringTable[HidKeboardUsageId_Enter]                  = "Enter";
		stringTable[HidKeboardUsageId_Escape]                 = "Escape";
		stringTable[HidKeboardUsageId_Backspace]              = "Backspace";
		stringTable[HidKeboardUsageId_Tab]                    = "Tab";
		stringTable[HidKeboardUsageId_Spacebar]               = "Spacebar";
		stringTable[HidKeboardUsageId_Minus]                  = "Minus";
		stringTable[HidKeboardUsageId_Equal]                  = "Equal";
		stringTable[HidKeboardUsageId_LeftBracket]            = "LeftBracket";
		stringTable[HidKeboardUsageId_RightBracket]           = "RightBracket";
		stringTable[HidKeboardUsageId_Backslash]              = "Backslash";
		stringTable[HidKeboardUsageId_NonUsHash]              = "NonUsHash";
		stringTable[HidKeboardUsageId_Semicolon]              = "Semicolon";
		stringTable[HidKeboardUsageId_Apostrophe]             = "Apostrophe";
		stringTable[HidKeboardUsageId_GraveAccent]            = "GraveAccent";
		stringTable[HidKeboardUsageId_Comma]                  = "Comma";
		stringTable[HidKeboardUsageId_Dot]                    = "Dot";
		stringTable[HidKeboardUsageId_Div]                    = "Div";
		stringTable[HidKeboardUsageId_CapsLock]               = "CapsLock";
		stringTable[HidKeboardUsageId_F1]                     = "F1";
		stringTable[HidKeboardUsageId_F2]                     = "F2";
		stringTable[HidKeboardUsageId_F3]                     = "F3";
		stringTable[HidKeboardUsageId_F4]                     = "F4";
		stringTable[HidKeboardUsageId_F5]                     = "F5";
		stringTable[HidKeboardUsageId_F6]                     = "F6";
		stringTable[HidKeboardUsageId_F7]                     = "F7";
		stringTable[HidKeboardUsageId_F8]                     = "F8";
		stringTable[HidKeboardUsageId_F9]                     = "F9";
		stringTable[HidKeboardUsageId_F10]                    = "F10";
		stringTable[HidKeboardUsageId_F11]                    = "F11";
		stringTable[HidKeboardUsageId_F12]                    = "F12";
		stringTable[HidKeboardUsageId_PrintScreen]            = "PrintScreen";
		stringTable[HidKeboardUsageId_ScrollLock]             = "ScrollLock";
		stringTable[HidKeboardUsageId_Pause]                  = "Pause";
		stringTable[HidKeboardUsageId_Insert]                 = "Insert";
		stringTable[HidKeboardUsageId_Home]                   = "Home";
		stringTable[HidKeboardUsageId_PageUp]                 = "PageUp";
		stringTable[HidKeboardUsageId_Delete]                 = "Delete";
		stringTable[HidKeboardUsageId_End]                    = "End";
		stringTable[HidKeboardUsageId_PageDown]               = "PageDown";
		stringTable[HidKeboardUsageId_RightArrow]             = "RightArrow";
		stringTable[HidKeboardUsageId_LeftArrow]              = "LeftArrow";
		stringTable[HidKeboardUsageId_DownArrow]              = "DownArrow";
		stringTable[HidKeboardUsageId_UpArrow]                = "UpArrow";
		stringTable[HidKeboardUsageId_KeypadNumLock]          = "KeypadNumLock";
		stringTable[HidKeboardUsageId_KeypadDiv]              = "KeypadDiv";
		stringTable[HidKeboardUsageId_KeypadMul]              = "KeypadMul";
		stringTable[HidKeboardUsageId_KeypadMinus]            = "KeypadMinus";
		stringTable[HidKeboardUsageId_KeypadPlus]             = "KeypadPlus";
		stringTable[HidKeboardUsageId_KeypadEnter]            = "KeypadEnter";
		stringTable[HidKeboardUsageId_Keypad1]                = "Keypad1";
		stringTable[HidKeboardUsageId_Keypad2]                = "Keypad2";
		stringTable[HidKeboardUsageId_Keypad3]                = "Keypad3";
		stringTable[HidKeboardUsageId_Keypad4]                = "Keypad4";
		stringTable[HidKeboardUsageId_Keypad5]                = "Keypad5";
		stringTable[HidKeboardUsageId_Keypad6]                = "Keypad6";
		stringTable[HidKeboardUsageId_Keypad7]                = "Keypad7";
		stringTable[HidKeboardUsageId_Keypad8]                = "Keypad8";
		stringTable[HidKeboardUsageId_Keypad9]                = "Keypad9";
		stringTable[HidKeboardUsageId_Keypad0]                = "Keypad0";
		stringTable[HidKeboardUsageId_KeypadDot]              = "KeypadDot";
		stringTable[HidKeboardUsageId_NonUsBackslash]         = "NonUsBackslash";
		stringTable[HidKeboardUsageId_Application]            = "Application";
		stringTable[HidKeboardUsageId_Power]                  = "Power";
		stringTable[HidKeboardUsageId_KeypadEqual]            = "KeypadEqual";
		stringTable[HidKeboardUsageId_F13]                    = "F13";
		stringTable[HidKeboardUsageId_F14]                    = "F14";
		stringTable[HidKeboardUsageId_F15]                    = "F15";
		stringTable[HidKeboardUsageId_F16]                    = "F16";
		stringTable[HidKeboardUsageId_F17]                    = "F17";
		stringTable[HidKeboardUsageId_F18]                    = "F18";
		stringTable[HidKeboardUsageId_F19]                    = "F19";
		stringTable[HidKeboardUsageId_F20]                    = "F20";
		stringTable[HidKeboardUsageId_F21]                    = "F21";
		stringTable[HidKeboardUsageId_F22]                    = "F22";
		stringTable[HidKeboardUsageId_F23]                    = "F23";
		stringTable[HidKeboardUsageId_F24]                    = "F24";
		stringTable[HidKeboardUsageId_Execute]                = "Execute";
		stringTable[HidKeboardUsageId_Help]                   = "Help";
		stringTable[HidKeboardUsageId_Menu]                   = "Menu";
		stringTable[HidKeboardUsageId_Select]                 = "Select";
		stringTable[HidKeboardUsageId_Stop]                   = "Stop";
		stringTable[HidKeboardUsageId_Again]                  = "Again";
		stringTable[HidKeboardUsageId_Undo]                   = "Undo";
		stringTable[HidKeboardUsageId_Cut]                    = "Cut";
		stringTable[HidKeboardUsageId_Copy]                   = "Copy";
		stringTable[HidKeboardUsageId_Paste]                  = "Paste";
		stringTable[HidKeboardUsageId_Find]                   = "Find";
		stringTable[HidKeboardUsageId_Mute]                   = "Mute";
		stringTable[HidKeboardUsageId_VolumeUp]               = "VolumeUp";
		stringTable[HidKeboardUsageId_VolumeDown]             = "VolumeDown";
		stringTable[HidKeboardUsageId_LockingCapsLock]        = "LockingCapsLock";
		stringTable[HidKeboardUsageId_LockingNumLock]         = "LockingNumLock";
		stringTable[HidKeboardUsageId_LockingScrollLock]      = "LockingScrollLock";
		stringTable[HidKeboardUsageId_KeypadComma]            = "KeypadComma";
		stringTable[HidKeboardUsageId_KeypadEqualSign]        = "KeypadEqualSign";
		stringTable[HidKeboardUsageId_International1]         = "International1";
		stringTable[HidKeboardUsageId_International2]         = "International2";
		stringTable[HidKeboardUsageId_International3]         = "International3";
		stringTable[HidKeboardUsageId_International4]         = "International4";
		stringTable[HidKeboardUsageId_International5]         = "International5";
		stringTable[HidKeboardUsageId_International6]         = "International6";
		stringTable[HidKeboardUsageId_International7]         = "International7";
		stringTable[HidKeboardUsageId_International8]         = "International8";
		stringTable[HidKeboardUsageId_International9]         = "International9";
		stringTable[HidKeboardUsageId_Lang1]                  = "Lang1";
		stringTable[HidKeboardUsageId_Lang2]                  = "Lang2";
		stringTable[HidKeboardUsageId_Lang3]                  = "Lang3";
		stringTable[HidKeboardUsageId_Lang4]                  = "Lang4";
		stringTable[HidKeboardUsageId_Lang5]                  = "Lang5";
		stringTable[HidKeboardUsageId_Lang6]                  = "Lang6";
		stringTable[HidKeboardUsageId_Lang7]                  = "Lang7";
		stringTable[HidKeboardUsageId_Lang8]                  = "Lang8";
		stringTable[HidKeboardUsageId_Lang9]                  = "Lang9";
		stringTable[HidKeboardUsageId_AlternateErase]         = "AlternateErase";
		stringTable[HidKeboardUsageId_SysReq]                 = "SysReq";
		stringTable[HidKeboardUsageId_Cancel]                 = "Cancel";
		stringTable[HidKeboardUsageId_Clear]                  = "Clear";
		stringTable[HidKeboardUsageId_Prior]                  = "Prior";
		stringTable[HidKeboardUsageId_Return]                 = "Return";
		stringTable[HidKeboardUsageId_Separator]              = "Separator";
		stringTable[HidKeboardUsageId_Out]                    = "Out";
		stringTable[HidKeboardUsageId_Oper]                   = "Oper";
		stringTable[HidKeboardUsageId_ClearAgain]             = "Clear/Again";
		stringTable[HidKeboardUsageId_CrSelProps]             = "CrSel/Props";
		stringTable[HidKeboardUsageId_ExSel]                  = "ExSel";
		stringTable[HidKeboardUsageId_Keypad00]               = "Keypad00";
		stringTable[HidKeboardUsageId_Keypad000]              = "Keypad000";
		stringTable[HidKeboardUsageId_ThousandsSeparator]     = "ThousandsSeparator";
		stringTable[HidKeboardUsageId_DecimalSeparator]       = "DecimalSeparator";
		stringTable[HidKeboardUsageId_CurrencyUnit]           = "CurrencyUnit";
		stringTable[HidKeboardUsageId_CurrencySubUnit]        = "CurrencySubUnit";
		stringTable[HidKeboardUsageId_KeypadLeftParenthesis]  = "KeypadLeftParenthesis";
		stringTable[HidKeboardUsageId_KeypadRightParenthesis] = "KeypadRightParenthesis";
		stringTable[HidKeboardUsageId_KeypadLeftBrace]        = "KeypadLeftBrace";
		stringTable[HidKeboardUsageId_KeypadRightBrace]       = "KeypadRightBrace";
		stringTable[HidKeboardUsageId_KeypadTab]              = "KeypadTab";
		stringTable[HidKeboardUsageId_KeypadBackspace]        = "KeypadBackspace";
		stringTable[HidKeboardUsageId_KeypadA]                = "KeypadA";
		stringTable[HidKeboardUsageId_KeypadB]                = "KeypadB";
		stringTable[HidKeboardUsageId_KeypadC]                = "KeypadC";
		stringTable[HidKeboardUsageId_KeypadD]                = "KeypadD";
		stringTable[HidKeboardUsageId_KeypadE]                = "KeypadE";
		stringTable[HidKeboardUsageId_KeypadF]                = "KeypadF";
		stringTable[HidKeboardUsageId_KeypadXor]              = "KeypadXor";
		stringTable[HidKeboardUsageId_KeypadCaret]            = "KeypadCaret";
		stringTable[HidKeboardUsageId_KeypadMod]              = "KeypadMod";
		stringTable[HidKeboardUsageId_KeypadLt]               = "KeypadLt";
		stringTable[HidKeboardUsageId_KeypadGt]               = "KeypadGt";
		stringTable[HidKeboardUsageId_KeypadAmpersand]        = "KeypadAmpersand";
		stringTable[HidKeboardUsageId_KeypadAnd]              = "KeypadAnd";
		stringTable[HidKeboardUsageId_KeypadPipe]             = "KeypadPipe";
		stringTable[HidKeboardUsageId_KeypadOr]               = "KeypadOr";
		stringTable[HidKeboardUsageId_KeypadColon]            = "KeypadColon";
		stringTable[HidKeboardUsageId_KeypadHash]             = "KeypadHash";
		stringTable[HidKeboardUsageId_KeypadSpace]            = "KeypadSpace";
		stringTable[HidKeboardUsageId_KeypadAt]               = "KeypadAt";
		stringTable[HidKeboardUsageId_KeypadExclamation]      = "KeypadExclamation";
		stringTable[HidKeboardUsageId_KeypadMemoryStore]      = "KeypadMemoryStore";
		stringTable[HidKeboardUsageId_KeypadMemoryRecall]     = "KeypadMemoryRecall";
		stringTable[HidKeboardUsageId_KeypadMemoryClear]      = "KeypadMemoryClear";
		stringTable[HidKeboardUsageId_KeypadMemoryAdd]        = "KeypadMemoryAdd";
		stringTable[HidKeboardUsageId_KeypadMemorySubtract]   = "KeypadMemorySubtract";
		stringTable[HidKeboardUsageId_KeypadMemoryMultiply]   = "KeypadMemoryMultiply";
		stringTable[HidKeboardUsageId_KeypadMemoryDivide]     = "KeypadMemoryDivide";
		stringTable[HidKeboardUsageId_KeypadPlusMinus]        = "KeypadPlusMinus";
		stringTable[HidKeboardUsageId_KeypadClear]            = "KeypadClear";
		stringTable[HidKeboardUsageId_KeypadClearEntry]       = "KeypadClearEntry";
		stringTable[HidKeboardUsageId_KeypadBinary]           = "KeypadBinary";
		stringTable[HidKeboardUsageId_KeypadOctal]            = "KeypadOctal";
		stringTable[HidKeboardUsageId_KeypadDecimal]          = "KeypadDecimal";
		stringTable[HidKeboardUsageId_KeypadHexadecimal]      = "KeypadHexadecimal";
		stringTable[HidKeboardUsageId_LeftControl]            = "LeftControl";
		stringTable[HidKeboardUsageId_LeftShift]              = "LeftShift";
		stringTable[HidKeboardUsageId_LeftAlt]                = "LeftAlt";
		stringTable[HidKeboardUsageId_LeftGui]                = "LeftGui";
		stringTable[HidKeboardUsageId_RightControl]           = "RightControl";
		stringTable[HidKeboardUsageId_RightShift]             = "RightShift";
		stringTable[HidKeboardUsageId_RightAlt]               = "RightAlt";
		stringTable[HidKeboardUsageId_RightGui]               = "RightGui";
	}
};

const char*
getHidKeboardUsageIdString(HidKeboardUsageId id) {
	static const char* stringTable[256] = { 0 };
	sl::callOnce(InitHidKeboardUsageIdStringTable(), stringTable);
	return (size_t)id < 256 ? stringTable[id] : "Unknown";
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class InitHidLedUsageIdStringTable {
public:
	void
	operator () (const char* stringTable[]) const {
		for (size_t i = 0; i < 256; i++)
			stringTable[i] = "Unknown";

		stringTable[HidLedUsageId_NumLock]                 = "NumLock";
		stringTable[HidLedUsageId_CapsLock]                = "CapsLock";
		stringTable[HidLedUsageId_ScrollLock]              = "ScrollLock";
		stringTable[HidLedUsageId_Compose]                 = "Compose";
		stringTable[HidLedUsageId_Kana]                    = "Kana";
		stringTable[HidLedUsageId_Power]                   = "Power";
		stringTable[HidLedUsageId_Shift]                   = "Shift";
		stringTable[HidLedUsageId_DoNotDisturb]            = "DoNotDisturb";
		stringTable[HidLedUsageId_Mute]                    = "Mute";
		stringTable[HidLedUsageId_ToneEnable]              = "ToneEnable";
		stringTable[HidLedUsageId_HighCutFilter]           = "HighCutFilter";
		stringTable[HidLedUsageId_LowCutFilter]            = "LowCutFilter";
		stringTable[HidLedUsageId_EqualizerEnable]         = "EqualizerEnable";
		stringTable[HidLedUsageId_SoundFieldOn]            = "SoundFieldOn";
		stringTable[HidLedUsageId_SurroundOn]              = "SurroundOn";
		stringTable[HidLedUsageId_Repeat]                  = "Repeat";
		stringTable[HidLedUsageId_Stereo]                  = "Stereo";
		stringTable[HidLedUsageId_SamplingRateDetect]      = "SamplingRateDetect";
		stringTable[HidLedUsageId_Spinning]                = "Spinning";
		stringTable[HidLedUsageId_Cav]                     = "Cav";
		stringTable[HidLedUsageId_Clv]                     = "Clv";
		stringTable[HidLedUsageId_RecordingFormatDetect]   = "RecordingFormatDetect";
		stringTable[HidLedUsageId_OffHook]                 = "OffHook";
		stringTable[HidLedUsageId_Ring]                    = "Ring";
		stringTable[HidLedUsageId_MessageWaiting]          = "MessageWaiting";
		stringTable[HidLedUsageId_DataMode]                = "DataMode";
		stringTable[HidLedUsageId_BatteryOperation]        = "BatteryOperation";
		stringTable[HidLedUsageId_BatteryOk]               = "BatteryOk";
		stringTable[HidLedUsageId_BatteryLow]              = "BatteryLow";
		stringTable[HidLedUsageId_Speaker]                 = "Speaker";
		stringTable[HidLedUsageId_Headset]                 = "Headset";
		stringTable[HidLedUsageId_Hold]                    = "Hold";
		stringTable[HidLedUsageId_Microphone]              = "Microphone";
		stringTable[HidLedUsageId_Coverage]                = "Coverage";
		stringTable[HidLedUsageId_NightMode]               = "NightMode";
		stringTable[HidLedUsageId_SendCalls]               = "SendCalls";
		stringTable[HidLedUsageId_CallPickup]              = "CallPickup";
		stringTable[HidLedUsageId_Conference]              = "Conference";
		stringTable[HidLedUsageId_StandBy]                 = "StandBy";
		stringTable[HidLedUsageId_CameraOn]                = "CameraOn";
		stringTable[HidLedUsageId_CameraOff]               = "CameraOff";
		stringTable[HidLedUsageId_OnLine]                  = "OnLine";
		stringTable[HidLedUsageId_OffLine]                 = "OffLine";
		stringTable[HidLedUsageId_Busy]                    = "Busy";
		stringTable[HidLedUsageId_Ready]                   = "Ready";
		stringTable[HidLedUsageId_PaperOut]                = "PaperOut";
		stringTable[HidLedUsageId_PaperJam]                = "PaperJam";
		stringTable[HidLedUsageId_Remote]                  = "Remote";
		stringTable[HidLedUsageId_Forward]                 = "Forward";
		stringTable[HidLedUsageId_Reverse]                 = "Reverse";
		stringTable[HidLedUsageId_Stop]                    = "Stop";
		stringTable[HidLedUsageId_Rewind]                  = "Rewind";
		stringTable[HidLedUsageId_FastForward]             = "FastForward";
		stringTable[HidLedUsageId_Play]                    = "Play";
		stringTable[HidLedUsageId_Pause]                   = "Pause";
		stringTable[HidLedUsageId_Record]                  = "Record";
		stringTable[HidLedUsageId_Error]                   = "Error";
		stringTable[HidLedUsageId_UsageSelectedIndicator]  = "UsageSelectedIndicator";
		stringTable[HidLedUsageId_UsageInUseIndicator]     = "UsageInUseIndicator";
		stringTable[HidLedUsageId_UsageMultiModeIndicator] = "UsageMultiModeIndicator";
		stringTable[HidLedUsageId_IndicatorOn]             = "IndicatorOn";
		stringTable[HidLedUsageId_IndicatorFlash]          = "IndicatorFlash";
		stringTable[HidLedUsageId_IndicatorSlowBlink]      = "IndicatorSlowBlink";
		stringTable[HidLedUsageId_IndicatorFastBlink]      = "IndicatorFastBlink";
		stringTable[HidLedUsageId_IndicatorOff]            = "IndicatorOff";
		stringTable[HidLedUsageId_FlashOnTime]             = "FlashOnTime";
		stringTable[HidLedUsageId_SlowBlinkOnTime]         = "SlowBlinkOnTime";
		stringTable[HidLedUsageId_SlowBlinkOffTime]        = "SlowBlinkOffTime";
		stringTable[HidLedUsageId_FastBlinkOnTime]         = "FastBlinkOnTime";
		stringTable[HidLedUsageId_FastBlinkOffTime]        = "FastBlinkOffTime";
		stringTable[HidLedUsageId_UsageIndicatorColor]     = "UsageIndicatorColor";
		stringTable[HidLedUsageId_IndicatorRed]            = "IndicatorRed";
		stringTable[HidLedUsageId_IndicatorGreen]          = "IndicatorGreen";
		stringTable[HidLedUsageId_IndicatorAmber]          = "IndicatorAmber";
		stringTable[HidLedUsageId_GenericIndicator]        = "GenericIndicator";
		stringTable[HidLedUsageId_SystemSuspend]           = "SystemSuspend";
		stringTable[HidLedUsageId_ExternalPowerConnected]  = "ExternalPowerConnected";
		stringTable[HidLedUsageId_IndicatorBlue]           = "IndicatorBlue";
		stringTable[HidLedUsageId_IndicatorOrange]         = "IndicatorOrange";
		stringTable[HidLedUsageId_GoodStatus]              = "GoodStatus";
		stringTable[HidLedUsageId_WarningStatus]           = "WarningStatus";
		stringTable[HidLedUsageId_RgbLed]                  = "RgbLed";
		stringTable[HidLedUsageId_RedLedChannel]           = "RedLedChannel";
		stringTable[HidLedUsageId_BlueLedChannel]          = "BlueLedChannel";
		stringTable[HidLedUsageId_GreenLedChannel]         = "GreenLedChannel";
		stringTable[HidLedUsageId_LedIntensity]            = "LedIntensity";
		stringTable[HidLedUsageId_SystemMicrophoneMute]    = "SystemMicrophoneMute";
		stringTable[HidLedUsageId_PlayerIndicator]         = "PlayerIndicator";
		stringTable[HidLedUsageId_Player1]                 = "Player1";
		stringTable[HidLedUsageId_Player2]                 = "Player2";
		stringTable[HidLedUsageId_Player3]                 = "Player3";
		stringTable[HidLedUsageId_Player4]                 = "Player4";
		stringTable[HidLedUsageId_Player5]                 = "Player5";
		stringTable[HidLedUsageId_Player6]                 = "Player6";
		stringTable[HidLedUsageId_Player7]                 = "Player7";
		stringTable[HidLedUsageId_Player8]                 = "Player8";
	}
};

const char*
getHidLedUsageIdString(HidLedUsageId id) {
	static const char* stringTable[256] = { 0 };
	sl::callOnce(InitHidLedUsageIdStringTable(), stringTable);
	return (size_t)id < 256 ? stringTable[id] : "Unknown";
}

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

class InitHidButtonUsageIdStringTable {
public:
	void
	operator () (const char* stringTable[]) const {
		for (size_t i = 0; i < 256; i++)
			stringTable[i] = "Unknown";

		stringTable[HidButtonUsageId_None]      = "None";
		stringTable[HidButtonUsageId_Primary]   = "Primary";
		stringTable[HidButtonUsageId_Secondary] = "Secondary";
		stringTable[HidButtonUsageId_Tertiary]  = "Tertiary";
	}
};

const char*
getHidButtonUsageIdString(HidButtonUsageId id) {
	static const char* stringTable[256] = { 0 };
	sl::callOnce(InitHidButtonUsageIdStringTable(), stringTable);
	return (size_t)id < 256 ? stringTable[id] : "Unknown";
}

//..............................................................................

typedef
const char*
GetHidUsageStringFunc(int id);

class InitHidGetUsageIdStringTable {
public:
	void
	operator () (GetHidUsageStringFunc* funcTable[]) const {
		memset(funcTable, 0, 256 * sizeof(GetHidUsageStringFunc*));

		funcTable[HidUsagePage_GenericDesktop] = (GetHidUsageStringFunc*)getHidGenericDesktopUsageIdString;
		funcTable[HidUsagePage_Keyboard] = (GetHidUsageStringFunc*)getHidKeboardUsageIdString;
		funcTable[HidUsagePage_Led] = (GetHidUsageStringFunc*)getHidLedUsageIdString;
		funcTable[HidUsagePage_Button] = (GetHidUsageStringFunc*)getHidButtonUsageIdString;
	}
};

sl::String
getHidUsageString(
	HidUsagePage page,
	uint_t id
) {
	static GetHidUsageStringFunc* funcTable[256] = { 0 };
	sl::callOnce(InitHidGetUsageIdStringTable(), funcTable);
	GetHidUsageStringFunc* func = (size_t)page < 256 ? funcTable[page] : NULL;

	return func ?
		sl::String(func(id)) :
		sl::formatString("%d", id);
}

//..............................................................................

} // namespace io
} // namespace axl
