// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AudioWatcher.generated.h"

UCLASS()
class CAPSTONE_API AAudioWatcher : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAudioWatcher();

	UFUNCTION(BlueprintCallable, Category = "Speech")
	void ProcessResponseText(const FString& ResponseText);
	UPROPERTY(BlueprintReadOnly, Category = "Speech")
	int result;

	// Function to process audio file
	UFUNCTION(BlueprintCallable, Category = "Speech")
	void ProcessAudioFile(const FString& WavFilePath);

	UFUNCTION(BlueprintCallable, Category = "File System")
	static bool DeleteFile(const FString& FilePath);

	UFUNCTION(BlueprintImplementableEvent, Category = "Setting")
	void SettingChange(int r);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	// API endpoint
	FString BaseUrl;
	TArray<int32> CommandValues;

	FString ConvertAudioToBase64(const FString& FilePath);

	void OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

};
