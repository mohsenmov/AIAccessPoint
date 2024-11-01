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

	UFUNCTION(BlueprintCallable, Category = "AI Communication")
	static bool GetIsRequestReady();

	static FString AIResponseText;
private:
	static bool _isRequestComplete;
	static FString ConstructJsonMessage(const FString& UserInput);
	static FString ParseAndConcatenateResponse(const FString& responseString);
	static void OnReceiveMessageFromAIResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	
};
