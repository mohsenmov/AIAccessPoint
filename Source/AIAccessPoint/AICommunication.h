// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AICommunication.generated.h"

/**
 * 
 */
UCLASS()
class AIACCESSPOINT_API UAICommunication : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "AI Communication")
	static void SendMessageToAI(const FString& Prompt);

	UFUNCTION(BlueprintCallable, Category = "AI Communication")
	static void ReceiveMessageFromAI(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
private:
	static FString ConstructJsonMessage(const FString& UserInput);
};
