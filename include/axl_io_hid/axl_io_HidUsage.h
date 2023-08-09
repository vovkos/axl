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

#pragma once

#define _AXL_IO_HIDUSAGE_H

#include "axl_io_HidPch.h"
#include "axl_sl_String.h"

namespace axl {
namespace io {

//..............................................................................

enum HidUsagePage {
	HidUsagePage_Undefined               = 0x00,
	HidUsagePage_GenericDesktop          = 0x01,
	HidUsagePage_SimulationControls      = 0x02,
	HidUsagePage_VrControls              = 0x03,
	HidUsagePage_SportControls           = 0x04,
	HidUsagePage_GameControls            = 0x05,
	HidUsagePage_GenericDeviceControls   = 0x06,
	HidUsagePage_Keyboard                = 0x07,
	HidUsagePage_Led                     = 0x08,
	HidUsagePage_Button                  = 0x09,
	HidUsagePage_Ordinal                 = 0x0a,
	HidUsagePage_TelephonyDevice         = 0x0b,
	HidUsagePage_Consumer                = 0x0c,
	HidUsagePage_Digitizers              = 0x0d,
	HidUsagePage_Haptics                 = 0x0e,
	HidUsagePage_PhysicalInputDevice     = 0x0f,
	HidUsagePage_Unicode                 = 0x10,
	HidUsagePage_SoC                     = 0x11,
	HidUsagePage_EyeHeadTrackers         = 0x12,
	HidUsagePage_AuxiliaryDisplay        = 0x14,
	HidUsagePage_Sensors                 = 0x20,
	HidUsagePage_MedicalInstrument       = 0x40,
	HidUsagePage_BrailleDisplay          = 0x41,
	HidUsagePage_LightingAndIllumination = 0x59,
	HidUsagePage_Monitor                 = 0x80,
	HidUsagePage_MonitorEnumerated       = 0x81,
	HidUsagePage_VesaVirtualControls     = 0x82,
	HidUsagePage_Power                   = 0x84,
	HidUsagePage_BatterySystem           = 0x85,
	HidUsagePage_BarcodeScanner          = 0x8c,
	HidUsagePage_Scales                  = 0x8d,
	HidUsagePage_MagneticStripeReader    = 0x8e,
	HidUsagePage_CameraControl           = 0x90,
	HidUsagePage_Arcade                  = 0x91,
	HidUsagePage_GamingDevice            = 0x92,
	HidUsagePage_FidoAlliance            = 0xf1d0,
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

const char*
getHidUsagePageString(HidUsagePage page);

//..............................................................................

// HidUsagePage_GenericDesktop (0x01)

enum HidGenericDesktopUsageId {
	HidGenericDesktopUsageId_Pointer                               = 0x01,  // CP
	HidGenericDesktopUsageId_Mouse                                 = 0x02,  // CA
	HidGenericDesktopUsageId_Joystick                              = 0x04,  // CA
	HidGenericDesktopUsageId_Gamepad                               = 0x05,  // CA
	HidGenericDesktopUsageId_Keyboard                              = 0x06,  // CA
	HidGenericDesktopUsageId_Keypad                                = 0x07,  // CA
	HidGenericDesktopUsageId_MultiAxisController                   = 0x08,  // CA
	HidGenericDesktopUsageId_TabletPcSystemControls                = 0x09,  // CA
	HidGenericDesktopUsageId_WaterCoolingDevice                    = 0x0a,  // CA
	HidGenericDesktopUsageId_ComputerChassisDevice                 = 0x0b,  // CA
	HidGenericDesktopUsageId_WirelessRadioControls                 = 0x0c,  // CA
	HidGenericDesktopUsageId_PortableDeviceControl                 = 0x0d,  // CA
	HidGenericDesktopUsageId_SystemMultiAxisController             = 0x0e,  // CA
	HidGenericDesktopUsageId_SpatialController                     = 0x0f,  // CA
	HidGenericDesktopUsageId_AssistiveControl                      = 0x10,  // CA
	HidGenericDesktopUsageId_DeviceDock                            = 0x11,  // CA
	HidGenericDesktopUsageId_DockableDevice                        = 0x12,  // CA
	HidGenericDesktopUsageId_CallStateManagementControl            = 0x13,  // CA
	HidGenericDesktopUsageId_X                                     = 0x30,  // DV
	HidGenericDesktopUsageId_Y                                     = 0x31,  // DV
	HidGenericDesktopUsageId_Z                                     = 0x32,  // DV
	HidGenericDesktopUsageId_Rx                                    = 0x33,  // DV
	HidGenericDesktopUsageId_Ry                                    = 0x34,  // DV
	HidGenericDesktopUsageId_Rz                                    = 0x35,  // DV
	HidGenericDesktopUsageId_Slider                                = 0x36,  // DV
	HidGenericDesktopUsageId_Dial                                  = 0x37,  // DV
	HidGenericDesktopUsageId_Wheel                                 = 0x38,  // DV
	HidGenericDesktopUsageId_HatSwitch                             = 0x39,  // DV
	HidGenericDesktopUsageId_CountedBuffer                         = 0x3a,  // CL
	HidGenericDesktopUsageId_ByteCount                             = 0x3b,  // DV
	HidGenericDesktopUsageId_MotionWakeup                          = 0x3c,  // OSC/DF
	HidGenericDesktopUsageId_Start                                 = 0x3d,  // OOC
	HidGenericDesktopUsageId_Select                                = 0x3e,  // OOC
	HidGenericDesktopUsageId_Vx                                    = 0x40,  // DV
	HidGenericDesktopUsageId_Vy                                    = 0x41,  // DV
	HidGenericDesktopUsageId_Vz                                    = 0x42,  // DV
	HidGenericDesktopUsageId_Vbrx                                  = 0x43,  // DV
	HidGenericDesktopUsageId_Vbry                                  = 0x44,  // DV
	HidGenericDesktopUsageId_Vbrz                                  = 0x45,  // DV
	HidGenericDesktopUsageId_Vno                                   = 0x46,  // DV
	HidGenericDesktopUsageId_FeatureNotification                   = 0x47,  // DV/DF
	HidGenericDesktopUsageId_ResolutionMultiplier                  = 0x48,  // DV
	HidGenericDesktopUsageId_Qx                                    = 0x49,  // DV
	HidGenericDesktopUsageId_Qy                                    = 0x4a,  // DV
	HidGenericDesktopUsageId_Qz                                    = 0x4b,  // DV
	HidGenericDesktopUsageId_Qw                                    = 0x4c,  // DV
	HidGenericDesktopUsageId_SystemControl                         = 0x80,  // CA
	HidGenericDesktopUsageId_SystemPowerDown                       = 0x81,  // OSC
	HidGenericDesktopUsageId_SystemSleep                           = 0x82,  // OSC
	HidGenericDesktopUsageId_SystemWakeUp                          = 0x83,  // OSC
	HidGenericDesktopUsageId_SystemContextMenu                     = 0x84,  // OSC
	HidGenericDesktopUsageId_SystemMainMenu                        = 0x85,  // OSC
	HidGenericDesktopUsageId_SystemAppMenu                         = 0x86,  // OSC
	HidGenericDesktopUsageId_SystemMenuHelp                        = 0x87,  // OSC
	HidGenericDesktopUsageId_SystemMenuExit                        = 0x88,  // OSC
	HidGenericDesktopUsageId_SystemMenuSelect                      = 0x89,  // OSC
	HidGenericDesktopUsageId_SystemMenuRight                       = 0x8a,  // RTC
	HidGenericDesktopUsageId_SystemMenuLeft                        = 0x8b,  // RTC
	HidGenericDesktopUsageId_SystemMenuUp                          = 0x8c,  // RTC
	HidGenericDesktopUsageId_SystemMenuDown                        = 0x8d,  // RTC
	HidGenericDesktopUsageId_SystemColdRestart                     = 0x8e,  // OSC
	HidGenericDesktopUsageId_SystemWarmRestart                     = 0x8f,  // OSC
	HidGenericDesktopUsageId_DpadUp                                = 0x90,  // OOC
	HidGenericDesktopUsageId_DpadDown                              = 0x91,  // OOC
	HidGenericDesktopUsageId_DpadRight                             = 0x92,  // OOC
	HidGenericDesktopUsageId_DpadLeft                              = 0x93,  // OOC
	HidGenericDesktopUsageId_IndexTrigger                          = 0x94,  // MC/DV
	HidGenericDesktopUsageId_PalmTrigger                           = 0x95,  // MC/DV
	HidGenericDesktopUsageId_Thumbstick                            = 0x96,  // CP
	HidGenericDesktopUsageId_SystemFunctionShift                   = 0x97,  // MC
	HidGenericDesktopUsageId_SystemFunctionShiftLock               = 0x98,  // OOC
	HidGenericDesktopUsageId_SystemFunctionShiftLockIndicator      = 0x99,  // DV
	HidGenericDesktopUsageId_SystemDismissNotification             = 0x9a,  // OSC
	HidGenericDesktopUsageId_SystemDoNotDisturb                    = 0x9b,  // OOC
	HidGenericDesktopUsageId_SystemDock                            = 0xa0,  // OSC
	HidGenericDesktopUsageId_SystemUndock                          = 0xa1,  // OSC
	HidGenericDesktopUsageId_SystemSetup                           = 0xa2,  // OSC
	HidGenericDesktopUsageId_SystemBreak                           = 0xa3,  // OSC
	HidGenericDesktopUsageId_SystemDebuggerBreak                   = 0xa4,  // OSC
	HidGenericDesktopUsageId_ApplicationBreak                      = 0xa5,  // OSC
	HidGenericDesktopUsageId_ApplicationDebuggerBreak              = 0xa6,  // OSC
	HidGenericDesktopUsageId_SystemSpeakerMute                     = 0xa7,  // OSC
	HidGenericDesktopUsageId_SystemHibernate                       = 0xa8,  // OSC
	HidGenericDesktopUsageId_SystemMicrophoneMute                  = 0xa9,  // OOC
	HidGenericDesktopUsageId_SystemDisplayInvert                   = 0xb0,  // OSC
	HidGenericDesktopUsageId_SystemDisplayInternal                 = 0xb1,  // OSC
	HidGenericDesktopUsageId_SystemDisplayExternal                 = 0xb2,  // OSC
	HidGenericDesktopUsageId_SystemDisplayBoth                     = 0xb3,  // OSC
	HidGenericDesktopUsageId_SystemDisplayDual                     = 0xb4,  // OSC
	HidGenericDesktopUsageId_SystemDisplayToggleIntExtMode         = 0xb5,  // OSC
	HidGenericDesktopUsageId_SystemDisplaySwapPrimarySecondary     = 0xb6,  // OSC
	HidGenericDesktopUsageId_SystemDisplayToggleLcdAutoscale       = 0xb7,  // OSC
	HidGenericDesktopUsageId_SensorZone                            = 0xc0,  // CL
	HidGenericDesktopUsageId_Rpm                                   = 0xc1,  // DV
	HidGenericDesktopUsageId_CoolantLevel                          = 0xc2,  // DV
	HidGenericDesktopUsageId_CoolantCriticalLevel                  = 0xc3,  // SV
	HidGenericDesktopUsageId_CoolantPump                           = 0xc4,  // US
	HidGenericDesktopUsageId_ChassisEnclosure                      = 0xc5,  // CL
	HidGenericDesktopUsageId_WirelessRadioButton                   = 0xc6,  // OOC
	HidGenericDesktopUsageId_WirelessRadioLed                      = 0xc7,  // OOC
	HidGenericDesktopUsageId_WirelessRadioSliderSwitch             = 0xc8,  // OOC
	HidGenericDesktopUsageId_SystemDisplayRotationLockButton       = 0xc9,  // OOC
	HidGenericDesktopUsageId_SystemDisplayRotationLockSliderSwitch = 0xca,  // OOC
	HidGenericDesktopUsageId_ControlEnable                         = 0xcb,  // DF
	HidGenericDesktopUsageId_DockableDeviceUniqueID                = 0xd0,  // DV
	HidGenericDesktopUsageId_DockableDeviceVendorID                = 0xd1,  // DV
	HidGenericDesktopUsageId_DockableDevicePrimaryUsagePage        = 0xd2,  // DV
	HidGenericDesktopUsageId_DockableDevicePrimaryUsageId          = 0xd3,  // DV
	HidGenericDesktopUsageId_DockableDeviceDockingState            = 0xd4,  // DF
	HidGenericDesktopUsageId_DockableDeviceDisplayOcclusion        = 0xd5,  // CL
	HidGenericDesktopUsageId_DockableDeviceObjectType              = 0xd6,  // DV
	HidGenericDesktopUsageId_CallActiveLed                         = 0xe0,  // OOC
	HidGenericDesktopUsageId_CallMuteToggle                        = 0xe1,  // OSC
	HidGenericDesktopUsageId_CallMuteLed                           = 0xe2,  // OOC
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

const char*
getHidGenericDesktopUsageIdString(HidGenericDesktopUsageId id);

//..............................................................................

// HidUsagePage_Keyboard (0x07)

enum HidKeboardUsageId {
	HidKeboardUsageId_ErrorRollOver           = 0x01,  // Sel
	HidKeboardUsageId_PostFail                = 0x02,  // Sel
	HidKeboardUsageId_ErrorUndefined          = 0x03,  // Sel
	HidKeboardUsageId_A                       = 0x04,  // Sel
	HidKeboardUsageId_B                       = 0x05,  // Sel
	HidKeboardUsageId_C                       = 0x06,  // Sel
	HidKeboardUsageId_D                       = 0x07,  // Sel
	HidKeboardUsageId_E                       = 0x08,  // Sel
	HidKeboardUsageId_F                       = 0x09,  // Sel
	HidKeboardUsageId_G                       = 0x0a,  // Sel
	HidKeboardUsageId_H                       = 0x0b,  // Sel
	HidKeboardUsageId_I                       = 0x0c,  // Sel
	HidKeboardUsageId_J                       = 0x0d,  // Sel
	HidKeboardUsageId_K                       = 0x0e,  // Sel
	HidKeboardUsageId_L                       = 0x0f,  // Sel
	HidKeboardUsageId_M                       = 0x10,  // Sel
	HidKeboardUsageId_N                       = 0x11,  // Sel
	HidKeboardUsageId_O                       = 0x12,  // Sel
	HidKeboardUsageId_P                       = 0x13,  // Sel
	HidKeboardUsageId_Q                       = 0x14,  // Sel
	HidKeboardUsageId_R                       = 0x15,  // Sel
	HidKeboardUsageId_S                       = 0x16,  // Sel
	HidKeboardUsageId_T                       = 0x17,  // Sel
	HidKeboardUsageId_U                       = 0x18,  // Sel
	HidKeboardUsageId_V                       = 0x19,  // Sel
	HidKeboardUsageId_W                       = 0x1a,  // Sel
	HidKeboardUsageId_X                       = 0x1b,  // Sel
	HidKeboardUsageId_Y                       = 0x1c,  // Sel
	HidKeboardUsageId_Z                       = 0x1d,  // Sel
	HidKeboardUsageId_1                       = 0x1e,  // Sel
	HidKeboardUsageId_2                       = 0x1f,  // Sel
	HidKeboardUsageId_3                       = 0x20,  // Sel
	HidKeboardUsageId_4                       = 0x21,  // Sel
	HidKeboardUsageId_5                       = 0x22,  // Sel
	HidKeboardUsageId_6                       = 0x23,  // Sel
	HidKeboardUsageId_7                       = 0x24,  // Sel
	HidKeboardUsageId_8                       = 0x25,  // Sel
	HidKeboardUsageId_9                       = 0x26,  // Sel
	HidKeboardUsageId_0                       = 0x27,  // Sel
	HidKeboardUsageId_Enter                   = 0x28,  // Sel
	HidKeboardUsageId_Escape                  = 0x29,  // Sel
	HidKeboardUsageId_Backspace               = 0x2a,  // Sel
	HidKeboardUsageId_Tab                     = 0x2b,  // Sel
	HidKeboardUsageId_Spacebar                = 0x2c,  // Sel
	HidKeboardUsageId_Minus                   = 0x2d,  // Sel
	HidKeboardUsageId_Equal                   = 0x2e,  // Sel
	HidKeboardUsageId_LeftBracket             = 0x2f,  // Sel
	HidKeboardUsageId_RightBracket            = 0x30,  // Sel
	HidKeboardUsageId_Backslash               = 0x31,  // Sel
	HidKeboardUsageId_NonUsHash               = 0x32,  // Sel
	HidKeboardUsageId_Semicolon               = 0x33,  // Sel
	HidKeboardUsageId_Apostrophe              = 0x34,  // Sel
	HidKeboardUsageId_GraveAccent             = 0x35,  // Sel
	HidKeboardUsageId_Comma                   = 0x36,  // Sel
	HidKeboardUsageId_Dot                     = 0x37,  // Sel
	HidKeboardUsageId_Div                     = 0x38,  // Sel
	HidKeboardUsageId_CapsLock                = 0x39,  // Sel
	HidKeboardUsageId_F1                      = 0x3a,  // Sel
	HidKeboardUsageId_F2                      = 0x3b,  // Sel
	HidKeboardUsageId_F3                      = 0x3c,  // Sel
	HidKeboardUsageId_F4                      = 0x3d,  // Sel
	HidKeboardUsageId_F5                      = 0x3e,  // Sel
	HidKeboardUsageId_F6                      = 0x3f,  // Sel
	HidKeboardUsageId_F7                      = 0x40,  // Sel
	HidKeboardUsageId_F8                      = 0x41,  // Sel
	HidKeboardUsageId_F9                      = 0x42,  // Sel
	HidKeboardUsageId_F10                     = 0x43,  // Sel
	HidKeboardUsageId_F11                     = 0x44,  // Sel
	HidKeboardUsageId_F12                     = 0x45,  // Sel
	HidKeboardUsageId_PrintScreen             = 0x46,  // Sel
	HidKeboardUsageId_ScrollLock              = 0x47,  // Sel
	HidKeboardUsageId_Pause                   = 0x48,  // Sel
	HidKeboardUsageId_Insert                  = 0x49,  // Sel
	HidKeboardUsageId_Home                    = 0x4a,  // Sel
	HidKeboardUsageId_PageUp                  = 0x4b,  // Sel
	HidKeboardUsageId_Delete                  = 0x4c,  // Sel
	HidKeboardUsageId_End                     = 0x4d,  // Sel
	HidKeboardUsageId_PageDown                = 0x4e,  // Sel
	HidKeboardUsageId_RightArrow              = 0x4f,  // Sel
	HidKeboardUsageId_LeftArrow               = 0x50,  // Sel
	HidKeboardUsageId_DownArrow               = 0x51,  // Sel
	HidKeboardUsageId_UpArrow                 = 0x52,  // Sel
	HidKeboardUsageId_KeypadNumLock           = 0x53,  // Sel
	HidKeboardUsageId_KeypadDiv               = 0x54,  // Sel
	HidKeboardUsageId_KeypadMul               = 0x55,  // Sel
	HidKeboardUsageId_KeypadMinus             = 0x56,  // Sel
	HidKeboardUsageId_KeypadPlus              = 0x57,  // Sel
	HidKeboardUsageId_KeypadEnter             = 0x58,  // Sel
	HidKeboardUsageId_Keypad1                 = 0x59,  // Sel
	HidKeboardUsageId_Keypad2                 = 0x5a,  // Sel
	HidKeboardUsageId_Keypad3                 = 0x5b,  // Sel
	HidKeboardUsageId_Keypad4                 = 0x5c,  // Sel
	HidKeboardUsageId_Keypad5                 = 0x5d,  // Sel
	HidKeboardUsageId_Keypad6                 = 0x5e,  // Sel
	HidKeboardUsageId_Keypad7                 = 0x5f,  // Sel
	HidKeboardUsageId_Keypad8                 = 0x60,  // Sel
	HidKeboardUsageId_Keypad9                 = 0x61,  // Sel
	HidKeboardUsageId_Keypad0                 = 0x62,  // Sel
	HidKeboardUsageId_KeypadDot               = 0x63,  // Sel
	HidKeboardUsageId_NonUsBackslash          = 0x64,  // Sel
	HidKeboardUsageId_Application             = 0x65,  // Sel
	HidKeboardUsageId_Power                   = 0x66,  // Sel
	HidKeboardUsageId_KeypadEqual             = 0x67,  // Sel
	HidKeboardUsageId_F13                     = 0x68,  // Sel
	HidKeboardUsageId_F14                     = 0x69,  // Sel
	HidKeboardUsageId_F15                     = 0x6a,  // Sel
	HidKeboardUsageId_F16                     = 0x6b,  // Sel
	HidKeboardUsageId_F17                     = 0x6c,  // Sel
	HidKeboardUsageId_F18                     = 0x6d,  // Sel
	HidKeboardUsageId_F19                     = 0x6e,  // Sel
	HidKeboardUsageId_F20                     = 0x6f,  // Sel
	HidKeboardUsageId_F21                     = 0x70,  // Sel
	HidKeboardUsageId_F22                     = 0x71,  // Sel
	HidKeboardUsageId_F23                     = 0x72,  // Sel
	HidKeboardUsageId_F24                     = 0x73,  // Sel
	HidKeboardUsageId_Execute                 = 0x74,  // Sel
	HidKeboardUsageId_Help                    = 0x75,  // Sel
	HidKeboardUsageId_Menu                    = 0x76,  // Sel
	HidKeboardUsageId_Select                  = 0x77,  // Sel
	HidKeboardUsageId_Stop                    = 0x78,  // Sel
	HidKeboardUsageId_Again                   = 0x79,  // Sel
	HidKeboardUsageId_Undo                    = 0x7a,  // Sel
	HidKeboardUsageId_Cut                     = 0x7b,  // Sel
	HidKeboardUsageId_Copy                    = 0x7c,  // Sel
	HidKeboardUsageId_Paste                   = 0x7d,  // Sel
	HidKeboardUsageId_Find                    = 0x7e,  // Sel
	HidKeboardUsageId_Mute                    = 0x7f,  // Sel
	HidKeboardUsageId_VolumeUp                = 0x80,  // Sel
	HidKeboardUsageId_VolumeDown              = 0x81,  // Sel
	HidKeboardUsageId_LockingCapsLock         = 0x82,  // Sel
	HidKeboardUsageId_LockingNumLock          = 0x83,  // Sel
	HidKeboardUsageId_LockingScrollLock       = 0x84,  // Sel
	HidKeboardUsageId_KeypadComma             = 0x85,  // Sel
	HidKeboardUsageId_KeypadEqualSign         = 0x86,  // Sel
	HidKeboardUsageId_International1          = 0x87,  // Sel
	HidKeboardUsageId_International2          = 0x88,  // Sel
	HidKeboardUsageId_International3          = 0x89,  // Sel
	HidKeboardUsageId_International4          = 0x8a,  // Sel
	HidKeboardUsageId_International5          = 0x8b,  // Sel
	HidKeboardUsageId_International6          = 0x8c,  // Sel
	HidKeboardUsageId_International7          = 0x8d,  // Sel
	HidKeboardUsageId_International8          = 0x8e,  // Sel
	HidKeboardUsageId_International9          = 0x8f,  // Sel
	HidKeboardUsageId_Lang1                   = 0x90,  // Sel
	HidKeboardUsageId_Lang2                   = 0x91,  // Sel
	HidKeboardUsageId_Lang3                   = 0x92,  // Sel
	HidKeboardUsageId_Lang4                   = 0x93,  // Sel
	HidKeboardUsageId_Lang5                   = 0x94,  // Sel
	HidKeboardUsageId_Lang6                   = 0x95,  // Sel
	HidKeboardUsageId_Lang7                   = 0x96,  // Sel
	HidKeboardUsageId_Lang8                   = 0x97,  // Sel
	HidKeboardUsageId_Lang9                   = 0x98,  // Sel
	HidKeboardUsageId_AlternateErase          = 0x99,  // Sel
	HidKeboardUsageId_SysReq                  = 0x9a,  // Sel
	HidKeboardUsageId_Cancel                  = 0x9b,  // Sel
	HidKeboardUsageId_Clear                   = 0x9c,  // Sel
	HidKeboardUsageId_Prior                   = 0x9d,  // Sel
	HidKeboardUsageId_Return                  = 0x9e,  // Sel
	HidKeboardUsageId_Separator               = 0x9f,  // Sel
	HidKeboardUsageId_Out                     = 0xa0,  // Sel
	HidKeboardUsageId_Oper                    = 0xa1,  // Sel
	HidKeboardUsageId_ClearAgain              = 0xa2,  // Sel
	HidKeboardUsageId_CrSelProps              = 0xa3,  // Sel
	HidKeboardUsageId_ExSel                   = 0xa4,  // Sel
	HidKeboardUsageId_Keypad00                = 0xb0,  // Sel
	HidKeboardUsageId_Keypad000               = 0xb1,  // Sel
	HidKeboardUsageId_ThousandsSeparator      = 0xb2,  // Sel
	HidKeboardUsageId_DecimalSeparator        = 0xb3,  // Sel
	HidKeboardUsageId_CurrencyUnit            = 0xb4,  // Sel
	HidKeboardUsageId_CurrencySubUnit         = 0xb5,  // Sel
	HidKeboardUsageId_KeypadLeftParenthesis   = 0xb6,  // Sel
	HidKeboardUsageId_KeypadRightParenthesis  = 0xb7,  // Sel
	HidKeboardUsageId_KeypadLeftBrace         = 0xb8,  // Sel
	HidKeboardUsageId_KeypadRightBrace        = 0xb9,  // Sel
	HidKeboardUsageId_KeypadTab               = 0xba,  // Sel
	HidKeboardUsageId_KeypadBackspace         = 0xbb,  // Sel
	HidKeboardUsageId_KeypadA                 = 0xbc,  // Sel
	HidKeboardUsageId_KeypadB                 = 0xbd,  // Sel
	HidKeboardUsageId_KeypadC                 = 0xbe,  // Sel
	HidKeboardUsageId_KeypadD                 = 0xbf,  // Sel
	HidKeboardUsageId_KeypadE                 = 0xc0,  // Sel
	HidKeboardUsageId_KeypadF                 = 0xc1,  // Sel
	HidKeboardUsageId_KeypadXor               = 0xc2,  // Sel
	HidKeboardUsageId_KeypadCaret             = 0xc3,  // Sel
	HidKeboardUsageId_KeypadMod               = 0xc4,  // Sel
	HidKeboardUsageId_KeypadLt                = 0xc5,  // Sel
	HidKeboardUsageId_KeypadGt                = 0xc6,  // Sel
	HidKeboardUsageId_KeypadAmpersand         = 0xc7,  // Sel
	HidKeboardUsageId_KeypadAnd               = 0xc8,  // Sel
	HidKeboardUsageId_KeypadPipe              = 0xc9,  // Sel
	HidKeboardUsageId_KeypadOr                = 0xca,  // Sel
	HidKeboardUsageId_KeypadColon             = 0xcb,  // Sel
	HidKeboardUsageId_KeypadHash              = 0xcc,  // Sel
	HidKeboardUsageId_KeypadSpace             = 0xcd,  // Sel
	HidKeboardUsageId_KeypadAt                = 0xce,  // Sel
	HidKeboardUsageId_KeypadExclamation       = 0xcf,  // Sel
	HidKeboardUsageId_KeypadMemoryStore       = 0xd0,  // Sel
	HidKeboardUsageId_KeypadMemoryRecall      = 0xd1,  // Sel
	HidKeboardUsageId_KeypadMemoryClear       = 0xd2,  // Sel
	HidKeboardUsageId_KeypadMemoryAdd         = 0xd3,  // Sel
	HidKeboardUsageId_KeypadMemorySubtract    = 0xd4,  // Sel
	HidKeboardUsageId_KeypadMemoryMultiply    = 0xd5,  // Sel
	HidKeboardUsageId_KeypadMemoryDivide      = 0xd6,  // Sel
	HidKeboardUsageId_KeypadPlusMinus         = 0xd7,  // Sel
	HidKeboardUsageId_KeypadClear             = 0xd8,  // Sel
	HidKeboardUsageId_KeypadClearEntry        = 0xd9,  // Sel
	HidKeboardUsageId_KeypadBinary            = 0xda,  // Sel
	HidKeboardUsageId_KeypadOctal             = 0xdb,  // Sel
	HidKeboardUsageId_KeypadDecimal           = 0xdc,  // Sel
	HidKeboardUsageId_KeypadHexadecimal       = 0xdd,  // Sel
	HidKeboardUsageId_LeftControl             = 0xe0,  // DV
	HidKeboardUsageId_LeftShift               = 0xe1,  // DV
	HidKeboardUsageId_LeftAlt                 = 0xe2,  // DV
	HidKeboardUsageId_LeftGui                 = 0xe3,  // DV
	HidKeboardUsageId_RightControl            = 0xe4,  // DV
	HidKeboardUsageId_RightShift              = 0xe5,  // DV
	HidKeboardUsageId_RightAlt                = 0xe6,  // DV
	HidKeboardUsageId_RightGui                = 0xe7,  // DV
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

const char*
getHidKeboardUsageIdString(HidKeboardUsageId id);

//..............................................................................

// HidUsagePage_Led (0x08)

enum HidLedUsageId {
	HidLedUsageId_NumLock                 = 0x01,  // OOC
	HidLedUsageId_CapsLock                = 0x02,  // OOC
	HidLedUsageId_ScrollLock              = 0x03,  // OOC
	HidLedUsageId_Compose                 = 0x04,  // OOC
	HidLedUsageId_Kana                    = 0x05,  // OOC
	HidLedUsageId_Power                   = 0x06,  // OOC
	HidLedUsageId_Shift                   = 0x07,  // OOC
	HidLedUsageId_DoNotDisturb            = 0x08,  // OOC
	HidLedUsageId_Mute                    = 0x09,  // OOC
	HidLedUsageId_ToneEnable              = 0x0A,  // OOC
	HidLedUsageId_HighCutFilter           = 0x0B,  // OOC
	HidLedUsageId_LowCutFilter            = 0x0C,  // OOC
	HidLedUsageId_EqualizerEnable         = 0x0D,  // OOC
	HidLedUsageId_SoundFieldOn            = 0x0E,  // OOC
	HidLedUsageId_SurroundOn              = 0x0F,  // OOC
	HidLedUsageId_Repeat                  = 0x10,  // OOC
	HidLedUsageId_Stereo                  = 0x11,  // OOC
	HidLedUsageId_SamplingRateDetect      = 0x12,  // OOC
	HidLedUsageId_Spinning                = 0x13,  // OOC
	HidLedUsageId_Cav                     = 0x14,  // OOC
	HidLedUsageId_Clv                     = 0x15,  // OOC
	HidLedUsageId_RecordingFormatDetect   = 0x16,  // OOC
	HidLedUsageId_OffHook                 = 0x17,  // OOC
	HidLedUsageId_Ring                    = 0x18,  // OOC
	HidLedUsageId_MessageWaiting          = 0x19,  // OOC
	HidLedUsageId_DataMode                = 0x1A,  // OOC
	HidLedUsageId_BatteryOperation        = 0x1B,  // OOC
	HidLedUsageId_BatteryOk               = 0x1C,  // OOC
	HidLedUsageId_BatteryLow              = 0x1D,  // OOC
	HidLedUsageId_Speaker                 = 0x1E,  // OOC
	HidLedUsageId_Headset                 = 0x1F,  // OOC
	HidLedUsageId_Hold                    = 0x20,  // OOC
	HidLedUsageId_Microphone              = 0x21,  // OOC
	HidLedUsageId_Coverage                = 0x22,  // OOC
	HidLedUsageId_NightMode               = 0x23,  // OOC
	HidLedUsageId_SendCalls               = 0x24,  // OOC
	HidLedUsageId_CallPickup              = 0x25,  // OOC
	HidLedUsageId_Conference              = 0x26,  // OOC
	HidLedUsageId_StandBy                 = 0x27,  // OOC
	HidLedUsageId_CameraOn                = 0x28,  // OOC
	HidLedUsageId_CameraOff               = 0x29,  // OOC
	HidLedUsageId_OnLine                  = 0x2A,  // OOC
	HidLedUsageId_OffLine                 = 0x2B,  // OOC
	HidLedUsageId_Busy                    = 0x2C,  // OOC
	HidLedUsageId_Ready                   = 0x2D,  // OOC
	HidLedUsageId_PaperOut                = 0x2E,  // OOC
	HidLedUsageId_PaperJam                = 0x2F,  // OOC
	HidLedUsageId_Remote                  = 0x30,  // OOC
	HidLedUsageId_Forward                 = 0x31,  // OOC
	HidLedUsageId_Reverse                 = 0x32,  // OOC
	HidLedUsageId_Stop                    = 0x33,  // OOC
	HidLedUsageId_Rewind                  = 0x34,  // OOC
	HidLedUsageId_FastForward             = 0x35,  // OOC
	HidLedUsageId_Play                    = 0x36,  // OOC
	HidLedUsageId_Pause                   = 0x37,  // OOC
	HidLedUsageId_Record                  = 0x38,  // OOC
	HidLedUsageId_Error                   = 0x39,  // OOC
	HidLedUsageId_UsageSelectedIndicator  = 0x3A,  // US
	HidLedUsageId_UsageInUseIndicator     = 0x3B,  // US
	HidLedUsageId_UsageMultiModeIndicator = 0x3C,  // UM
	HidLedUsageId_IndicatorOn             = 0x3D,  // Sel
	HidLedUsageId_IndicatorFlash          = 0x3E,  // Sel
	HidLedUsageId_IndicatorSlowBlink      = 0x3F,  // Sel
	HidLedUsageId_IndicatorFastBlink      = 0x40,  // Sel
	HidLedUsageId_IndicatorOff            = 0x41,  // Sel
	HidLedUsageId_FlashOnTime             = 0x42,  // DV
	HidLedUsageId_SlowBlinkOnTime         = 0x43,  // DV
	HidLedUsageId_SlowBlinkOffTime        = 0x44,  // DV
	HidLedUsageId_FastBlinkOnTime         = 0x45,  // DV
	HidLedUsageId_FastBlinkOffTime        = 0x46,  // DV
	HidLedUsageId_UsageIndicatorColor     = 0x47,  // UM
	HidLedUsageId_IndicatorRed            = 0x48,  // Sel
	HidLedUsageId_IndicatorGreen          = 0x49,  // Sel
	HidLedUsageId_IndicatorAmber          = 0x4A,  // Sel
	HidLedUsageId_GenericIndicator        = 0x4B,  // OOC
	HidLedUsageId_SystemSuspend           = 0x4C,  // OOC
	HidLedUsageId_ExternalPowerConnected  = 0x4D,  // OOC
	HidLedUsageId_IndicatorBlue           = 0x4E,  // Sel
	HidLedUsageId_IndicatorOrange         = 0x4F,  // Sel
	HidLedUsageId_GoodStatus              = 0x50,  // OOC
	HidLedUsageId_WarningStatus           = 0x51,  // OOC
	HidLedUsageId_RgbLed                  = 0x52,  // CL
	HidLedUsageId_RedLedChannel           = 0x53,  // DV
	HidLedUsageId_BlueLedChannel          = 0x54,  // DV
	HidLedUsageId_GreenLedChannel         = 0x55,  // DV
	HidLedUsageId_LedIntensity            = 0x56,  // DV
	HidLedUsageId_SystemMicrophoneMute    = 0x57,  // OOC
	HidLedUsageId_PlayerIndicator         = 0x60,  // NAry
	HidLedUsageId_Player1                 = 0x61,  // Sel
	HidLedUsageId_Player2                 = 0x62,  // Sel
	HidLedUsageId_Player3                 = 0x63,  // Sel
	HidLedUsageId_Player4                 = 0x64,  // Sel
	HidLedUsageId_Player5                 = 0x65,  // Sel
	HidLedUsageId_Player6                 = 0x66,  // Sel
	HidLedUsageId_Player7                 = 0x67,  // Sel
	HidLedUsageId_Player8                 = 0x68,  // Sel
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

const char*
getHidLedUsageIdString(HidLedUsageId id);

//..............................................................................

// HidUsagePage_Button (0x09)

enum HidButtonUsageId {
	HidButtonUsageId_None      = 0,
	HidButtonUsageId_Primary   = 1,
	HidButtonUsageId_Secondary = 2,
	HidButtonUsageId_Tertiary  = 3,

	// buttons 4, 5, etc.
};

// . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .

const char*
getHidButtonUsageIdString(HidButtonUsageId id);

//..............................................................................

sl::String
getHidUsageString(
	HidUsagePage page,
	uint_t id
);

//..............................................................................

} // namespace io
} // namespace axl
