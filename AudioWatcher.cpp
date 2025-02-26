#include "AudioWatcher.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "HAL/FileManager.h"
#include "Misc/FileHelper.h"
#include "JsonObjectConverter.h"
#include "Serialization/JsonSerializer.h"
#include "HAL/FileManager.h"
#include "Misc/Base64.h"

AAudioWatcher::AAudioWatcher()
{
    PrimaryActorTick.bCanEverTick = false;
    BaseUrl = TEXT("https://");   // url
    result = -1;
}

void AAudioWatcher::BeginPlay()
{
    Super::BeginPlay();
}

void AAudioWatcher::ProcessAudioFile(const FString& WavFilePath)
{
    FString Base64Audio = ConvertAudioToBase64(WavFilePath);
    if (Base64Audio.IsEmpty())
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to convert audio file to Base64."));
        return;
    }

    // JSON 데이터 생성
    TSharedPtr<FJsonObject> RequestJson = MakeShared<FJsonObject>();
    RequestJson->SetStringField(TEXT("app_id"), TEXT(""));   // id
    RequestJson->SetStringField(TEXT("name"), TEXT(""));     // name

    TArray<TSharedPtr<FJsonValue>> ItemArray;
    ItemArray.Add(MakeShared<FJsonValueString>(TEXT("")));   // key
    RequestJson->SetArrayField(TEXT("item"), ItemArray);

    TArray<TSharedPtr<FJsonValue>> ParamArray;
    ParamArray.Add(MakeShared<FJsonValueString>(Base64Audio));
    RequestJson->SetArrayField(TEXT("param"), ParamArray);

    // JSON 문자열로 직렬화
    FString RequestBody;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&RequestBody);
    FJsonSerializer::Serialize(RequestJson.ToSharedRef(), Writer);

    //UE_LOG(LogTemp, Log, TEXT("Request Body: %s"), *RequestBody);

    // HTTP 요청 생성
    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = FHttpModule::Get().CreateRequest();
    HttpRequest->SetVerb(TEXT("POST"));
    HttpRequest->SetURL(BaseUrl);
    HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
    HttpRequest->SetHeader(TEXT("cache-control"), TEXT("no-cache"));
    HttpRequest->SetContentAsString(RequestBody);

    // 요청 완료 콜백
    HttpRequest->OnProcessRequestComplete().BindUObject(this, &AAudioWatcher::OnResponseReceived);
    HttpRequest->ProcessRequest();
}

FString AAudioWatcher::ConvertAudioToBase64(const FString& FilePath)
{
    TArray<uint8> AudioData;
    if (FFileHelper::LoadFileToArray(AudioData, *FilePath))
    {
        return FBase64::Encode(AudioData);
    }
    return TEXT("");
}

void AAudioWatcher::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
    if (bWasSuccessful && Response.IsValid())
    {
        FString ResponseText = Response->GetContentAsString();
        UE_LOG(LogTemp, Log, TEXT("API Response: %s"), *ResponseText);

        // JSON 데이터 처리
        ProcessResponseText(ResponseText);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("HTTP Request Failed or Invalid Response."));
    }
}

void AAudioWatcher::ProcessResponseText(const FString& ResponseText)
{
    TArray<FString> Words;
    ResponseText.ParseIntoArray(Words, TEXT(" "), true);

    CommandValues.Empty();
    result = -1;
    if (Words.Num() < 2) return;

    // 명령 추출
    for (int32 i = 0; i < Words.Num() - 1; ++i)
    {
        UE_LOG(LogTemp, Log, TEXT("%s"), *Words[i]);
        if (Words[i] == TEXT("소리"))
        {
            if (Words[i + 1].Contains(TEXT("올려")))
            {
                CommandValues.Add(0);
                break;
            }
            else if (Words[i + 1].Contains(TEXT("내려")))
            {
                CommandValues.Add(1);
                break;
            }
        }
        else if (Words[i] == TEXT("밝기") || Words[i] == TEXT("발기"))
        {
            if (Words[i + 1].Contains(TEXT("올려")))
            {
                CommandValues.Add(2);
                break;
            }
            else if (Words[i + 1].Contains(TEXT("내려")))
            {
                CommandValues.Add(3);
                break;
            }
        }
        else if (Words[i] == TEXT("감도"))
        {
            if (Words[i + 1].Contains(TEXT("올려")))
            {
                CommandValues.Add(4);
                break;
            }
            else if (Words[i + 1].Contains(TEXT("내려")))
            {
                CommandValues.Add(5);
                break;
            }
        }
        else if (Words[i] == TEXT("비"))
        {
            if (Words[i + 1].Contains(TEXT("내려")))
            {
                CommandValues.Add(6);
                break;
            }
            else if (Words[i + 1].Contains(TEXT("멈춰")))
            {
                CommandValues.Add(7);
                break;
            }
        }
    }
    if (CommandValues.IsEmpty()) {

    }
    else {
        UE_LOG(LogTemp, Log, TEXT("%d"), CommandValues.Top());
        result = CommandValues.Top();
        UE_LOG(LogTemp, Log, TEXT("%d"), result);
        SettingChange(result);
    }
    
}

bool AAudioWatcher::DeleteFile(const FString& FilePath)
{
    // 파일이 존재하는지 확인
    if (!FPaths::FileExists(FilePath))
    {
        UE_LOG(LogTemp, Warning, TEXT("File does not exist: %s"), *FilePath);
        return false;
    }

    // 파일 삭제 시도
    if (IFileManager::Get().Delete(*FilePath))
    {
        UE_LOG(LogTemp, Log, TEXT("File deleted successfully: %s"), *FilePath);
        return true;
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Failed to delete file: %s"), *FilePath);
        return false;
    }
}
