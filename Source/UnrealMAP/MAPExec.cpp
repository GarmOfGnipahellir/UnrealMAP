// Fill out your copyright notice in the Description page of Project Settings.

#include "CoreMinimal.h"
#include "MAPLog.h"
#include "Misc/CoreMisc.h"

#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)

struct FMAPExec : FSelfRegisteringExec
{
	FMAPExec()
	{
	}

protected:
	// Begin FExec Interface
	virtual bool Exec_Dev(UWorld* InWorld, const TCHAR* Cmd, FOutputDevice& Ar) override;
	// End FExec Interface
};

FMAPExec MAPExecInstance;

bool FMAPExec::Exec_Dev(UWorld* InWorld, const TCHAR* Cmd, FOutputDevice& Ar)
{
	if (!InWorld)
	{
		return false;
	}

	bool bHandled = false;

	if (FParse::Command(&Cmd, TEXT("MAPExportTBConfig")))
	{
		UE_LOG(LogMAP, Display, TEXT("MAPExportTBConfig Exec..."))
		bHandled = true;
	}

	return bHandled;
}

#endif
