// Fill out your copyright notice in the Description page of Project Settings.


#include "AICommunication.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"

FString UAICommunication::AIResponseText;
bool UAICommunication::_isRequestComplete;
void UAICommunication::SendMessageToAI(const FString& Prompt)
{
    //AIResponseText = "";
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
    
    // make and set content
    FString JsonPayload = FString::Printf(TEXT("{\"model\": \"llama3.2\", \"prompt\": \"%s\",\"format\": \"json\",\"stream\": false}"), *Prompt);
    request->SetContentAsString(JsonPayload);
    request->OnProcessRequestComplete().BindStatic(&UAICommunication::OnReceiveMessageFromAIResponse);
    request->ProcessRequest();
}

FString UAICommunication::ReceiveMessageFromAI()
{
    // displays the result
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
    _isRequestComplete = true;
    // check for errors and/or lack of success
    if (!bWasSuccessful || !Response.IsValid())
    {
        UE_LOG(LogTemp, Error, TEXT("invalid and/or unsuccessful"));
        AIResponseText = TEXT("Invalid");
        return;
    }

    int32 ResponseCode = Response->GetResponseCode();
    if (ResponseCode != 200)
    {
        UE_LOG(LogTemp, Error, TEXT("Received error codes %d"), ResponseCode);
        AIResponseText = TEXT("Received error codes %d") + ResponseCode;
        return;
    }

    // extract the response
    FString responseString = Response->GetContentAsString();
    UE_LOG(LogTemp, Log, TEXT("Raw response: %s"), *responseString);
    TSharedPtr<FJsonObject> jsonResult;
    TSharedRef<TJsonReader<>> reader = TJsonReaderFactory<>::Create(responseString);


    if (FJsonSerializer::Deserialize(reader, jsonResult) && jsonResult.IsValid())
    {
        
        FString CurrAIResponse;
        bool isDone;
        jsonResult->TryGetBoolField(TEXT("done"), isDone);

        if (!isDone) {
            UE_LOG(LogTemp, Warning, TEXT("response not done"));
            AIResponseText = TEXT("bad http response");
            return;
        }

        if (jsonResult->TryGetStringField(TEXT("response"), CurrAIResponse))
        {
            // successful
            UE_LOG(LogTemp, Log, TEXT("AI Response: %s"), *CurrAIResponse);
            FString concatenatedResponse = ParseAndConcatenateResponse(CurrAIResponse);
            AIResponseText = concatenatedResponse;//AIResponseText + jsonResult->GetStringField(TEXT("response"));
            
            return;
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("Response not in JSON"));
            AIResponseText = TEXT("Bad response");
            return;
        }
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to parse JSON"));
        AIResponseText = TEXT("Bad response");
        return;
    }
}

// helper to create JSON pay load.
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

FString UAICommunication::ParseAndConcatenateResponse(const FString& responseString)
{
    TSharedPtr<FJsonObject> JsonObject;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(responseString);

    FString ConcatenatedResponse;

    if (FJsonSerializer::Deserialize(Reader, JsonObject) && JsonObject.IsValid())
    {
        for (const auto& Pair : JsonObject->Values)
        {
            ConcatenatedResponse += Pair.Value->AsString() + TEXT(" ");
        }
    }

    return ConcatenatedResponse.TrimEnd(); 
}


