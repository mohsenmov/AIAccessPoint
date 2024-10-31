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

void UAICommunication::ReceiveMessageFromAI(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
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