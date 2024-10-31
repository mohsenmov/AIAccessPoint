// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "HTTP.h"
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
	static FString ReceiveMessageFromAI();

	static FString AIResponseText;
private:
	static FString ConstructJsonMessage(const FString& UserInput);
	static void OnReceiveMessageFromAIResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	
};
