// Fill out your copyright notice in the Description page of Project Settings.


#include "AICommunication.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
void UAICommunication::SendMessageToAI(const FString& Prompt)
{
	FString url = "http://localhost:8000/api/generate";
	// create request
	FHttpModule& httpModule = FHttpModule::Get();
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> request = httpModule.CreateRequest();
	
	// set headers and verbs
	request->SetVerb(TEXT("POST"));
    request->SetURL(url);
    request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));

    // make and set content
    FString content = ConstructJsonMessage(Prompt);
    request->SetContentAsString(content);
    request->OnProcessRequestComplete().BindStatic(&UAICommunication::ReceiveMessageFromAI);
    request->ProcessRequest();
}

FString UAICommunication::ReceiveMessageFromAI(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
    // check for errors and/or lack of success
    if (!bWasSuccessful || !Response.IsValid())
    {
        UE_LOG(LogTemp, Error, TEXT("invalid and/or unsuccessful"));
        return;
    }

    int32 ResponseCode = Response->GetResponseCode();
    if (ResponseCode != 200)
    {
        UE_LOG(LogTemp, Error, TEXT("Received error codes %d"), ResponseCode);
        return;
    }
    // extract the response
    FString responseString = Response->GetContentAsString();
    TSharedPtr<FJsonObject> jsonResult;
    TSharedRef<TJsonReader<>> reader = TJsonReaderFactory<>::Create(responseString);

    if (FJsonSerializer::Deserialize(reader, jsonResult) && jsonResult.IsValid())
    {
        FString AIResponse;
        if (jsonResult->TryGetStringField("response", AIResponse))
        {
            // successful, return the response
            UE_LOG(LogTemp, Log, TEXT("AI Response: %s"), *AIResponse);
            return AIResponse;
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Response field not found in JSON"));
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to parse JSON response"));
    }

}

FString ConstructJsonMessage(const FString& UserInput) {
    
    TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());

    JsonObject->SetStringField(TEXT("model"), TEXT("llama3.2"));
    JsonObject->SetStringField(TEXT("prompt"), UserInput);
    JsonObject->SetBoolField(TEXT("stream"), false);
    FString JsonString;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonString);
    FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

    return JsonString;
}