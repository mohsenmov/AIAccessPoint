// Fill out your copyright notice in the Description page of Project Settings.


#include "AICommunication.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"

FString UAICommunication::AIResponseText;
FString UAICommunication::lastResponse;
bool UAICommunication::_isRequestComplete;
void UAICommunication::SendMessageToAI(const FString& Prompt)
{
    _isRequestComplete = false;
    // check if empty
    if (Prompt.IsEmpty())
    {
        UE_LOG(LogTemp, Warning, TEXT("Prompt is empty. No request will be sent."));
        return;
    }

	FString url = "http://localhost:8000/api/generate";
	// create request
	FHttpModule& httpModule = FHttpModule::Get();
	TSharedRef<IHttpRequest, ESPMode::ThreadSafe> request = httpModule.CreateRequest();
	
	// set headers and verbs
	request->SetVerb(TEXT("POST"));
    request->SetURL(url);
    request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
    
    //FString JsonPayload = FString::Printf(TEXT("{\"model\": \"llama3.2\", \"prompt\": \"%s\",\"format\": \"json\", \"stream\": false}"), *Prompt);
    // make and set content
    FString content = ConstructJsonMessage(Prompt);
    request->SetContentAsString(content);

    request->OnProcessRequestComplete().BindStatic(&UAICommunication::OnReceiveMessageFromAIResponse);
    request->ProcessRequest();
}

FString UAICommunication::ReceiveMessageFromAI()
{
    // displays the result?
    if (_isRequestComplete) {

        return AIResponseText;
    }
    else {
        return TEXT("No Response yet");
    }
}

bool UAICommunication::GetIsRequestReady()
{
    return _isRequestComplete;
}

// helpers:
void UAICommunication::OnReceiveMessageFromAIResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
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
    UE_LOG(LogTemp, Log, TEXT("Raw response: %s"), *responseString);
    TSharedPtr<FJsonObject> jsonResult;
    TSharedRef<TJsonReader<>> reader = TJsonReaderFactory<>::Create(responseString);

    if (FJsonSerializer::Deserialize(reader, jsonResult) && jsonResult.IsValid())
    {
        FString AIResponse;
        if (jsonResult->TryGetStringField(TEXT("response"), AIResponse))
        {
            // successful
            UE_LOG(LogTemp, Log, TEXT("AI Response: %s"), *AIResponse);

            AIResponseText = AIResponse;
            _isRequestComplete = true;
            return;
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Response not in JSON"));
            return;
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to parse JSON"));
        return;
    }
}

FString UAICommunication::ConstructJsonMessage(const FString& UserInput) {
    
    TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());

    JsonObject->SetStringField(TEXT("model"), TEXT("llama3.2"));
    JsonObject->SetStringField(TEXT("prompt"), UserInput);
    JsonObject->SetStringField(TEXT("format"), "json");
    JsonObject->SetBoolField(TEXT("stream"), false);

    FString JsonString;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&JsonString);
    FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

    return JsonString;
}



