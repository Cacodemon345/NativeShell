#include "precomp.h"

/*
 *****************************************************************************
 * CreateNativeProcess - Create a native process
 * file_name: full path to .exe, in DOS format
 * cmd_line: arguments for process
 *
 * Returns: STATUS_SUCCESS or STATUS_UNSUCCESSFUL
 *****************************************************************************
 */

NTSTATUS CreateNativeProcess(IN PCWSTR file_name, IN PCWSTR cmd_line, OUT PHANDLE hProcess)
{
 UNICODE_STRING fname, nt_file;
 PCWSTR file_part; 
 UNICODE_STRING EnvString, NullString, UnicodeSystemDriveString;
 NTSTATUS status; // Status
 UNICODE_STRING imgname; // ImageName
 UNICODE_STRING imgpath; // Nt ImagePath
 UNICODE_STRING dllpath; // Nt DllPath (DOS Name)
 UNICODE_STRING cmdline; // Nt CommandLine
 UNICODE_STRING curdir; // Nt CurrentDirectory.
 PRTL_USER_PROCESS_PARAMETERS processparameters; // ProcessParameters
 RTL_USER_PROCESS_INFORMATION processinformation={0}; // ProcessInformation
 WCHAR Env[2] = { 0,0 }; // Process Envirnoment
 PKUSER_SHARED_DATA SharedData = (PKUSER_SHARED_DATA)USER_SHARED_DATA; // Kernel Shared Data
 WCHAR CurrentDirectory[MAX_PATH]; // Current Directory.

 *hProcess = NULL;

 RtlDosPathNameToNtPathName_U(file_name, &nt_file, &file_part, NULL);
 RtlCliGetCurrentDirectory(CurrentDirectory);
 
 RtlInitUnicodeString(&imgpath, nt_file.Buffer); // Image path
 RtlInitUnicodeString(&imgname, file_part); // Image name
 RtlInitUnicodeString(&dllpath, SharedData->NtSystemRoot); // DLL Path is %SystemRoot%
 RtlInitUnicodeString(&cmdline, cmd_line); // Command Line parameters
 RtlInitUnicodeString(&curdir, CurrentDirectory); // Current directory
 
 status = RtlCreateProcessParameters(&processparameters, &imgname, &dllpath, &curdir, &cmdline, Env, 0, 0, 0, 0);

 if (!NT_SUCCESS(status))
 {
   RtlCliDisplayString("RtlCreateProcessParameters failed\n");
   return STATUS_UNSUCCESSFUL;
 }

 DbgPrint("Launching Process: %s, DllPath=%s, CmdLine=%s", &imgname, &dllpath, &cmdline);
 status = RtlCreateUserProcess(&imgpath, OBJ_CASE_INSENSITIVE, processparameters,
                                NULL, NULL, NULL, FALSE, NULL, NULL, &processinformation);

 if (!NT_SUCCESS(status))
 {
   RtlCliDisplayString("RtlCreateUserProcess failed\n");
   return STATUS_UNSUCCESSFUL;
 }
 
 status = NtResumeThread(processinformation.ThreadHandle, NULL);

  if (!NT_SUCCESS(status))
 {
   RtlCliDisplayString("NtResumeThread failed\n");
   return STATUS_UNSUCCESSFUL;
 }

 *hProcess = processinformation.ProcessHandle;

  return STATUS_SUCCESS;
}
